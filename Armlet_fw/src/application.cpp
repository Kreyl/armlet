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

#include "evt_mask.h"
#include "kl_sd.h"
#include "sound.h"
#include "ff.h"
#include "../MusicProcessor/intention.h"

App_t App;

#if 1 // ================================ Time =================================
static void TimeTmrCallback(void *p);

class Time_t {
public:
    uint8_t H, M, S,S_total;
    VirtualTimer ITimer;
    void Reset() {
        chVTReset(&ITimer);
        chVTSet(&ITimer, MS2ST(1000), TimeTmrCallback, NULL);
    }
    void Init ()
    {
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
    chEvtSignalI(App.PThd, EVTMASK_NEWSECOND);
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
static Keylock_t Keylock;


// Timer callback
void KeylockTmrCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMASK_KEYLOCK);
    chSysUnlockFromIsr();
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
#define KEY_A   0
#define KEY_B   1
#define KEY_C   2
#define KEY_X   3
#define KEY_Y   4
#define KEY_Z   5
#define KEY_L   6
#define KEY_E   7
#define KEY_R   8

#if 0
#define KEY_A   Keys.Status[0]
#define KEY_B   Keys.Status[1]
#define KEY_C   Keys.Status[2]
#define KEY_X   Keys.Status[3]
#define KEY_Y   Keys.Status[4]
#define KEY_Z   Keys.Status[5]
#define KEY_L   Keys.Status[6]
#define KEY_E   Keys.Status[7]
#define KEY_R   Keys.Status[8]
#endif

static inline void KeyPressedHandler(uint8_t ID) {
    switch (ID) {
        case KEY_A:
            App.Loudness++;
            if(App.Loudness >= 255) App.Loudness = 255;
            Sound.SetVolume(App.Loudness);
            break;
        case KEY_B:
            App.Loudness--;
            if(App.Loudness <= 0) App.Loudness = 0;
            Sound.SetVolume(App.Loudness);
            break;
        case KEY_C:
            break;
        case KEY_X:
            break;
        case KEY_Y:
            break;
        case KEY_Z:
            break;
        case KEY_L:
            break;
        case KEY_E:
            if(App.IsPlay) {
                App.IsPlay = false;
                Lcd.Printf(58, 118, clYellow, clBlack, "PLAY");
                Sound.Stop();

            }
            else {
                App.IsPlay = true;
                Lcd.Printf(58, 118, clRed, clBlack, "STOP");
                Sound.Play(SD.Filename);
            }
            break;
        case KEY_R:
            uint32_t Len;
            FRESULT r;
            Sound.Stop();
            do {
                r = SD.GetNext();
                Len = strlen(SD.Filename);
                if(r == FR_NO_FILE) SD.GetFirst("/");
            } while (strcmp(&SD.Filename[Len-3], "mp3") != 0);
            if(App.IsPlay) Sound.Play(SD.Filename);
            break;

        default:
            break;
    }
}

static inline void KeysHandler() {
    Keylock.TimerReset();   // Reset timer as Any key pressed or released
    if(Keylock.Locked) {
//        // Just unlock, do not handle pressed keys
//        //if(Keys.AnyPressed()) Keylock.Unlock();   // Check if any key pressed, not released, not holded
//        if((KEYLOCKER1.State == ksPressed) and (KEYLOCKER2.State == ksPressed)) {
//            Keylock.Unlock();
//            Keylock.TimerSet();     // Start autolock timer
//            Vibro.Vibrate(ShortBrr);	//doesnt work?!
//        }
//    }
//    // Check if lock
//    else if((KEYLOCKER1.State == ksPressed) and (KEYLOCKER2.State == ksPressed)) {
//        Keylock.Lock();
    }
    // Not locked, not lock pressed
    else {
        for(uint8_t i=0; i<KEYS_CNT; i++) {
            if(Keys.Status[i].HasChanged) {
                if(Keys.Status[i].State == ksReleased) {
                }
                else KeyPressedHandler(i);
            }
        } // for
        Keylock.TimerSet(); // Start autolock timer
    } // if not locked
    Keys.Reset();
}
#endif


// ===========================app thread ===============================
static WORKING_AREA(waAppThread, 1024);
__attribute__((noreturn))
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    while(true) App.Task();
}
void App_t::Task() {
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    if(EvtMsk & EVTMASK_KEYS) {
        char Name[20];
        uint32_t Len = strlen(SD.Filename);
        if(FileLen > Len - 19) {
            PFileName = (uint8_t*)SD.Filename;
            FileLen = 0;
        }
        memcpy(Name, PFileName, 19);
        PFileName += 1;
        FileLen ++;
        Name[19] = '\0';
        Lcd.Printf(1, 62, clCyan, clBlack, "%S", Name);
        KeysHandler();
    }
    if(EvtMsk & EVTMASK_PLAY_ENDS) {
        uint32_t Len;
        FRESULT r;
        if(IsPlay) {
            do {
                r = SD.GetNext();
                Len = strlen(SD.Filename);
                if(r == FR_NO_FILE) SD.GetFirst("/");
            } while (strcmp(&SD.Filename[Len-3], "mp3") != 0);
            Sound.Play(SD.Filename);
        }
    }
}

void App_t::Init() {
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    Sound.RegisterAppThd(PThd);
    Time.Init();
    Time.Reset();
    IsPlay = false;
    Loudness = 180;
    PFileName = (uint8_t*)SD.Filename;
}


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
