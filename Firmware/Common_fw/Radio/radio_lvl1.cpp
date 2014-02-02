/*
 * lvl1_assym.cpp
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "cc1101.h"
#include "SensorTable.h"

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
        for(uint8_t i=R_EMANATOR_BOTTOM_ID; i<=R_EMANATOR_TOP_ID; i++) {
            CC.SetChannel(i);
            int8_t RSSI=0;
            uint8_t RxRslt = CC.ReceiveSync(306, &PktRx, &RSSI);
            if(RxRslt == OK) {
    //            Uart.Printf("%u %d\r", PktRx.ID, RSSI);
                uint8_t Lvl = (uint8_t)(RSSI + 120);
                SnsTable.PutSnsInfo(PktRx.ID, Lvl);
            }
        } // for
        SnsTable.SendEvtReady();
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
