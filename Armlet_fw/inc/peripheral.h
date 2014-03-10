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

enum ChunkKind_t {ckNormal=0, ckStop=1, ckRepeat=2};

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

#if 1 // ================================= Beep ================================
struct BeepChunk_t {
    uint8_t Volume;   // 0 means silence, 10 means top
    uint16_t Freq_Hz;
    uint16_t Time_ms;
    ChunkKind_t ChunkKind;
};
#define BEEP_CHUNK_SZ   sizeof(BeepChunk_t)
class Beeper_t {
private:
    VirtualTimer ITmr;
    const BeepChunk_t *IPFirstChunk;
public:
    void BeepI(const BeepChunk_t *PSequence);
    void Beep(const BeepChunk_t *PSequence) {   // Beep with this function
        IPFirstChunk = PSequence;
        chSysLock();
        BeepI(PSequence);
        chSysUnlock();
    }
    void Stop() {
        chSysLock();
        if(chVTIsArmedI(&ITmr)) chVTResetI(&ITmr);
        chSysUnlock();
        IPin.Set(0);
    }
    void Init();
    void Shutdown();
    // Inner use
    PwmPin_t IPin;
};
extern Beeper_t Beeper;
#endif

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
