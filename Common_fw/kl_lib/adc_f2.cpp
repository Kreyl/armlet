/*
 * adc_f2.cpp
 *
 *  Created on: 25 рту. 2013 у.
 *      Author: kreyl
 */

#include "adc_f2.h"

void Adc_t::Init() {
    rccEnableADC1(FALSE);   // Enable digital clock
    SetupClk(adcDiv4);      // Setup ADCCLK
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;
    Enable();               // May configure only enabled ADC
    SetChannelCount(ADC_CHANNEL_CNT);
    for(uint8_t i = 0; i < ADC_CHANNEL_CNT; i++) ChannelConfig(AdcChannels[i]);
}

void Adc_t::SetChannelCount(uint32_t Count) {
    ADC1->SQR1 &= ~ADC_SQR1_L;  // Clear count
    ADC1->SQR1 |= (Count - 1) << 20;
}

void Adc_t::ChannelConfig(AdcChnl_t ACfg) {
    uint32_t Offset;
    // Setup sampling time of the channel
    if(ACfg.N <= 9) {
        Offset = ACfg.N * 3;
        ADC1->SMPR2 &= ~((uint32_t)0b111 << Offset);    // Clear bits
        ADC1->SMPR2 |= ACfg.SampleTime << Offset;       // Set new bits
    }
    else {
        Offset = (ACfg.N - 10) * 3;
        ADC1->SMPR1 &= ~((uint32_t)0b111 << Offset);
        ADC1->SMPR1 |= ACfg.SampleTime << Offset;
    }
    // Put channel number in appropriate place in sequence
    if(ACfg.SeqIndx <= 6) {
        Offset = (ACfg.SeqIndx - 1) * 5;
        ADC1->SQR3 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR3 |= (uint32_t)(ACfg.N << Offset);
    }
    else if(ACfg.SeqIndx <= 12) {
        Offset = (ACfg.SeqIndx - 7) * 5;
        ADC1->SQR2 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR2 |= (uint32_t)(ACfg.N << Offset);
    }
    else if(ACfg.SeqIndx <= 16) {
        Offset = (ACfg.SeqIndx - 13) * 5;
        ADC1->SQR1 &= ~(uint32_t)(0b11111 << Offset);
        ADC1->SQR1 |= (uint32_t)(ACfg.N << Offset);
    }
}
