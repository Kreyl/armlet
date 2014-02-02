/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "evt_mask.h"
#include "cc1101.h"
#include "cmd_uart.h"

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOC
#define DBG_PIN1    13
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

rLevel1_t rLevel1;

#if 1 // ================================ Task =================================
static WORKING_AREA(warLvl1Thread, 256);
__attribute__((noreturn))
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(true) rLevel1.ITask();
}

void rLevel1_t::ITask() {
    chThdSleepMilliseconds(99);
    DBG1_SET();
    CC.Recalibrate();   // Recalibrate manually every cycle, as auto recalibration disabled
    // Transmit
    PktTx.CycleN = chTimeNow();
    CC.TransmitSync(&PktTx);
    DBG1_CLR();
}
#endif

#if 1 // ============================
void rLevel1_t::Init(uint8_t ASelfID) {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
#endif
    // Init RadioPkt
    PktTx.ID = ASelfID;
    PktTx.CycleN = 4;
    PktTx.TimeOwnerID = PktTx.ID;

    // Init radioIC
    CC.Init();
    CC.SetTxPower(Pwr0dBm);
    CC.SetPktSize(RPKT_SZ);
    CC.SetChannel(ASelfID);
    // Variables
    // Thread
    chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
}
#endif
