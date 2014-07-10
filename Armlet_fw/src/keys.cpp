/*
 * keys.cpp
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#include "keys.h"
#include "ch.h"
#include "application.h"
#include "evt_mask.h"

Keys_t Keys;

// ==== Keys Thread ====
static WORKING_AREA(waKeysThread, 128);
__attribute__((noreturn))
static void KeysThread(void *arg) {
    chRegSetThreadName("Keys");
    Keys.ITask();
}

__attribute__((noreturn))
void Keys_t::ITask() {
//    KeyEvtInfo_t Evt;
    while(true) {
        chThdSleepMilliseconds(KEYS_POLL_PERIOD_MS);
        // Check keys
        for(uint8_t i=0; i<KEYS_CNT; i++) {
            bool PressedNow = !PinIsSet(KeyData[i].PGpio, KeyData[i].Pin);
            // ==== Key Press ====
            if(PressedNow and !Key[i].IsPressed) {
                Key[i].IsPressed = true;
                Key[i].IsLongPress = false;
                Key[i].IsRepeating = false;

                KeyEvtInfo_t IEvt;
                IEvt.NKeys = 0;
                for(uint8_t j=0; j<KEYS_CNT; j++) {
                    if(Key[j].IsPressed) {
                        IEvt.KeyID[IEvt.NKeys] = j;
                        IEvt.NKeys++;
                        if((j != i) and !IsCombo) {
                            IsCombo = true;
                            AddEvtToQueue({keCancel, 1, {j}});
                        }
                    }
                } // for j
                if(IEvt.NKeys == 1) {   // Single key pressed
                    IsCombo = false;
                    IEvt.Type = kePress;
                    // Reset timers
                    RepeatTimer = chTimeNow();
                    LongPressTimer = chTimeNow();
                }
                else IEvt.Type = keCombo;
                AddEvtToQueue(IEvt);
            }
            // ==== Key Release ====
            else if(!PressedNow and Key[i].IsPressed) {
                Key[i].IsPressed = false;
                // Check if combo completely released
                if(IsCombo) {
                    IsCombo = false;
                    for(uint8_t j=0; j<KEYS_CNT; j++) {
                        if(Key[j].IsPressed) {
                            IsCombo = true;
                            break;
                        }
                    }
                } // if combo
                // Send evt if not combo
                else AddEvtToQueue({keRelease, 1, {i}});
            }
            // ==== Long Press ====
            else if(PressedNow and Key[i].IsPressed and !IsCombo) {
                // Check if long press
                if(!Key[i].IsLongPress) {
                    if(TimeElapsed(&LongPressTimer, KEY_LONGPRESS_DELAY_MS)) {
                        Key[i].IsLongPress = true;
                        AddEvtToQueue({keLongPress, 1, {i}});
                    }
                }
                // Check if repeat
                if(!Key[i].IsRepeating) {
                    if(TimeElapsed(&RepeatTimer, KEYS_DELAY_BEFORE_REPEAT_MS)) {
                        Key[i].IsRepeating = true;
                        AddEvtToQueue({keRepeat, 1, {i}});
                    }
                }
                else {
                    if(TimeElapsed(&RepeatTimer, KEY_REPEAT_PERIOD_MS)) {
                        AddEvtToQueue({keRepeat, 1, {i}});
                    }
                }
            } // if still pressed
        } // for
    } // while 1
}

void Keys_t::Init() {
    for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupIn(KeyData[i].PGpio, KeyData[i].Pin, pudPullUp);
    // Create and start thread
    chThdCreateStatic(waKeysThread, sizeof(waKeysThread), NORMALPRIO, (tfunc_t)KeysThread, NULL);
}

void Keys_t::AddEvtToQueue(KeyEvtInfo_t Evt) {
//    Uart.Printf("EvtType=%u; Keys: ", Evt.Type);
//    for(uint8_t i=0; i<Evt.NKeys; i++) Uart.Printf("%u ", Evt.KeyID[i]);
//    Uart.Printf("\r\n");
    if(App.PThd == nullptr) return;
    chSysLock();
    EvtBuf.Put(&Evt);
    chEvtSignalI(App.PThd, EVTMASK_KEYS);
    chSysUnlock();
}
