/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"

EventSource EvtSrcKey;
KeyStatus_t KeyStatus[KEYS_CNT];

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
static msg_t KeysThread(void *arg) {
    (void)arg;
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
            if(KeyStatus[i].State != Now) {
                KeyStatus[i].State = Now;
                KeyStatus[i].HasChanged = true;
//                Uart.Printf("Key %u\r", i);
                FHasChanged = true;
            }
        } // for

        if(FHasChanged) chEvtBroadcast(&EvtSrcKey);
    }
    return 0;
}

// ==== Keys methods ====
void KeysInit() {
    chEvtInit(&EvtSrcKey);
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KEY_GPIO, KeyPin[i], pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, KeysThread, NULL);
}

void KeysRegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask) {
    chEvtRegisterMask(&EvtSrcKey, PEvtLstnr, EvtMask);
}

void KeysShutdown() {
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupAnalog(KEY_GPIO, KeyPin[i]);
}
