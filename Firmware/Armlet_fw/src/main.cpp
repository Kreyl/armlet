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
#include "radio_lvl1.h"
#include "cmd_uart.h"
#include "power.h"
#include "pill.h"
#include "MassStorage.h"
#include "ff.h"

#include "application.h"


static inline void Init();

int main() {
    // ==== Setup clock ====
    uint8_t ClkResult = 1;
    // 12 MHz/6 = 2; 2*192 = 384; 384/8 = 48 (preAHB divider); 384/8 = 48 (USB clock)
    Clk.SetupPLLDividers(6, 192, pllSysDiv8, 8);
    // 48/2 = 24 MHz core clock. APB1 & APB2 clock derive on AHB clock
//    Clk.SetupFlashLatency(48);  // Setup Flash Latency for clock in MHz
//    Clk.SetupBusDividers(ahbDiv1, apbDiv2, apbDiv2);
    Clk.SetupFlashLatency(12);  // Setup Flash Latency for clock in MHz
    Clk.SetupBusDividers(ahbDiv4, apbDiv1, apbDiv1);
    if((ClkResult = Clk.SwitchToPLL()) == 0) Clk.HSIDisable();
    Clk.UpdateFreqValues();
//    Clk.LSIEnable();        // To allow RTC to run

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
    // Report problem with clock if any
    if(ClkResult) Uart.Printf("Clock failure\r");

    while(TRUE) {
        chThdSleepMilliseconds(999);
//        chSysLock();
//        chSchGoSleepS(THD_STATE_SUSPENDED); // Forever
//        chSysUnlock();
        // Ctrl-Alt-Del
//        if(!PinIsSet(KEY_GPIO, 3) and !PinIsSet(KEY_GPIO, 4) and !PinIsSet(KEY_GPIO, 9)) {
//            REBOOT();
//        }
    }
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("Atlantis Sys=%u usb=%u\r", Clk.AHBFreqHz, Clk.UsbSdioFreqHz);

    SD.Init();
    // Read config
    uint8_t ID=9;
//    iniReadUint32("Radio", "ID", "settings.ini", &ID);
//    Uart.Printf("ID=%u\r", ID);

//    Lcd.Init();
    //Lcd.Printf(11, 11, clGreen, clBlack, "Ostranna BBS");

//    Usb.Init();
//    MassStorage.Init();
//    chThdSleepMilliseconds(450);
//    Usb.Connect();

//    Keys.Init();
//    Beeper.Init();
//    Vibro.Init();
//    IR.TxInit();
//    IR.RxInit();
//    Power.Init();
//    PillInit();
//    Sound.Init();
//    Sound.Play("angel.mp3");
//    Sound.Play("alive.wav");
    rLevel1.Init(ID);
    App.Init();
}
