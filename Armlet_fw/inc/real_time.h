/*
 * real_time.h
 *
 *  Created on: 07 февр. 2014 г.
 *      Author: r.leonov
 */

#ifndef REAL_TIME_H_
#define REAL_TIME_H_

#include "kl_lib_f2xx.h"

#define SYNCHRO_TIMEOUT         8000

struct TimeContanier_t {
private:
public:
    union {
        uint8_t TimeBuf[3];
        struct {
            uint8_t HH;
            uint8_t MM;
            uint8_t SS;
        };
    }__attribute__ ((__packed__));
    void ResetTime()    { HH = 0; MM = 0; SS = 0; }
    void SetTime(uint32_t Ams) {
        HH = (Ams/1000)/3600;
        MM = ((Ams/1000)/60) - (HH*60);
        SS = (Ams/1000) - ((HH*3600) + (MM*60));
    }
};

class Time_t {
private:
    TimeContanier_t Time, *PTime;
    void DisableWriteProtection()   { RTC->WPR = 0xCA; RTC->WPR = 0x53; }
    void EnableWriteProtection()    { RTC->WPR = 0XFF; }
    uint8_t WaitConfiguration();
    uint8_t EnterInit();
    void ExitInit()                 { RTC->ISR &= RTC_ISR_INIT; }
    void EnableBackup()             { PWR->CR |= PWR_CR_DBP; }
public:
    Time_t() : PTime(&Time) {}
    void Init();
};

extern Time_t RTU;

#endif /* REAL_TIME_H_ */
