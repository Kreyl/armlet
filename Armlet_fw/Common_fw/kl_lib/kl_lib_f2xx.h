/*
 * kl_lib_f205.h
 *
 *  Created on: 10.12.2012
 *      Author: kreyl
 */

#ifndef KL_LIB_F2XX_H_
#define KL_LIB_F2XX_H_

#include "stm32.h"
#include "stdint-gcc.h"
#include "ch.h"
#include "hal.h"
#include "clocking.h"
#include "string.h"     // for memcpy
#include <cstdlib>      // for strtoul

#if 1 // ============================ General ==================================
#define PACKED __attribute__ ((__packed__))
#ifndef countof
#define countof(A)  (sizeof(A)/sizeof(A[0]))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Functional type
typedef void (*ftVoidVoid)(void);

// Return values
#define OK              0
#define FAILURE         1
#define TIMEOUT         2
#define BUSY            3
#define NEW             4
#define IN_PROGRESS     5
#define LAST            6
#define CMD_ERROR       7
#define WRITE_PROTECT   8
#define CMD_UNKNOWN     9
#define EMPTY_STRING    10
#define NOT_A_NUMBER    11

// Binary semaphores
#define NOT_TAKEN       false
#define TAKEN           true

enum BitOrder_t {boMSB, boLSB};
enum LowHigh_t  {Low, High};
enum RiseFall_t {Rising, Falling, RisingFalling};

// Simple pseudofunctions
#define MIN(a, b)   ( ((a)<(b))? (a) : (b) )
#define MAX(a, b)   ( ((a)>(b))? (a) : (b) )
#define TRIM_VALUE(v, Max)  { if(v > Max) v = Max; }
#define IS_LIKE(v, precise, deviation)  (((precise - deviation) < v) and (v < (precise + deviation)))

#define ANY_OF_2(a, b1, b2)             (((a)==(b1)) or ((a)==(b2)))
#define ANY_OF_3(a, b1, b2, b3)         (((a)==(b1)) or ((a)==(b2)) or ((a)==(b3)))
#define ANY_OF_4(a, b1, b2, b3, b4)     (((a)==(b1)) or ((a)==(b2)) or ((a)==(b3)) or ((a)==(b4)))
#define ANY_OF_5(a, b1, b2, b3, b4, b5) (((a)==(b1)) or ((a)==(b2)) or ((a)==(b3)) or ((a)==(b4)) or ((a)==(b5)))

static inline uint16_t BuildUint16(uint8_t Lo, uint8_t Hi) {
    uint16_t r = Hi;
    r <<= 8;
    r |= Lo;
    return r;
}

static inline uint32_t BuildUint32(uint8_t Lo, uint8_t MidLo, uint8_t MidHi, uint8_t Hi) {
    uint32_t r = Hi;
    r <<= 8;
    r |= MidHi;
    r <<= 8;
    r |= MidLo;
    r <<= 8;
    r |= Lo;
    return r;
}

// Convert
class Convert {
public:
    static void U16ToArrAsBE(uint8_t *PArr, uint16_t N) {
        uint8_t *p8 = (uint8_t*)&N;
        *PArr++ = *(p8 + 1);
        *PArr   = *p8;
    }
    static void U32ToArrAsBE(uint8_t *PArr, uint32_t N) {
        uint8_t *p8 = (uint8_t*)&N;
        *PArr++ = *(p8 + 3);
        *PArr++ = *(p8 + 2);
        *PArr++ = *(p8 + 1);
        *PArr   = *p8;
    }
    static uint16_t ArrToU16AsBE(uint8_t *PArr) {
        uint16_t N;
        uint8_t *p8 = (uint8_t*)&N;
        *p8++ = *(PArr + 1);
        *p8 = *PArr;
        return N;
    }
    static uint32_t ArrToU32AsBE(uint8_t *PArr) {
        uint32_t N;
        uint8_t *p8 = (uint8_t*)&N;
        *p8++ = *(PArr + 3);
        *p8++ = *(PArr + 2);
        *p8++ = *(PArr + 1);
        *p8 = *PArr;
        return N;
    }
    static void U16ChangeEndianness(uint16_t *p) { *p = __REV16(*p); }
    static void U32ChangeEndianness(uint32_t *p) { *p = __REV(*p); }
    static inline uint8_t TryStrToUInt32(char* S, uint32_t *POutput) {
        if(*S == '\0') return EMPTY_STRING;
        char *p;
        *POutput = strtoul(S, &p, 0);
        return (*p == 0)? OK : NOT_A_NUMBER;
    }
    static inline uint8_t TryStrToInt32(char* S, int32_t *POutput) {
        if(*S == '\0') return EMPTY_STRING;
        char *p;
        *POutput = strtol(S, &p, 0);
        return (*p == '\0')? OK : NOT_A_NUMBER;
    }
};

