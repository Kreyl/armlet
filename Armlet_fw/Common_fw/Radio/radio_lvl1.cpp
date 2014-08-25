/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "evt_mask.h"
#include "application.h"
#include "cc1101.h"
#include "cmd_uart.h"
#include "peripheral.h"
#include "mesh_lvl.h"

//#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOC
#define DBG_PIN1    15
#define DBG1_SET()  PinSet(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinClear(DBG_GPIO1, DBG_PIN1)
#endif

rLevel1_t Radio;

#if 1 // ================================ Task =================================
static WORKING_AREA(warLvl1Thread, 256);
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    while(true) Radio.ITask();
}

//#define TX
//#define LED_RX
void rLevel1_t::ITask() {
    while(true) {
        if(Mesh.IsInit) {
            uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS); /* wait mesh cycle */

            if(EvtMsk & EVTMSK_MESH_RX) IMeshRx();

            if(EvtMsk & EVTMSK_MESH_TX) {
#if 1 // printf pkt
//                    Uart.Printf("rTxPkt: %u %u %u %u %u %u %u  {%u %u %u %d %u %u %u} \r",
//                            Mesh.PktTx.SenderInfo.Mesh.SelfID,
//                            Mesh.PktTx.SenderInfo.Mesh.CycleN,
//                            Mesh.PktTx.SenderInfo.Mesh.TimeOwnerID,
//                            Mesh.PktTx.SenderInfo.Mesh.TimeAge,
//                            Mesh.PktTx.SenderInfo.State.Reason,
//                            Mesh.PktTx.SenderInfo.State.Location,
//                            Mesh.PktTx.SenderInfo.State.Emotion,
//                            Mesh.PktTx.AlienID,
//                            Mesh.PktTx.AlienInfo.Mesh.Hops,
//                            Mesh.PktTx.AlienInfo.Mesh.Timestamp,
//                            Mesh.PktTx.AlienInfo.Mesh.TimeDiff,
//                            Mesh.PktTx.AlienInfo.State.Reason,
//                            Mesh.PktTx.AlienInfo.State.Location,
//                            Mesh.PktTx.AlienInfo.State.Emotion
//                            );
#endif
                CC.TransmitSync(&Mesh.PktTx); /* Pkt was prepared in Mesh Thd */
                Mesh.ITxEnd();
            } // Mesh Tx
        } // Mesh Init
        else chThdSleepMilliseconds(41);
    }
}
#endif

#if 1 // ==== Mesh Rx Cycle ====

static void RxEnd(void *p) {
//    Uart.Printf("RxEnd, t=%u\r", chTimeNow());
    Radio.Valets.InRx = false;
}

void rLevel1_t::IMeshRx() {
//    Uart.Printf("Rx Start, t=%u\r", chTimeNow());
    int8_t RSSI = 0;
    Valets.RxEndTime = (chTimeNow()) + CYCLE_TIME - SLOT_TIME;
    Valets.InRx = true;
    chEvtSignal(Mesh.IPPktHanlderThread, EVTMSK_MESH_PKT_RDY);
    chVTSet(&Valets.RxVT, MS2ST(CYCLE_TIME-SLOT_TIME), RxEnd, nullptr); /* Set VT */

    // TODO: while(1) -> currtime -> break
    do {
        Valets.CurrentTime = chTimeNow();
        uint8_t RxRslt = CC.ReceiveSync(Valets.RxEndTime - Valets.CurrentTime, &Mesh.PktRx, &RSSI);
        if(RxRslt == OK) { // Pkt received correctly
            Mesh.MsgBox.Post({chTimeNow(), RSSI, &Mesh.PktRx});  /* SendMsg to MeshThd with PktRx structure */
#if 1 // printf RxPkt
//            Uart.Printf("rRxPkt: %u %u %u %u %u %u %u  {%u %u %u %d %u %u %u} %d \r",
//                    Mesh.PktRx.SenderInfo.Mesh.SelfID,
//                    Mesh.PktRx.SenderInfo.Mesh.CycleN,
//                    Mesh.PktRx.SenderInfo.Mesh.TimeOwnerID,
//                    Mesh.PktRx.SenderInfo.Mesh.TimeAge,
//                    Mesh.PktRx.SenderInfo.State.Reason,
//                    Mesh.PktRx.SenderInfo.State.Location,
//                    Mesh.PktRx.SenderInfo.State.Emotion,
//                    Mesh.PktRx.AlienID,
//                    Mesh.PktRx.AlienInfo.Mesh.Hops,
//                    Mesh.PktRx.AlienInfo.Mesh.Timestamp,
//                    Mesh.PktRx.AlienInfo.Mesh.TimeDiff,
//                    Mesh.PktRx.AlienInfo.State.Reason,
//                    Mesh.PktRx.AlienInfo.State.Location,
//                    Mesh.PktRx.AlienInfo.State.Emotion,
//                    RSSI
//                    );
#endif
        } // Pkt Ok
    } while(Radio.Valets.InRx);
    Mesh.SendEvent(EVTMSK_MESH_RX_END);
}
#endif

#if 1 // ============================
void rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
    DBG1_CLR();
#endif
    // Init radioIC
    CC.Init();
    CC.SetTxPower(CC_Pwr0dBm);
    CC.SetChannel(MESH_CHANNEL);
    CC.SetPktSize(MESH_PKT_SZ);
    // Thread
    rThd = chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
}
#endif
