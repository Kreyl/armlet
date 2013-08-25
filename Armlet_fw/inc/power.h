/*
 * power.h
 *
 *  Created on: 09.04.2013
 *      Author: kreyl
 */

#ifndef POWER_H_
#define POWER_H_

#include "kl_lib_f2xx.h"
#include "adc_f2.h"

#define PWR_EXTERNAL_PIN    9
#define PWR_EXTERNAL_GPIO   GPIOA
#define PWR_CHARGING_PIN    3
#define PWR_CHARGING_GPIO   GPIOC
#define PWR_BATTERY_PIN     0
#define PWR_BATTERY_GPIO    GPIOC

#define PWR_MEASUREMENT_INTERVAL_MS    999

// Adc
#define ADC_CHNL            10
#define ADC_VREF_MV         3300

class Pwr_t {
private:
    Adc_t Adc;
    bool WasExternal;
//    EventSource IEvtSrcPwrChange;
    uint8_t mV2Percent(uint16_t mV);
public:
    void Task();
    bool ExternalPwrOn() { return  PinIsSet(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN); }
    bool IsCharging()    { return !PinIsSet(PWR_CHARGING_GPIO, PWR_CHARGING_PIN); }
    uint16_t Voltage_mV;
    uint8_t RemainingPercent;
//    void RegisterEvtChange(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcPwrChange, PEvtLstnr, EvtMask); }
    void Init();
    void EnterStandby();
};
extern Pwr_t Power;

void ShutdownPeriphery();

#endif /* POWER_H_ */
