/*
 * lvl1_assym.h
 *
 *  Created on: 28.03.2013
 *      Author: kreyl
 */

/*
 * Level 1 is responsible to sync rpkt exchange and therefore has
 * high priority.
 */

#ifndef LVL1_ASSYM_H_
#define LVL1_ASSYM_H_

#include "ch.h"
#include "hal.h"

// ============================== Pkt_t ========================================
struct rPkt_t {
    uint8_t ID;
    uint32_t CycleN;
    uint8_t TimeOwnerID;
    uint8_t TimeAge;
} __attribute__ ((__packed__));
#define RPKT_SZ     sizeof(rPkt_t)

// =========================== Address space ===================================
#define R_NO_ID                 0xFF
// Emanators
#define R_EMANATOR_BOTTOM_ID    120
#define R_EMANATOR_TOP_ID       122
#define R_EMANATOR_CNT          ((1+R_EMANATOR_TOP_ID)-R_EMANATOR_BOTTOM_ID)

// ============================== Timings ======================================

#if 1 //============================ Level1 ====================================
struct Row_t {
    uint8_t ID;
    uint8_t Level;
};

#define MAX_ROW_CNT     99
struct Table_t {
    uint32_t Size;
    Row_t Rows[MAX_ROW_CNT];
};

class rLevel1_t {
private:
    Thread *PAppThd;
    // ==== Rx ====
    Table_t ITable[2];
    rPkt_t PktRx;       // Local rPkt to receive
    // ==== Tx ====
    rPkt_t PktTx;       // Local rPkt to transmit
public:
    Table_t *PCurrentTable;
    void Init(uint8_t ASelfID);
    void Shutdown();
    void RegisterAppThd(Thread *PThd) { PAppThd = PThd; }
    // Inner use
    void ITask();
};

extern rLevel1_t rLevel1;
#endif

#endif /* LVL1_ASSYM_H_ */