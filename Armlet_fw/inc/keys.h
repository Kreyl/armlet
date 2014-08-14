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
#include "kl_buf.h"

// Keys ports and pins
struct KeyData_t {
    GPIO_TypeDef *PGpio;
    uint16_t Pin;
};
const KeyData_t KeyData[] = {
        {GPIOD, 0},  // A
        {GPIOD, 1},  // B
        {GPIOD, 3},  // C
        {GPIOD, 9},  // X
        {GPIOD, 10}, // Y
        {GPIOD, 11}, // Z
        {GPIOD, 4},  // L
        {GPIOD, 7},  // E
        {GPIOD, 8},  // R
};

#define KEYS_CNT                    countof(KeyData)
#define KEYS_POLL_PERIOD_MS         72
#define KEY_REPEAT_PERIOD_MS        306
#define KEY_LONGPRESS_DELAY_MS      801
#define KEYS_DELAY_BEFORE_REPEAT_MS (KEY_REPEAT_PERIOD_MS + KEY_LONGPRESS_DELAY_MS)

#define KEYS_EVT_Q_LEN              7

enum KeyName_t {keyA=0, keyB=1, keyC=2, keyX=3, keyY=4, keyZ=5, keyL=6, keyE=7, keyR=8};

// Switch-on key
#define KEY_PWRON       keyA
#define KEY_PWRON_GPIO  KeyData[KEY_PWRON].PGpio
#define KEY_PWRON_PIN   KeyData[KEY_PWRON].Pin

// Key status
struct Key_t {
    bool IsPressed, IsRepeating, IsLongPress;
};

enum KeyEvt_t {kePress, keLongPress, keRelease, keCancel, keRepeat, keCombo};
struct KeyEvtInfo_t {
    KeyEvt_t Type;
    uint8_t NKeys;
    uint8_t KeyID[KEYS_CNT];
};

class Keys_t {
private:
    Key_t Key[KEYS_CNT];
    systime_t RepeatTimer, LongPressTimer;
    bool IsCombo;
    void AddEvtToQueue(KeyEvtInfo_t Evt);
public:
    void Init();
    void Shutdown() { for(uint8_t i=0; i<KEYS_CNT; i++) PinSetupAnalog(KeyData[i].PGpio, KeyData[i].Pin); }
    // Events
    CircBuf_t<KeyEvtInfo_t, KEYS_EVT_Q_LEN> EvtBuf;

    // Inner use
    void ITask();
};

extern Keys_t Keys;

#endif /* KEYS_H_ */
