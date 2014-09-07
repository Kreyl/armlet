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

#include "BeepSequences.h"
#include "VibroSequences.h"

#include "application.h"
#include "atlantis_music_tree.h"
#include "..\AtlGui\atlgui.h"

extern SDCDriver SDCD1;

static inline void Init();

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

    bool IsPressed = false;
    while(TRUE) {
        chThdSleepMilliseconds(99);
        if(!PinIsSet(KeyData[keyA].PGpio, KeyData[keyA].Pin) and !IsPressed) {
            IsPressed = true;
            Sound.Play("moon.mp3");
        }
        else if(PinIsSet(KeyData[keyA].PGpio, KeyData[keyA].Pin) and IsPressed) {
            IsPressed = false;
        }
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("\rTesting   AHB freq=%uMHz", Clk.AHBFreqHz/1000000);
    Lcd.Init();
    Lcd.Printf(0, 0, clGreen, clBlack, "Testing Firmware");

    // ==== SD ====
    SD.Init();
    if(SD.IsReady) Lcd.Printf(0, 10, clGreen, clBlack, "SD capacity: %u", SDCD1.capacity);
    else Lcd.Printf(0, 10, clRed, clBlack, "SD Init Failure");

    // ==== Beeper & Vibro ====
    Beeper.Init();
    Vibro.Init();
    Lcd.Printf(0, 20, clGreen, clBlack, "Beep & Vibrate");
    Beeper.Beep(BeepPillOk);
    Vibro.Vibrate(BrrBrr);
    chThdSleepMilliseconds(450);
    Vibro.Vibrate(BrrBrr);
    chThdSleepMilliseconds(450);

// ==== Keys ====
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KeyData[i].PGpio, KeyData[i].Pin, pudPullUp);
#if 1
    Lcd.Printf(0, 30, clYellow, clBlack, "Press A  ");
    while(PinIsSet(KeyData[keyA].PGpio, KeyData[keyA].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press B  ");
    while(PinIsSet(KeyData[keyB].PGpio, KeyData[keyB].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press C  ");
    while(PinIsSet(KeyData[keyC].PGpio, KeyData[keyC].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press X  ");
    while(PinIsSet(KeyData[keyX].PGpio, KeyData[keyX].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press Y  ");
    while(PinIsSet(KeyData[keyY].PGpio, KeyData[keyY].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press Z  ");
    while(PinIsSet(KeyData[keyZ].PGpio, KeyData[keyZ].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press L  ");
    while(PinIsSet(KeyData[keyL].PGpio, KeyData[keyL].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press E  ");
    while(PinIsSet(KeyData[keyE].PGpio, KeyData[keyE].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clYellow, clBlack, "Press R  ");
    while(PinIsSet(KeyData[keyR].PGpio, KeyData[keyR].Pin));
    Vibro.Vibrate(BrrBrr);
    Lcd.Printf(0, 30, clGreen,  clBlack, "Keys OK  ");
#endif

    // ==== IR ====
    IR.RxInit();
    MassStorage.Init();
    Power.Init();

    Sound.Init();
    Sound.SetVolume(START_VOL_CONST);

    PillMgr.Init();

    App.Init();
    Lcd.Printf(0, 90, clGreen, clBlack, "Pill disconnected ");

    Radio.Init();

    Lcd.Printf(0, 100, clYellow, clBlack, "Press A to play mp3");
}
