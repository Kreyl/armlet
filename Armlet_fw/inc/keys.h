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
        4,  // L
        7,  // E
        8,  // R
};

#define KEYS_CNT    9
#define KEYS_POLL_PERIOD_MS     72

// Key status
enum KeyState_t {ksReleased, ksPressed};

struct KeyStatus_t {
    KeyState_t State;
    bool HasChanged;
    bool JustPressed() { return HasChanged and (State == ksPressed); }
};

class Keys_t {
public:
    EventSource EvtSrcKey;
    VirtualTimer ITmr;
    KeyStatus_t Status[KEYS_CNT];
    void Init();
    void RegisterEvt(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&EvtSrcKey, PEvtLstnr, EvtMask); }
    void Shutdown() { for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupAnalog(KEY_GPIO, KeyPin[i]); }
    // Status
    void Reset() { for(uint8_t i=0; i<KEYS_CNT; i++) Status[i].HasChanged = false; }
    bool AnyPressed() {
        for(uint8_t i=0; i<KEYS_CNT; i++)
            if(Status[i].JustPressed())
                return true;   // Do not react on holded buttons
        return false;
    }
};

extern Keys_t Keys;

//#define KEY_A   Keys.Status[0]
//#define KEY_B   Keys.Status[1]
//#define KEY_C   Keys.Status[2]
//#define KEY_X   Keys.Status[3]
//#define KEY_Y   Keys.Status[4]
//#define KEY_Z   Keys.Status[5]
//#define KEY_L   Keys.Status[6]
//#define KEY_E   Keys.Status[7]
//#define KEY_R   Keys.Status[8]

#endif /* KEYS_H_ */
