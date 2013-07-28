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
#define BEEP_END    {-1, 0, 0}

const BeepChunk_t ShortBeep[] = {
        {9, 45, 2000},
        BEEP_END
};

const BeepChunk_t BeepBeep[] = {
        {9, 54, 2000},
        {0, 54},
        {9, 54, 2000},
        BEEP_END
};


#endif /* BEEPSEQUENCES_H_ */
