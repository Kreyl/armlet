/*
 * mesh_lvl.h
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */

#ifndef MESH_LVL_H_
#define MESH_LVL_H_

#include "kl_lib_f2xx.h"
#include "cmd_uart.h"
#include "evt_mask.h"
#include "radio_lvl1.h"
#include "msg_box.h"
#include "BeepSequences.h"

/*********************** MESH ******************************
 *  |_|_|_|_..._|_|_|_|_|_..._|_|_|_|_|_..._|_|   SLOTS
 *  |_____________|_______...___|_____________|   CYCLES
 *  |_____________________..._________________|   SUPER_CYCLE
 */

#define TABLE_SEND_N        3     /* send SnsTable after n cycles */
#define MAX_ABONENTS        100   /* max ID, started from 1 */

#define MESH_CHANNEL        1     /* mesh RF channel */
#define SLOT_TIME           4     /* ms */

#define COUNT_OF_CYCLES     5     /* count of cycles in supercycle */
#define CYCLE_TIME          (uint32_t)(SLOT_TIME * MAX_ABONENTS)
#define S_CYCLE_TIME        (uint32_t)(CYCLE_TIME * COUNT_OF_CYCLES)


#define GET_RND_VALUE(Top)  ( ( (Random(chTimeNow()) ) % Top ))
#define START_OF_EPOCH      4000 // ms
#define END_OF_EPOCH        4294967295 // ms = 2^32

#define TIME_AGE_THRESHOLD  99 // in cycles


/* Time specific */
#define TIME_SZ             5  //  "hh:mm\0"
#define TIME_SZ_LONG        8   // "hh:mm:ss\0"
#define MESH_MS_IN_DAY      (uint32_t)86400000

class Time {
public:
    static void PutTimeTo(char *PStr, uint8_t HH, uint8_t MM, char Sep) {
        char digit;
        digit = HH/10;
        digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        PStr[0] = digit;
        digit = HH%10;
        PStr[1] = digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        PStr[2] = Sep;
        digit = MM/10;
        PStr[3] = digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        digit = MM%10;
        PStr[4] = (digit < 10)? '0'+digit : 'A'+digit-10;
    }
};


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
#define MESH_TIM            TIM5
#define RND_TBL_BUFFER_SZ   50

class Mesh_t {
private:
    uint8_t RndTableBuf[RND_TBL_BUFFER_SZ];
    uint8_t *PRndTable;
    uint32_t AbsCycle;
    uint32_t CurrCycle;
    uint32_t RxCycleN;
    uint32_t SleepTime;
    bool NeedUpdateTime;
    char TimeSeparator;
    uint32_t SelfID;
    uint8_t NeedToSendTable;

    Timer_t CycleTmr;
    void NewRxCycle()       { RxCycleN = *PRndTable; PRndTable++; if(PRndTable > RndTableBuf + RND_TBL_BUFFER_SZ) PRndTable = RndTableBuf;   }
    void IncCurrCycle()     { AbsCycle++; CurrCycle++; if(CurrCycle >= COUNT_OF_CYCLES) { CurrCycle = 0; NewRxCycle(); } }

    void NewCycle();
    void TableSend();
    void UpdateTimer(bool NeedUpdate, uint32_t NewTime, uint32_t WakeUpSysTime);
    bool DispatchPkt(uint32_t *PTime, uint32_t *PWakeUpSysTime);
    void ResetTimeAge(uint8_t ID)     { rLevel1.ResetTimeAge(ID);       }
    uint8_t GetTimeAge()              { return rLevel1.GetTimeAge();    }
    uint8_t GetMeshID()               { return rLevel1.GetTimeOwner();  }

public:
    Mesh_t() :  PRndTable(RndTableBuf),
                AbsCycle(0),
                CurrCycle(COUNT_OF_CYCLES),
                RxCycleN(*PRndTable),
                SleepTime(0),
                NeedUpdateTime(false),
                TimeSeparator(':'),
                SelfID(0),
                NeedToSendTable(0),
                CycleTmr(MESH_TIM),
                IPThread(NULL) {}

    Thread *IPThread;
    CircBufPkt_t PktBuf;
    uint32_t GetAbsTime()       { return (AbsCycle);             }
    uint32_t GetAbsTimeMS()     { return (AbsCycle*CYCLE_TIME);  }
    uint8_t GetAstronomicTime(char *PToStr);
    void SetCurrCycleN(uint32_t ANew)   { AbsCycle = ANew; CurrCycle = 0; NewRxCycle(); }
    MsgBox_t<mshMsg_t, RPKT_SZ> MsgBox;
    void Init(uint32_t ID);
    void ITask();

    void IIrqHandler();
};

extern Mesh_t Mesh;
#endif

#endif /* MESH_LVL_H_ */
