/*
 * kl_adc.h
 *
 *  Created on: 12.04.2013
 *      Author: kreyl
 */

#ifndef KL_ADC_H_
#define KL_ADC_H_

#include "kl_lib_f2xx.h"
#include "clocking.h"

// =============================== Constants ===================================
// ADC sampling_times
#define ADC_SampleTime_1_5Cycles                     ((uint32_t)0x00000000)
#define ADC_SampleTime_7_5Cycles                     ((uint32_t)0x00000001)
#define ADC_SampleTime_13_5Cycles                    ((uint32_t)0x00000002)
#define ADC_SampleTime_28_5Cycles                    ((uint32_t)0x00000003)
#define ADC_SampleTime_41_5Cycles                    ((uint32_t)0x00000004)
#define ADC_SampleTime_55_5Cycles                    ((uint32_t)0x00000005)
#define ADC_SampleTime_71_5Cycles                    ((uint32_t)0x00000006)
#define ADC_SampleTime_239_5Cycles                   ((uint32_t)0x00000007)

// DMA
#define ADC_DMA             STM32_DMA2_STREAM4
#define ADC_DMA_MODE        STM32_DMA_CR_CHSEL(0) |   /* DMA2 Stream4 Channel0 */ \
                            DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_HWORD | \
                            STM32_DMA_CR_PSIZE_HWORD | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                            STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */

// ================================= Config ====================================
struct AdcChnl_t {
    uint8_t N;              // Number of channel
    uint32_t SampleTime;
    uint32_t SeqIndx;       // Number of sequence, starts with 1, not zero
};

// ADC channels config: config it.
const AdcChnl_t AdcChannels[] = {
        {10, ADC_SampleTime_55_5Cycles, 1},
        {10, ADC_SampleTime_55_5Cycles, 2},
        {10, ADC_SampleTime_55_5Cycles, 3},
        {10, ADC_SampleTime_55_5Cycles, 4},
        {10, ADC_SampleTime_55_5Cycles, 5},
        {10, ADC_SampleTime_55_5Cycles, 6},
        {10, ADC_SampleTime_55_5Cycles, 7},
        {10, ADC_SampleTime_55_5Cycles, 8},
        {10, ADC_SampleTime_55_5Cycles, 9},
        {10, ADC_SampleTime_55_5Cycles, 10},
        {10, ADC_SampleTime_55_5Cycles, 11},
        {10, ADC_SampleTime_55_5Cycles, 12},
        {10, ADC_SampleTime_55_5Cycles, 13},
        {10, ADC_SampleTime_55_5Cycles, 14},
        {10, ADC_SampleTime_55_5Cycles, 15},
        {10, ADC_SampleTime_55_5Cycles, 16},
};

#define ADC_CHANNEL_CNT     countof(AdcChannels)
#define ADC_BUF_SZ          ADC_CHANNEL_CNT

// =================================== Types ===================================
enum ADCDiv_t {
    adcDiv2 = (uint32_t)(0b00 << 16),
    adcDiv4 = (uint32_t)(0b01 << 16),
    adcDiv6 = (uint32_t)(0b10 << 16),
    adcDiv8 = (uint32_t)(0b11 << 16),
};

class Adc_t {
private:
    inline void SetupClk(ADCDiv_t Div) { ADC->CCR |= (uint32_t)Div; }
    void SetChannelCount(uint32_t Count);
    void StartConversion() { ADC1->CR2 |= ADC_CR2_SWSTART; }
    void Enable() { ADC1->CR2 = ADC_CR2_ADON; }
    void ChannelConfig(AdcChnl_t ACfg);
//    void ContModeEnable() { ADC1->CFGR1 = ADC_CFGR1_CONT; }  // Enable continuos conversion
    inline bool ConversionCompleted() { return (ADC1->SR & ADC_SR_EOC); }
    inline uint16_t IResult() { return ADC1->DR; }
public:
    uint16_t Result[ADC_BUF_SZ];
    void Init();
    void Measure();
    void Disable();
    void ClockOff() { rccDisableADC1(FALSE); }
};

#endif /* KL_ADC_H_ */
