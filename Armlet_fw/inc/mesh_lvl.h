/*
 * mesh_lvl.h
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */

#ifndef MESH_LVL_H_
#define MESH_LVL_H_

#include "kl_lib_f2xx.h"
#include "evt_mask.h"
#include "radio_lvl1.h"
#include "msg_box.h"

//#define SELF_ID             15
#define MAX_ABONENTS        100   // max ID, started from 1

#define MESH_CHANNEL        1
#define SLOT_TIME           4  // (uint32_t)( (((BYTE_RATE * (RPKT_SZ + 8)) * 5) * 2) / 1000)  // ms

#define COUNT_OF_CYCLES     10
#define CYCLE_TIME          (uint32_t)(SLOT_TIME * MAX_ABONENTS)
#define S_CYCLE_TIME        (uint32_t)(CYCLE_TIME * COUNT_OF_CYCLES)



#define GET_RND_VALUE(Top)  ( ( (Random(chTimeNow()) ) % Top ))
#define START_OF_EPOCH      4000 // ms
#define END_OF_EPOCH        4294967295 // ms = 2^32
#define TIME_AGE_THRESHOLD  99


#if 1 // ======================== Circ Buf of Pkt ==============================
#define CIRC_PKT_BUF_SZ     5 // MAX_ABONENTS FIXME: 5 size set to debug only

class CircBufPkt_t {
private:
    mshMsg_t PktBuf[CIRC_PKT_BUF_SZ];
    mshMsg_t *PRPkt;
    mshMsg_t *PWPkt;
    uint8_t FilledCount;
public:
    CircBufPkt_t() :
        PRPkt(PktBuf), PWPkt(PktBuf),
        FilledCount(0) {}

    uint8_t GetFilledSlots()        { return (uint8_t)FilledCount; }
    uint8_t GetFreeSlots()          { return (uint8_t)(MAX_ABONENTS-FilledCount); }
    void WritePkt(mshMsg_t Ptr)     { *PWPkt++ = Ptr; FilledCount++; if(PWPkt >= (PktBuf + CIRC_PKT_BUF_SZ)) PWPkt = PktBuf; }
    void ReadPkt(mshMsg_t *Ptr)     { *Ptr = *PRPkt++; if(PRPkt >= (PktBuf + CIRC_PKT_BUF_SZ)) PRPkt = PktBuf; FilledCount--;}

};


#endif


#if 1// ============================== Mesh Class =============================
#define MESH_TIM        TIM5
#define RND_TBL_BUFFER_SZ    50

class Mesh_t {
private:
    uint8_t RndTableBuf[RND_TBL_BUFFER_SZ];
    uint8_t *PRndTable;
    uint32_t AbsCycle;
    uint32_t CurrCycle;
    uint32_t RxCycleN;
    uint32_t SleepTime;
    bool NeedUpdateTime;
    uint32_t SelfID;

    Timer_t CycleTmr;
    void NewRxCycle()       { RxCycleN = *PRndTable; PRndTable++; if(PRndTable > RndTableBuf + RND_TBL_BUFFER_SZ) PRndTable = RndTableBuf;   }
    void IncCurrCycle()     { AbsCycle++; CurrCycle++; if(CurrCycle >= COUNT_OF_CYCLES) { CurrCycle = 0; NewRxCycle(); } }

public:
    Mesh_t() :  PRndTable(RndTableBuf),
                AbsCycle(0),
                CurrCycle(COUNT_OF_CYCLES),
                RxCycleN(*PRndTable),
                SleepTime(0),
                NeedUpdateTime(false),
                SelfID(0),
                CycleTmr(MESH_TIM),
                IPThread(NULL) {}

    Thread *IPThread;
    CircBufPkt_t PktBuf;
    uint32_t GetAbsTime()       { return (AbsCycle);             }
    uint32_t GetAbsTimeMS()     { return (AbsCycle*CYCLE_TIME);  }
    void SetCurrCycleN(uint32_t ANew)   { AbsCycle = ANew; CurrCycle = 0; NewRxCycle(); }
    MsgBox_t<mshMsg_t, RPKT_SZ> MsgBox;
    void Init(uint32_t ID);
    void ITask();

    void IIrqHandler();
};

extern Mesh_t Mesh;
#endif

#endif /* MESH_LVL_H_ */
