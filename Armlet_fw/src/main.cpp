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

#include "flashloader_support.h"

#include "application.h"
#include "atlantis_music_tree.h"
#include "..\AtlGui\atlgui.h"

static inline void Init();
#define CLEAR_SCREEN_FOR_DEBUG
//#define UART_MESH_DEBUG
#define UART_EMOTREE_DEBUG
int main() {
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
    Clk.LSIEnable();        // To allow RTC to run //FIXME is it required?

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    while(TRUE) {
        chThdSleepMilliseconds(2700);
//        Uart.Printf("\r_abW");
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("\rAtlantis   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
//    Uart.Printf("\r%X", clAtlFront);


    SD.Init();
    // Read config
    iniReadUint32("Radio", "ID", "settings.ini", &App.ID);
    Uart.Printf("\rID=%u", App.ID);

    Lcd.Init();
    Lcd.Cls(clAtlBack);

    #ifndef CLEAR_SCREEN_FOR_DEBUG
    Lcd.Printf(11, 11, clGreen, clBlack, "Ostranna BBS Tx %u", ID);
#endif
//    Lcd.DrawBmpFile(0, 0, "splash.bmp");
//    Lcd.DrawBmpFile(100, 18, "GUI/intentions/enabled/X.bmp");

 //   Init_emotionTreeMusicNodeFiles_FromFile(filename);
 //   Print_emotionTreeMusicNodeFiles_ToUART();

    Keys.Init();
    Beeper.Init();
//    Vibro.Init();

//    IR.TxInit();
//    IR.RxInit();
    MassStorage.Init();
    Power.Init();



    Sound.Init();
    Sound.SetVolume(START_VOL_CONST);

//    PillMgr.Init();
//    Sound.Play("fon-WhiteTower.mp3", 1000000);//"alive.wav");
#if 1
    Init_emotionTreeMusicNodeFiles_FromFileIterrator();
    App.Init();
    AtlGui.Init();

    rLevel1.Init(App.ID);
    Mesh.Init(App.ID);

<<<<<<< HEAD
    Uart.Printf("\rInit done");
=======
    Uart.Printf("mAIN INIT DONE\r");
#endif
>>>>>>> 6defb1ce2e15b926a2cc70d158a1275fbdc3db3a
}
