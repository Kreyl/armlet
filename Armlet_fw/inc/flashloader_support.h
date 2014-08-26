/*
 * flashloader_support.h
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */

#ifndef FLASHLOADER_SUPPORT_H_
#define FLASHLOADER_SUPPORT_H_


#include "kl_lib_f2xx.h"
#include "evt_mask.h"

#define SYSTEM_MEMORY_ADDR      (uint32_t)0x1FFF0000 // System memory Information Block

void GoToDFU();
void boot_jump(uint32_t Address);

enum Iwdg_t {
    wdg_ON, wdg_OFF
};

class Btldr_t {
private:
    void iwdgEnable() {
        const uint32_t LsiFreq = 42000;
        IWDG->KR = 0x5555;                 // Write Access
        IWDG->PR = IWDG_PR_PR_1 | IWDG_PR_PR_0; // set prescaler
        IWDG->RLR = LsiFreq/256;                // set preload
        IWDG->KR = 0xCCCC;                      // enable wdtg
    }
    void wwdgEnable() {
        rccEnableAPB1(RCC_APB1ENR_WWDGEN, FALSE);   // enable wwdg clock
        WWDG->CFR  = ((uint32_t)0x00000180); // /8
        WWDG->CFR |= 0x7F;  // window value
        WWDG->CR = 0x7F;
        WWDG->CR |= 0x80; // enable wwdg
    }
public:
    Thread* TerminatorThd;
    void dfuJumpIn(Iwdg_t wdg_state) {
        if(wdg_state == wdg_ON) wwdgEnable();
        if(TerminatorThd != nullptr) chEvtSignal(TerminatorThd, EVTMSK_DFU_REQUEST);
    }
};

extern Btldr_t Bootloader;

#endif /* FLASHLOADER_SUPPORT_H_ */
