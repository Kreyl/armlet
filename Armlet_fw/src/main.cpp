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
#include "pill.h"

#include "radio_lvl1.h"
#include "mesh_lvl.h"

#include "ff.h"

#include "application.h"
#include "atlantis_music_tree.h"
#include "..\AtlGui\atlgui.h"
static inline void Init();
#define CLEAR_SCREEN_FOR_DEBUG
//#define UART_MESH_DEBUG
#define UART_EMOTREE_DEBUG
int main() {
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
    Clk.SetupFlashLatency(16);  // Setup Flash Latency for clock in MHz
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/4 = 12 MHz core clock. APB1 & APB2 clock derive on AHB clock
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();
    Clk.LSIEnable();        // To allow RTC to run

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");
    while(TRUE) {
        chThdSleepMilliseconds(1000);
        //Sound.tmp(9);
        Uart.Printf("Pos: %u\r\n", Sound.GetPosition());

  //      Lcd.Printf(11, 21, clWhite, clBlack, "%ums   ", Mesh.GetAbsTimeMS());
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("Atlantis Armlet\r");

    SD.Init();
    // Read config
    uint32_t ID=0;
    iniReadUint32("Radio", "ID", "settings.ini", &ID);
    Uart.Printf("ID=%u\r", ID);

    Lcd.Init();
#ifndef CLEAR_SCREEN_FOR_DEBUG
    Lcd.Printf(11, 11, clGreen, clBlack, "Ostranna BBS Tx %u", ID);
#endif
//    Lcd.DrawBmpFile(0, 0, "splash.bmp");
//    Lcd.DrawBmpFile(100, 18, "GUI/intentions/enabled/X.bmp");

 //   Init_emotionTreeMusicNodeFiles_FromFile(filename);
 //   Print_emotionTreeMusicNodeFiles_ToUART();

//    Keys.Init();
//    Beeper.Init();
//    Vibro.Init();

//    IR.TxInit();
//    IR.RxInit();
//    Power.Init();
    //Power.Task();
//    PillInit();

//    Init_emotionTreeMusicNodeFiles_FromFileIterrator();

    Sound.Init();
    Sound.SetVolume(200);

    Sound.Play("fon-WhiteTower.mp3", 1000000);//"alive.wav");

//    App.Init();
//    AtlGui.Init();
//
//    rLevel1.Init(ID);
//    Mesh.Init(ID);
}
