/*
 * mesh_lvl.h
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */

#ifndef MESH_LVL_H_
#define MESH_LVL_H_

#include "evt_mask.h"
#include "cc1101.h"
#include "radio_lvl1.h"
#include "msg_box.h"
#include "stdlib.h"

#include "mesh_params.h"
#include "AlienTable.h"
#include "application.h"

#ifdef STM32F2XX
#include "kl_lib_f2xx.h"
#else
#include "kl_lib_L15x.h"
#endif

#if 1 // ======================== Circ Buf of Pkt ==============================
//#define CIRC_PKT_BUF_SZ     5 // 5 size set to debug only
#ifdef ARMLET
#define CIRC_PKT_BUF_SZ     MAX_ABONENTS
#else
#define CIRC_PKT_BUF_SZ     (MAX_ABONENTS/10)
#endif

struct CircBufString_t {
    uint32_t Timestamp;
    int8_t RSSI;
    MeshPkt_t RadioPkt;
};

class CircBufPkt_t {
private:
    CircBufString_t PktBuf[CIRC_PKT_BUF_SZ];
    CircBufString_t *PRPkt;
    CircBufString_t *PWPkt;
    uint8_t FilledCount;
public:
    CircBufPkt_t() :
        PRPkt(PktBuf), PWPkt(PktBuf),
        FilledCount(0) {}

    uint8_t GetFilledSlots()        { return (uint8_t)FilledCount; }
    uint8_t GetFreeSlots()          { return (uint8_t)(CIRC_PKT_BUF_SZ-FilledCount); }
    void WritePkt(mshMsg_t Ptr)     {
        PWPkt->Timestamp            = Ptr.Timestamp;
        PWPkt->RSSI                 = Ptr.RSSI;
        PWPkt->RadioPkt.SenderInfo  = Ptr.pRadioPkt->SenderInfo;
        PWPkt->RadioPkt.AlienID     = Ptr.pRadioPkt->AlienID;
        PWPkt->RadioPkt.AlienInfo   = Ptr.pRadioPkt->AlienInfo;
        PWPkt++;
        FilledCount++;
        if(PWPkt >= (PktBuf + CIRC_PKT_BUF_SZ)) PWPkt = PktBuf;
    }
    void ReadPkt(CircBufString_t *Ptr)     {
        *Ptr = *PRPkt++;
        if(PRPkt >= (PktBuf + CIRC_PKT_BUF_SZ)) PRPkt = PktBuf;
        FilledCount--;
    }
};
#endif

#if 1// ============================== Mesh Class =============================
#define MESH_TIM                TIM7
#define MESH_TIM_IRQ            TIM7_IRQn
#define MESH_TIM_IRQ_HANDLER    TIM7_IRQHandler

#define RND_TBL_BUFFER_SZ       25


class Time {
public:
    static void PutTimeTo(char *PStr, uint8_t HH, uint8_t MM, char Sep) {
        char digit;
        digit = HH/10;
        digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        if(digit != '0') { PStr[0] = digit; }
        else PStr[0] = ' ';
        digit = HH%10;
        PStr[1] = digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        PStr[2] = Sep;
        digit = MM/10;
        PStr[3] = digit = (digit < 10)? '0'+digit : 'A'+digit-10;
        digit = MM%10;
        PStr[4] = (digit < 10)? '0'+digit : 'A'+digit-10;
    }
    static void NotRelevant(char *PStr) {
        PStr[0] = PStr[1] = PStr[3] = PStr[4] = '-';
    }
};

class Mesh_t {
private:
    uint8_t RndTableBuf[RND_TBL_BUFFER_SZ];
    uint8_t *PRndTable;

    uint32_t AbsCycle;
    uint32_t  CurrCycle;
    uint32_t  RxCycleN;
    uint32_t SleepTime;
    // Synchronization
    bool GetPrimaryPkt;
    bool WasSynchronize;
    // Time Specific
    char TimeSeparator;
    uint16_t PriorityID;
    uint32_t NewCycleN;
    int32_t CycleCorrection;
    uint32_t TimeToWakeUp;
    Timer_t CycleTmr;
    CircBufString_t MeshMsg;

    void INewRxCycle()       {
        PRndTable++;
        if(PRndTable > RndTableBuf + RND_TBL_BUFFER_SZ) PRndTable = RndTableBuf;
        RxCycleN = *PRndTable;
    }

