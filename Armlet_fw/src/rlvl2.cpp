/*
 * rlvl2.cpp
 *
 *  Created on: 01 марта 2014 г.
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
    PInTbl->RowCnt = rand() % 5;
    for(uint8_t i=0; i<PInTbl->RowCnt; i++) {
         int tmpval=rand() % 5;
        int is_new=1;
        //
        while(is_new<0)
        {
            for(int j=0;j<i;j++)
            {
               if(PInTbl->Rows[i].ID==tmpval)
                   is_new=-1;
            }
            if(is_new==-1)
            {
            tmpval++;
            tmpval = tmpval %5;
            }
            else
                continue;
        }

        PInTbl->Rows[i].ID=tmpval;
      //  PInTbl->Rows[i].ID = rand() % 5;

        PInTbl->Rows[i].Level = rand() % 10 + 1;
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
