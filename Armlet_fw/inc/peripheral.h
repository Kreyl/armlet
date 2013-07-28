/*
 * peripheral.h
 *
 *  Created on: 18.01.2013
 *      Author: kreyl
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include <stdint.h>
#include "kl_lib_f2xx.h"

// ==================================== Vibro ==================================
struct VibroChunk_t {
    int8_t Intencity;  // 0 means off, 1...100 means intencity, -1 means end
    uint16_t Time_ms;
} PACKED;
#define VIBRO_CHUNK_SZ   sizeof(VibroChunk_t)
class Vibrator_t {
private:
    VirtualTimer ITmr;
public:
    void Vibrate(const VibroChunk_t *PSequence) { // Vibrate with this function
        chSysLock();
        VibrateI(PSequence);
        chSysUnlock();
    }
    void Vibrate(uint32_t ms);
    void Init();
    void Shutdown();
    // Inner use
    PwmPin_t IPin;
    void VibrateI(const VibroChunk_t *PSequence);
};
extern Vibrator_t Vibro;

// ==================================== Beep ===================================
struct BeepChunk_t {
    int8_t VolumePercent;   // 0 means silence, 1...100 means volume, -1 means end
    uint16_t Time_ms;
    uint16_t Freq_Hz;
} PACKED;
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)
class Beeper_t {
private:
    VirtualTimer ITmr;
public:
    void BeepI(const BeepChunk_t *PSequence);
    void Beep(const BeepChunk_t *PSequence) {   // Beep with this function
        chSysLock();
        BeepI(PSequence);
        chSysUnlock();
    }
    void Beep(uint32_t ms);
    void Init();
    void Shutdown();
    // Inner use
    PwmPin_t IPin;
};
extern Beeper_t Beeper;

// ================================= Pin control ===============================
#define PIN_GPIO    GPIOD
#define PIN_NUMBER  14
class Pin_t {
private:
    VirtualTimer ITmr;
public:
    void Init() { PinSetupOut(PIN_GPIO, PIN_NUMBER, omPushPull); }
    void High() { PinSet(PIN_GPIO, PIN_NUMBER); }
    void Low()  { PinClear(PIN_GPIO, PIN_NUMBER); }
    void Pulse(uint32_t ms);
};

extern Pin_t Pin;

#endif /* PERIPHERAL_H_ */
