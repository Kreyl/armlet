/*
 * power.cpp
 *
 *  Created on: 09.04.2013
 *      Author: kreyl
 */

#include "power.h"
#include "keys.h"
#include "peripheral.h"
#include "lvl1_assym.h"
#include "infrared.h"

Pwr_t Power;
static Thread *PThr;

// Adc callback function
//static void AdcCalback(ADCDriver *adcp, adcsample_t *buffer, size_t n);
// Total number of channels to be sampled by a single ADC operation
#define ADC_GRP1_NUM_CHANNELS   1
// Depth of the conversion buffer, channels are sampled four times each
#define ADC_GRP1_BUF_DEPTH      4
// ADC samples buffer
//static adcsample_t ISamples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

// ADC conversion group
// Mode:        Linear buffer, 4 samples of 1 channel, SW triggered.
// Channels:    IN11   (48 cycles sample time)
//static const ADCConversionGroup AdcGrpCfg = {
//    FALSE,
//    ADC_GRP1_NUM_CHANNELS,
//    AdcCalback,         // Callback function associated to the group
//    NULL,               // Error callback
//    // ==== HW dependent part ====
//    // ADC CR1 register initialization data
//    0,
//    // ADC CR2 register initialization data
//    ADC_CR2_SWSTART,    // Start conversion on ADC start
//    // ADC SMPR1: sample times for channels 10...18.
//    ADC_SMPR1_SMP_AN10(ADC_SAMPLE_56),
//    // ADC SMPR2: sample times for channels 0...9.
//    0,
//    //  ADC SQR1: Conversion group sequence 13...16 + sequence length
//    ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
//    // ADC SQR2: Conversion group sequence 7...12
//    0,
//    // ADC SQR3: Conversion group sequence 1...6
//    ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
//};

// ============================== Implementation ===============================
static WORKING_AREA(waPwrThread, 128);
static msg_t PwrThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Pwr");
    Power.Task();
    return 0;
}

void Pwr_t::Task() {
    bool WasExternal = false;
    while(1) {
        chThdSleepMilliseconds(PWR_MEASUREMENT_INTERVAL_MS);
        // Check if power src changed
        if(WasExternal and !ExternalPwrOn()) {
            WasExternal = false;
            chEvtBroadcast(&IEvtSrcPwrChange);
            //Uart.Printf("Disconnected\r");
        }
        else if(!WasExternal and ExternalPwrOn()) {
            WasExternal = true;
            chEvtBroadcast(&IEvtSrcPwrChange);
            //Uart.Printf("Connected\r");
        }

        //if(IsCharging()) Uart.Printf("Charging\r");

        // ==== ADC ====
//        adcStart(&ADCD1, NULL);
//        adcStartConversion(&ADCD1, &AdcGrpCfg, ISamples, ADC_GRP1_BUF_DEPTH);
//        chSysLock();
//        chSchGoSleepS(THD_STATE_SUSPENDED); // Will wake by Conversion completed Irq
//        chSysUnlock();
//        adcStop(&ADCD1);    // Stop ADC to save power
//
//        adcsample_t v = (ISamples[0] + ISamples[1] + ISamples[2] + ISamples[3]) / 4;
//        // Calculate voltage
//        int32_t tmp = 2 * v * ADC_VREF_MV / 4095;
//        Power.Voltage_mV = (uint16_t)tmp;
//        // Calculate percent
//        tmp = ((tmp - BAT_0_PERCENT_MV) * 100) / (BAT_100_PERCENT_MV - BAT_0_PERCENT_MV);
//        if(tmp < 0) tmp = 0;
//        if(tmp > 100) tmp = 100;
//        Power.RemainingPercent = (uint8_t)tmp;
//        //Uart.Printf("Adc=%u; U=%u; %=%u\r", v, Power.Voltage_mV, Power.RemainingPercent);
    }
}

// ADC end conversion callback
//void AdcCalback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
//    (void) buffer; (void) n;
//    chSysLockFromIsr();
//    if(PThr->p_state == THD_STATE_SUSPENDED) chSchReadyI(PThr);
//    chSysUnlockFromIsr();
//}

// Adc is initialized within HAL init
void Pwr_t::Init() {
    // GPIO
    PinSetupIn(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN, pudPullDown);
    PinSetupIn(PWR_CHARGING_GPIO, PWR_CHARGING_PIN, pudPullUp);
    PinSetupAnalog(PWR_BATTERY_GPIO,  PWR_BATTERY_PIN);
    // Event
    chEvtInit(&IEvtSrcPwrChange);
    // Create and start thread
    PThr = chThdCreateStatic(waPwrThread, sizeof(waPwrThread), LOWPRIO, PwrThread, NULL);
}

//static RTCWakeup wakeupspec;

void Pwr_t::EnterStandby() {
//    // Clear flags to setup wakeup. Will not work without this.
//    RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F | RTC_ISR_TSOVF | RTC_ISR_TSF);
//    wakeupspec.wakeup = ((uint32_t)4) << 16;    // select 1 Hz clock source
//    wakeupspec.wakeup |= 9;                     // set counter value to 9. Period will be 9+1 seconds
//    rtcSetPeriodicWakeup_v2(&RTCD1, &wakeupspec);
//    //Uart.Printf("%06X, %06X, %06X, %06X\r", RTC->CR, RTC->ISR, RTC->PRER, RTC->WUTR);
//    // Enter standby
//    chSysLock();
//    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // Set DEEPSLEEP bit
//    // Flash stopped in stop mode, Enter Standby mode
//    PWR->CR = PWR_CR_FPDS | PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF;
//    RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F | RTC_ISR_TSOVF | RTC_ISR_TSF);
//    __WFI();
//    chSysUnlock();
}

void ShutdownPeriphery() {
    // Shutdown periphery
    KeysShutdown();
    Beeper.Shutdown();
    Vibro.Shutdown();
    IR.Shutdown();
    rLevel1.Shutdown();
}

