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

// ===================== Implementation ==================================== //
void Btldr_t::JumpIn() {
    Clk.SwitchToHSI();
    __disable_irq();
    SysTick->CTRL = 0;
    SCB->VTOR = 0x17FF0000;
    __enable_irq();
    boot_jump(SYSTEM_MEMORY_ADDR);
    while(1);
}


void Btldr_t::JumpToAddr(uint32_t Addr){
    Clk.SwitchToHSI();
    __disable_irq();
    SysTick->CTRL = 0;
    SCB->VTOR = Addr - 0x08000000;
    __enable_irq();
    boot_jump(Addr);
    while(1);
}


