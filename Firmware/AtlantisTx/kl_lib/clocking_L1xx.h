/*
 * clocking_f0.h
 *
 *  Created on: 20.01.2013
 *      Author: kreyl
 */

#ifndef CLOCKING_H_
#define CLOCKING_H_

#include "stm32l1xx.h"

/*
 * Right after reset, CPU works on internal MSI source.
 * To switch to external src (HSE) without dividing (i.e. SysClk == CrystalFreq),
 * call SwitchToHSE(), and then optionally HSIDisable().
 * To switch from HSE to HSI, call SwitchToHSI() then optionally HSEDisable().
 * To switch to PLL, disable it first with PLLDisable(), then setup dividers
 * with SetupPLLDividers(), then call SwitchToPLL(). Then disable HSI if needed.
 *
 * Do not forget to update Freq values after switching.
 *
 * AHB  freq max = 32 MHz;
 * APB1 freq max = 32 MHz;
 * APB2 freq max = 32 MHz;
 */

#define CRYSTAL_FREQ_HZ     8000000     // Freq of external crystal, change accordingly
#define HSI_FREQ_HZ         HSI_VALUE   // Freq of internal generator, not adjustable
#define LSI_FREQ_HZ         37000       // Freq of internal generator, not adjustable

enum ClkSrc_t {csHSI, csHSE, csPLL, csMSI};
enum PllMul_t {
    pllMul3 = 0b0000,
    pllMul4 = 0b0001,
    pllMul6 = 0b0010,
    pllMul8 = 0b0011,
    pllMul12= 0b0100,
    pllMul16= 0b0101,
    pllMul24= 0b0110,
    pllMul32= 0b0111,
    pllMul48= 0b1000,
};

enum PllDiv_t {pllDiv2=0b01, pllDiv3=0b10, pllDiv4=0b11};

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

//enum ADCDiv_t {
//    adcDiv2=RCC_CFGR_ADCPRE_DIV2,
//    adcDiv4=RCC_CFGR_ADCPRE_DIV4,
//    adcDiv6=RCC_CFGR_ADCPRE_DIV6,
//    adcDiv8=RCC_CFGR_ADCPRE_DIV8
//};

class Clk_t {
private:
    uint8_t HSEEnable();
    uint8_t HSIEnable();
    uint8_t PLLEnable();
    uint8_t MSIEnable();
public:
    // Frequency values
    uint32_t AHBFreqHz;     // HCLK: AHB Bus, Core, Memory, DMA; 32 MHz max
    uint32_t APB1FreqHz;    // PCLK1: APB1 Bus clock; 32 MHz max
    uint32_t APB2FreqHz;    // PCLK2: APB2 Bus clock; 32 MHz max
    // SysClk switching
    uint8_t SwitchToHSI();
    uint8_t SwitchToHSE();
    uint8_t SwitchToPLL();
    uint8_t SwitchToMSI();
    void HSEDisable() { RCC->CR &= ~RCC_CR_HSEON; }
    void HSIDisable() { RCC->CR &= ~RCC_CR_HSION; }
    void PLLDisable() { RCC->CR &= ~RCC_CR_PLLON; }
    void MSIDisable() { RCC->CR &= ~RCC_CR_MSION; }
    void SetupBusDividers(AHBDiv_t AHBDiv, APBDiv_t APB1Div, APBDiv_t APB2Div);
    uint8_t SetupPLLMulDiv(PllMul_t PllMul, PllDiv_t PllDiv);
    void UpdateFreqValues();
    void SetupFlashLatency(uint8_t AHBClk_MHz);
    // Other clocks
//    void SetupAdcClk(ADCDiv_t ADCDiv);
//    void LsiEnable() {
//        RCC->CSR |= RCC_CSR_LSION;
//        while(!(RCC->CSR & RCC_CSR_LSIRDY));
//    }
//    void LsiDisable() { RCC->CSR &= RCC_CSR_LSION; }
};

extern Clk_t Clk;

// =============================== V Core ======================================
enum VCore_t {vcore1V2=0b11, vcore1V5=0b10, vcore1V8=0b01};
extern VCore_t VCore;
void SetupVCore(VCore_t AVCore);

#endif /* CLOCKING_H_ */
