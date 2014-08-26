/*
 * flashloader_support.cpp
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */
#include "stm32f2xx.h"
#include "flashloader_support.h"

Btldr_t Bootloader;

void boot_jump(uint32_t Address) {
    __ASM("LDR SP, [R0]");
    __ASM("LDR PC, [R0, #4]");
}
