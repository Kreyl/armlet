/*
 * BeepSequences.h
 *
 *  Created on: 03.02.2013
 *      Author: kreyl
 */

#ifndef BEEPSEQUENCES_H_
#define BEEPSEQUENCES_H_

/*
 * This file contains sequences of beeps.
 * Every sequence is an array of BeepCmd_t:
 struct BeepCmd_t {
    uint8_t VolumePercent;
    uint32_t Time_ms;
    uint16_t Freq_Hz;
  };
 */

#include "peripheral.h"
#if 1 // ============================= Beep ====================================
/* Every sequence is an array of BeepCmd_t:
 struct BeepChunk_t {
    uint8_t Volume;   // 0 means silence, 10 means top
    uint16_t Freq_Hz;
    uint16_t Time_ms;
    ChunkKind_t ChunkKind;
  };
*/
#define BEEP_VOLUME     1   // set to 10 in production, and to 1 when someone sleeps near

const BeepChunk_t ShortBeep[] = {
        {BEEP_VOLUME, 1975, 54, ckStop},
};

const BeepChunk_t BeepBeep[] = {
        {BEEP_VOLUME, 1975, 54, ckNormal},
        {0, 0, 54, ckNormal},
        {BEEP_VOLUME, 1975, 54, ckStop},
};

// Pill
const BeepChunk_t BeepPillOk[] = {
        {BEEP_VOLUME, 1975, 180, ckNormal},
        {BEEP_VOLUME, 2489, 180, ckNormal},
        {BEEP_VOLUME, 2960, 180, ckStop},
};

const BeepChunk_t BeepPillBad[] = {
        {BEEP_VOLUME, 1568, 360, ckStop},
};


// Health states
const BeepChunk_t BeepDeath[] = {
        {BEEP_VOLUME, 1975, 2000, ckNormal},
        {0, 0, 10000, ckRepeat},
};
const BeepChunk_t BeepRedFast[] = {
        {BEEP_VOLUME, 1975, 54, ckNormal},
        {0, 0, 54, ckRepeat},
};
#endif

#endif /* BEEPSEQUENCES_H_ */
