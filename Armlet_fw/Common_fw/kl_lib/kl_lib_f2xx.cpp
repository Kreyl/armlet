/*
 * kl_lib_f0.cpp
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#include "kl_lib_f2xx.h"
#include "cmd_uart.h"
#include <stdarg.h>
#include <string.h>

// ================================ Timer ======================================
void Timer_t::Init(TIM_TypeDef *Tmr) {
    ITmr = Tmr;
    if     (ITmr == TIM1)  { rccEnableTIM1(FALSE); }
    else if(ITmr == TIM2)  { rccEnableTIM2(FALSE); }
    else if(ITmr == TIM3)  { rccEnableTIM3(FALSE); }
    else if(ITmr == TIM4)  { rccEnableTIM4(FALSE); }
    else if(ITmr == TIM5)  { rccEnableTIM5(FALSE); }
    else if(ITmr == TIM7)  { rccEnableTIM7(FALSE); }
    else if(ITmr == TIM8)  { rccEnableTIM8(FALSE); }
    else if(ITmr == TIM9)  { rccEnableAPB2(RCC_APB2ENR_TIM9EN, FALSE); }
    else if(ITmr == TIM10) { rccEnableAPB2(RCC_APB2ENR_TIM10EN, FALSE); }
    else if(ITmr == TIM11) { rccEnableAPB2(RCC_APB2ENR_TIM11EN, FALSE); }
    else if(ITmr == TIM12) { rccEnableAPB1(RCC_APB1ENR_TIM12EN, FALSE); }
    else if(ITmr == TIM13) { rccEnableAPB1(RCC_APB1ENR_TIM13EN, FALSE); }
    else if(ITmr == TIM14) { rccEnableAPB1(RCC_APB1ENR_TIM14EN, FALSE); }
    // Clock src
    if(ANY_OF_5(ITmr, TIM1, TIM8, TIM9, TIM10, TIM11)) PClk = &Clk.APB2FreqHz;
    else PClk = &Clk.APB1FreqHz;
}

void Timer_t::PwmInit(GPIO_TypeDef *GPIO, uint16_t N, uint8_t Chnl, Inverted_t Inverted) {
    // GPIO
    if     (ANY_OF_2(ITmr, TIM1, TIM2))       PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
    else if(ANY_OF_3(ITmr, TIM3, TIM4, TIM5)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
    else if(ANY_OF_4(ITmr, TIM8, TIM9, TIM10, TIM11)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
    else if(ANY_OF_3(ITmr, TIM12, TIM13, TIM14)) PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
    // Enable outputs for advanced timers
    ITmr->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;
    // Output
    uint16_t tmp = (Inverted == invInverted)? 0b111 : 0b110; // PWM mode 1 or 2
    switch(Chnl) {
        case 1:
            PCCR = &ITmr->CCR1;
            ITmr->CCMR1 |= (tmp << 4);
            ITmr->CCER  |= TIM_CCER_CC1E;
            break;

        case 2:
            PCCR = &ITmr->CCR2;
            ITmr->CCMR1 |= (tmp << 12);
            ITmr->CCER  |= TIM_CCER_CC2E;
            break;

        case 3:
            PCCR = &ITmr->CCR3;
            ITmr->CCMR2 |= (tmp << 4);
            ITmr->CCER  |= TIM_CCER_CC3E;
            break;

        case 4:
            PCCR = &ITmr->CCR4;
            ITmr->CCMR2 |= (tmp << 12);
            ITmr->CCER  |= TIM_CCER_CC4E;
            break;

        default: break;
    }
}

// ================================ PWM pin ====================================
void PwmPin_t::Init(GPIO_TypeDef *GPIO, uint16_t N, TIM_TypeDef* PTim, uint8_t Chnl, uint16_t TopValue, bool Inverted) {
    Tim = PTim;
    if(Tim == TIM1) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
        rccEnableTIM1(FALSE);
    }
    else if(Tim == TIM2) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF1);
        rccEnableTIM2(FALSE);
    }
    else if(Tim == TIM3) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
        rccEnableTIM3(FALSE);
    }
    else if(Tim == TIM4) {
            PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
            rccEnableTIM4(FALSE);
    }
    else if(Tim == TIM5) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF2);
        rccEnableTIM5(FALSE);
    }
    else if(Tim == TIM8) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
        rccEnableTIM8(FALSE);
    }
    else if(Tim == TIM9) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
        rccEnableAPB2(RCC_APB2ENR_TIM9EN, FALSE);
    }
    else if(Tim == TIM10) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
        rccEnableAPB2(RCC_APB2ENR_TIM10EN, FALSE);
    }
    else if(Tim == TIM11) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF3);
        rccEnableAPB2(RCC_APB2ENR_TIM11EN, FALSE);
    }
    else if(Tim == TIM12) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
        rccEnableAPB1(RCC_APB1ENR_TIM12EN, FALSE);
    }
    else if(Tim == TIM13) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
        rccEnableAPB1(RCC_APB1ENR_TIM13EN, FALSE);
    }
    else if(Tim == TIM14) {
        PinSetupAlterFunc(GPIO, N, omPushPull, pudNone, AF9);
        rccEnableAPB1(RCC_APB1ENR_TIM14EN, FALSE);
    }

    // Clock src
    if(ANY_OF_5(Tim, TIM1, TIM8, TIM9, TIM10, TIM11)) PClk = &Clk.APB2FreqHz;
    else PClk = &Clk.APB1FreqHz;

    // Common
    Tim->CR1 = TIM_CR1_CEN; // Enable timer, set clk division to 0, AutoReload not buffered
    Tim->CR2 = 0;
    Tim->ARR = TopValue;
    Tim->BDTR = TIM_BDTR_MOE | TIM_BDTR_AOE;

    // Output
    uint16_t tmp = Inverted? 0b111 : 0b110; // PWM mode 1 or 2
    switch(Chnl) {
        case 1:
            PCCR = &Tim->CCR1;
            Tim->CCMR1 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC1E;
            break;

        case 2:
            PCCR = &Tim->CCR2;
            Tim->CCMR1 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC2E;
            break;

        case 3:
            PCCR = &Tim->CCR3;
            Tim->CCMR2 |= (tmp << 4);
            Tim->CCER  |= TIM_CCER_CC3E;
            break;

        case 4:
            PCCR = &Tim->CCR4;
            Tim->CCMR2 |= (tmp << 12);
            Tim->CCER  |= TIM_CCER_CC4E;
            break;

        default: break;
    }
    *PCCR = 0;
}

void PwmPin_t::SetFreqHz(uint32_t FreqHz) {
    uint32_t divider = Tim->ARR * FreqHz;
    if(divider == 0) return;
    uint32_t FPrescaler = *PClk / divider;
    if(FPrescaler != 0) FPrescaler--;   // do not decrease in case of high freq
    Tim->PSC = (uint16_t)FPrescaler;
}

// ================================= DEBUG =====================================
void chDbgPanic(const char *msg1) {
    Uart.PrintNow("\r");
    Uart.PrintNow(msg1);
    Uart.PrintNow(" @");
    Uart.PrintNow(chThdSelf()->p_name);
}

// ================================= Random ====================================
uint32_t Random(uint32_t TopValue) {
    rccEnableAHB2(RCC_AHB2ENR_RNGEN, FALSE);    // Enable clock
    RNG->CR |= RNG_CR_RNGEN;                    // Enable generator
    while(!(RNG->SR & RNG_SR_DRDY));            // Wait until ready
    uint32_t Rnd = RNG->DR;
    Rnd = Rnd % (TopValue + 1);
    rccDisableAHB2(RCC_AHB2ENR_RNGEN, FALSE);   // Stop clock
    return Rnd;
}

// =============================== I2C =========================================
void i2cDmaIrqHandler(void *p, uint32_t flags) {
    chSysLockFromIsr();
    //Uart.Printf("===T===");
    Thread *PThd = ((i2c_t*)p)->PRequestingThread;
    if (PThd != NULL) {
        ((i2c_t*)p)->PRequestingThread = NULL;
        chSchReadyI(PThd);
    }
    chSysUnlockFromIsr();
}

void i2c_t::Init(
        I2C_TypeDef *pi2c,
        GPIO_TypeDef *PGpio,
        uint16_t SclPin,
        uint16_t SdaPin,
        uint32_t BitrateHz,
        const stm32_dma_stream_t *APDmaTx,
        const stm32_dma_stream_t *APDmaRx
    ) {
    ii2c = pi2c;
    IPGpio = PGpio;
    ISclPin = SclPin;
    ISdaPin = SdaPin;
    IBitrateHz = BitrateHz;
    Standby();
    Resume();

    // ==== DMA ====
    // Here only unchanged parameters of the DMA are configured.
    DmaChnl = 3;   // I2C3
    if      (ii2c == I2C1) DmaChnl = 1;
    else if (ii2c == I2C2) DmaChnl = 7;

    // Setup Dma TX
    PDmaTx = APDmaTx;
    dmaStreamAllocate(PDmaTx, IRQ_PRIO_MEDIUM, i2cDmaIrqHandler, this);
    dmaStreamSetPeripheral(PDmaTx, &ii2c->DR);
    // Setup Dma RX
    PDmaRx = APDmaRx;
    dmaStreamAllocate(PDmaRx, IRQ_PRIO_MEDIUM, i2cDmaIrqHandler, this);
    //dmaStreamAllocate(PDmaRx, 1, i2cDmaRXIrqHandler, NULL);
    dmaStreamSetPeripheral(PDmaRx, &ii2c->DR);
}

void i2c_t::Standby() {
    if      (ii2c == I2C1) { rccResetI2C1(); rccDisableI2C1(FALSE); }
    else if (ii2c == I2C2) { rccResetI2C2(); rccDisableI2C2(FALSE); }
    else if (ii2c == I2C3) { rccResetI2C3(); rccDisableI2C3(FALSE); }
    // Disable GPIOs
    PinSetupAnalog(IPGpio, ISclPin);
    PinSetupAnalog(IPGpio, ISdaPin);
}

void i2c_t::Resume() {
    Error = false;
    // ==== GPIOs ====
    PinSetupAlterFunc(IPGpio, ISclPin, omOpenDrain, pudNone, AF4);
    PinSetupAlterFunc(IPGpio, ISdaPin, omOpenDrain, pudNone, AF4);
    // ==== Clock and reset ====
    if      (ii2c == I2C1) { rccEnableI2C1(FALSE); rccResetI2C1(); }
    else if (ii2c == I2C2) { rccEnableI2C2(FALSE); rccResetI2C2(); }
    else if (ii2c == I2C3) { rccEnableI2C3(FALSE); rccResetI2C3(); }
    // Minimum clock is 2 MHz
    uint32_t ClkMhz = Clk.APB1FreqHz / 1000000;
    uint16_t tmpreg = ii2c->CR2;
    tmpreg &= (uint16_t)~I2C_CR2_FREQ;
    if(ClkMhz < 2)  ClkMhz = 2;
    if(ClkMhz > 30) ClkMhz = 30;
    tmpreg |= ClkMhz;
    ii2c->CR2 = tmpreg;
    ii2c->CR1 &= (uint16_t)~I2C_CR1_PE; // Disable i2c to setup TRise & CCR
    ii2c->TRISE = (uint16_t)(((ClkMhz * 300) / 1000) + 1);
    // 16/9
    tmpreg = (uint16_t)(Clk.APB1FreqHz / (IBitrateHz * 25));
    if(tmpreg == 0) tmpreg = 1; // minimum allowed value
    tmpreg |= I2C_CCR_FS | I2C_CCR_DUTY;
    ii2c->CCR = tmpreg;
    ii2c->CR1 |= I2C_CR1_PE;    // Enable i2c back
    // ==== DMA ====
    ii2c->CR2 |= I2C_CR2_DMAEN;
}

void i2c_t::Reset() {
    Standby();
    Resume();
}

uint8_t i2c_t::CmdWriteRead(uint8_t Addr,
        uint8_t *WPtr, uint8_t WLength,
        uint8_t *RPtr, uint8_t RLength) {
    if(IBusyWait() != OK) return FAILURE;
    // Clear flags
    ii2c->SR1 = 0;
    while(RxIsNotEmpty()) (void)ii2c->DR;   // Read DR until it empty
    ClearAddrFlag();
    // Start transmission
    SendStart();
    if(WaitEv5() != OK) return FAILURE;
    SendAddrWithWrite(Addr);
    if(WaitEv6() != OK) { SendStop(); return FAILURE; }
    ClearAddrFlag();
    // Start TX DMA if needed
    if(WLength != 0) {
        if(WaitEv8() != OK) return FAILURE;
        dmaStreamSetMemory0(PDmaTx, WPtr);
        dmaStreamSetMode   (PDmaTx, I2C_DMATX_MODE);
        dmaStreamSetTransactionSize(PDmaTx, WLength);
        chSysLock();
        PRequestingThread = chThdSelf();
        dmaStreamEnable(PDmaTx);
        chSchGoSleepS(THD_STATE_SUSPENDED); // Sleep until end
        chSysUnlock();
        dmaStreamDisable(PDmaTx);
    }
    // Read if needed
    if(RLength != 0) {
        if(WaitEv8() != OK) return FAILURE;
        // Send repeated start
        SendStart();
        if(WaitEv5() != OK) return FAILURE;
        SendAddrWithRead(Addr);
        if(WaitEv6() != OK) { SendStop(); return FAILURE; }
        // If single byte is to be received, disable ACK before clearing ADDR flag
        if(RLength == 1) AckDisable();
        else AckEnable();
        ClearAddrFlag();
        dmaStreamSetMemory0(PDmaRx, RPtr);
        dmaStreamSetMode   (PDmaRx, I2C_DMARX_MODE);
        dmaStreamSetTransactionSize(PDmaRx, RLength);
        DmaLastTransferSet(); // Inform DMA that this is last transfer => do not ACK last byte
        chSysLock();
        PRequestingThread = chThdSelf();
        dmaStreamEnable(PDmaRx);
        chSchGoSleepS(THD_STATE_SUSPENDED); // Sleep until end
        chSysUnlock();
        dmaStreamDisable(PDmaRx);
    } // if != 0
    SendStop();
    return OK;
}

uint8_t i2c_t::CmdWriteWrite(uint8_t Addr,
        uint8_t *WPtr1, uint8_t WLength1,
        uint8_t *WPtr2, uint8_t WLength2) {
    if(IBusyWait() != OK) return FAILURE;
    // Clear flags
    ii2c->SR1 = 0;
    while(RxIsNotEmpty()) (void)ii2c->DR;   // Read DR until it empty
    ClearAddrFlag();
    // Start transmission
    SendStart();
    if(WaitEv5() != OK) return FAILURE;
    SendAddrWithWrite(Addr);
    if(WaitEv6() != OK) { SendStop(); return FAILURE; }
    ClearAddrFlag();
    // Start TX DMA if needed
    if(WLength1 != 0) {
        if(WaitEv8() != OK) return FAILURE;
        dmaStreamSetMemory0(PDmaTx, WPtr1);
        dmaStreamSetMode   (PDmaTx, I2C_DMATX_MODE);
        dmaStreamSetTransactionSize(PDmaTx, WLength1);
        chSysLock();
        PRequestingThread = chThdSelf();
        dmaStreamEnable(PDmaTx);
        chSchGoSleepS(THD_STATE_SUSPENDED); // Sleep until end
        chSysUnlock();
        dmaStreamDisable(PDmaTx);
    }
    if(WLength2 != 0) {
        if(WaitEv8() != OK) return FAILURE;
        dmaStreamSetMemory0(PDmaTx, WPtr2);
        dmaStreamSetMode   (PDmaTx, I2C_DMATX_MODE);
        dmaStreamSetTransactionSize(PDmaTx, WLength2);
        chSysLock();
        PRequestingThread = chThdSelf();
        dmaStreamEnable(PDmaTx);
        chSchGoSleepS(THD_STATE_SUSPENDED); // Sleep until end
        chSysUnlock();
        dmaStreamDisable(PDmaTx);
    }
    WaitBTF();
    SendStop();
    return OK;
}

// ==== Flag operations ====
// Busy flag
uint8_t i2c_t::IBusyWait() {
    uint8_t RetryCnt = 4;
    while(RetryCnt--) {
        if(!(ii2c->SR2 & I2C_SR2_BUSY)) return OK;
        chThdSleepMilliseconds(1);
    }
    Error = true;
    return TIMEOUT;
}

// BUSY, MSL & SB flags
uint8_t i2c_t::WaitEv5() {
    uint32_t RetryCnt = 450;
    while(RetryCnt--) {
        uint16_t Flag1 = ii2c->SR1;
        uint16_t Flag2 = ii2c->SR2;
        if((Flag1 & I2C_SR1_SB) and (Flag2 & (I2C_SR2_MSL | I2C_SR2_BUSY))) return OK;
    }
    Error = true;
    return FAILURE;
}

uint8_t i2c_t::WaitEv6() {
    uint32_t RetryCnt = 45;
    uint16_t Flag1;
    do {
        Flag1 = ii2c->SR1;
        if((RetryCnt-- == 0) or (Flag1 & I2C_SR1_AF)) return FAILURE;   // Fail if timeout or NACK
    } while(!(Flag1 & I2C_SR1_ADDR)); // ADDR set when Address is sent and ACK received
    return OK;
}

uint8_t i2c_t::WaitEv8() {
    uint32_t RetryCnt = 45;
    while(RetryCnt--)
        if(ii2c->SR1 & I2C_SR1_TXE) return OK;
    Error = true;
    return TIMEOUT;
}

uint8_t i2c_t::WaitRx() {
    uint32_t RetryCnt = 450;
    while(RetryCnt--)
        if(ii2c->SR1 & I2C_SR1_RXNE) return OK;
    return TIMEOUT;
}

uint8_t i2c_t::WaitStop() {
    uint32_t RetryCnt = 450;
    while(RetryCnt--)
        if(ii2c->CR1 & I2C_CR1_STOP) return OK;
    return TIMEOUT;
}

uint8_t i2c_t::WaitBTF() {
    uint32_t RetryCnt = 450;
    while(RetryCnt--)
        if(ii2c->SR1 & I2C_SR1_BTF) return OK;
    return TIMEOUT;
}
