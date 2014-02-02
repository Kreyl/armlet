/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "cc1101.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    14
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

rLevel1_t rLevel1;

// Radiotask
void rLevel1_t::ITask() {
    while(true) {
        // New cycle begins
        CC.Recalibrate();   // Recalibrate manually every cycle, as auto recalibration disabled
        int8_t RSSI=0;
        uint8_t RxRslt = CC.ReceiveSync(306, &PktRx, &RSSI);
        if(RxRslt == OK) {
            Uart.Printf("%u %d\r", PktRx.ID, RSSI);
        }
//        chThdSleepMilliseconds(45);

#ifdef TX
        // Transmit
        DBG1_SET();
        CC.TransmitSync(&PktTx);
        DBG1_CLR();
        chThdSleepMilliseconds(99);
#elif defined LED_RX
        Color_t Clr;
        uint8_t RxRslt = CC.ReceiveSync(306, &PktRx);
        if(RxRslt == OK) {
            Uart.Printf("%d\r", PktRx.RSSI);
            Clr = clWhite;
            if     (PktRx.RSSI < -100) Clr = clRed;
            else if(PktRx.RSSI < -90) Clr = clYellow;
            else if(PktRx.RSSI < -80) Clr = clGreen;
            else if(PktRx.RSSI < -70) Clr = clCyan;
            else if(PktRx.RSSI < -60) Clr = clBlue;
            else if(PktRx.RSSI < -50) Clr = clMagenta;
        }
        else Clr = clBlack;
        Led.SetColor(Clr);
#else
        //IterateEmanators();
//        Uart.Printf("%d\r", Damage);

#endif
    } // while true


//    if(++SlotN >= RSLOT_CNT) SlotN = 0;
//    PktTx.SlotN = SlotN;
//    // If not transmitting long pkt already, try to get 0; // new data pkt
//    if((ITxHdr.State == OK) or (ITxHdr.State == FAILURE) or (ITxHdr.State == BUSY)) {
//        if(ITxBuf.Get((uint8_t*)&ITxHdr, sizeof(rPktHeader_t)) == OK) ITxHdr.State = NEW;   // Will not be transmitted until correct slot
//    }
//    // Transmit data if 0; // new and in correct slot, or if first pkt is already sent
//    if(((ITxHdr.State == NEW) and InsideCorrectSlot()) or (ITxHdr.State == IN_PROGRESS)) PrepareDataPkt();
//    else PreparePingPkt();
//
//    CC.Transmit(&PktTx);
//
//    // ==== Pkt transmitted, enter RX ====
//    uint8_t RxRslt = CC.Receive(R_RX_WAIT_MS, &PktRx);
//    //Uart.Printf("Rx: %u\r", RxRslt);
//    // Process result
//    if(RxRslt == OK) {
//        //Uart.Printf("Rx Slot=%u; ID=%u; Srv=%X\r", SlotN, PktRx.rID, PktRx.Srv);
//        // If data received, put it to queue
//        if(PktRx.Srv & R_CMD_DATA) HandleRxDataPkt();
//        // Check reply to data containing pkt
//        if((ITxHdr.State == LAST) and (PktRx.Srv & R_ACK)) ICompleteTx(OK);    // If transmission completed
//        if(((ITxHdr.State == IN_PROGRESS) or (ITxHdr.State == LAST)) and !(PktRx.Srv & R_ACK)) ICompleteTx(BUSY);
//    } // if ok
//    else {  // No answer
//        // If data pkt in progress, fail it
//        if((ITxHdr.State == IN_PROGRESS) or (ITxHdr.State == LAST)) ICompleteTx(FAILURE);
//    } // if rx
}

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 256);
__attribute__ ((__noreturn__))
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(1) rLevel1.ITask();
}

// ================================= Common ====================================
void rLevel1_t::Init(uint8_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);
    //CC.SetTxPower(PwrMinus6dBm);
    CC.SetPktSize(RPKT_SZ);
    CC.SetChannel(1);
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
}
