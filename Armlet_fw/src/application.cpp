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
//#include "RxTable.h"
#include "..\AtlGui\atlgui.h"
#include "energy.h"

#include "flashloader_support.h"
#include "usb_f2.h"
#include "kl_lib_f2xx.h"

#include "sd_log.h"
#include "console.h"
#include "mesh_lvl.h"

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
    uint8_t rslt;
    KeyEvtInfo_t Evt;

    while((rslt = Keys.EvtBuf.Get(&Evt)) == OK) {
        // Debug
        Uart.Printf("\rKeyEvtType=%u; Keys: ", Evt.Type);
        for(uint8_t i=0; i<Evt.NKeys; i++) Uart.Printf("%u ", Evt.KeyID[i]);

        //drop suspend timer!
        //AtlGui.screen_suspend_timer=0;
        AtlGui.is_suspend_timer_run=false;
        AtlGui.screen_suspend_timer=0;
        if(AtlGui.is_screen_suspended)
        {
            AtlGui.TurnOnScreen();
            return;
        }
        if(Evt.Type==kePress) {
            // Beep/Vibrate if click succeeded
            if(AtlGui.ButtonIsClicked(Evt.KeyID[0])) {
//                Beeper.Beep(ShortBeep);
                Vibro.Vibrate(ShortBrr);
                Uart.Printf("\rAtlGui.ButtonIsClicked(Evt.KeyID[0]) TRUE ");
            }
            else
                Uart.Printf("\rAtlGui.ButtonIsClicked(Evt.KeyID[0]) FALSE ");
        }
        else if(Evt.Type==keRelease)
        {
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keRelease);
        }
        else if(Evt.Type==keLongPress)
        {
            if(AtlGui.is_locked && Evt.KeyID[0]!=6)//если не лок и залочена - вернуться
                    continue;
            if(Evt.KeyID[0] == KEY_PWRON)
            {
#ifdef                BRACELET_TEST_MODE_VALS
                App.DropData();
#endif
                //выключение
                App.SaveData();
                Log.Shutdown( );
                chThdSleepMilliseconds(250);
                AtlGui.ShowSplashscreen();

                chThdSleepMilliseconds(1000);
                Power.EnterStandby();
            }
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keLongPress);
        }
        else if(Evt.Type==keRepeat)
        {
            AtlGui.ButtonIsReleased(Evt.KeyID[0],keRepeat);
        }
        else if(Evt.Type==keCancel)
        {
            AtlGui.RenderFullScreen(AtlGui.current_state);
        }

        else if(Evt.Type==keCombo)
        {
            if(AtlGui.is_locked)//если не лок и залочена - вернуться
                    continue;

            AtlGui.ReasonFlag=false;
            //    void EnterStandby();
            //void Reset()
           // Программное включение, выключение (длинная А)+
            //, перезагрузка, с перепрошивкой, если есть файл на флэшке (ARX),
            //переход в бутлоадер (ABCXYZ),
            //сброс состояния персонажа с перезагрузкой (ABC Z LER)
               //if(Evt.NKeys==3 && Evt.KeyID[0]==1 && Evt.KeyID[1]==3 && Evt.KeyID[2]==4)//bcx
               if(Evt.NKeys==3 && Evt.KeyID[0]==keyA && Evt.KeyID[1]==keyX && Evt.KeyID[2]==keyR)//arx
               {
                   App.SaveData();
                   chThdSleepMilliseconds(250);
                   Uart.Printf(" GO REBOOT  %d\r", Evt.NKeys);
                   Log.Shutdown( );
                   chThdSleepMilliseconds(250);
                   AtlGui.ShowSplashscreen();
                   chThdSleepMilliseconds(1000);
                   Power.Reset();

               }
               if(
                  Evt.NKeys==6 &&
                  Evt.KeyID[0]==keyA && Evt.KeyID[1]==keyB && Evt.KeyID[2]==keyC &&
                  Evt.KeyID[3]==keyX && Evt.KeyID[4]==keyY && Evt.KeyID[5]==keyZ
               )//abc xyz
               {


                   Log.Shutdown( );
                   chThdSleepMilliseconds(250);
                   Uart.Printf(" GO BOOTLOADER %d\r", Evt.NKeys);
                   AtlGui.ShowSplashscreen();
                   chThdSleepMilliseconds(1000);
                   Bootloader.dfuJumpIn(wdg_ON);
               }
               if(
                  Evt.NKeys==7 &&
                  Evt.KeyID[0]==keyA && Evt.KeyID[1]==keyB && Evt.KeyID[2]==keyC &&
                  Evt.KeyID[3]==keyZ && Evt.KeyID[4]==keyL && Evt.KeyID[5]==keyE && Evt.KeyID[6]==keyR
               )//abc z ler
               {

                   App.DropData();
                   App.SaveData();
                   Uart.Printf(" GO REBOOT ( &DROP state!) %d\r", Evt.NKeys);
                   chThdSleepMilliseconds(250);
                 //  Bootloader.dfuJumpIn(wdg_ON);
                   //TODO drop character to initial here
                   Log.Shutdown( );
                   chThdSleepMilliseconds(250);
                   AtlGui.ShowSplashscreen();
                   chThdSleepMilliseconds(1000);
                   Power.Reset();
               }
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
}
#endif
#if 1 // ========================== OnPillConnect ==============================
static void OnPillConnect() {

}
#endif