// IRQ priorities
#define IRQ_PRIO_LOW            15  // Minimum
#define IRQ_PRIO_MEDIUM         9
#define IRQ_PRIO_HIGH           7
#define IRQ_PRIO_VERYHIGH       4 // Higher than systick

// DMA
#define DMA_PRIORITY_LOW        STM32_DMA_CR_PL(0b00)
#define DMA_PRIORITY_MEDIUM     STM32_DMA_CR_PL(0b01)
#define DMA_PRIORITY_HIGH       STM32_DMA_CR_PL(0b10)
#define DMA_PRIORITY_VERYHIGH   STM32_DMA_CR_PL(0b11)

#endif

#if 1 // ============================== Power ==================================
#define REBOOT()                SCB_AIRCR = (AIRCR_VECTKEY | 0x04)

static inline void EnableBackupAccess() {
    rccEnablePWRInterface(FALSE);
    PWR->CR |= PWR_CR_DBP;
}
static inline void DisableBackupAccess() {
    PWR->CR &= ~PWR_CR_DBP;
    rccDisablePWRInterface(FALSE);
}

// RegN = 0...19
static inline uint32_t ReadBackupRegister(uint32_t RegN) {
    volatile uint32_t tmp = RTC_BASE + 0x50 + (RegN * 4);
    return (*(volatile uint32_t *)tmp);
}

static inline void WriteBackupRegister(uint32_t RegN, uint32_t Data) {
    volatile uint32_t tmp = RTC_BASE + 0x50 + (RegN * 4);
    *(volatile uint32_t *)tmp = Data;
}


#endif

#if 1 // =========================== Time ======================================
static inline bool TimeElapsed(systime_t *PSince, uint32_t Delay_ms) {
    chSysLock();
    bool Rslt = (systime_t)(chTimeNow() - *PSince) > MS2ST(Delay_ms);
    if(Rslt) *PSince = chTimeNow();
    chSysUnlock();
    return Rslt;
}
#endif

#if 1 // ================== Single pin manipulations ===========================
enum PinOutMode_t {
    omPushPull  = 0,
    omOpenDrain = 1
};
enum PinPullUpDown_t {
    pudNone = 0b00,
    pudPullUp = 0b01,
    pudPullDown = 0b10
};
enum PinSpeed_t {
    ps2MHz  = 0b00,
    ps25MHz = 0b01,
    ps50MHz = 0b10,
    ps100MHz = 0b11
};
enum PinAF_t {
    AF0=0, AF1=1, AF2=2, AF3=3, AF4=4, AF5=5, AF6=6, AF7=7,
    AF8=8, AF9=9,AF10=10, AF11=11, AF12=12, AF13=13, AF14=14, AF15=15
};

