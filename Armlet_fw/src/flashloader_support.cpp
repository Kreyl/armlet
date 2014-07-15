/*
 * flashloader_support.cpp
 *
 *  Created on: 27.01.2013
 *      Author: Roma Jam
 */
#include "stm32f2xx.h"
#include "flashloader_support.h"

Btldr_t Bootloader;

// ===================== Implementation ==================================== //
void Btldr_t::JumpToAddr(uint32_t Addr){
//    typedef void (*pFunction)(void);
//    pFunction Jump_To_Application;
//    vu32* JumpAddr;          // variable that will be loaded with the start address of the application
//    const vu32* AppAddr = (vu32*) Addr;
//    JumpAddr = (vu32*) AppAddr[1]; // get jump address from application vector table
//    Jump_To_Application = (pFunction) JumpAddr; // load this address into function pointer
//    SCB_ICSR = ICSR_PENDSVCLR;     // Clear pending interrupts just to be on the safe side
//
//    /* Disable all interrupts */
//    for(uint8_t i=0; i<8; i++) NVIC->ICER[i] = NVIC->IABR[i];
//
//    /* set stack pointer as in application's vector table */
//    __set_MSP((u32) (AppAddr[0]));
//    Jump_To_Application();



    uint32_t bootaddr = Addr;     // Set start adress in System Memory
    volatile uint32_t jumpaddr;
    void (*boot_fn)(void) = 0;                      // Initialize jump function

    jumpaddr = *(__IO uint32_t*) (bootaddr + 4);    // prepare jump address
    boot_fn = (void (*)(void)) jumpaddr;            // prepare jumping function
    __set_MSP(*(__IO uint32_t*) bootaddr);          // initialize user application's stack pointer
    // jump
    boot_fn();
}



