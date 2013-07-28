/*
 * infrared.cpp
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#include "infrared.h"

Infrared_t IR;

// ============================= Interrupts ====================================
extern "C" {
CH_IRQ_HANDLER(EXTI9_5_IRQHandler) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    EXTI->PR = (1 << 5);  // Clean irq flag
    IR.IRxEdgeIrq();
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}

// Wrapper for Tx Completed IRQ
void IrTxcIrq(void *p, uint32_t flags) {
    dmaStreamDisable(IR_TX_DMA_STREAM);
    IR.IStopModulator(); // Stop master timer
    IR.IsBusy = false;
}
} // "C"

void Infrared_t::IRxEdgeIrq() {
    if(IWaitingEdge == Falling) { // Falling edge received, start measure
        RxTimer.SetCounter(0);
        RxIrqWaitRising();
        IWaitingEdge = Rising;
    }
    else {  // Rising edge received, stop timer
        int32_t t = RxTimer.GetCounter();
        RxIrqWaitFalling();
        IWaitingEdge = Falling;
        // Put time to queue if it is long enough
        if(t > IR_BOTTOM_BOUND_US) chMBPostI(&imailbox, t);
    }
}

// =============================== Implementation ==============================
static WORKING_AREA(waIRRxThread, 128);
static msg_t IRRxThread(void *arg) {
    (void)arg;
    chRegSetThreadName("IRRx");
    IR.IRxTask();
    return 0;
}

void Infrared_t::IStartPkt() {
    IRxW = 0;
    IBitCnt = 0;
    IReceivingData = true;
    IBitDelay = IR_TIMEOUT_MS;
}

void Infrared_t::IRxTask() {
    msg_t Msg;
    while(1) {
        // Fetch byte from queue
        if(chMBFetch(&imailbox, &Msg, IBitDelay) == RDY_OK) {
            //Uart.Printf("%u\r", Msg);
            PieceType_t Piece = ProcessInterval(Msg);
            switch(Piece) {
                case ptHeader: IStartPkt(); break;
                case ptOne:
                    if(IReceivingData) IAppend(1);
                    else ICancelPkt();
                    break;
                case ptZero:
                    if(IReceivingData) IAppend(0);
                    else ICancelPkt();
                    break;
                default: ICancelPkt(); break;
            } // switch
            // Check if Rx completed
            if(IBitCnt == 14) {
                ICancelPkt();
                RxWord = IRxW << 2;
                chEvtBroadcast(&IEvtSrcIrRx);
            } // if completed
        } // if msg
    } // while 1
}

void Infrared_t::TxInit() {
    // ==== Carrier timer ====
    Carrier.Init();
    Carrier.Enable();
    Carrier.PwmInit(IR_CARRIER_GPIO, IR_CARRIER_PIN, IR_CARRIER_CHNL, invNotInverted);
    Carrier.SetUpdateFrequency(IR_CARRIER_HZ);
    MaxPower = Carrier.GetTopValue() / 2;
    // Setup master-slave
    Carrier.SetTriggerInput(IR_CARRIER_TRG_IN);  // Slave using TIM5 as input (ITR2)
    Carrier.SlaveModeSelect(smReset);            // Slave mode: reset on trigger
    Carrier.MasterModeSelect(mmReset);           // Master mode: trigger output on Reset
    Carrier.DmaOnTriggerEnable();                // Request DMA on trigger output == req on Reset = req on input
    //Carrier.PwmSet(MaxPower);   // Debug

    // ==== Modulation timer ====
    Modulator.Init();
    Modulator.Enable();
    Modulator.SetTopValue(IR_TICK_US-1);    // Delay in us
    Modulator.MasterModeSelect(mmUpdate);   // Master mode: Update is used as TRGO
    Modulator.SetupPrescaler(1000000);      // Input Freq: 1 MHz => one tick = 1 uS
    Modulator.Disable();

    // Debug
//    PinSetupAlterFunc(GPIOA, 0, omPushPull, pudNone, AF2);  // Debug pin
//    TIM5->CCMR1 = (0b110<<4);   // PWM mode1
//    TIM5->CCR1 = IR_TICK_US/2;
//    TIM5->CCER = TIM_CCER_CC1E; // Enable output1

    // ==== DMA ==== Here only the unchanged parameters of the DMA are configured
    dmaStreamAllocate     (IR_TX_DMA_STREAM, IRQ_PRIO_LOW, IrTxcIrq, NULL);
    dmaStreamSetPeripheral(IR_TX_DMA_STREAM, Carrier.PCCR);
    dmaStreamSetMemory0   (IR_TX_DMA_STREAM, TxPwrBuf);
    dmaStreamSetMode      (IR_TX_DMA_STREAM,
            STM32_DMA_CR_CHSEL(IR_TX_DMA_CHNL)
            | DMA_PRIORITY_MEDIUM
            | STM32_DMA_CR_MSIZE_HWORD
            | STM32_DMA_CR_PSIZE_HWORD
            | STM32_DMA_CR_MINC         // Memory pointer increase
            | STM32_DMA_CR_DIR_M2P      // Direction is memory to peripheral
           // | STM32_DMA_CR_CIRC         // Enable circular buffer
            | STM32_DMA_CR_TCIE         // Enable Transmission Complete IRQ
            );

    // ==== Variables ====
    IsBusy = false;
}

void Infrared_t::RxInit() {
    chEvtInit(&IEvtSrcIrRx);
    // GPIO
    PinSetupOut(IR_RX_PWR_GPIO, IR_RX_PWR_PIN, omPushPull); // }
    PinSet(IR_RX_PWR_GPIO, IR_RX_PWR_PIN);                  // } Power
    PinSetupIn(IR_RX_IN_GPIO, IR_RX_IN_PIN, pudNone);       // Input
    // ==== Timer ====
    RxTimer.Init();
    RxTimer.Enable();
    RxTimer.SetTopValue(0xFFFF);        // Maximum
    RxTimer.SetupPrescaler(1000000);    // Input Freq: 1 MHz => one tick = 1 uS
    //RxTimer.Disable();
    // ==== Input queue ====
    chMBInit(&imailbox, IRxBuf, IR_RXBUF_SZ);
    // ==== Receiving thread ====
    chThdCreateStatic(waIRRxThread, sizeof(waIRRxThread), NORMALPRIO, IRRxThread, NULL);

    // ==== IRQ ==== PC5
    rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, FALSE); // Enable sys cfg controller
    SYSCFG->EXTICR[1] &= 0xFFFFFF0F;    // EXTI5 is connected to PortC
    SYSCFG->EXTICR[1] |= 0x00000020;    // EXTI5 is connected to PortC
    // Configure EXTI line
    EXTI->IMR  |=  IR_IRQ_MASK;         // Interrupt mode enabled
    EXTI->EMR  &= ~IR_IRQ_MASK;         // Event mode disabled
    RxIrqWaitFalling();
    EXTI->PR    =  IR_IRQ_MASK;         // Clean irq flag
    nvicEnableVector(EXTI9_5_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_HIGH));
}

void Infrared_t::Shutdown() {
    PinSetupAnalog(IR_RX_PWR_GPIO,  IR_RX_PWR_PIN);
    PinSetupAnalog(IR_RX_IN_GPIO,   IR_RX_IN_PIN);
    PinSetupAnalog(IR_CARRIER_GPIO, IR_CARRIER_PIN);
}

uint8_t Infrared_t::TransmitWord(uint16_t wData, uint8_t PwrPercent) {
//    Uart.Printf("T\r");
    if(IsBusy) return BUSY;
    IsBusy = true;
    // Calculate power
    uint32_t Pwr = (MaxPower * PwrPercent) / 100;
    // Fill buffer with powers depending on data
    uint16_t *p = TxPwrBuf;
    // Header
    *p++ = Pwr;
    *p++ = Pwr;
    *p++ = Pwr;
    *p++ = Pwr;
    // Data
    for(uint8_t i=0; i<IR_BIT_CNT; i++) {
        *p++ = 0;                       // Off = Space
        *p++ = Pwr;                     // '0' is single ON
        if(wData & 0x8000) *p++ = Pwr;  // '1' is double ON
        wData <<= 1;
    }
    *p++ = 0;                           // Off - finishing delay

    // Enable DMA
    uint8_t CountToTx = p - TxPwrBuf;
    dmaStreamSetTransactionSize(IR_TX_DMA_STREAM, CountToTx);
    dmaStreamEnable(IR_TX_DMA_STREAM);
    // Start master timer
    Modulator.GenerateUpdateEvt();  // Generate update: clear counter and request DMA
    Modulator.Enable();
    return OK;
}