// Set/clear
static inline void PinSet    (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BSRRL = (uint32_t)(1<<APinNumber); }
static inline void PinClear  (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->BSRRH = (uint32_t)(1<<APinNumber); }
static inline void PinToggle (GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { PGpioPort->ODR  ^= (uint32_t)(1<<APinNumber); }
// Check state
static inline bool PinIsSet(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) { return (PGpioPort->IDR & (uint32_t)(1<<APinNumber)); }
// Setup
static void PinClockEnable(GPIO_TypeDef *PGpioPort) {
    if     (PGpioPort == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if(PGpioPort == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if(PGpioPort == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if(PGpioPort == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if(PGpioPort == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
}
// GPIOA, 1, omPushPull/omOpenDrain, pudNone/pudPullUp/pudPullDown, ps2MHz/ps25MHz/ps50MHz/ps100MHz
static inline void PinSetupOut(
        GPIO_TypeDef *PGpioPort,
        const uint16_t APinNumber,
        const PinOutMode_t PinOutMode,
        const PinPullUpDown_t APullUpDown = pudNone,
        const PinSpeed_t ASpeed = ps50MHz) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));  // clear previous bits
    PGpioPort->MODER |= 0b01 << (APinNumber*2);     // Set 0; // new bits
    // Setup output type
    PGpioPort->OTYPER &= ~(1<<APinNumber);
    PGpioPort->OTYPER |= (uint32_t)PinOutMode << APinNumber;
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
    // Setup speed
    PGpioPort->OSPEEDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->OSPEEDR |= (uint32_t)ASpeed << (APinNumber*2);
}
static inline void PinSetupIn(
        GPIO_TypeDef *PGpioPort,
        const uint16_t APinNumber,
        const PinPullUpDown_t APullUpDown
        ) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));          // clear previous bits
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
}
static inline void PinSetupAnalog(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER |= 0b11 << (APinNumber*2);  // Set 0; // new bits
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)pudNone << (APinNumber*2);
}
static inline void PinSetupAlterFunc(
        GPIO_TypeDef *PGpioPort,
        const uint16_t APinNumber,
        const PinOutMode_t PinOutMode,
        const PinPullUpDown_t APullUpDown,
        const PinAF_t AAlterFunc,
        const PinSpeed_t ASpeed = ps50MHz) {
    // Clock
    PinClockEnable(PGpioPort);
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));  // clear previous bits
    PGpioPort->MODER |= 0b10 << (APinNumber*2);     // Set 0; // new bits
    // Setup output type
    PGpioPort->OTYPER &= ~(1<<APinNumber);
    PGpioPort->OTYPER |= (uint32_t)PinOutMode << APinNumber;
    // Setup Pull-Up or Pull-Down
    PGpioPort->PUPDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->PUPDR |= (uint32_t)APullUpDown << (APinNumber*2);
    // Setup speed
    PGpioPort->OSPEEDR &= ~(0b11 << (APinNumber*2)); // clear previous bits
    PGpioPort->OSPEEDR |= (uint32_t)ASpeed << (APinNumber*2);
    // Setup Alternate Function
    uint32_t n = (APinNumber <= 7)? 0 : 1;      // 0 if 0...7, 1 if 8..15
    uint32_t Shift = 4 * ((APinNumber <= 7)? APinNumber : APinNumber - 8);
    PGpioPort->AFR[n] &= ~(0b1111 << Shift);
    PGpioPort->AFR[n] |= (uint32_t)AAlterFunc << Shift;
}

// Fast switch
static inline void PinFastOutPP(GPIO_TypeDef *PGpioPort, const uint16_t APinNumber) {
    // Setup mode
    PGpioPort->MODER &= ~(0b11 << (APinNumber*2));  // clear previous bits
    PGpioPort->MODER |= 0b01 << (APinNumber*2);     // Set 0; // new bits
}

class PwmPin_t {
private:
    uint32_t *PClk;
    TIM_TypeDef* Tim;
public:
    __IO uint32_t *PCCR;    // Made public to allow DMA
    void SetFreqHz(uint32_t FreqHz);
    void Init(GPIO_TypeDef *GPIO, uint16_t N, TIM_TypeDef* PTim, uint8_t Chnl, uint16_t TopValue, bool Inverted=false);
    void Set(uint16_t Value) { *PCCR = Value; }
    void Off() { *PCCR = 0; }
};
#endif

