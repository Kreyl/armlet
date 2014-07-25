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
#include "intention.h"
#include "RxTable.h"
#include "..\AtlGui\atlgui.h"
#include "energy.h"

#include "flashloader_support.h"
#include "usb_f2.h"
App_t App;

#if 1 // ============================ Timers ===================================
// Pill check
void TmrPillCheckCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_PILL_CHECK);
    chVTSetI(&App.TmrPillCheck, MS2ST(T_PILL_CHECK_MS), TmrPillCheckCallback, nullptr);
    chSysUnlockFromIsr();
}
#if UART_RX_ENABLED
// Uart Rx
void TmrUartRxCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThd, EVTMSK_UART_RX_POLL);
    chVTSetI(&App.TmrUartRx, MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#endif

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
    chEvtSignalI(App.PThd, EVTMSK_NEWSECOND);
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
    chSysLockFromIsr();
//    chEvtSignalI(App.PThd, EVTMASK_KEYLOCK);
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
static inline void KeysHandler() {
    uint8_t rslt;
    KeyEvtInfo_t Evt;

    while((rslt = Keys.EvtBuf.Get(&Evt)) == OK) {
        // Debug
        Uart.Printf("\rKeyEvtType=%u; Keys: ", Evt.Type);
        for(uint8_t i=0; i<Evt.NKeys; i++) Uart.Printf("%u ", Evt.KeyID[i]);

        if(Evt.Type==kePress) {
            // Beep/Vibrate if click succeeded
            if(AtlGui.ButtonIsClicked(Evt.KeyID[0])) {
//                Beeper.Beep(ShortBeep);
                Vibro.Vibrate(ShortBrr);
            }
        }
        else if(Evt.Type==keRelease)
        {
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keRelease);
        }
        else if(Evt.Type==keLongPress)
        {
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keLongPress);
        }
        else if(Evt.Type==keRepeat)
        {
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keRepeat);
        }

        //                if(Keys.Status[i].State == ksReleased) {
        //                    //KEY RELEASE
        //                    Uart.Printf(" !AtlGui.ButtonIsReleased\r");
        //                    AtlGui.ButtonIsReleased(i);
        //                    Uart.Printf(" !!!AtlGui.ButtonIsReleased\r");
        //                   // ArmletApi::OnButtonRelease(i);
        //                  Beeper.Beep(ShortBeep); //doesnt work?!
        //                }
        //                else {
        //                    Uart.Printf(" !AtlGui.ButtonIsClicked\r");
        //                    AtlGui.ButtonIsClicked(i);
        //                    Uart.Printf(" !!!AtlGui.ButtonIsClicked\r");
        //key debug info end
    }

//    Keylock.TimerReset();   // Reset timer as Any key pressed or released
//    if(Keylock.Locked) {
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
//    }
//    // Not locked, not lock pressed
//    else {
//        for(uint8_t i=0; i<KEYS_CNT; i++) {
//            if(Keys.Status[i].HasChanged) {

//                    //KEY PRESS
////                    Beeper.Beep(ShortBeep);
//    /*            	if(i==0)
//                	{
//                		Sound.Stop();
//                		Uart.Printf("sound stop");
//                	}
//                	if(i==1)
//                	{
//                		Sound.Play("church_bells.wav");
//                		Uart.Printf("church_bells.wav");
//                	}
//                	if(i==2)
//                	{
//                		Sound.Play("techview.wav");
//                		Uart.Printf("techview.wav");
//                	}
//                	*/
//                    Vibro.Vibrate(ShortBrr);
//                    //ArmletApi::OnButtonPress(i);
//                }
//            }
//        } // for
//        Keylock.TimerSet(); // Start autolock timer
//    } // if not locked
//    Keys.Reset();
}
#endif

// ================================= App thread ================================
static WORKING_AREA(waAppThread, 1024);
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
            Uart.Printf("\rApp PlayEnd");
            //играть музыку по текущей эмоции
            PlayNewEmo(SICD.last_played_emo,1);
        }
