/*
 * SensorTable.cpp
 *
 *  Created on: 02 ����. 2014 �.
 *      Author: Kreyl
 */

#include "ch.h"
#include "SensorTable.h"
#include "evt_mask.h"

SnsTable_t SnsTable;

void SnsTable_t::ISwitchTableI() {
    if(PCurrTbl == &ITbl[0]) {
        PCurrTbl = &ITbl[1];
        PTable   = &ITbl[0];
    }
    else {
        PCurrTbl = &ITbl[0];
        PTable   = &ITbl[1];
    }
    PCurrTbl->Size = 0; // Reset table
}

void SnsTable_t::PutSnsInfo(uint8_t ID, uint8_t Level) {
    if(PCurrTbl->Size >= MAX_ROW_CNT) return;
    for(uint32_t i=0; i<PCurrTbl->Size; i++) {
        if(PCurrTbl->Row[i].ID == ID) {
            PCurrTbl->Row[i].Level = Level;
            return;
        }
    }
    PCurrTbl->Row[PCurrTbl->Size].ID = ID;
    PCurrTbl->Row[PCurrTbl->Size].Level = Level;
    PCurrTbl->Size++;
}

void SnsTable_t::SendEvtReady() {
    chSysLock();
    ISwitchTableI();
    if(IPThd != nullptr) chEvtSignalI(IPThd, EVTMSK_TABLE_READY);
    chSysUnlock();
}