#if 1 // ==== External IRQ ====
enum ExtiTrigType_t {ttRising, ttFalling, ttRisingFalling};
class IrqPin_t {
private:
    uint32_t IIrqChnl;
    GPIO_TypeDef *IGPIO;
    uint8_t IPinNumber;
public:
    void SetTriggerType(ExtiTrigType_t ATriggerType) {
        uint32_t IrqMsk = 1 << IPinNumber;
        switch(ATriggerType) {
            case ttRising:
                EXTI->FTSR &= ~IrqMsk; // Falling trigger disabled
                EXTI->RTSR |=  IrqMsk; // Rising trigger enabled
                break;
            case ttFalling:
                EXTI->FTSR |=  IrqMsk; // Falling trigger enabled
                EXTI->RTSR &= ~IrqMsk; // Rising trigger disabled
                break;
            case ttRisingFalling:
                EXTI->FTSR |=  IrqMsk; // Falling trigger enabled
                EXTI->RTSR |=  IrqMsk; // Rising trigger enabled
                break;
        } // switch
    }
    void Setup(GPIO_TypeDef *GPIO, const uint8_t APinNumber, ExtiTrigType_t ATriggerType) {
        IGPIO = GPIO;
        IPinNumber = APinNumber;
        rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, FALSE); // Enable sys cfg controller
        // Connect EXTI line to the pin of the port
        uint8_t Indx   = APinNumber / 4;            // Indx of EXTICR register
        uint8_t Offset = (APinNumber & 0x03) * 4;   // Offset in EXTICR register
        SYSCFG->EXTICR[Indx] &= ~((uint32_t)0b1111 << Offset);  // Clear port-related bits
        // GPIOA requires all zeroes => nothing to do in this case
        if     (GPIO == GPIOB) SYSCFG->EXTICR[Indx] |= (uint32_t)0b0001 << Offset;
        else if(GPIO == GPIOC) SYSCFG->EXTICR[Indx] |= (uint32_t)0b0010 << Offset;
        else if(GPIO == GPIOD) SYSCFG->EXTICR[Indx] |= (uint32_t)0b0011 << Offset;
        else if(GPIO == GPIOE) SYSCFG->EXTICR[Indx] |= (uint32_t)0b0100 << Offset;
        // Configure EXTI line
        uint32_t IrqMsk = 1 << APinNumber;
        EXTI->IMR  |=  IrqMsk;      // Interrupt mode enabled
        EXTI->EMR  &= ~IrqMsk;      // Event mode disabled
        SetTriggerType(ATriggerType);
        EXTI->PR    =  IrqMsk;      // Clean irq flag
        // Get IRQ channel
        if     ((APinNumber >= 0)  and (APinNumber <= 4))  IIrqChnl = EXTI0_IRQn + APinNumber;
        else if((APinNumber >= 5)  and (APinNumber <= 9))  IIrqChnl = EXTI9_5_IRQn;
        else if((APinNumber >= 10) and (APinNumber <= 15)) IIrqChnl = EXTI15_10_IRQn;
    }
    // Enable/disable NVIC vector
    void EnableIrq(const uint32_t Priority) { nvicEnableVector(IIrqChnl, CORTEX_PRIORITY_MASK(Priority)); }
    void DisableIrq() { nvicDisableVector(IIrqChnl); }
    void GenerateIrq() { EXTI->SWIER |= 1 << IPinNumber; }
    void CleanIrqFlag() { EXTI->PR = (1 << IPinNumber); }
    // Standard pin functions
    bool IsHi() { return PinIsSet(IGPIO, IPinNumber); }
};
#endif

// ================================= Random ====================================
// Returns [0; TopValue]
uint32_t Random(uint32_t TopValue);

