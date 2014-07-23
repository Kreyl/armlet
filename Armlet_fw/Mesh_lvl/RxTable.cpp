/*
 * RxTable.cpp
 *
 *  Created on: 08 июля 2014 г.
 *      Author: r.leonov
 */

#include "RxTable.h"

RxTable_t RxTable;

void RxTable_t::ISwitchTable() {
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

void RxTable_t::PutRxInfo(uint16_t ID, int8_t RSSI, state_t *P) {
    if(ID == App.ID) return; //
    /* Get Level (in %) from RSSI (in dBm) */
    uint8_t Level = 0;
    RSSI::Cut(RSSI, &RSSI);
    RSSI::ToPercent(RSSI, &Level);
//    Uart.Printf("[RxTable.cpp] Put:%u,%d,%u\r\n", ID, RSSI, Level);
    if(PCurrTbl->Size >= RX_TABLE_SZ) return;
    for(uint32_t i=0; i<PCurrTbl->Size; i++) {
        if(PCurrTbl->Row[i].ID == ID) {
            PCurrTbl->Row[i].Level   = MAX(PCurrTbl->Row[i].Level, Level);
            PCurrTbl->Row[i].State  = *P;
            return;
        }
    }
    PCurrTbl->Row[PCurrTbl->Size].ID = ID;
    PCurrTbl->Row[PCurrTbl->Size].Level = Level;
    PCurrTbl->Row[PCurrTbl->Size].State = *P;
    PCurrTbl->Size++;
}

void RxTable_t::SendEvtReady() {
    chSysLock();
    ISwitchTable();
    if(IPThd != nullptr) chEvtSignalI(IPThd, RX_TABLE_READY_EVT);
    else {
//        Uart.Printf("[RxTable.cpp] IPthd = nullptr\r\n");
    }
    chSysUnlock();
}