// ================================= App thread ================================
static WORKING_AREA(waAppThread, 8192);
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
            if(SICD.is_global_stop_active && GSCS.stop_reason_type==gsHerInfo)
                GSCS.OnMusicStopHerInfo();
            else
            //играть музыку по текущей эмоции
            PlayNewEmo(SICD.last_played_emo,1);
        }
#if 1 //EVTMASK_RADIO on/off
        if(EvtMsk & EVTMSK_SENS_TABLE_READY) {

            //Uart.Printf("\r\nApp TabGet, s=%u, t=%u", RxTable.PTable->Size, chTimeNow());
            /*
        for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
            Uart.Printf("\r\nID=%u; Pwr=%u", RxTable.PTable->Row[i].ID, RxTable.PTable->Row[i].Level);
        }
*/
        UpdateState();//эта штука берет локацию из таблицы

            //а весь оставшийся код - из суммы таблиц

            Table_buff.Recalc_storage();
            //подстановка фиктивного фона!
            if(Table_buff.PTable->current_row_size<COPY_RX_TABLE_SZ-1)//0
            {
                Table_buff.PTable->current_row_size++; //1
                Table_buff.PTable->Row[Table_buff.PTable->current_row_size-1].ID=REASON_BG; //0
                Table_buff.PTable->Row[Table_buff.PTable->current_row_size-1].Level=this->locationThreshold+1;//100
                Table_buff.PTable->Row[Table_buff.PTable->current_row_size-1].Reason=0;
              //  Uart.Printf("\r No signals, add fon to incoming reasons!");

            }
//RxTable
            int val1= MIN(NUMBER_OF_REASONS, Table_buff.PTable->current_row_size);
            CurrentIntentionArraySize = val1;
            int j=0;
            //сбрасываем  human syupport
            for(int kl=0;kl<MAX_USER_INTENTIONS_ARRAY_SIZE;kl++)
                ArrayOfUserIntentions[kl].human_support_number=0;

            for(int i=0; i<val1; i++) {
                if(Table_buff.PTable->Row[i].ID >= NUMBER_OF_REASONS || (Table_buff.PTable->Row[i].ID < 0) /*|| (SnsTable.PTable->Row[i].Level < 70)*/ ) {
                    CurrentIntentionArraySize--;
                    continue;
                }
                //call reboot or reset
                if(Table_buff.PTable->Row[i].ID==REASON_REBOOT)
                    CallLustraReboot();
                if(Table_buff.PTable->Row[i].ID==REASON_RESET)
                    CallLustraReset();
                //init tuman
                //Uart.Printf("\rREASON_ON_TABLE: %d indx %d id %d",Table_buff.PTable->Row[i].Reason,i,Table_buff.PTable->Row[i].ID);
                if(Table_buff.PTable->Row[i].Reason ==(uint16_t)REASON_MSOURCE || Table_buff.PTable->Row[i].Reason== REASON_MPROJECT)
                    OnGetTumanMessage(App.SelfID);

                //location reduce75

                if((Table_buff.PTable->Row[i].ID>=LOCATION_ID_START && Table_buff.PTable->Row[i].ID<=LOCATIONS_ID_END))
                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->locationThreshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }

                else if((Table_buff.PTable->Row[i].ID>=FORESTA_ID_START && Table_buff.PTable->Row[i].ID<=FORESTA_ID_END) ||
                        (Table_buff.PTable->Row[i].ID>=FORESTBC_ID_START && Table_buff.PTable->Row[i].ID<=FORESTBC_ID_END))
                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->forestTheshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }
                else if(Table_buff.PTable->Row[i].ID>=EMOTION_FIX_ID_START && Table_buff.PTable->Row[i].ID<=EMOTION_FIX_ID_END)

                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->emotionFixTheshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }
                else if(Table_buff.PTable->Row[i].ID>=LODGE_ID_START && Table_buff.PTable->Row[i].ID<=LODGE_ID_END)

                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->lodgeTheshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }
                else if(Table_buff.PTable->Row[i].ID>=MOB_ID_START && Table_buff.PTable->Row[i].ID<=MOB_ID_END)

                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->mobThreshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }
                else if(Table_buff.PTable->Row[i].ID>=CHARACTER_ID_START && Table_buff.PTable->Row[i].ID<=CHARACTER_ID_END)
                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->characterThreshold);
                    if(zero_signal_incoming_cut(j)) continue;
                    //manipulate with power on people, 1 min to play, 20 min Silence begin
                    int rid=Table_buff.PTable->Row[i].ID;
                    //init age
                    int32_t curr_time_ms=Mesh.GetAbsTimeMS();

                    //Uart.Printf("\rMEET HUMAN rid %d",rid);
                    //init //TODO test it!!
                    if( reasons[rid].age==0)
                    {
                       // Uart.Printf("\rMEET HUMAN ZERO AGE rid %d",rid);
                        reasons[rid].age=curr_time_ms;
                    }
                   // uint32_t d1=curr_time_ms-reasons[rid].age;

                    //if d>20m-restart!
                    if(curr_time_ms-reasons[rid].age>CHARACTER_MS_DIFF_SILENCE)
                    {
                       // Uart.Printf("\rMEET HUMAN CHARACTER>_MS_DIFF_SILENCE rid %d",rid);
                        reasons[rid].age=curr_time_ms;
                    }
                    else if(curr_time_ms-reasons[rid].age>=CHARACTER_MS_DIFF_MEETING)
                    {
                       // Uart.Printf("\rMEET HUMAN CHARACTER>CHARACTER_MS_DIFF_MEETING rid %d",rid);
                       // ArrayOfIncomingIntentions[j].power512=0;
                        CurrentIntentionArraySize--;
                        continue;
                        //не делать силу 0,инорировать!
                    }
                   // Uart.Printf("\rMEET HUMAN AGE %u CT%uD1 %u POWER %d",reasons[rid].age,curr_time_ms,d1, ArrayOfIncomingIntentions[j].power512);
                    //manipulate with power on people, 1 min to play, 20 min Silence end
                }
                else if(Table_buff.PTable->Row[i].ID>=MIST_ID_START && Table_buff.PTable->Row[i].ID<=MIST_ID_END)
                {
                    ArrayOfIncomingIntentions[j].power512=recalc_signal_pw_thr(Table_buff.PTable->Row[i].Level,this->mistThreshold);
                    if(zero_signal_incoming_cut(j)) continue;
                }
                else
                ArrayOfIncomingIntentions[j].power512 = Table_buff.PTable->Row[i].Level/*-70*/;
                ArrayOfIncomingIntentions[j].reason_indx = Table_buff.PTable->Row[i].ID;
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
           // for(int ik=0;ik<CurrentIntentionArraySize;ik++)
            //    Uart.Printf("\rCURRENT_INCOMING_ARRAY_WEIGHTED rid %d, whgt%d, size %d",ArrayOfIncomingIntentions[ik].reason_indx,ArrayOfIncomingIntentions[ik].power512,CurrentIntentionArraySize);

            //добавляем массив игроцких резонов в общий
            PushPlayerReasonToArrayOfIntentions();
               // сюда - все массивы резонов с других источников!
            if(!SICD.is_global_stop_active)
            {
                //пересчитываем резоны
                int reason_id=MainCalculateReasons();

                if(reason_id!=-1 && reason_id!=-2 &&  reason_id!=-3) {
                    Uart.Printf("\rID to play=%d",reason_id);
#ifdef BRACELET_TEST_MODE_VALS
                    DrawReasonToPlay(reason_id);
#endif
                    //new reason to play!remade on absent files!!
                    if(GetRealEmoForEmoToPlay(reasons[reason_id].eID) != SICD.last_played_emo)
                    {
                        //check if it's user reason,if any, set already played flag
                        UserReasonFlagRecalc(SICD.last_intention_index_winner);//тут должэн стоять прошлый победитель!!
                        PlayNewEmo(reasons[reason_id].eID,3);
                        SICD.last_reason_active_armlet=reason_id;
                        //send to tail and redraw any not tailed active user intention
                        CheckUserIntentionsOnSwitchToTail(reason_id);
                    }
                }
                if(reason_id==-3) {
                    PlayNewEmo(0,4);
                }
            }
        }
