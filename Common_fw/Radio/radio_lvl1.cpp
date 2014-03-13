/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "cc1101.h"
#include "SensorTable.h"
#include "evt_mask.h"

#include "lcd2630.h"
#include "mesh_lvl.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    6
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

rLevel1_t rLevel1;

// Radiotask
void rLevel1_t::ITask() {
    while(true) {
        // New cycle begins
        CC.Recalibrate();   // Recalibrate manually every cycle, as auto recalibration disabled
#if 1 //  ==== Emanators ====
//        for(uint8_t i=R_EMANATOR_BOTTOM_ID; i<=R_EMANATOR_TOP_ID; i++) {
//            CC.SetChannel(i);
//            int8_t RSSI=0;
//            uint8_t RxRslt = CC.ReceiveSync(306, &PktRx, &RSSI);
//            if(RxRslt == OK) {
//    //            Uart.Printf("%u %d\r", PktRx.ID, RSSI);
//                uint8_t Lvl = (uint8_t)(RSSI + 120);
//                SnsTable.PutSnsInfo(PktRx.ID, Lvl);
//            }
//        } // for
//        SnsTable.SendEvtReady();
#endif

#if 1 // ==== Mesh v.2 ====
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        PktTx.TimeAge++;
        if(EvtMsk & EVTMSK_MESH_TX) {
            PktTx.CycleN = Mesh.GetAbsTime();
//            Uart.Printf("RadioTx\r");
            if(PktTx.TimeAge > TIME_AGE_THRESHOLD) { ResetTimeAge(PktTx.ID); }
            CC.TransmitSync(&PktTx);
        }
        if(EvtMsk & EVTMSK_MESH_RX) {
            int8_t RSSI = 0;
            RxTmt = CYCLE_TIME;
            RxStartTime = chTimeNow();
//            Uart.Printf("RxStart, t=%u\r", chTimeNow());
            do {
                Time = chTimeNow();
                uint8_t RxRslt = CC.ReceiveSync(RxTmt, &PktRx, &RSSI);
                if(RxRslt == OK) { // Pkt received correctly
//                    Uart.Printf("Slot %u, ID=%u:%u, %d\r", i, PktRx.ID, PktRx.CycleN, RSSI);
                    Mesh.MsgBox.Post({chTimeNow(), PktRx, RSSI}); /* SendMsg to MeshThd with PktRx structure */
                } // Pkt Ok
                RxTmt = ((chTimeNow() - Time) > 0)? RxTmt - (chTimeNow() - Time) : 0;
            } while ((chTimeNow() - RxStartTime) < CYCLE_TIME);
//            Uart.Printf("RxEnd, t=%u\r", chTimeNow());
            chEvtSignal(Mesh.IPThread, EVTMSK_UPDATE_CYCLE);

        }
#endif
    } // while true
}

// ================================= Thread ====================================
static WORKING_AREA(warLvl1Thread, 256);
__attribute__ ((__noreturn__))
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(1) rLevel1.ITask();
}

// ================================= Common ====================================
void rLevel1_t::Init(uint32_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull, pudNone);
#endif

    if(ASelfID == 0) {
        Uart.Printf("rLvl1 WrongID\r");
        return;
    }

    PktTx.ID = (uint8_t)ASelfID;
    PktTx.CycleN = 0;
    ResetTimeAge(PktTx.ID);

    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);
    //CC.SetTxPower(PwrMinus6dBm);
    CC.SetPktSize(RPKT_SZ);
    // Init thread. High priority is required to satisfy timings.
    PrThd = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
}
