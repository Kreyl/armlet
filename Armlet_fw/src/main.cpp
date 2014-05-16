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

#include "ff.h"

#include "application.h"
#include "atlantis_music_tree.h"

static inline void Init();

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
        chThdSleepMilliseconds(999);
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("Armlet Player\r");

    SD.Init();
    // Read config

    Lcd.Init();
    Lcd.Printf(10, 11, clGreen, clBlack, "ARMLET MP3 PLAYER");
    Lcd.Printf(124, 118, clYellow, clBlack, ">>>");
    Lcd.Printf(2, 118, clYellow, clBlack, "<<<");
    Lcd.Printf(58, 118, clYellow, clBlack, "PLAY");

    Keys.Init();
    Beeper.Init();
//    Vibro.Init();

//    IR.TxInit();
//    IR.RxInit();
//    Power.Init();
//    PillInit();

    Sound.Init();
    Sound.SetVolume(180);
//    Sound.Play("Beasty Boys - Sabotage.mp3");

    uint32_t Len;
    SD.GetFirst("/");
    do {
        SD.GetNext();
        Len = strlen(SD.Filename);
    } while (strcmp(&SD.Filename[Len-3], "mp3") != 0);

    App.Init();
}
