/*
 * pill.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: g.kruglov
 */

#include "pill.h"

Pill_t Pill[PILL_CNT];
bool PillsHaveChanged;

static i2c_t i2c;

void PillReset();

// =============================== Checker =====================================
void PillChecker() {
    bool OldState;
    // Check if i2c error
    if(i2c.Error) {
        Uart.Printf("Err\r");
        PillReset();
    }
    // Discover if pill status changed
    PillsHaveChanged = false;
    for(uint8_t i=0; i<PILL_CNT; i++) {
        OldState = Pill[i].Connected;       // Save current state
        Pill[i].CheckIfConnected();
        if(Pill[i].Connected != OldState) PillsHaveChanged = true;
    } // for
}

void PillDeinit() {
    PinClear(PERIPH_PWR_GPIO, PERIPH_PWR_PIN);
    chThdSleepMilliseconds(1);  // Allow power to fade
    PinSetupAnalog(PERIPH_PWR_GPIO, PERIPH_PWR_PIN);    // Power
    i2c.Standby();
}

void PillInit() {
    PillDeinit();
    PinSetupOut(PERIPH_PWR_GPIO, PERIPH_PWR_PIN, omPushPull);   // Power
    PinSet(PERIPH_PWR_GPIO, PERIPH_PWR_PIN);
    chThdSleepMilliseconds(1);  // Allow power to rise
    i2c.Init(PILL_I2C, PILL_I2C_GPIO, PILL_SCL_PIN, PILL_SDA_PIN, PILL_I2C_BITRATE_HZ, PILL_DMATX, PILL_DMARX);
    // Pills
    for(uint8_t i=0; i<PILL_CNT; i++) Pill[i].Init(i);
}

void PillReset() {
    PillDeinit();
    PinSetupOut(PERIPH_PWR_GPIO, PERIPH_PWR_PIN, omPushPull);   // Power
    PinSet(PERIPH_PWR_GPIO, PERIPH_PWR_PIN);
    chThdSleepMilliseconds(1);  // Allow power to rise
    i2c.Resume();
}

// =============================== Pill_t ======================================
void Pill_t::Init(uint8_t IcAddr) {
    IAddr = IcAddr;
    Connected = false;
}

uint8_t Pill_t::Read(uint8_t *Ptr, uint8_t Length) {
    uint8_t WordAddress = PILL_START_ADDR;
    uint8_t Rslt = i2c.CmdWriteRead(EEADDR+IAddr, &WordAddress, 1, Ptr, Length);
    Connected = (Rslt == OK);
    if(i2c.Error == true) PillReset();
    return Rslt;
}

uint8_t Pill_t::CheckIfConnected() {
    uint8_t Rslt = i2c.CmdWriteWrite(EEADDR+IAddr, NULL, 0, NULL, 0);
    Connected = (Rslt == OK);
    if(i2c.Error == true) PillReset();
    return Rslt;
}

uint8_t Pill_t::Write(uint8_t *Ptr, uint8_t Length) {
    uint8_t WordAddress = PILL_START_ADDR;
    uint8_t Rslt = OK;
    // Write page by page
    while(Length) {
        uint8_t ToWriteCnt = (Length > PILL_PAGE_SZ)? PILL_PAGE_SZ : Length;
        Rslt = i2c.CmdWriteWrite(EEADDR+IAddr, &WordAddress, 1, Ptr, ToWriteCnt);
        if(Rslt == OK) {
            chThdSleepMilliseconds(5);   // Allow memory to complete writing
            Length -= ToWriteCnt;
            Ptr += ToWriteCnt;
            WordAddress += ToWriteCnt;
        }
        else break;
    }
    Connected = (Rslt == OK);
    if(i2c.Error == true) PillReset();
    return Rslt;
}