#if 1 // ============================== Timers =================================
enum TmrTrigInput_t {tiITR0=0x00, tiITR1=0x10, tiITR2=0x20, tiITR3=0x30, tiTIED=0x40, tiTI1FP1=0x50, tiTI2FP2=0x60, tiETRF=0x70};
enum TmrMasterMode_t {mmReset=0x00, mmEnable=0x10, mmUpdate=0x20, mmComparePulse=0x30, mmCompare1=0x40, mmCompare2=0x50, mmCompare3=0x60, mmCompare4=0x70};
enum TmrSlaveMode_t {smDisable=0, smEncoder1=1, smEncoder2=2, smEncoder3=3, smReset=4, smGated=5, smTrigger=6, smExternal=7};
enum Inverted_t {invNotInverted, invInverted};

class Timer_t {
private:
    TIM_TypeDef* ITmr;
    uint32_t *PClk;
public:
    __IO uint32_t *PCCR;    // Made public to allow DMA
    // Common
    void Init(TIM_TypeDef* Tmr);
    void Deinit();
    inline void Enable()  { ITmr->CR1 |=  TIM_CR1_CEN; }
    inline void Disable() { ITmr->CR1 &= ~TIM_CR1_CEN; }
    inline void SetUpdateFrequency(uint32_t FreqHz) { SetTopValue(*PClk / FreqHz); }
    inline void SetTopValue(uint32_t Value) { ITmr->ARR = Value; }
    inline uint32_t GetTopValue() { return ITmr->ARR; }
    inline void SetupPrescaler(uint32_t PrescaledFreqHz) { ITmr->PSC = (*PClk / PrescaledFreqHz) - 1; }
    inline void SetCounter(uint32_t Value) { ITmr->CNT = Value; }
    inline uint32_t GetCounter() { return ITmr->CNT; }
    // Master/Slave
    inline void SetTriggerInput(TmrTrigInput_t TrgInput) {
        uint16_t tmp = ITmr->SMCR;
        tmp &= ~TIM_SMCR_TS;   // Clear bits
        tmp |= (uint16_t)TrgInput;
        ITmr->SMCR = tmp;
    }
    inline void MasterModeSelect(TmrMasterMode_t MasterMode) {
        uint16_t tmp = ITmr->CR2;
        tmp &= ~TIM_CR2_MMS;
        tmp |= (uint16_t)MasterMode;
        ITmr->CR2 = tmp;
    }
    inline void SlaveModeSelect(TmrSlaveMode_t SlaveMode) {
        uint16_t tmp = ITmr->SMCR;
        tmp &= ~TIM_SMCR_SMS;
        tmp |= (uint16_t)SlaveMode;
        ITmr->SMCR = tmp;
    }
    // DMA, Irq, Evt
    inline void DmaOnTriggerEnable() { ITmr->DIER |= TIM_DIER_TDE; }
    inline void GenerateUpdateEvt()  { ITmr->EGR = TIM_EGR_UG; }
    inline void IrqOnTriggerEnable() { ITmr->DIER |= TIM_DIER_UIE; }
    inline void ClearIrqPendingBit() { ITmr->SR &= ~TIM_SR_UIF;    }
    // PWM
    void PwmInit(GPIO_TypeDef *GPIO, uint16_t N, uint8_t Chnl, Inverted_t Inverted);
    void PwmSet(uint16_t Value) { *PCCR = Value; }
};
#endif

#if 1 // =============================== IWDG ==================================
enum IwdgPre_t {
    iwdgPre4 = 0b000,
    iwdgPre8 = 0b001,
    iwdgPre16 = 0b010,
    iwdgPre32 = 0b011,
    iwdgPre64 = 0b100,
    iwdgPre128 = 0b101,
    iwdgPre256 = 0b110
};

