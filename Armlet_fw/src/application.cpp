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
#include "pill_mgr.h"
#include "infrared.h"
#include "power.h"

#include "evt_mask.h"
#include "kl_sd.h"
#include "sound.h"
#include "ff.h"

#include "flashloader_support.h"
#include "usb_f2.h"
#include "kl_lib_f2xx.h"

#include "console.h"
#include "mesh_lvl.h"
#include "reasons.h"

App_t App;
//#define SCREEN_SUSPEND_TIMER



#define PILLTYPEWEED 1
#define PILLTYPELSD 2
#define PILLTYPEKAKT 3
#define PILLTYPEHER 4
#ifndef BRACELET_TEST_MODE_VALS
#define CHARACTER_MS_DIFF_SILENCE 1200000
#define CHARACTER_MS_DIFF_MEETING   60000
#endif
#ifdef BRACELET_TEST_MODE_VALS
#define CHARACTER_MS_DIFF_SILENCE   30000
#define CHARACTER_MS_DIFF_MEETING   10000
#endif
//============================csv dirs==========================================
static const char* FDirList2[] = {
        "/",
};

MusList_t CSVList = {
        sizeof(FDirList2)/sizeof(FDirList2[0]),
        0,
        FDirList2
};
#if 1 // ============================ Timers ===================================
// Pill check
void TmrPillCheckCallback(void *p) {
    chSysLockFromIsr();
    if(App.PThd != nullptr) chEvtSignalI(App.PThd, EVTMSK_PILL_CHECK);
    chVTSetI(&App.TmrPillCheck, MS2ST(T_PILL_CHECK_MS), TmrPillCheckCallback, nullptr);
    chSysUnlockFromIsr();
}
#if UART_RX_ENABLED
// Uart Rx
void TmrUartRxCallback(void *p) {
    chSysLockFromIsr();
    if(App.PThd != nullptr) chEvtSignalI(App.PThd, EVTMSK_UART_RX_POLL);
    chVTSetI(&App.TmrUartRx, MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#endif

#if 1 // ================================ Time =================================
static void TimeTmrCallback(void *p);

class Time_t {
public:
    uint8_t H, M, S;
    int S_total;
    VirtualTimer ITimer;
    void Reset() {
        chVTReset(&ITimer);
        chVTSet(&ITimer, MS2ST(1000), TimeTmrCallback, NULL);
    }
    void Init () {
        H = 0;
        M = 0;
        S = 0;
        S_total = 0;
    }
};
static Time_t Time;

void TimeTmrCallback(void *p) {
	//Uart.Printf("!!call  TimeTmrCallback()\r");
    chSysLockFromIsr();
    chVTSetI(&Time.ITimer, MS2ST(1000), TimeTmrCallback, NULL);
    if(++Time.S == 60) {
        Time.S = 0;
        if(++Time.M == 60) {
            Time.M = 0;
            if(++Time.H == 24) Time.H = 0;
        }
    }
    Time.S_total++;
    if(App.PThd != nullptr) chEvtSignalI(App.PThd, EVTMSK_NEWSECOND);
    chSysUnlockFromIsr();
}
#endif

#if 1 // ========================== Keys lock / unlock =========================
#define KEYLOCKER1              KEY_X   // }
#define KEYLOCKER2              KEY_Z   // } Keys locking

static void KeylockTmrCallback(void *p);
#define AUTOLOCK_TIMEOUT_MS     18000
class Keylock_t {
private:
    VirtualTimer VTimer;
public:
    bool Locked;
    void TimerSet() { chVTSet(&VTimer, MS2ST(AUTOLOCK_TIMEOUT_MS), KeylockTmrCallback, NULL); }
    void TimerReset() { chVTReset(&VTimer); }
    void Lock();
    void Unlock();
};
//static Keylock_t Keylock; // FIXME


// Timer callback
void KeylockTmrCallback(void *p) {  // FIXME: is it still in use?
//    chSysLockFromIsr();
//    chEvtSignalI(App.PThd, EVTMASK_KEYLOCK);
//    chSysUnlockFromIsr();
}

void Keylock_t::Lock() {
    Locked = true;
    Uart.Printf("Lock\r");
    Lcd.Shutdown();
}

void Keylock_t::Unlock() {
    Locked = false;
    Uart.Printf("Unlock\r");
    Lcd.Init();
    // Redraw display
   // LcdHasChanged = true;
}
#endif

#if 1 // =========================== Key handler ===============================
static inline void KeysHandler() {
//    uint8_t rslt;
//    KeyEvtInfo_t Evt;
//    while((rslt = Keys.EvtBuf.Get(&Evt)) == OK) {}
}
#endif

// ================================= App thread ================================
static WORKING_AREA(waAppThread, 16384);
__attribute__((noreturn))
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    App.Task();
}

__attribute__((__noreturn__))
void App_t::Task() {
    uint32_t EvtMsk;
    bool PillWasConnected = false;
    while(true) {
        EvtMsk = chEvtWaitAny(ALL_EVENTS);

        if(EvtMsk & EVTMSK_KEYS) {
//            Uart.Printf("\rApp Keys");

            KeysHandler();
        }

        if(EvtMsk & EVTMASK_PLAY_ENDS) {
            	Uart.Printf("\rPlayAnother");
            }
#if 1 //EVTMASK_RADIO on/off
        if(EvtMsk & EVTMSK_SENS_TABLE_READY) {
            Uart.Printf("App TabGet, s=%u, t=%u\r\n", RxTable.PTable->Size, chTimeNow());
/*
            for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
                Uart.Printf("\r\nID=%u; Pwr=%u", RxTable.PTable->Row[i].ID, RxTable.PTable->Row[i].Level);
            }
*/
            UpdateState();//эта штука берет локацию из таблицы
        }
#endif

#if 1 // ==== New second ====
        if(EvtMsk & EVTMSK_NEWSECOND) {
        } // New second
#endif
#if 1   // ==== Check pill ====
        if(EvtMsk & EVTMSK_PILL_CHECK) {
            bool IsNowConnected = (PillMgr.CheckIfConnected(PILL_I2C_ADDR) == OK);
            if(IsNowConnected and !PillWasConnected) {  // OnConnect
                PillWasConnected = true;
                // Read Charge Count
                uint8_t rslt = PillMgr.Read(PILL_I2C_ADDR, PILL_START_ADDR, &Pill, sizeof(Pill_t));
                if(rslt == OK) {
                    Uart.Printf("\rPill: %d %d", Pill.Type, Pill.ChargeCnt);
                    if(Pill.ChargeCnt > 0) {    // Check charge count, decrease it and write it back
                        Pill.ChargeCnt--;
                        rslt = PillMgr.Write(PILL_I2C_ADDR, (PILL_START_ADDR + PILL_CHARGECNT_ADDR), &Pill.ChargeCnt, sizeof(Pill.ChargeCnt));
                        if(rslt == OK) {
                            Uart.Printf("\rConnect: %d", Pill.ChargeCnt);
                            Beeper.Beep(BeepPillOk);
                        } // if rslt ok
                        else Beeper.Beep(BeepPillBad);  // Pill write failed
                    } // if chargecnt > 0
                    else Beeper.Beep(BeepPillBad); // ChargeCnt == 0
                } // if rslt ok
            } // OnConnect
            else if(!IsNowConnected and PillWasConnected) PillWasConnected = false;
        } // if EVTMSK_PILL_CHECK
#endif

#if 1 // ==== New battery state ====
        if(EvtMsk & EVTMSK_NEW_POWER_STATE) {
            Lcd.DrawBatteryState();
        }
#endif

#if 1 // == Uart Rx ==
#if UART_RX_ENABLED
        if(EvtMsk & EVTMSK_UART_RX_POLL) {
            Uart.PollRx(); // Check if new cmd received
        }
#endif
#endif
    } // while true
}
void App_t::UpdateState() {
    uint8_t SignalPwr = 0;
    uint16_t LocationID = 0;
    uint16_t tmpID=0;
    for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
        tmpID = RxTable.PTable->Row[i].ID;
        if(tmpID < LOCATION_ID_START || LOCATIONS_ID_END > tmpID )    {
            if(RxTable.PTable->Row[i].Level > SignalPwr) {
                SignalPwr = RxTable.PTable->Row[i].Level;
                LocationID = tmpID;
            } // if Signal present
        } // if location correct
    }
    if(LocationID) {
        CurrInfo.Location = LocationID;
        LocationValid();
    }
    else LocationInvalid();
}
void App_t::Init() {
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    Sound.RegisterAppThd(PThd);
    on_run=0;

#if UART_RX_ENABLED
    chVTSet(&TmrUartRx,    MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
#endif

    Time.Init();
    Time.Reset();
}

#if 1 // ======================= Command processing ============================
#if UART_RX_ENABLED
void App_t::OnUartCmd(Cmd_t *PCmd) {
//    Uart.Printf("%S\r", PCmd->Name);
    uint32_t dw32 __attribute__((unused));  // May be unused in some cofigurations
    if(PCmd->NameIs("#Ping")) {
        Uart.Ack(OK);
    }

#if 1 // Mesh
    else if(PCmd->NameIs("#SetMeshCycle")) {
        uint32_t NewCycle;
        if(PCmd->TryConvertTokenToNumber(&NewCycle) == OK) {  // Next token is number
            Console.SetTime_Ack(Mesh.SetNewAbsCycleN(NewCycle));
        }
    }

    else if(PCmd->NameIs("#GetMeshInfo")) {
        Console.GetMeshInfo_Ack(OK);
    }
#endif


    else if(PCmd->NameIs("#Boot")) {
        Uart.Printf("\rDFU request");
        // Setup TIMER
        Bootloader.dfuJumpIn(wdg_ON);
    }

    // TODO: debug only
    else if(PCmd->NameIs("#Splash")) {
    	Uart.Printf("\rManually splash");
    	Sound.Play("splash.mp3");
    }

    else if(*PCmd->Name == '#') Uart.Ack(CMD_UNKNOWN);  // reply only #-started stuff
}
#endif

#endif
