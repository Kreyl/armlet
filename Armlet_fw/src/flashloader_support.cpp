/*
 * flashloader_support.cpp
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */
#include "stm32f2xx.h"
#include "flashloader_support.h"

// ===================== Implementation ==================================== //
void JumpToFlashLdr(void){
    uint32_t bootaddr = FLASHLOADER_START_ADDR;
    volatile uint32_t jumpaddr;
    void (*boot_fn)(void) = 0;

    // prepare jump address
    jumpaddr = *(__IO uint32_t*) (bootaddr + 4);
    // prepare jumping function
    boot_fn = (void (*)(void)) jumpaddr;
    // initialize user application's stack pointer
    __set_MSP(*(__IO uint32_t*) bootaddr);
    // jump
    boot_fn();
}
