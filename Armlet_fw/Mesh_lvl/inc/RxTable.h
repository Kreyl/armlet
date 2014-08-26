/*
 * RxTable.h
 *
 *  Created on: 08 июля 2014 г.
 *      Author: r.leonov
 */

#ifndef RXTABLE_H_
#define RXTABLE_H_

#include "kl_lib_f2xx.h"
#include "mesh_params.h"
#include "evt_mask.h"
#include "cmd_uart.h"
#include "application.h"

// IR
#define MAX_IR_LUSTRA_CNT       50
// Radio
#define BOTTOM_TRSHHLD_RSSI     -100
#define TOP_TRSHLD_RSSI         -35
#define RX_TABLE_READY_EVT      EVTMSK_SENS_TABLE_READY
#define RX_TABLE_SZ             (MAX_ABONENTS + MAX_IR_LUSTRA_CNT)

#define RESULT                  uint8_t

//pyton translation for db
//[22:19:36] Jolaf: str(tuple(1 + int(sqrt(float(i) / 65) * 99) for i in xrange(0, 65 + 1)))
const int DbTranslate[66] ={1, 13, 18, 22, 25, 28, 31, 33, 35, 37, 39, 41, 43, 45, 46, 48, 50, 51, 53, 54, 55, 57, 58, 59, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 86, 87, 88, 89, 90, 91, 92, 92, 93, 94, 95, 96, 96, 97, 98, 99, 100};


struct RxTableRow_t {
    uint16_t    ID;
    uint8_t     Level;
    state_t     State;
};

struct Table_t {
    uint32_t Size;
    RxTableRow_t Row[RX_TABLE_SZ];
};

class RSSI {
public:
    static void Cut(int8_t InRaram, int8_t *POut) {
        int8_t tmp = InRaram;
        if(tmp < BOTTOM_TRSHHLD_RSSI) tmp = BOTTOM_TRSHHLD_RSSI;
        else if(tmp > TOP_TRSHLD_RSSI) tmp = TOP_TRSHLD_RSSI;
        *POut = tmp;
    }

    static void ToPercent(int8_t InParam, uint8_t *POut) {
        InParam += 100;    // 0...65
        *POut = DbTranslate[InParam]; // now it in 1..100
    }
};

class RxTable_t {
private:
    Table_t ITbl[2], *PCurrTbl;
    Thread *IPThd;
    Semaphore WriteFlag;
    void ISwitchTable();
public:
    RxTable_t(): PCurrTbl(&ITbl[0]), IPThd(nullptr), PTable(&ITbl[1]) {
        chSemInit(&WriteFlag, 1);
    }
    Table_t *PTable;
    void RegisterAppThd(Thread *PThd) { IPThd = PThd; }
    RESULT PutRxInfo(uint16_t ID, int8_t RSSI, state_t *P);
    void SendEvtReady();
};

extern RxTable_t RxTable;


#endif /* RXTABLE_H_ */
