/*
 * power.h
 *
 *  Created on: 09.04.2013
 *      Author: kreyl
 */

#ifndef POWER_H_
#define POWER_H_

#include "kl_lib_f2xx.h"

#define PWR_EXTERNAL_PIN    9
#define PWR_EXTERNAL_GPIO   GPIOA
#define PWR_CHARGING_PIN    3
#define PWR_CHARGING_GPIO   GPIOC
#define PWR_BATTERY_PIN     0
#define PWR_BATTERY_GPIO    GPIOC

#define PWR_MEASUREMENT_INTERVAL_MS    999

// Adc
#define ADC_CHNL    10
// Constants
#define BAT_0_PERCENT_MV    3650
#define BAT_100_PERCENT_MV  4140
#define ADC_VREF_MV         3300

class Pwr_t {
private:
    EventSource IEvtSrcPwrChange;
public:
    void Task();
    bool ExternalPwrOn() { return  PinIsSet(PWR_EXTERNAL_GPIO, PWR_EXTERNAL_PIN); }
    bool IsCharging()    { return !PinIsSet(PWR_CHARGING_GPIO, PWR_CHARGING_PIN); }
    uint16_t Voltage_mV;
    uint8_t RemainingPercent;
    void RegisterEvtChange(EventListener *PEvtLstnr, uint8_t EvtMask) { chEvtRegisterMask(&IEvtSrcPwrChange, PEvtLstnr, EvtMask); }
    void Init();
    void EnterStandby();
};
extern Pwr_t Power;

void ShutdownPeriphery();

#endif /* POWER_H_ */