class IWDG_t {
private:
    void EnableAccess() { IWDG->KR = 0x5555; }
    void SetPrescaler(IwdgPre_t Prescaler) { IWDG->PR = (uint32_t)Prescaler; }
    void SetReload(uint16_t Reload) { IWDG->RLR = Reload; }
public:
    void Reload() { IWDG->KR = 0xAAAA; }
    void Enable() { IWDG->KR = 0xCCCC; }
    void SetTimeout(uint32_t ms) {
        EnableAccess();
        SetPrescaler(iwdgPre256);
        uint32_t Count = (ms * (LSI_FREQ_HZ/1000)) / 256;
        TRIM_VALUE(Count, 0xFFF);
        SetReload(Count);
        Reload();
    }
    bool ResetOccured() {
        if(RCC->CSR & RCC_CSR_WDGRSTF) {
            RCC->CSR |= RCC_CSR_RMVF;   // Clear flags
            return true;
        }
        else return false;
    }
};
#endif

#if 1 // ============================== SPI ====================================
enum CPHA_t {cphaFirstEdge, cphaSecondEdge};
enum CPOL_t {cpolIdleLow, cpolIdleHigh};
enum SpiBaudrate_t {
    sbFdiv2   = 0b000,
    sbFdiv4   = 0b001,
    sbFdiv8   = 0b010,
    sbFdiv16  = 0b011,
    sbFdiv32  = 0b100,
    sbFdiv64  = 0b101,
    sbFdiv128 = 0b110,
    sbFdiv256 = 0b111,
};

class Spi_t {
private:
    SPI_TypeDef *PSpi;
public:
    void Setup(SPI_TypeDef *Spi, BitOrder_t BitOrder,
            CPOL_t CPOL, CPHA_t CPHA, SpiBaudrate_t Baudrate) {
        PSpi = Spi;
        // Clocking
        if     (PSpi == SPI1) { rccEnableSPI1(FALSE); }
        else if(PSpi == SPI2) { rccEnableSPI2(FALSE); }
        // Mode: Master, NSS software controlled and is 1, 8bit, NoCRC, FullDuplex
        uint16_t tmp = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR;
        if(BitOrder == boLSB) tmp |= SPI_CR1_LSBFIRST;    // MSB/LSB
        if(CPOL == cpolIdleHigh) tmp |= SPI_CR1_CPOL;     // CPOL
        if(CPHA == cphaSecondEdge) tmp |= SPI_CR1_CPHA;   // CPHA
        tmp |= ((uint16_t)Baudrate) << 3;                 // Baudrate
        PSpi->CR1 = tmp;
        PSpi->CR2 = 0;
        PSpi->I2SCFGR &= ~((uint16_t)SPI_I2SCFGR_I2SMOD);       // Disable I2S
    }
    void Enable () { PSpi->CR1 |=  SPI_CR1_SPE; }
    void Disable() { PSpi->CR1 &= ~SPI_CR1_SPE; }
    uint8_t ReadWriteByte(uint8_t AByte) {
        PSpi->DR = AByte;
        while(!(PSpi->SR & SPI_SR_RXNE));  // Wait for SPI transmission to complete
        return PSpi->DR;
    }
    // Flags
    void WaitBsyLo() { while(PSpi->SR & SPI_SR_BSY); }
    void WaitTxEHi() { while(!(PSpi->SR & SPI_SR_TXE)); }
    void ClearOVR() { (void)PSpi->DR; (void)PSpi->SR; (void)PSpi->DR; }
    // IRQ
    void SetupIrq(uint32_t Priority) {
        if     (PSpi == SPI1) nvicEnableVector(SPI1_IRQn, CORTEX_PRIORITY_MASK(Priority));
        else if(PSpi == SPI2) nvicEnableVector(SPI2_IRQn, CORTEX_PRIORITY_MASK(Priority));
    }
    void EnableIrqRxNE()  { PSpi->CR2 |=  SPI_CR2_RXNEIE; }
    void EnableIrqTxE()   { PSpi->CR2 |=  SPI_CR2_TXEIE;  }
    void DisableIrqRxNE() { PSpi->CR2 &= ~SPI_CR2_RXNEIE; }
    void DisableIrqTxE()  { PSpi->CR2 &= ~SPI_CR2_TXEIE;  }
    // DMA
    void EnableTxDma() { PSpi->CR2 |= SPI_CR2_TXDMAEN; }
    void EnableRxDma() { PSpi->CR2 |= SPI_CR2_RXDMAEN; }
};
#endif

