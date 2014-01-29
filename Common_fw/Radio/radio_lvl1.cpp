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
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake up by rTmr
    chSysUnlock();
    // ==== Sleep ended, transmit 0; // new pkt ====
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
static WORKING_AREA(warLvl1Thread, 1024);
__attribute__ ((__noreturn__))
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(1) rLevel1.ITask();
}

// ================================= Common ====================================
void rLevel1_t::Init(uint16_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif
    // Init Rx & Tx buffers
    // Init radioIC
    CC.Init();
    CC.SetTxPower(PwrPlus12dBm);
    //CC.SetTxPower(PwrMinus6dBm);
    // Variables
    CC.SetChannel(1);
    // Init thread. High priority is required to satisfy timings.
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
}
