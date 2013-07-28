/*
 * infrared.h
 *
 *  Created on: 12.03.2013
 *      Author: kreyl
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "kl_lib_f2xx.h"

/*
 *  ModTmr outputs a TRGO at every Update event, i.e., every 600us.
 *  CarTmr as slave receives signal from ModTmr and uses it to Reset.
 *  CarTmr outputs a TRGO at every Reset event - i.e., every 600us (since it is
 *  Reset by ModTmr).
 *  CarTmr uses TRGO as DMA request; i.e. DMA request generated at every 600us.
 *  Buffer contains power level for every 600us chunk of packet;
 *  i.e. "PWM on for 2400us, off for 600us" translates to {Pwr, Pwr, Pwr, Pwr, 0}
 *  in Buf. (Pwr is power level).
 */

#define IR_CARRIER_HZ       56000
#define IR_BIT_CNT          14

#define IR_CARRIER_TMR      TIM3
#define IR_CARRIER_CHNL     2
#define IR_CARRIER_GPIO     GPIOC
#define IR_CARRIER_PIN      7
#define IR_CARRIER_TRG_IN   tiITR2  // Slave using TIM5 as input (ITR2)

#define IR_MODULATION_TMR   TIM5

#define IR_TX_DMA_STREAM    STM32_DMA1_STREAM4  // }
#define IR_TX_DMA_CHNL      5                   // } TIM3 trig

// Receive section, Level0
#define IR_RX_PWR_GPIO      GPIOC
#define IR_RX_PWR_PIN       6
#define IR_RX_IN_GPIO       GPIOC
#define IR_RX_IN_PIN        5
#define IR_RX_TIMER         TIM8
#define IR_IRQ_MASK         ((uint32_t)0x20)    // Line 5
#define IR_RXBUF_SZ         180 // 18o received bits

// Rx Level1
//#define IR_MAX_PKT_SZ       5

// Delays, uS
#define IR_TICK_US          600 // Protocol smallest time unit, us
/* Header = 4 * IR_TICK_US
 * Space  = 1 * IR_TICK_US
 * Zero   = 1 * IR_TICK_US
 * One    = 2 * IR_TICK_US
 */

// Timings
#define IR_HEADER_US        2400
#define IR_ZERO_US          600
#define IR_ONE_US           1200
#define IR_DEVIATION_US     150
#define IR_BOTTOM_BOUND_US  (IR_ZERO_US - IR_DEVIATION_US)
#define IR_TIMEOUT_MS       2

enum PieceType_t {ptHeader, ptZero, ptOne, ptError};

class Infrared_t {
private:
    uint16_t TxPwrBuf[4+1 + IR_BIT_CNT*(2+1) + 1];    // Buffer of power values: header + all one's + 1 delay after
    uint32_t MaxPower;
    Timer_t Carrier, Modulator, RxTimer;
    // Rx Level0
    RiseFall_t IWaitingEdge;
    inline void RxIrqWaitFalling() {
        EXTI->RTSR &= ~IR_IRQ_MASK; // Rising trigger disabled
        EXTI->FTSR |=  IR_IRQ_MASK; // Falling trigger enabled
    }
    inline void RxIrqWaitRising() {
        EXTI->RTSR |=  IR_IRQ_MASK; // Rising trigger enabled
        EXTI->FTSR &= ~IR_IRQ_MASK; // Falling trigger disabled
    }
    msg_t IRxBuf[IR_RXBUF_SZ];
    Mailbox imailbox;
    // Rx Level1
    EventSource IEvtSrcIrRx;
    uint16_t IRxW;
    uint8_t IBitCnt;
    bool IReceivingData;
    uint32_t IBitDelay;
    inline void IStartPkt();
    inline void ICancelPkt() { IReceivingData = false; IBitDelay = TIME_INFINITE; IBitCnt = 0; }
    inline void IAppend(uint8_t Bit) { IRxW <<= 1; IRxW |= Bit; IBitCnt++; }
    inline PieceType_t ProcessInterval(uint16_t Duration) {
        if     (IS_LIKE(Duration, IR_HEADER_US, IR_DEVIATION_US)) return ptHeader;
        else if(IS_LIKE(Duration, IR_ZERO_US,   IR_DEVIATION_US)) return ptZero;
        else if(IS_LIKE(Duration, IR_ONE_US,    IR_DEVIATION_US)) return ptOne;
        else return ptError;
    }
public:
    Infrared_t():
        MaxPower(0),
        Carrier(IR_CARRIER_TMR), Modulator(IR_MODULATION_TMR), RxTimer(IR_RX_TIMER),
        IWaitingEdge(Falling),
        IRxW(0), IBitCnt(0), IReceivingData(false), IBitDelay(TIME_INFINITE),
        IsBusy(false), RxWord(0) { }
    bool IsBusy;
    uint16_t RxWord;
    void TxInit();
    void RxInit();
    void Shutdown();
    uint8_t TransmitWord(uint16_t wData, uint8_t PwrPercent);
    void RegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcIrRx, PEvtLstnr, EvtMask); }
    // Inner use
    void IStopModulator() { Modulator.Disable(); }
    void IRxEdgeIrq();
    void IRxTask();
};

extern Infrared_t IR;

#endif /* INFRARED_H_ */
