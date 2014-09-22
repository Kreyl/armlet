/*
 * mesh_lvl.cpp
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */


#include "mesh_lvl.h"
#include "radio_lvl1.h"
#include "peripheral.h"
#include "application.h"
#include "RxTable.h"
//#define MESH_DBG        // On/Off Debug Message in MeshLvl

Mesh_t Mesh;

#if 1 // ================================= Thread ====================================
extern "C" {
CH_IRQ_HANDLER(MESH_TIM_IRQ_HANDLER) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    Mesh.IIrqHandler();
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}
}

static WORKING_AREA(waMeshLvlThread, 512);
__attribute__ ((__noreturn__))
static void MeshLvlThread(void *arg) {
    chRegSetThreadName("MeshLvl");
    while(1) Mesh.ITask();
}

static WORKING_AREA(waMeshPktHandlerThd, 128);
__attribute__ ((__noreturn__))
static void MeshPktHandlerThd(void *arg) {
    chRegSetThreadName("MeshPktThd");
    while(1) {
        mshMsg_t MeshPkt;
        chEvtWaitAny(EVTMSK_MESH_PREPARE);
//        Uart.Printf("\r\n-- Wait pkts -- ");
        while(true) {
            if(Mesh.MsgBox.TryFetchMsg(&MeshPkt) == OK) {
                Mesh.PktBucket.WritePkt(MeshPkt);
                Mesh.UndispathedPktCnt--;
            }
            if(Mesh.UndispathedPktCnt == 0 && !Radio.Valets.InRx) break;

        }
//        Uart.Printf("\r\n-- BucketEnd, pkts= %u, t=%u --", Mesh.UndispathedPktCnt, chTimeNow());
        Mesh.SendEvent(EVTMSK_MESH_PKT_ENDS);
    }
}
#endif

void Mesh_t::Init() {
    if(App.SelfID == 0) {
        Uart.Printf("\rMsh: WrongID");
        return;
    }
    /* Create thread Bucket Handler */
    IPPktHanlderThread = chThdCreateStatic(waMeshPktHandlerThd, sizeof(waMeshPktHandlerThd), NORMALPRIO, (tfunc_t)MeshPktHandlerThd, NULL);
    UpdateSleepTime();          /* count sleep time due to App.ID */
    MsgBox.Init();
    IGenerateRandomTable(RND_TBL_BUFFER_SZ);

    IResetTimeAge(App.SelfID, 0);   /* TimeAge = 0; TimeOwner = App.ID */
    IPktPutCycle(AbsCycle);     /* CycleN = AbsCycle */
    AlienTable.UpdateSelf(AbsCycle);  /* Timestamp = AbsCycle; Send info to console */
    PreparePktPayload();

    /* Create Mesh Thread */
    IPThread = chThdCreateStatic(waMeshLvlThread, sizeof(waMeshLvlThread), NORMALPRIO, (tfunc_t)MeshLvlThread, NULL);

    nvicEnableVector(MESH_TIM_IRQ, CORTEX_PRIORITY_MASK(IRQ_PRIO_HIGH));
    CycleTmr.Init(MESH_TIM);
    UpdatePrescaler();
    CycleTmr.SetTopValue(CYCLE_TIME-1);
    CycleTmr.SetCounter(0);
    CycleTmr.IrqOnTriggerEnable();
    IsInit = true;

    chThdSleepMilliseconds(121); /* Do radio thd sleep 41 */  //TODO: define sleep time

    CycleTmr.Enable();           /* Enable Cycle Timer */
    Uart.Printf("\r\n[mesh_lvl.cpp] initID=%u", App.SelfID);
}

void Mesh_t::ITask() {
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);

    if(EvtMsk & EVTMSK_MESH_NEW_CYC) INewCycle();

    if(EvtMsk & EVTMSK_MESH_RX_END) IUpdateTimer();

    if(EvtMsk & EVTMSK_MESH_PKT_ENDS) IPktHandler();
}

#if 1 // ==== Inner functions ====

void Mesh_t::INewCycle() {
//    Beeper.Beep(BeepShort);
    IIncCurrCycle();
    ITimeAgeCounter();
    AlienTable.UpdateSelf(AbsCycle);  /* Timestamp = AbsCycle; Send info to console */
    //Uart.Printf("\r\nCycle=%u, t=%u", AbsCycle, chTimeNow());
    // ==== RX ====
    if(CurrCycle == RxCycleN) chEvtSignal(Radio.rThd, EVTMSK_MESH_RX);
    // ==== TX ====
    else {
        PreparePktPayload();
        if(SleepTime > 0) chThdSleepMilliseconds(SleepTime);
        chEvtSignal(Radio.rThd, EVTMSK_MESH_TX);
        IWaitTxEnd();
    }
}

