/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"

Keys_t Keys;

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
__attribute__((noreturn))
static void KeysThread(void *arg) {
    chRegSetThreadName("Keys");
    uint8_t i;
    KeyState_t Now;
    bool FHasChanged;
    while(1) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        FHasChanged = false;
        for(i=0; i<KEYS_CNT; i++) {
            Now = PinIsSet(KEY_GPIO, KeyPin[i])? ksReleased : ksPressed;
            if(Keys.Status[i].State != Now) {
                Keys.Status[i].State = Now;
                Keys.Status[i].HasChanged = true;
//                Uart.Printf("Key %u\r", i);
                FHasChanged = true;
            }
        } // for
        if(FHasChanged) chEvtBroadcast(&Keys.EvtSrcKey);
    } // while 1
}

// ==== Keys methods ====
void Keys_t::Init() {
    chEvtInit(&EvtSrcKey);
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KEY_GPIO, KeyPin[i], pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, (tfunc_t)KeysThread, NULL);
}
