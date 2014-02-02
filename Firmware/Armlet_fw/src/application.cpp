/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "lcd2630.h"
#include "peripheral.h"
#include "BeepSequences.h"
#include "VibroSequences.h"
#include "keys.h"
#include "pill.h"
#include "infrared.h"
#include "power.h"

#include "radio_lvl1.h"
#include "SensorTable.h"
#include "evt_mask.h"
#include "kl_sd.h"
#include "sound.h"

App_t App;

static VirtualTimer ITmr;
void MeasureTmrCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMASK_TIMER);
    chVTSetI(&ITmr, MS2ST(999), MeasureTmrCallback, nullptr);
    chSysUnlockFromIsr();
}

#if 1 // ============================= Threads =================================
static WORKING_AREA(waAppThread, 256);
__attribute__((noreturn))
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMSK_TABLE_READY) {
            if(SnsTable.PTable->Size != 0) {
                for(uint32_t i=0; i<SnsTable.PTable->Size; i++) {
                    Uart.Printf("ID=%u; Lvl=%u\r", SnsTable.PTable->Row[i].ID, SnsTable.PTable->Row[i].Level);
                }
                Uart.Printf("\r");
            }
            else Uart.Printf("Nobody there\r");
        } // if EVTMSK_TABLE_READY

        if(EvtMsk & EVTMASK_TIMER) {
            SnsTable.SendEvtReady();
        }
    } // while 1
}

#endif

#if 1 // ======================= Command processing ============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b;
//    FRESULT res;
    switch(CmdCode) {
        case 0x01:
            b = OK;
            Uart.Cmd(0x90, &b, 1);
            break;

//        case 0x51:  // GetID
//            Uart.Printf("ID=%u\r", rLevel1.GetID());
//            break;
//
//        case 0x52:  // SetID
//            b = PData[0];
//            res = f_open(&SD.File, "settings.ini", FA_CREATE_ALWAYS | FA_WRITE);
//            if(res == FR_OK) {
//                f_printf(&SD.File, "[Radio]\r\nID=%u\r\n", b);
//                f_close(&SD.File);
//                Uart.Printf("Written\r");
//            }
//            rLevel1.SetID(b);
//            Uart.Printf("New ID=%u\r", rLevel1.GetID());
//            break;

        default: break;
    } // switch
}
#endif

#if 1 // ============================ App class ================================
void App_t::Init() {
    chVTSet(&ITmr, MS2ST(999), MeasureTmrCallback, nullptr);
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    SnsTable.RegisterAppThd(PThd);
}
#endif
