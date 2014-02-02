/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: Armlet2South
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "kl_lib_L15x.h"
#include "ch.h"
#include "hal.h"
#include "clocking_L1xx.h"
#include "cmd_uart.h"
#include "radio_lvl1.h"

static inline void Init();

#define LED_ON()    PinSet(GPIOB, 0)
#define LED_OFF()   PinClear(GPIOB, 0)

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V5);
    //Clk.SetupFlashLatency(24);  // Setup Flash Latency for clock in MHz
//    Clk.SetupBusDividers(ahbDiv1, apbDiv1, apbDiv1);
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();
    // ==== Init Hard & Soft ====
    Init();
//    if(ClkResult) Uart.Printf("Clock failure\r");

    while(true) {
        //chThdSleepMilliseconds(999);
        chSysLock();
        chThdSleepS(TIME_INFINITE); // Forever
        chSysUnlock();
    } // while
}

void Init() {
    Uart.Init(256000);
    Uart.Printf("AtlantisTX AHB=%u; APB1=%u; APB2=%u\r", Clk.AHBFreqHz, Clk.APB1FreqHz, Clk.APB2FreqHz);
    PinSetupOut(GPIOB, 0, omPushPull);
    LED_ON();

    rLevel1.Init(121);  // FIXME: read ID somehow
}
