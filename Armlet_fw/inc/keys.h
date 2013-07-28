/*
 * keys.h
 *
 *  Created on: 07.02.2013
 *      Author: kreyl
 */

#ifndef KEYS_H_
#define KEYS_H_

#include "hal.h"
#include "kl_lib_f2xx.h"

// Keys ports and pins
#define KEY_GPIO    GPIOD
const uint16_t KeyPin[] = {
        0,  // A
        1,  // B
        3,  // C
        9,  // X
        10, // Y
        11, // Z
        4,  // 4
        7,  // 5
        8,  // 6
};

#define KEYS_CNT    9
#define KEYS_POLL_PERIOD_MS     72

// Key status
enum KeyState_t {ksReleased, ksPressed};

struct KeyStatus_t {
    KeyState_t State;
    bool HasChanged;
};

extern KeyStatus_t KeyStatus[KEYS_CNT];

void KeysInit();
void KeysRegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask);
void KeysShutdown();

#endif /* KEYS_H_ */