void Mesh_t::IPktHandler(){
    PriorityID = IGetTimeOwner();
    while(PktBucket.GetFilledSlots() != 0) {
        PktBucket.ReadPkt(&MeshMsg); /* Read Pkt from Buffer */
        /* Put information from Pkt */
        AlienTable.PutSender(AbsCycle, &MeshMsg.RadioPkt.SenderInfo);
        int32_t CycleDiff = AbsCycle - MeshMsg.RadioPkt.SenderInfo.Mesh.CycleN;
        AlienTable.PutAlien(MeshMsg.RadioPkt.AlienID, CycleDiff, &MeshMsg.RadioPkt.AlienInfo);

        /*Put Information to RxTable */
        RxTable.PutRxInfo(MeshMsg.RadioPkt.SenderInfo.Mesh.SelfID, MeshMsg.RSSI, &MeshMsg.RadioPkt.SenderInfo.State);

        sender_mesh_t *pSM = &MeshMsg.RadioPkt.SenderInfo.Mesh;
        if( (pSM->TimeOwnerID < PriorityID) ||
               ((pSM->TimeOwnerID == PriorityID) &&
                   (pSM->TimeAge < IGetTimeAge())) ) {  /* compare TimeAge */
            CycleTmr.Disable();
            GetPrimaryPkt = true;                        // received privilege pkt
            PriorityID = pSM->TimeOwnerID;
            IResetTimeAge(PriorityID, pSM->TimeAge);
            NewCycleN = pSM->CycleN + 1;   // TODO: cycle number increment: nedeed of not? Seems to be needed.
            CycleCorrection = AbsCycle - NewCycleN;
            *PTimeToWakeUp = MeshMsg.Timestamp - MESH_PKT_TIME - (SLOT_TIME*(PriorityID-1)) + CYCLE_TIME;
        }
    }
    SendEvent(EVTMSK_MESH_RX_END);
}

void Mesh_t::IUpdateTimer() {
    RxTable.SendEvtReady();
    if(GetPrimaryPkt) {
        uint32_t timeNow = chTimeNow();
        while(*PTimeToWakeUp < timeNow) {
            *PTimeToWakeUp += CYCLE_TIME;
            NewCycleN += 1;
        }
        SetNewAbsCycleN(NewCycleN);
        AlienTable.TimeCorrection(CycleCorrection);
        CycleTmr.SetCounter(0);
        GetPrimaryPkt = false;
        if(*PTimeToWakeUp > chTimeNow()) chThdSleepUntil(*PTimeToWakeUp); /* TODO: Thinking carefully about asynch switch on Timer with Virtual timer */
        else Uart.Printf("\r\n[mesh_lvl.cpp] WT!");
        CycleTmr.Enable();
    }
}

void Mesh_t::PreparePktPayload() {
    PktTx.SenderInfo.Mesh.SelfID = App.SelfID;         /* need if App.ID changed by the Uart command */
    PktTx.SenderInfo.State = App.CurrInfo;
    IPktPutCycle(AbsCycle);
    AlienInfo_t *AlienDataStr;
    uint16_t NextID = 0;
    NextID = AlienTable.GetID();
    AlienDataStr = AlienTable.GetInfo(NextID);
    PktTx.AlienID = NextID;
    PktTx.AlienInfo = *AlienDataStr;
}

uint8_t Mesh_t::GetAstronomicTime(char *PToStr, uint8_t MaxLen) {
    if(MaxLen < TIME_SZ) return FAILURE;
    else {
        memset(PToStr, 0, MaxLen);
        uint32_t Time;
        Time = AbsCycle * CYCLE_TIME;          // Time from start of epoch
        uint8_t Days;
        Days = Time / MESH_MS_IN_DAY;           // now we got how many days procced after start
        if(Days > 0) Time -= (Days * MESH_MS_IN_DAY);   // now we have days independent time if needed
        Time /= 1000; // in seconds
        uint8_t hh, mm;
    //    uint8_t ss;
        hh = Time/3600;
        if(hh > 0) Time -= hh*3600;
        mm = Time/60;
        if(mm > 0) Time -= mm*60;
    //    ss = Time;
//        TimeSeparator = (TimeSeparator == ':')? ' ' : ':';
        Time::PutTimeTo(PToStr, hh, mm, TimeSeparator);
        return OK;
    }
}

void Mesh_t::IWaitTxEnd() {
    chSysLock();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chSysUnlock();
}

void Mesh_t::ITxEnd() {
    chSysLock();
    if(IPThread->p_state == THD_STATE_SUSPENDED) chSchReadyI(IPThread);
    chSysUnlock();
}

void Mesh_t::IIrqHandler() {
    CycleTmr.ClearIrqPendingBit();
    if(IPThread != nullptr) chEvtSignalI(IPThread, EVTMSK_MESH_NEW_CYC);
}
#endif
