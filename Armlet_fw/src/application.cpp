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
#include "SensorTable.h"
#include "..\AtlGui\atlgui.h"
#include "energy.h"
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
static inline void KeysHandler() { // FIXME
    uint8_t rslt;
    KeyEvtInfo_t Evt;


    while((rslt = Keys.EvtBuf.Get(&Evt)) == OK) {
        //key debug info
        Uart.Printf("KeyEvtType=%u; Keys: ", Evt.Type);
        for(uint8_t i=0; i<Evt.NKeys; i++) Uart.Printf("%u ", Evt.KeyID[i]);
        Uart.Printf("\r\n");

        if(Evt.Type==kePress)
        {
            AtlGui.ButtonIsClicked(Evt.KeyID[0]);
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



// ===========================app thread ===============================
static WORKING_AREA(waAppThread, 1024);
__attribute__((noreturn))
static void AppThread(void *arg) {
    chRegSetThreadName("App");
    while(true) App.Task();
}
    /*
    uint32_t EvtMsk;
    int onrun=0;
    Uart.Printf("!!call  App_t::AppThread()\r");
    Keys.RegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
    Sound.RegisterEvtPlayEnd(&EvtListenerSound,EVTMASK_PLAY_ENDS);
    while(true) {
        EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMASK_KEYS)
        	{
        	 Uart.Printf("!!KeysHandler called  App_t::AppThread()\r");
        	KeysHandler();
        	}
        if(EvtMsk & EVTMASK_RADIO) {
        //   Uart.Printf("!!EVTMASK_RADIO called  App_t::AppThread() %d\r", RLvl2.PTable->RowCnt);
           Uart.Printf("!!EVTMASK_RADIO called  App_t::AppThread() %d\r", SnsTable.PTable->Size);
           // continue;
            int val1= MIN((uint32_t)reasons_number, SnsTable.PTable->Size);

            //copy to ArrayOfIncomingIntentions
            CurrentIntentionArraySize=val1;
            for(int i=0;i<val1;i++)
            {
                ArrayOfIncomingIntentions[i].power256=SnsTable.PTable->Row[i].Level;
                ArrayOfIncomingIntentions[i].reason_indx=SnsTable.PTable->Row[i].ID;
                Uart.Printf("radio_in int_id %d, int_pw %d, val1 %d\r",ArrayOfIncomingIntentions[i].reason_indx,ArrayOfIncomingIntentions[i].power256,val1);
            }

            Uart.Printf("radio incoming ends!!!\r");
            int reason_id=MainCalculateReasons();
            if(reason_id==-1)
                //играть фон
            {
                strcpy(appbufftmp,GetFileNameToPlayFromEmoId(0));
                Sound.Play(appbufftmp);
                Uart.Printf(appbufftmp);
                Uart.Printf("\r");
            }
            else
                //играть музыку по резону, если у нас всё еще тот же победитель - не трогать музыку.
            if(reason_id!=-1 && reason_id!=-2 &&  reason_id!=-3)
            {
                Uart.Printf("REASON to play %d\r",reason_id);
                strcpy(appbufftmp,GetFileNameToPlayFromEmoId(reasons[reason_id].eID));
                Sound.Play(appbufftmp);
                Uart.Printf(appbufftmp);
                Uart.Printf("\r");
            }
            if(reason_id==-3)
            {
                strcpy(appbufftmp,GetFileNameToPlayFromEmoId(0));
                Sound.Play(appbufftmp);
                Uart.Printf(appbufftmp);
                Uart.Printf("\r");
            }

         //   CalculateIntentionsRadioChange();
           // int chmaxval=-1;
           // int chmax_indx=-1;
           // for(int i=0;i<val1;i++)
           // {
              //  if(chmaxval<


           // }

        }

        if(EvtMsk & EVTMASK_PLAY_ENDS) {
            //пересчитываем суммарные резоны
            // по резону победителю считаем  новую эмоцию и включаем рандомный трек из неё.
            Uart.Printf("music ends!!!\r");
            int reason_id=MainCalculateReasons();
            if(reason_id==-1)
                //играть фон
            {
                strcpy(appbufftmp,GetFileNameToPlayFromEmoId(0));
                Sound.Play(appbufftmp);
                Uart.Printf(appbufftmp);
                Uart.Printf("\r");
            }
            else
                //играть музыку по резону
            {
                strcpy(appbufftmp,GetFileNameToPlayFromEmoId(reasons[reason_id].eID));
                Sound.Play(appbufftmp);
                Uart.Printf(appbufftmp);
                Uart.Printf("\r");
            }

        	//int rval=GetRandomEmoToPlay();
        	//strcpy(appbufftmp,GetFileNameToPlayFromEmoId(rval));
        	// Sound.Play(appbufftmp);
        	// Uart.Printf(appbufftmp);
        	//Uart.Printf("sound track ended");
        	//Uart.Printf("music ends!!!\r");
        }
        if(EvtMsk & EVTMASK_NEWSECOND) {
        	// Uart.Printf("New_second!");

        	if(Time.S_total % 4 ==0)
        	{
        		//CalculateIntentionsRadioChange();
        		//PrintSCIDToUart();
        		//Uart.Printf("every 4 sec\r");
        	}
        	 if(onrun==0)
        	 {
        		 onrun=1;
        		 Sound.Play("church_bells.wav");
        		 Uart.Printf("church_bells.wav");
        	 }
        	// int rval=GetRandomEmoToPlay();
        	// GetFileNameToPlayFromEmoId(rval);
        	// Uart.Printf("!!App_t::AppThread() random id=%d, name = %s \r",
        	//		 rval,emotions[rval].name );//,GetFileNameToPlayFromEmoId(rval));
            /// Time.DecreaseSecond();
            // if(Time.IsZero()) App.StopEverything();
            // else Interface.ShowTimeNow();
       	}

    }//while 1
}
        // Keys
        if(EvtMsk & EVTMSK_KEY_START)            KeyStart();
        if(App.State == asIdle) {
           if(EvtMsk & EVTMSK_KEY_TIME_UP)      KeyTimeUp();
            if(EvtMsk & EVTMSK_KEY_TIME_DOWN)    KeyTimeDown();
            if(EvtMsk & EVTMSK_KEY_CURRENT_UP)   KeyCurrentUp();
            if(EvtMsk & EVTMSK_KEY_CURRENT_DOWN) KeyCurrentDown();
        }

        // Time
        if(EvtMsk & EVTMSK_NEWSECOND) {
            Time.DecreaseSecond();
            if(Time.IsZero()) App.StopEverything();
            else Interface.ShowTimeNow();
        }

        // Measurement
        if(EvtMsk & EVTMSK_MEASURE_TIME) Measure.StartMeasurement();
        if(EvtMsk & EVTMSK_MEASUREMENT_DONE) {
            Interface.DisplayBattery();
            if(App.State == asCurrent) {
                Interface.DisplayCurrentMeasured();
                // Check if overcurrent (current regulation loop failure)
                int32_t tmp = Measure.GetResult(CURRENT_CHNL);
                tmp = Current.Adc2uA(tmp);
                if((tmp - Current.uA) > 504) {
                    App.StopEverything();
                    Interface.ShowFailure();
                }
            }

       // }
    //} // while 1
*/

void App_t::Task() {
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    if(EvtMsk & EVTMASK_KEYS) {
        Uart.Printf("App Keys\r");
        KeysHandler();
    }
    if(EvtMsk & EVTMASK_PLAY_ENDS) {
        Uart.Printf("App PlayEnd\r");
        //играть музыку по текущей эмоции
        PlayNewEmo(SICD.last_played_emo,1);
    }
#if 1 //EVTMASK_RADIO on/off
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
            Uart.Printf("ID to play=%d\r",reason_id);
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
    if(EvtMsk & EVTMASK_NEWSECOND) {
       //  Uart.Printf("New_second!");
        AtlGui.AddSuspendScreenTimer(1);
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
//            Uart.Printf("church_bells.wav");
        }
        else if(AtlGui.is_splash_screen_onrun==1 )
        {
            //черезсекундувключить основной
            AtlGui.CallStateScreen(0);
            AtlGui.is_splash_screen_onrun=2;
        }
        //гашение крана

    }
#endif
}

void App_t::Init() {
    State = asIdle;
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    SnsTable.RegisterAppThd(PThd);
    Sound.RegisterAppThd(PThd);
    on_run=0;

    Time.Init();
    Time.Reset();
//    InitArrayOfUserIntentions();
}


#if 1 // ======================= Command processing ============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b;
    switch(CmdCode) {
        case 0x01:
            b = OK;
            Uart.Cmd(0x90, &b, 1);
            break;
        default: break;
    } // switch
}
#endif
