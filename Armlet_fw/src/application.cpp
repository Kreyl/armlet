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
#define PILLTYPEHER 3
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
            if(Evt.KeyID[0] == KEY_PWRON)
            {
                //выключение
                App.SaveData();
                Log.Shutdown( );
                chThdSleepMilliseconds(250);
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
#if 1 //EVTMASK_RADIO on/off
        if(EvtMsk & EVTMSK_SENS_TABLE_READY) {
/*
            Uart.Printf("\r\nApp TabGet, s=%u, t=%u", RxTable.PTable->Size, chTimeNow());

        for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
            Uart.Printf("\r\nID=%u; Pwr=%u", RxTable.PTable->Row[i].ID, RxTable.PTable->Row[i].Level);
        }
*/
        UpdateLocation();

            int val1= MIN((uint32_t)reasons_number, RxTable.PTable->Size);
            CurrentIntentionArraySize = val1;
            int j=0;
            //сбрасываем  human syupport
            for(int kl=0;kl<MAX_USER_INTENTIONS_ARRAY_SIZE;kl++)
                ArrayOfUserIntentions[kl].human_support_number=0;

            for(int i=0; i<val1; i++) {
                if(RxTable.PTable->Row[i].ID >= reasons_number || (RxTable.PTable->Row[i].ID < 0) /*|| (SnsTable.PTable->Row[i].Level < 70)*/ ) {
                    CurrentIntentionArraySize--;
                    continue;
                }
                ArrayOfIncomingIntentions[j].power256 = RxTable.PTable->Row[i].Level/*-70*/;
                ArrayOfIncomingIntentions[j].reason_indx = RxTable.PTable->Row[i].ID;
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
            if(!SICD.is_global_stop_active)
            {
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
        }
#endif

#if 1 // ==== New second ====
        if(EvtMsk & EVTMSK_NEWSECOND) {

#ifndef SCREEN_SUSPEND_TIMER
            AtlGui.AddSuspendScreenTimer(1);
#endif

            //UPDATE user intentions timers
            if(!SICD.is_global_stop_active)
            {
                if(UpdateUserIntentionsTime(1))
                    CheckAndRedrawFinishedReasons();

                if(Time.S_total% SEC_TO_SELF_REDUCE ==0)
                    Energy.AddEnergy(-1);
            }
            else
                GSCS.OnNewSec();
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
                            else if(Pill.Type==PILLTYPEHER)
                                ArrayOfUserIntentions[SI_HER].TurnOn();
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
void App_t::UpdateLocation() {
    uint8_t SignalPwr = 0;
    uint8_t LocationID = 0;
    uint16_t tmpID=0;
    for(uint32_t i=0; i<RxTable.PTable->Size; i++) {
        tmpID = RxTable.PTable->Row[i].ID;
        if( (tmpID >= first_location_id && tmpID <= last_location_id) ||
            (tmpID >= first_emotion_fix_id && tmpID <= last_emotion_fix_id) )    {
            if(RxTable.PTable->Row[i].Level > SignalPwr) {
                SignalPwr = RxTable.PTable->Row[i].Level;
                LocationID = tmpID;
            } // if Signal present
        } // if location correct
    }
    if(LocationID) CurrInfo.Location = LocationID;
}
void App_t::Init() {
    PThd = chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
    RxTable.RegisterAppThd(PThd);
    Sound.RegisterAppThd(PThd);
    on_run=0;

#if UART_RX_ENABLED
    chVTSet(&TmrUartRx,    MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
#endif

    Time.Init();
    Time.Reset();
    ParseCsvFileToEmotions("character.csv");
    InitArrayOfUserIntentions();
    InitButtonsToUserReasons();
    LoadData();
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
#endif
    //klfprintf
    //energy to buff
    //energy to file
    f_printf(&file,"#energy");
    f_printf(&file,"%d",Energy.GetEnergy());
    //weed, lambda welcome!
    f_printf(&file,"#narcograss");
    if(ArrayOfUserIntentions[SI_WEED].current_time>=0)
        f_printf(&file,"%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"#narcoher");
    if(ArrayOfUserIntentions[SI_HER].current_time>=0)
        f_printf(&file,"%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"#narcolsd");
    if(ArrayOfUserIntentions[SI_LSD].current_time>=0)
        f_printf(&file,"%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"#narcoTrain");
    if(ArrayOfUserIntentions[SI_KRAYK].current_time>=0)
        f_printf(&file,"%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"%d",NARCO_IS_OFF_STATE);

    f_printf(&file,"#nacroManiac");
    if(ArrayOfUserIntentions[SI_MANIAC].current_time>=0)
        f_printf(&file,"%d",NARCO_IS_ON_STATE);
    else
        f_printf(&file,"%d",NARCO_IS_OFF_STATE);

   // f_write(&file, DataFileBuff, buff_size, &bw);
    f_close(&file);
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
    SD.iniReadInt32("character", "power", "settings.ini", &pwr);//draka
    SD.iniReadInt32("character", "readyToKill", "settings.ini", &rk);
    SD.iniReadInt32("character", "readyToKillTime", "settings.ini", &rkt);

    WriteDrakaTimeFromPower(pwr);//TODO test
    WriteReadyToKillTimer(rkt);//TODO test
    WriteRadyToKill(rk);//TODO test
    int32_t addict;
    SD.iniReadInt32("character", "addict", "settings.ini", &addict);
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
}
void App_t::DropData()
{
    Energy.SetEnergy(START_ENERGY);
    int32_t addict;
    SD.iniReadInt32("character", "addict", "settings.ini", &addict);
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
    ArrayOfUserIntentions[SI_KRAYK].current_time=-1;
    ArrayOfUserIntentions[SI_MANIAC].current_time=-1;
}
void App_t::LoadData()
{
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
       while(f_gets(DataFileBuff, SD_STRING_SZ, &file) != nullptr)
       {
           if(strncmp(DataFileBuff,"#",1)==0)
               continue;
           else
               line_num++;
           int int_val;
           int_val=strtol(DataFileBuff,NULL,10);
           if(line_num==1)
           {
               if(int_val==START_ENERGY)
               {
                   Uart.Printf("App_t::LoadData()  Default energy found, loading stopped");
                   return;
               }
               Energy.SetEnergy(int_val);
           }
           if(line_num==2 && int_val==1)//weed
               ArrayOfUserIntentions[SI_WEED].current_time=0;
           if(line_num==3 && int_val==1)//her
               ArrayOfUserIntentions[SI_HER].current_time=0;
           if(line_num==4 && int_val==1)//lsd
               ArrayOfUserIntentions[SI_LSD].current_time=0;
           if(line_num==5 && int_val==1)//krayk
               ArrayOfUserIntentions[SI_KRAYK].current_time=0;
           if(line_num==6 && int_val==1)//manyac
               ArrayOfUserIntentions[SI_MANIAC].current_time=0;
       }

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
    if(int_val>256)
        int_val=256;
    for(int i=0;i<reasons_number;i++)
        if(strcmp(reasons[i].name,int_name)==0)
        {reason_id=i;break;}
    int emo_id=-1;
    //emotions_number
    for(int i=0;i<emotions_number;i++)
        if(strcmp(emotions[i].name,emo_name)==0)
            emo_id=i;
    if(reason_id>=0 && emo_id>=0)
    {
        reasons[reason_id].weight=int_val;
        reasons[reason_id].eID=emo_id;
        Uart.Printf("\r App_t::WriteInentionStringToData  connect, reason_name=%s, emo_name=%s ",int_name,emo_name);
    }
    else
        Uart.Printf("\r App_t::WriteInentionStringToData Fail to connect, reason_name=%s, emo_name=%s ",int_name,emo_name);

}
uint8_t App_t::ParseCsvFileToEmotions(const char* filename)
{

    Uart.Printf("\rParseCsvFileToEmotions beg\r");
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
                if(c=='\n' || c=='\r' )
                    break;
                else
                    strlen++;
                if(sep1==-1 && c==CSV_SEPARATOR_CHAR)
                    sep1=i;
                if(sep1!=-1 &&c==CSV_SEPARATOR_CHAR)
                    sep2=i;
            }

            if(sep1==0 ||sep1==sep2-1||sep2==strlen-1)
                continue;
            strncpy(reasonstr,p,sep1);

            reasonstr[sep1]='\0';

            strncpy(toiintstr,p+sep1+1,sep2-sep1-1);
            toiintstr[sep2-sep1-1]='\0';

            strncpy(emostr,p+sep2+1,strlen-sep2-1);
            emostr[strlen-sep2-1]='\0';

            int int_val;
            int_val=strtol(toiintstr,NULL,10);
            WriteInentionStringToData(reasonstr,int_val,emostr);
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
        Bootloader.dfuJumpIn(wdg_ON);
    }

    else if(*PCmd->Name == '#') Uart.Ack(CMD_UNKNOWN);  // reply only #-started stuff
}
#endif
#endif
