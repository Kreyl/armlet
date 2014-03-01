/*
 * rlvl2.cpp
 *
 *  Created on: 01 ����� 2014 �.
 *      Author: Kreyl
 */

#include "rlvl2.h"

#include "ch.h"
#include "application.h"
#include "evt_mask.h"
#include <cstdlib>

rLevel2_t RLvl2;

static WORKING_AREA(waRlvl2Thread, 256);
static void Rlvl2Thread(void *arg) {
    chRegSetThreadName("Rlvl2");
    while(true) RLvl2.ITask();
}

void rLevel2_t::ITask() {
    // DEBUG
    PInTbl->RowCnt = rand() % 7;
    for(uint8_t i=0; i<PInTbl->RowCnt; i++) {
        PInTbl->Rows[i].ID = rand() % 50;
        PInTbl->Rows[i].Level = rand() % 100 + 1;
    }
    chThdSleepMilliseconds(999);
    // Switch tables
    chSysLock();
    PTable = PInTbl;
    if(PInTbl == &ITbl[0]) PInTbl = &ITbl[1];
    else PInTbl = &ITbl[0];
    chSysUnlock();
    chEvtSignal(App.PThd, EVTMASK_RADIO);
}

void rLevel2_t::Init() {
    PTable = &ITbl[0];
    PInTbl = &ITbl[1];
    chThdCreateStatic(waRlvl2Thread, sizeof(waRlvl2Thread), NORMALPRIO, (tfunc_t)Rlvl2Thread, NULL);
}