#endif

#if 1 // ==== New second ====
        if(EvtMsk & EVTMSK_NEWSECOND) {

#ifndef SCREEN_SUSPEND_TIMER
            //таймер выключения крана неактивенпока работает драка!
            if(!(SICD.is_global_stop_active==true && GSCS.stop_reason_type==gsDraka))
                AtlGui.AddSuspendScreenTimer(1);
#endif

            //UPDATE user intentions timers
            if(!SICD.is_global_stop_active)
            {
                if(UpdateUserIntentionsTime(1))
                    CheckAndRedrawFinishedReasons();
            }
            else if(GSCS.OnNewSec())
                CheckAndRedrawFinishedReasons();
            //too rare to stop
            if(Time.S_total % SEC_TO_SELF_REDUCE ==0)
            {
                Energy.AddEnergy(-1);
              //  Uart.Printf("\renergy self reduced, stotal=%d",Time.S_total);
            }
#ifdef BRACELET_TEST_MODE_VALS
            if(Time.S_total==5)
            {

                Uart.Printf("\rTEST HERINfO");
                //тест зависимости
                Energy.SetEnergy(10);
                GSCS.BeginStopCalculations(gsHerInfo);
                //ArrayOfUserIntentions[SI_HER].TurnOn();
                //тестважныхлюдей
                //тесттумана
                //OnGetTumanMessage(App.SelfID);
            }
#endif
#ifndef BRACELET_TEST_MODE_VALS
            if(Time.S_total % 300 ==0)
            {
                App.SaveData();
                chThdSleepMilliseconds(250);
            }
#endif
            if(Time.S_total % 6 ==0)
            {
                AtlGui.RenderNameTimeBat();
                //CalculateIntentionsRadioChange();
                //PrintSCIDToUart();
                //Uart.Printf("every 4 sec\r");
                //Sound.SetVolume(0);
              ///  Sound.Stop();
            }
            if(on_run==0) {
                on_run=1;
                AtlGui.ShowSplashscreen();
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
                            Beeper.Beep(BeepPillOk);
                            OnPillConnect();
                            //1 - марихуана, 2 - лсд, 3 - героин
                            if(Pill.Type==PILLTYPEWEED)
                                ArrayOfUserIntentions[SI_WEED].TurnOn();
                            else if(Pill.Type==PILLTYPELSD)
                                ArrayOfUserIntentions[SI_LSD].TurnOn();
                            else if(Pill.Type==PILLTYPEKAKT)
                                ArrayOfUserIntentions[SI_KAKT].TurnOn();
                            else if(Pill.Type==PILLTYPEHER)
                                GSCS.BeginStopCalculations(gsHerInfo);
                                //ArrayOfUserIntentions[SI_HER].TurnOn();
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
    uint8_t LocationID = 0;
    uint16_t tmpID=0;
    for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
        tmpID = RxTable.PTable->Row[i].ID;
        if( tmpID<CHARACTER_ID_START || CHARACTER_ID_END>tmpID )    {
            if(RxTable.PTable->Row[i].Level > SignalPwr) {
                SignalPwr = RxTable.PTable->Row[i].Level;
                LocationID = tmpID;
            } // if Signal present
        } // if location correct
    }
    if(LocationID) CurrInfo.Location = LocationID;
    CurrInfo.Emotion=SICD.last_played_emo;
    CurrInfo.Reason=SICD.last_reason_active_armlet;
    CurrInfo.Energy=Energy.GetEnergy();

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
    char *S = nullptr;//character_HWilliams.csv
   // char *S = "character_HWilliams.csv";//character_HWilliams.csv
    SD.GetNthFileByPrefix(&CSVList,"character_", 0, &S);

//    if(SD.PrepareToReadDirs(&CSVList) == FR_OK) {
//        // Count files available
//        char *S = nullptr;
//        while(SD.GetNext(&S) == FR_OK)
    Table_buff.Init();
    Uart.Printf("\rCharacter file: %s", S);
    ParseCsvFileToEmotions(S);
    LoadCharacterSettings();
    InitArrayOfUserIntentions();
    InitButtonsToUserReasons();

    LoadData();
    //Uart.Printf("\r APP::INIt, stotal=%d",Time.S_total);
}

