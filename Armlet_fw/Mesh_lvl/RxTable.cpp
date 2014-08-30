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

RESULT RxTable_t::PutRxInfo(uint16_t ID, int8_t RSSI, state_t *P) {
    chSemWait(&WriteFlag);
    RESULT rslt = OK;
    uint8_t Level = 0;
    if(ID == App.ID) goto lblPutRxEnd;  // Do not add self ID
    if(PCurrTbl->Size >= RX_TABLE_SZ) goto lblPutRxEnd; // Table is full
    /* Get Level (in %) from RSSI (in dBm) */
    RSSI::Cut(RSSI, &RSSI);
    RSSI::ToPercent(RSSI, &Level);
//    Uart.Printf("\r\n[RxTable.cpp] Put:%u,%d,%u", ID, RSSI, Level);
    // Iterate IDs to find same ID if any
    for(uint32_t i=0; i<PCurrTbl->Size; i++) {
        if(PCurrTbl->Row[i].ID == ID) {
            PCurrTbl->Row[i].Level   = MAX(PCurrTbl->Row[i].Level, Level);
            if(P != nullptr) PCurrTbl->Row[i].State  = *P;
            else PCurrTbl->Row[i].State  = (state_t){0,0,0};
            goto lblPutRxEnd;
        }
    }
    // No such ID found, append table
    PCurrTbl->Row[PCurrTbl->Size].ID = ID;
    PCurrTbl->Row[PCurrTbl->Size].Level = Level;
    if(P != nullptr) PCurrTbl->Row[PCurrTbl->Size].State = *P;
    else PCurrTbl->Row[PCurrTbl->Size].State  = (state_t){0,0,0};
    PCurrTbl->Size++;
    lblPutRxEnd:
    chSemSignal(&WriteFlag);
    return rslt;
}

void RxTable_t::SendEvtReady() {
    chSysLock();
    ISwitchTable();
    if(App.PThd != nullptr) chEvtSignalI(App.PThd, RX_TABLE_READY_EVT);
    else Uart.Printf("\r[RxTable.cpp] IPthd = nullptr");
    chSysUnlock();
}