#if 1 // ============================== I2C ====================================
#define I2C_DMATX_MODE  STM32_DMA_CR_CHSEL(DmaChnl) |   \
                        DMA_PRIORITY_LOW | \
                        STM32_DMA_CR_MSIZE_BYTE | \
                        STM32_DMA_CR_PSIZE_BYTE | \
                        STM32_DMA_CR_MINC |     /* Memory pointer increase */ \
                        STM32_DMA_CR_DIR_M2P |  /* Direction is memory to peripheral */ \
                        STM32_DMA_CR_TCIE       /* Enable Transmission Complete IRQ */

#define I2C_DMARX_MODE  STM32_DMA_CR_CHSEL(DmaChnl) |   \
                        DMA_PRIORITY_LOW | \
                        STM32_DMA_CR_MSIZE_BYTE | \
                        STM32_DMA_CR_PSIZE_BYTE | \
                        STM32_DMA_CR_MINC |         /* Memory pointer increase */ \
                        STM32_DMA_CR_DIR_P2M |      /* Direction is peripheral to memory */ \
                        STM32_DMA_CR_TCIE           /* Enable Transmission Complete IRQ */

class i2c_t {
private:
    uint16_t DmaChnl;
    I2C_TypeDef *ii2c;
    GPIO_TypeDef *IPGpio;
    uint16_t ISclPin, ISdaPin;
    uint32_t IBitrateHz;
    void SendStart()  { ii2c->CR1 |= I2C_CR1_START; }
    void SendStop()   { ii2c->CR1 |= I2C_CR1_STOP; }
    void AckEnable()  { ii2c->CR1 |= I2C_CR1_ACK; }
    void AckDisable() { ii2c->CR1 &= ~I2C_CR1_ACK; }
    bool RxIsNotEmpty()  { return (ii2c->SR1 & I2C_SR1_RXNE); }
    void ClearAddrFlag() { (void)ii2c->SR1; (void)ii2c->SR2; }
    void DmaLastTransferSet() { ii2c->CR2 |= I2C_CR2_LAST; }
    // Address and data
    void SendAddrWithWrite(uint8_t Addr) { ii2c->DR = (uint8_t)(Addr<<1); }
    void SendAddrWithRead (uint8_t Addr) { ii2c->DR = ((uint8_t)(Addr<<1)) | 0x01; }
    void SendData(uint8_t b) { ii2c->DR = b; }
    uint8_t ReceiveData() { return ii2c->DR; }
    // Flags operations
    uint8_t IBusyWait();
    uint8_t WaitEv5();
    uint8_t WaitEv6();
    uint8_t WaitEv8();
    uint8_t WaitAck();
    uint8_t WaitRx();
    uint8_t WaitStop();
    uint8_t WaitBTF();
public:
    bool Error;
    Thread *PRequestingThread;
    const stm32_dma_stream_t *PDmaTx, *PDmaRx;
    void Init(I2C_TypeDef *pi2c, GPIO_TypeDef *PGpio, uint16_t SclPin, uint16_t SdaPin, uint32_t BitrateHz,
            const stm32_dma_stream_t *APDmaTx, const stm32_dma_stream_t *APDmaRx);
    void Standby();
    void Resume();
    void Reset();
    uint8_t CmdWriteRead(uint8_t Addr, uint8_t *WPtr, uint8_t WLength, uint8_t *RPtr, uint8_t RLength);
    uint8_t CmdWriteWrite(uint8_t Addr, uint8_t *WPtr1, uint8_t WLength1, uint8_t *WPtr2, uint8_t WLength2);
};
#endif

#endif /* KL_LIB_F2XX_H_ */