    void IIncCurrCycle()     {
        AbsCycle++;
        CurrCycle++;
        if(CurrCycle >= MESH_COUNT_OF_CYCLES) {
            CurrCycle = 0;
            INewRxCycle();
        }
    }
    void IGenerateRandomTable(uint32_t Size) {
        srand(App.SelfID);
        RndTableBuf[0] = 1;
        for(uint8_t i=1; i<RND_TBL_BUFFER_SZ; i++) {
            RndTableBuf[i] = GET_RND_VALUE(MESH_COUNT_OF_CYCLES);
        }
        RxCycleN = *PRndTable;
        // TODO: check random table value
    }

    void INewCycle();
    void IUpdateTimer();
    void IPktPutCycle(uint32_t NewCycle)            { PktTx.SenderInfo.Mesh.CycleN = NewCycle; }
    void IPktPutTimeOwner(uint16_t NewTimeOwner)    { PktTx.SenderInfo.Mesh.TimeOwnerID = NewTimeOwner; }
    uint16_t IGetTimeOwner()                        { return PktTx.SenderInfo.Mesh.TimeOwnerID; }
    void ITimeAgeCounter() {
        if(PktTx.SenderInfo.Mesh.SelfID != PktTx.SenderInfo.Mesh.TimeOwnerID) {
            PktTx.SenderInfo.Mesh.TimeAge++;
            if(PktTx.SenderInfo.Mesh.TimeAge > TIME_AGE_THRESHOLD) IResetTimeAge(App.SelfID, 0);
        }
    }
    void IResetTimeAge(uint16_t NewID, uint8_t TA)  { PktTx.SenderInfo.Mesh.TimeAge = TA; PktTx.SenderInfo.Mesh.TimeOwnerID = NewID; }
    uint8_t IGetTimeAge()                           { return PktTx.SenderInfo.Mesh.TimeAge; }

public:
    Mesh_t() :
                PRndTable(RndTableBuf),
                AbsCycle(START_CYCLE),
                CurrCycle(0),
                RxCycleN(*PRndTable),
                SleepTime(0),
                GetPrimaryPkt(false),
                WasSynchronize(false),
                TimeSeparator(':'),
                PriorityID(0),
                NewCycleN(0),
                CycleCorrection(0),
                TimeToWakeUp(0),
                IPThread(NULL),
                IPPktHanlderThread(NULL),
                IsInit(false),
                UndispathedPktCnt(0) {}

    Thread *IPThread, *IPPktHanlderThread;
    bool IsInit;
    uint16_t UndispathedPktCnt;

    void UpdateSleepTime()              { SleepTime = ((App.SelfID-1)*SLOT_TIME); }
    uint32_t GetCycleN()                { return (AbsCycle);             }
    uint32_t GetAbsTimeMS()             { return (AbsCycle*CYCLE_TIME);  }
    uint8_t GetAstronomicTime(char *PToStr, uint8_t MaxLen);
    int32_t SetNewAbsCycleN(uint32_t ANew)   {
        int32_t Diff = AbsCycle;
        AbsCycle = ANew;
        CurrCycle = AbsCycle % MESH_COUNT_OF_CYCLES;
        INewRxCycle();
        Diff = AbsCycle - Diff;
        if(!WasSynchronize) WasSynchronize = true;
        return Diff;
    }
    void Init();
    void Halt() {
        CycleTmr.Disable();
        IsInit = false;
    }
    void Resume() {
        CycleTmr.Enable();
        IsInit = true;
    }
    void UpdatePrescaler() {
        if(Clk.AHBFreqHz == 12000000) CycleTmr.SetupPrescaler(1000);
        else if(Clk.AHBFreqHz == 48000000) CycleTmr.SetupPrescaler(500);
        else CycleTmr.SetupPrescaler(1000);
    }

    MeshPkt_t PktRx, PktTx;
    MsgBox_t<mshMsg_t, MESH_PKT_SZ> MsgBox;
    CircBufPkt_t PktBucket;

    void ITask();
    void IWaitTxEnd();
    void ITxEnd();
    void IIrqHandler();
    void IPktHandler();
    void SendEvent(eventmask_t mask)  { chEvtSignal(IPThread,mask); }
    void PreparePktPayload(uint32_t CycleN);
    void PrepareNextPkt();
};

extern Mesh_t Mesh;


#endif

#endif /* MESH_LVL_H_ */
