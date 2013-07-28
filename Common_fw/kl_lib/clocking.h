/*
 * clocking.h
 *
 *  Created on: 20.01.2013
 *      Author: kreyl
 */

#ifndef CLOCKING_H_
#define CLOCKING_H_

#include "stm32f2xx.h"

/*
 * Right after reset, CPU works on internal (HSI) source.
 * To switch to external src (HSE) without dividing (i.e. SysClk == CrystalFreq),
 * call SwitchToHSE(), and then optionally HSIDisable().
 * To switch from HSE to HSI, call SwitchToHSI() then optionally HSEDisable().
 * To switch to PLL, disable it first with PLLDisable(), then setup dividers
 * with SetupPLLDividers(), then call SwitchToPLL(). Then disable HSI if needed.
 *
 * Do not forget to update Freq values after switching.
 *
 * Keep in mind that Flash latency need to be increased at higher speeds.
 * Tune it with SetupFlashLatency.
 *
 * AHB  freq max = 120 MHz;
 * APB1 freq max = 30 MHz;
 * APB2 freq max = 60 MHz.
 */

#define CRYSTAL_FREQ_HZ     12000000    // Freq of external crystal, change accordingly
#define HSI_FREQ_HZ         HSI_VALUE   // Freq of internal generator, not adjustable

enum ClkSrc_t {csHSI, csHSE, csPLL};
enum AHBDiv_t {
    ahbDiv1=0b0000,
    ahbDiv2=0b1000,
    ahbDiv4=0b1001,
    ahbDiv8=0b1010,
    ahbDiv16=0b1011,
    ahbDiv64=0b1100,
    ahbDiv128=0b1101,
    ahbDiv256=0b1110,
    ahbDiv512=0b1111
};
enum APBDiv_t {apbDiv1=0b000, apbDiv2=0b100, apbDiv4=0b101, apbDiv8=0b110, apbDiv16=0b111};
enum PllSysDiv_P_t {pllSysDiv2=0b00, pllSysDiv4=0b01, pllSysDiv6=0b10, pllSysDiv8=0b11};
enum Mco1Src_t {mco1HSI=0x00000000, mco1LSE=0x00200000, mco1HSE=0x00400000, mco1PLL=0x00600000};
enum McoDiv_t {mcoDiv1=0x00000000, mcoDiv2=0x04000000, mcoDiv3=0x05000000, mcoDiv4=0x06000000, mcoDiv5=0x07000000};

class Clk_t {
private:
    uint8_t HSEEnable();
    uint8_t HSIEnable();
    uint8_t PLLEnable();
public:
    // Frequency values
    uint32_t AHBFreqHz;     // HCLK: AHB Buses, Core, Memory, DMA; 120 MHz max
    uint32_t APB1FreqHz;    // PCLK1: APB1 Bus clock; 30 MHz max
    uint32_t APB2FreqHz;    // PCLK2: APB2 Bus clock; 60 MHz max
    uint32_t UsbSdioFreqHz; // Clock is intended to be 48 MHz
    // Clk switching
    uint8_t SwitchToHSI();
    uint8_t SwitchToHSE();
    uint8_t SwitchToPLL();
    void HSEDisable() { RCC->CR &= ~RCC_CR_HSEON; }
    void HSIDisable() { RCC->CR &= ~RCC_CR_HSION; }
    void PLLDisable() { RCC->CR &= ~RCC_CR_PLLON; }
    void LSIEnable();
    // Dividers
    void SetupBusDividers(AHBDiv_t AHBDiv, APBDiv_t APB1Div, APBDiv_t APB2Div);
    uint8_t SetupPLLDividers(uint8_t InputDiv_M, uint16_t Multi_N, PllSysDiv_P_t SysDiv_P, uint8_t UsbDiv_Q);
    void UpdateFreqValues();
    uint8_t SetupFlashLatency(uint8_t AHBClk_MHz, uint16_t Voltage_mV=3300);
    // Clock output
    void MCO1Enable(Mco1Src_t Src, McoDiv_t Div);
};

extern Clk_t Clk;

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
extern "C" {
void __early_init(void);
}

#endif /* CLOCKING_H_ */