#if 0 //EVTMASK_RADIO on/off
        if(EvtMsk & EVTMSK_SENS_TABLE_READY) {
#ifdef       UART_MESH_DEBUG
        Uart.Printf("App TabGet, s=%u, t=%u\r", SnsTable.PTable->Size, chTimeNow());

        // FIXME: Lcd Clear before next ID print
        for(uint8_t i=0; i<5; i++) {
            Lcd.Printf(11, 31+(i*10), clRed, clBlack,"               ");
        }

        for(uint32_t i=0; i<SnsTable.PTable->Size; i++) {
            Uart.Printf(" ID=%u; Pwr=%u\r", SnsTable.PTable->Row[i].ID, SnsTable.PTable->Row[i].Level);
            Lcd.Printf(11, 31+(i*10), clRed, clBlack,"ID=%u; Pwr=%u", SnsTable.PTable->Row[i].ID, SnsTable.PTable->Row[i].Level);
        }
#endif
            //перекладываем данные с радио в массив текущих резонов
            int val1= MIN((uint32_t)reasons_number, SnsTable.PTable->Size);
            CurrentIntentionArraySize = val1;
            int j=0;
            //сбрасываем  human syupport
            for(int kl=0;kl<MAX_USER_INTENTIONS_ARRAY_SIZE;kl++)
                ArrayOfUserIntentions[kl].human_support_number=0;

            for(int i=0; i<val1; i++) {
                if(SnsTable.PTable->Row[i].ID >= reasons_number || (SnsTable.PTable->Row[i].ID < 0) /*|| (SnsTable.PTable->Row[i].Level < 70)*/ ) {
                    CurrentIntentionArraySize--;
                    continue;
                }
                ArrayOfIncomingIntentions[j].power256 = SnsTable.PTable->Row[i].Level/*-70*/;
                ArrayOfIncomingIntentions[j].reason_indx = SnsTable.PTable->Row[i].ID;
                //если входной резон пользователя - пользовательский, добавляем его в челподдержку
                for(int kk=0;kk<MAX_USER_INTENTIONS_ARRAY_SIZE;kk++)
                {
                    int PTABLE_REASON =-1;
                    if(PTABLE_REASON==ArrayOfUserIntentions[kk].reason_indx)
                    {
                        ArrayOfUserIntentions[kk].human_support_number++;
                        break;
                    }
                }

                j++;
            }
            //добавляем массив игроцких резонов в общий
            PushPlayerReasonToArrayOfIntentions();
               // сюда - все массивы резонов с других источников!

            //пересчитываем резоны
            int reason_id=MainCalculateReasons();

            if(reason_id!=-1 && reason_id!=-2 &&  reason_id!=-3) {
                Uart.Printf("\rID to play=%d",reason_id);
                //new reason to play!
                if(reasons[reason_id].eID != SICD.last_played_emo)
                {
                    //check if it's user reason,if any, set already played flag
                    UserReasonFlagRecalc(SICD.last_intention_index_winner);//тут должэн стоять прошлый победитель!!
                    PlayNewEmo(reasons[reason_id].eID,3);
                }
            }
            if(reason_id==-3) {
                PlayNewEmo(0,4);
            }
        }
#endif
#if 1 // ==== New second ====
        if(EvtMsk & EVTMSK_NEWSECOND) {
//            Uart.Printf("\rNewSecond");
// @KL           AtlGui.AddSuspendScreenTimer(1);
            //UPDATE user intentions timers
            if(UpdateUserIntentionsTime(1))
                CheckAndRedrawFinishedReasons();

            if(Time.S_total% SEC_TO_SELF_REDUCE ==0)
                Energy.AddEnergy(-1);
            if(Time.S_total % 6 ==0)
            {
                //CalculateIntentionsRadioChange();
                //PrintSCIDToUart();
                //Uart.Printf("every 4 sec\r");
                //Sound.SetVolume(0);
              ///  Sound.Stop();
            }
            if(on_run==0)
            {
                on_run=1;
                // включить сплеш скрин
                AtlGui.ShowSplashscreen();
    //            Sound.Play("church_bells.wav");
            }
            else if(AtlGui.is_splash_screen_onrun==1 )
            {
                //через секунду включить основной
                AtlGui.CallStateScreen(0);
                AtlGui.is_splash_screen_onrun=2;
            }
            //гашение крана
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
                            // Here is OnPillConnect
                        } // if rslt ok
                    } // if chargecnt > 0
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

void App_t::Init() {
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    RxTable.RegisterAppThd(PThd);
    Sound.RegisterAppThd(PThd);
    on_run=0;

#if UART_RX_ENABLED
    chVTSet(&TmrUartRx,    MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
#endif

    Time.Init();
    Time.Reset();
//    InitArrayOfUserIntentions();
}

#if 1 // ======================= Command processing ============================
#if UART_RX_ENABLED
void App_t::OnUartCmd(Cmd_t *PCmd) {
//    Uart.Printf("%S\r", PCmd->Name);
    uint32_t dw32 __attribute__((unused));  // May be unused in some cofigurations
    if(PCmd->NameIs("#Ping")) Uart.Ack(OK);

    else if(PCmd->NameIs("#Boot")) {
        Uart.Printf("\rDFU request");
        Bootloader.DFU_request();
    }

    else if(*PCmd->Name == '#') Uart.Ack(CMD_UNKNOWN);  // reply only #-started stuff
}
#endif
#endif
