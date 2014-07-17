/*
 * flashloader_support.h
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */

#ifndef FLASHLOADER_SUPPORT_H_
#define FLASHLOADER_SUPPORT_H_


#include "kl_lib_f2xx.h"

#define SYSTEM_MEMORY_ADDR      (uint32_t)0x1FF00000 // System memory Information Block


class Btldr_t {
private:
public:
    uint8_t CheckUserCode(uint32_t Addr);
    void JumpToAddr(uint32_t Addr);
    void Iwdt_enable() {
        const uint32_t LsiFreq = 42000;
        IWDG->KR = IWDG_KR_KEY;                 // Write Access
        IWDG->PR = IWDG_PR_PR_1 | IWDG_PR_PR_0; // set prescaler
        IWDG->RLR = LsiFreq/128;                // set preload
        IWDG->KR = 0xCCCC;                      // enable wdtg
    }
};

extern Btldr_t Bootloader;

#endif /* FLASHLOADER_SUPPORT_H_ */
