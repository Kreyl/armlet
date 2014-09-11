/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: klNfcF0
 *
 * Created on May 27, 2011, 6:37 PM
 */

#include "ch.h"
#include "hal.h"

#include "kl_lib_f2xx.h"
#include "lcd2630.h"
#include "peripheral.h"
#include "kl_sd.h"
#include "sound.h"
#include "keys.h"
#include "infrared.h"
#include "cmd_uart.h"
#include "power.h"
#include "pill_mgr.h"

#include "radio_lvl1.h"
#include "mesh_lvl.h"

#include "ff.h"
#include "MassStorage.h"
#include "sd_log.h"

#include "flashloader_support.h"

#include "application.h"
#include "atlantis_music_tree.h"
#include "..\AtlGui\atlgui.h"

static inline void Init();
#define CLEAR_SCREEN_FOR_DEBUG
//#define UART_MESH_DEBUG

int main() {
    // Check if IWDG reset occured => power-off occured
    if(Iwdg.ResetOccured()) {
        // Setup key input
        PinSetupIn(KEY_PWRON_GPIO, KEY_PWRON_PIN, pudPullUp);
        // Wait a little
        for(volatile uint32_t i=0; i<2700; i++);
        // Check if key is pressed
        if(!PinIsSet(KEY_PWRON_GPIO, KEY_PWRON_PIN)) {
            // If not enough time passed, increase timer and enter standby
            if(!Power.BtnPressTimerDone()) {
                Power.IncreaseBtnPressTimer();
                Power.EnterStandby();
            }
        }
        else { // If key not pressed, enter sleep again
            Power.ResetBtnPressTimer();
            Power.EnterStandby();
        }
        // Time to wake
        Power.ResetBtnPressTimer();
        DisableBackupAccess();
    }

    // ==== Setup clock ====
    Clk.UpdateFreqValues();
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(12);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    Bootloader.TerminatorThd = chThdSelf();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    while(TRUE) {
        uint32_t EvtMsk;
        EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMSK_DFU_REQUEST) {
            Log.Shutdown();
            Usb.Shutdown();
            MassStorage.Reset();
            // execute boot
            __disable_irq();
            chSysLock();
            Clk.SwitchToHSI();
            SysTick->CTRL = 0;
            SCB->VTOR = 0x17FF0000;
            __enable_irq();
            boot_jump(SYSTEM_MEMORY_ADDR);
            while(1);
            chSysUnlock();
        }
//        Uart.Printf("\r_abW");
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("\rAtlantis   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);

    //in case uart not working 0
#if 0 //const_cast for defines test
#define DSTRING "GAGAGA"
#define DSTRING2 "GAGAGA2"
    char * str1 =const_cast<char *> ("GAGAGA");
    char * str2;
    str2= const_cast<char *> (DSTRING2);

    if(strcmp(str1,const_cast<char *> (DSTRING))==0)
        Uart.Printf("\r DSTRING  str %s OK\r",str1);
    if(strcmp(str2,const_cast<char *> (DSTRING2))==0)
        Uart.Printf("\r DSTRING2  str %s OK\r",str2);
#endif
#if 1
    SD.Init();

    Log.Init();
    // Read config
    SD.iniReadInt32("Radio", "id", "settings.ini", &App.SelfID);
    SD.iniReadInt32("Radio", "locationThreshold", "settings.ini", &App.locationThreshold);
    SD.iniReadInt32("Radio", "forestTheshold", "settings.ini", &App.forestTheshold);
    SD.iniReadInt32("Radio", "mistThreshold", "settings.ini", &App.mistThreshold);
    SD.iniReadInt32("Radio", "characterThreshold", "settings.ini", &App.characterThreshold);
    SD.iniReadInt32("Radio", "IRlevel", "settings.ini", &App.IRlevel);
    SD.iniReadInt32("Radio", "lodgeTheshold", "settings.ini", &App.lodgeTheshold);
    SD.iniReadInt32("Radio", "emotionFixTheshold", "settings.ini", &App.emotionFixTheshold);
    SD.iniReadInt32("Radio", "mobThreshold", "settings.ini", &App.mobThreshold);
    Uart.Printf("\rID=%u", App.SelfID);
    Log.Printf("ID=%u", App.SelfID);

    Lcd.Init();
    Lcd.Cls(clAtlBack);

    #ifndef CLEAR_SCREEN_FOR_DEBUG
    Lcd.Printf(11, 11, clGreen, clBlack, "Ostranna BBS Tx %u", ID);
    #endif

    Keys.Init();
    Beeper.Init();
    Vibro.Init();

    IR.RxInit();
    MassStorage.Init();
    Power.Init();

    Sound.Init();
    Sound.SetVolume(START_VOL_CONST);

    PillMgr.Init();
    Init_emotionTreeMusicNodeFiles_FromFileIterrator();
    App.Init();
    AtlGui.Init();

    Radio.Init();
    Mesh.Init();

    Uart.Printf("\rInit done");
    Log.Printf("Init done");
#endif
}
