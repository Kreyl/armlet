/*
 * real_time.cpp
 *
 *  Created on: 07 февр. 2014 г.
 *      Author: r.leonov
 */


#include "real_time.h"

Time_t RTU; // Real Time Unit, based on RTC

void Time_t::Init() {
    rccEnableAPB1(RCC_APB1ENR_PWREN, false);
    WaitConfiguration();

}


uint8_t Time_t::WaitConfiguration() {
    uint8_t Status;
    uint32_t SynchroCounter = 0;
    DisableWriteProtection();
    do {
        if(RTC->ISR & RTC_ISR_RSF) {
            Status = 0;
            continue;
        }
        SynchroCounter++;
    } while(SynchroCounter < SYNCHRO_TIMEOUT);
    Status = 1;
    EnableWriteProtection();
    return Status;
}

uint8_t Time_t::EnterInit() {
    uint32_t SynchroCounter = 0;
    if(RTC->ISR & RTC_ISR_INITF) return 0;  // if already in init mode
    RTC->ISR = RTC_ISR_INIT;
    do {
        if(RTC->ISR & RTC_ISR_INITF) return 0;
        SynchroCounter++;
    } while(SynchroCounter < SYNCHRO_TIMEOUT);
    return 1;
}