void App_t::SaveData()
{
    FIL file;
    int open_code= f_open (
            &file,
       "/state.ini",
       FA_WRITE
    );
    if(open_code!=0)
        return;
   // UINT bw;
   // int buff_size;
#if 0
//#0 is not, 1 is yes
#energy
50
#narcograss
0
#narcoher
0
#narcolsd
0
#narcoTrain
0
#nacroManiac
0
#isdefault
#endif
    //klfprintf
    //energy to buff
    //energy to file
    f_printf(&file,"#0 is not, 1 is yes");
    f_printf(&file,"\r\n#energy");
    f_printf(&file,"\r\n%d",Energy.GetEnergy());
    //weed, lambda welcome!
    f_printf(&file,"\r\n#narcograss");
    if(ArrayOfUserIntentions[SI_WEED].current_time>=0)
        f_printf(&file,"\r\n%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"\r\n%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"\r\n#narcoher");
    //если -2  - пишем windrawal time от !![2.)
    //если -1 пишем NARCO_IS_OFF_STATE
    //если >=0 - пишем windrawal time=2
    if(ArrayOfUserIntentions[SI_HER].current_time==-2)
    {
        if(ArrayOfUserIntentions[SI_WITHDRAWAL].current_time>2)
            f_printf(&file,"\r\n%d",ArrayOfUserIntentions[SI_WITHDRAWAL].current_time);
        else
            f_printf(&file,"\r\n%d",2);
    }
    else if(ArrayOfUserIntentions[SI_HER].current_time>=0)
        f_printf(&file,"\r\n%d",2);
    else //-1
        f_printf(&file,"\r\n%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"\r\n#narcoTrain");
    if(ArrayOfUserIntentions[SI_KRAYK].current_time==-2)
    {
        if(ArrayOfUserIntentions[SI_WITHDRAWAL].current_time>2)
            f_printf(&file,"\r\n%d",ArrayOfUserIntentions[SI_WITHDRAWAL].current_time);
        else
            f_printf(&file,"\r\n%d",2);
    }
    else if(ArrayOfUserIntentions[SI_KRAYK].current_time>=0)
        f_printf(&file,"\r\n%d",2);
    else //-1
        f_printf(&file,"\r\n%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"\r\n#nacroManiac");
    if(ArrayOfUserIntentions[SI_MANIAC].current_time==-2)
    {
        if(ArrayOfUserIntentions[SI_WITHDRAWAL].current_time>2)
            f_printf(&file,"\r\n%d",ArrayOfUserIntentions[SI_WITHDRAWAL].current_time);
        else
            f_printf(&file,"\r\n%d",2);
    }
    else if(ArrayOfUserIntentions[SI_MANIAC].current_time>=0)
        f_printf(&file,"\r\n%d",2);
    else //-1
        f_printf(&file,"\r\n%d",NARCO_IS_OFF_STATE);


    f_printf(&file,"\r\n#Default marker , 1 if default, 0 otherwise");
    if(Energy.is_default_cfg==true)
        f_printf(&file,"\r\n%d",NARCO_IS_ON_STATE);
     else
         f_printf(&file,"\r\n%d",NARCO_IS_OFF_STATE);
   // f_write(&file, DataFileBuff, buff_size, &bw);
    f_close(&file);
    Uart.Printf("\r!!!ArrayOfUserIntentions[SI_MANIAC].current_time%d",ArrayOfUserIntentions[SI_MANIAC].current_time);
    Uart.Printf("App_t::SaveData done");
}
void App_t::LoadCharacterSettings()
{
//    0 - нету
//    1 - сидит на траве
//    2 - сидит на ЛСД
//    3 - сидит на героине
//    4 - маньяк
//    5 - это Крайк
    //драка хранится напрямую в userintentions. power - ширина плато, от энергии +-1!
    //убийство хранится в userintentions   readyToKill = ширина плато*Energymoddecreased(default_energy)
    //убийство хранится в userintentions   readyToKill =Time ширина плато*Energymodincreased(default_energy)
    int32_t pwr,rk,rkt;
    SD.iniReadInt32("character", "fightPower", "settings.ini", &pwr);//draka
    SD.iniReadInt32("character", "readyToKillInSeconds", "settings.ini", &rk);
    SD.iniReadInt32("character", "readyToKillForMinutes", "settings.ini", &rkt);



    WriteDrakaTimeFromPower(pwr);//TODO test
    //Uart.Printf("\r PWR%u, RKT %u, RK %u, RKT*60 %u , drop%d",pwr,rkt,rk,rkt*60, ArrayOfUserIntentions[SI_FIGHT].time_on_plateau);
   // WriteReadyToKillTimer(rkt*60);//TODO test
    WriteFrontTime(rk,SI_MURDER);//TODO test
    WriteMidTime(rkt*60,SI_MURDER);//TODO test
    WriteTailTime(rkt*60/5,SI_MURDER);//TODO test
    int32_t addict;
    SD.iniReadInt32("character", "addiction", "settings.ini", &addict);
    if(addict==1)
            ArrayOfUserIntentions[SI_WEED].current_time=0;
    if(addict==2)
    {
            ArrayOfUserIntentions[SI_HER].current_time=-2;
            ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=0;
    }
    if(addict==3)
            ArrayOfUserIntentions[SI_LSD].current_time=0;
    if(addict==4)
    {
            ArrayOfUserIntentions[SI_MANIAC].current_time=-2;
            ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=0;
    }
    if(addict==5)
    {
            ArrayOfUserIntentions[SI_KRAYK].current_time=-2;
            ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=0;
    }
    Energy.SetEnergy(START_ENERGY);
}
void App_t::DropData()
{
    Energy.SetEnergy(START_ENERGY);
    Energy.is_default_cfg=true;
    int32_t addict;
    SD.iniReadInt32("character", "addiction", "settings.ini", &addict);
    if(addict==1)
        ArrayOfUserIntentions[SI_WEED].current_time=0;
    if(addict==2)
            ArrayOfUserIntentions[SI_HER].current_time=0;
    if(addict==3)
            ArrayOfUserIntentions[SI_LSD].current_time=0;
    if(addict==4)
            ArrayOfUserIntentions[SI_MANIAC].current_time=0;
    if(addict==5)
            ArrayOfUserIntentions[SI_KRAYK].current_time=0;
    ArrayOfUserIntentions[SI_WEED].current_time=-1;
    ArrayOfUserIntentions[SI_HER].current_time=-1;
    ArrayOfUserIntentions[SI_LSD].current_time=-1;
    ArrayOfUserIntentions[SI_KAKT].current_time=-1;
    ArrayOfUserIntentions[SI_KRAYK].current_time=-1;
    ArrayOfUserIntentions[SI_MANIAC].current_time=-1;
}
void App_t::LoadData()
{
    //TEST!!
    Energy.SetEnergy(START_ENERGY);
    FIL file;
       int open_code= f_open (
               &file,
          "/state.ini",
          FA_READ
       );
       if(open_code!=0)
           return;
       //till file end
       int line_num=0;
       int data_rdy[6];
       int st_en=START_ENERGY;
       while(f_gets(DataFileBuff, SD_STRING_SZ, &file) != nullptr)
       {
           if(strncmp(DataFileBuff,"#",1)==0)
               continue;
           else
               line_num++;
           int int_val;
           int_val=strtol(DataFileBuff,NULL,10);
           if(line_num==1)
               st_en=int_val;
//           if(line_num==1)
//           {
//               if(int_val==START_ENERGY)
//               {
//                   Uart.Printf("\rApp_t::LoadData()  Default energy found, loading stopped");
//                   return;
//               }
//               else
//                   Uart.Printf("\rApp_t::LoadData()   energy found %d, loading",int_val);
//               Energy.SetEnergy(int_val);
//           }

           //если -2  - пишем windrawal time от !![2.)
           //если -1 пишем NARCO_IS_OFF_STATE
           //если >=0 - пишем windrawal time=2
           if(line_num>=2 && line_num<=7)
           {
               if(int_val==NARCO_IS_ON_STATE)//weed
                   data_rdy[line_num-2]=0;
               else if(int_val==NARCO_IS_OFF_STATE)
                   data_rdy[line_num-2]=-1;
               else
                   data_rdy[line_num-2]=int_val;

               //ERROR
               if(data_rdy[line_num-2]<-2)
                   int_val=NARCO_IS_OFF_STATE;
           }
           //default!
           if( line_num==7)
           {
               if(data_rdy[5]==0)
               {
                   Uart.Printf("\rApp_t::LoadData()  Default marker found, loading stopped");
                   return;
               }
               else
                   Uart.Printf("\rApp_t::LoadData()   Default marker found negative %d, loading",int_val);
           }
           if( line_num==7)
           {
               ArrayOfUserIntentions[SI_WEED].current_time=data_rdy[0];
               if(data_rdy[1] == 0 || data_rdy[1]==-1)
                   ArrayOfUserIntentions[SI_HER].current_time=data_rdy[1];
               else
               {
                   ArrayOfUserIntentions[SI_HER].current_time=-2;
                   ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=data_rdy[1];
               }
               ArrayOfUserIntentions[SI_LSD].current_time=data_rdy[2];
               ArrayOfUserIntentions[SI_KAKT].current_time=-1;
              // ArrayOfUserIntentions[SI_KRAYK].current_time=data_rdy[3];
              // ArrayOfUserIntentions[SI_MANIAC].current_time=data_rdy[4];
               if(data_rdy[3] == 0 || data_rdy[3]==-1)
                   ArrayOfUserIntentions[SI_KRAYK].current_time=data_rdy[3];
               else
               {
                   ArrayOfUserIntentions[SI_KRAYK].current_time=-2;
                   ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=data_rdy[3];
               }
               if(data_rdy[4] == 0 || data_rdy[4]==-1)
                   ArrayOfUserIntentions[SI_MANIAC].current_time=data_rdy[4];
               else
               {
                   ArrayOfUserIntentions[SI_MANIAC].current_time=-2;
                   ArrayOfUserIntentions[SI_WITHDRAWAL].current_time=data_rdy[4];
               }
               Energy.SetEnergy(st_en);
           }
       }
       if(line_num!=7)
           Uart.Printf("\rApp_t::LoadData()  No line fore default marker - load default values ");

     //  UINT bw;
     //  int buff_size;
       //energy to buff
       //f_read(

       //energy to data

       f_close(&file);
       Uart.Printf("App_t::LoadData done");
}
void App_t::WriteInentionStringToData(char * int_name, int int_val, char * emo_name)
{
    int reason_id=-1;
    if(int_val<0)
        int_val=0;
    if(int_val>512)
        int_val=512;

    for(int i=0;i<NUMBER_OF_REASONS;i++)
        if(strcmp(reasons[i].name,int_name)==0)
        {reason_id=i;break;}

    int emo_id=-1;
    //emotions_number
    for(int i=0;i<NUMBER_OF_EMOTIONS;i++)
        if(strcmp(emotions[i].name,emo_name)==0)
            emo_id=i;

    if(reason_id>=0 && emo_id>=0)
    {
        reasons[reason_id].weight=int_val;
        reasons[reason_id].eID=emo_id;

        Uart.Printf("\r App_t::WriteInentionStringToData  connect, reason_name=%s, emo_name=%s ,r_id %d",int_name,emo_name,reason_id);
    }
    else
        Uart.Printf("\r App_t::WriteInentionStringToData Fail to connect, reason_name=%s, emo_name=%s ",int_name,emo_name);

}
uint8_t App_t::ParseCsvFileToEmotions(const char* filename)
{
  //  Uart.Printf("\rParseCsvFileToEmotions filename %S\r",filename);
    FRESULT rslt;
    FIL file;
        // Open file
        rslt = f_open(&file, filename, FA_READ+FA_OPEN_EXISTING);
        if(rslt != FR_OK) {
            if (rslt == FR_NO_FILE) Uart.Printf("\r%S: not found", filename);
            else Uart.Printf("\r%S: openFile error: %u", filename, rslt);
            return FAILURE;
        }
        // Check if zero file
        if(file.fsize == 0) {
            f_close(&file);
            Uart.Printf("\rEmpty file");
            return FAILURE;
        }
      //  strcpy(toiintstr,"aaaaaaa");
      //  strcpy(reasonstr,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      //  strcpy(emostr,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        //till file end

        while(f_gets(BuffStr, SD_STRING_SZ, &file) != nullptr)
        {

            if(strncmp(BuffStr,"#",1)==0)
                continue;

            int sep1=-1,sep2=-1;
            int strlen=0;
            TCHAR c,*p = BuffStr;

            for(int i=0;i<SD_STRING_SZ;i++)
            {
                c=BuffStr[i];
                if(c=='\n' || c=='\r' ||c== '\0')
                    break;
                else
                    strlen++;
                if(sep1==-1 && c==CSV_SEPARATOR_CHAR)
                    sep1=i;
                if(sep1!=-1 &&c==CSV_SEPARATOR_CHAR)
                    sep2=i;
            }
#if 1
            if(sep1==0 ||sep1==sep2-1||sep2==strlen-1)
                continue;
            strncpy(reasonstr,p,sep1);
            reasonstr[sep1]='\0';

           // Uart.Printf("\rParseCsvFileToEmotions strncpy2 sep1+1 !%d! sep2-sep1-1 !%d!",sep1+1,sep2-sep1-1);

            //ЭТА СТРОКА strcpy ВЫЗЫВАЕТ 20:35:40.623> stack overflow @idle если на входе character_HWilliams.csv
            //результат работы - нормальный.
            strncpy(toiintstr,p+sep1+1,sep2-sep1-1);
            toiintstr[sep2-sep1-1]='\0';


            strncpy(emostr,p+sep2+1,strlen-sep2-1);
            emostr[strlen-sep2-1]='\0';

            int int_val=-1;
            int_val=strtol(toiintstr,NULL,10);
            Uart.Printf("\rParseCsvFileToEmotions emoint !%d! ",int_val);
            WriteInentionStringToData(reasonstr,int_val,emostr);
            //WriteInentionStringToData(reasonstr,int_val*10,emostr);
#endif
           // Uart.Printf("\rParseCsvFileToEmotions sep1 %d sep2 %d eol !%d!",sep1,sep2,strlen);
#if 0
            //Uart.Printf("\rParseCsvFileToEmotions rstr !%s! toi !%s! emo !%s!",reasonstr,toiintstr,emostr);
            Uart.Printf("\rParseCsvFileToEmotions rstr !%s! ",reasonstr);
            Uart.Printf("\rParseCsvFileToEmotions toi !%s! ",toiintstr);
            Uart.Printf("\rParseCsvFileToEmotions emo !%s! ",emostr);
            Uart.Printf("\rParseCsvFileToEmotions emoint !%d! ",int_val);
#endif

        }

        Uart.Printf("\rParseCsvFileToEmotions end\r");
        f_close(&file);
      return OK;
}
bool  App_t::zero_signal_incoming_cut(int AOII_indx)
{
    if(ArrayOfIncomingIntentions[AOII_indx].power512<=0)
    {
        CurrentIntentionArraySize--;
        return true;
    }
    return false;
}
int App_t::recalc_signal_pw_thr(int pw,int thr)
{
    if(pw<0 || thr<0)
    {
        Uart.Printf("App_t::recalc_signal_pw_thr error: pw=%d,thr=%d",pw,thr);
        return 0;
    }
    if(thr>=100)
        return 0;
    if(thr==0)
        return pw;
    if(pw<thr)
        return 0;
    if(pw>100)
    {
        Uart.Printf("App_t::recalc_signal_pw_thr pw>100!, %d, thr %d",pw, thr );
        return 100;
    }
    return (pw-thr)*100/(100-thr);

}
void App_t::CallLustraReboot()
{
    if(Time.S_total<60)
        return;
    App.SaveData();
    chThdSleepMilliseconds(250);
    Uart.Printf("\r GO REBOOT  LUSTRA\r");
    Log.Shutdown( );
    chThdSleepMilliseconds(250);
    AtlGui.ShowSplashscreen();
    chThdSleepMilliseconds(1000);
    Power.Reset();

}
void App_t::CallLustraReset()
{
    if(Time.S_total<60)
        return;
    App.DropData();
    App.SaveData();
    Uart.Printf("\r GO REBOOT ( &DROP state!) LUSTRA \r");
    chThdSleepMilliseconds(250);
    Log.Shutdown( );
    chThdSleepMilliseconds(250);
    AtlGui.ShowSplashscreen();
    chThdSleepMilliseconds(1000);
    Power.Reset();
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

    else if(*PCmd->Name == '#') Uart.Ack(CMD_UNKNOWN);  // reply only #-started stuff
}
#endif

void TableSt_t::ParseTableToStorage()
{
    int val1= MIN((uint32_t)NUMBER_OF_REASONS, RxTable.PTable->Size);
    current_row_size=0;
    //цикл по входной таблице!
    for(int i=0;i<val1;i++)
    {

        if(current_row_size==COPY_RX_TABLE_SZ)//полностью заполненная таблица, надеюсь никогда не будет.
        {
            int id_found=-1;
            for(int mm=0;mm<current_row_size;mm++)
                if(this->Row[mm].Level<RxTable.PTable->Row[i].Level)
                {
                    id_found=mm;
                    break;
                }
            if(id_found>=0)
            {
                this->Row[id_found].ID=RxTable.PTable->Row[i].ID;
                this->Row[id_found].Level=RxTable.PTable->Row[i].Level;
                this->Row[id_found].Reason=RxTable.PTable->Row[i].State.Reason;
            }
            continue;
        }

       //notmal turn
       this->Row[i].ID=RxTable.PTable->Row[i].ID;
       this->Row[i].Level=RxTable.PTable->Row[i].Level;
       this->Row[i].Reason=RxTable.PTable->Row[i].State.Reason;
       current_row_size++;
    }
}
void RxTableSt_t::Recalc_storage()
{    //move id
    current_table_id++;
    if(current_table_id>=NUM_ROWS_COPY_RX_TABLE)
        current_table_id=0;
    RxStorageResult.current_row_size=0;
    //get data
    this->RxStorage[current_table_id].ParseTableToStorage();
    //calculate_summary
    for(int i=0;i<NUM_ROWS_COPY_RX_TABLE;i++)
        for( int j=0;j<RxStorage[i].current_row_size;j++)
        {
            int val_pos=RxStorageResult.current_row_size;
            for(int mm=0;mm<RxStorageResult.current_row_size;mm++)
                if(RxStorage[i].Row[j].ID==RxStorageResult.Row[mm].ID)
                {
                    val_pos=mm;
                    break;
                }
            if(val_pos<COPY_RX_TABLE_SZ)
            {
                if(val_pos==RxStorageResult.current_row_size)//новое поле
                {
                    RxStorageResult.current_row_size++;
                    memcpy(&(RxStorageResult.Row[val_pos]),&(RxStorage[i].Row[j]),sizeof(RowSt_t));
                }
                else if(RxStorageResult.Row[val_pos].Level<RxStorage[i].Row[j].Level)//если новое поле сильнее старого - писать новое
                    memcpy(&(RxStorageResult.Row[val_pos]),&(RxStorage[i].Row[j]),sizeof(RowSt_t));
            }
            else//если места нет - находим ближайший слабее и пишем поверх него??
            {
                for(int mm=0;mm<RxStorageResult.current_row_size;mm++)
                    if(RxStorageResult.Row[mm].Level<RxStorage[i].Row[j].Level)//если новое поле сильнее старого - писать новое
                         memcpy(&(RxStorageResult.Row[mm]),&(RxStorage[i].Row[j]),sizeof(RowSt_t));
            }
        }
}
void RxTableSt_t::Init(){
    for(int i=0;i<NUM_ROWS_COPY_RX_TABLE;i++)
        this->RxStorage[i].current_row_size=0;
    this->current_table_id=0;
    RxStorageResult.current_row_size=0;
    this->PTable =&RxStorageResult;
}
void App_t::CheckUserIntentionsOnSwitchToTail(int reason_id)
{
    bool is_need_redraw=false;
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
        if(ArrayOfUserIntentions[i].process_type==PROCESS_NORMAL && i!=SI_DEATH && i!=SI_FIGHT)
        if(ArrayOfUserIntentions[i].current_time>0)
        if(UIIsONTail(i)==true)
        {
            ArrayOfUserIntentions[i].OnChangedEmo(i);
            is_need_redraw=true;
            //пользовательское, не выключено, не в хвосте
            //move to tail, redraw
            Uart.Printf("\rCheckUserIntentionsOnSwitchToTail MOVED TO TAIL SI %d",i);
        }
    if(is_need_redraw)
        CheckAndRedrawFinishedReasons();

}
#endif
