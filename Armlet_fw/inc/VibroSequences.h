/*
 * VibroSequences.h
 *
 *  Created on: 12.02.2013
 *      Author: kreyl
 */

#ifndef VIBROSEQUENCES_H_
#define VIBROSEQUENCES_H_

/*
 * This file contains sequences of Vibro.
 * Every sequence is an array of BeepCmd_t:
 struct BeepCmd_t {
    uint8_t VolumePercent;
    uint32_t Time_ms;
    uint16_t Freq_Hz;
};

 * Do not exceed BEEP_MAX_CHUNK_COUNT! (9)
 */

#include "peripheral.h"
#define VIBRO_END    {-1, 0}

const VibroChunk_t ShortBrr[] = {
        {100, 45},
        VIBRO_END
};

const VibroChunk_t BrrBrr[] = {
        {100, 54},
        {0, 54},
        {100, 54},
        VIBRO_END
};


#endif /* VIBROSEQUENCES_H_ */
