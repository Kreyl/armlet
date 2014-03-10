/*
 * mesh_lvl.cpp
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */


#include "mesh_lvl.h"
#include "radio_lvl1.h"
#include "SensorTable.h"

Mesh_t Mesh;

// ================================= Thread ====================================
extern "C" {
CH_IRQ_HANDLER(TIM5_IRQHandler) {
    CH_IRQ_PROLOGUE();
    chSysLockFromIsr();
    Mesh.IIrqHandler();
    chSysUnlockFromIsr();
    CH_IRQ_EPILOGUE();
}
}

static WORKING_AREA(waMeshLvlThread, 128);
__attribute__ ((__noreturn__))
static void MeshLvlThread(void *arg) {
    chRegSetThreadName("MeshLvl");
    while(1) Mesh.ITask();
}


void Mesh_t::ITask() {
    // Catch Event
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    if(EvtMsk & EVTMSK_NEW_CYCLE) {
        Uart.Printf("i,%u, t=%u\r", AbsCycle, chTimeNow());
        IncCurrCycle();
        // RX
        if(CurrCycle == RxCycleN) {
            chEvtSignal(rLevel1.PrThd, EVTMSK_MESH_RX);
            mshMsg_t MeshPkt;
            uint32_t Timeout = CYCLE_TIME; /* Wait Answer for Cycle Time */
            do {
                if(MsgBox.TryFetchMsg(&MeshPkt) == OK) PktBuf.WritePkt(MeshPkt); // Put Msg to CircBuffer
                Timeout--;
            } while(Timeout != 0);
        }
        // TX
        else {
            chThdSleepMilliseconds(SleepTime);
            chEvtSignal(rLevel1.PrThd, EVTMSK_MESH_TX);
        }
    }
    if(EvtMsk & EVTMSK_UPDATE_CYCLE) {

//        uint8_t NumPkts = PktBuf.GetFilledSlots();
        uint8_t PriorityID = (uint8_t)SELF_ID;
        uint32_t NewAbsTime=0;
        uint32_t NextCycleStart=0;
        mshMsg_t MeshMsg;
        if(PktBuf.GetFilledSlots() != 0) {
            do {
                PktBuf.ReadPkt(&MeshMsg);
    //            Uart.Printf("ID=%u %d\r", MeshMsg.PktRx.ID, MeshMsg.RSSI);
                if(PriorityID > MeshMsg.PktRx.ID) {                /* Priority time checking */
                    CycleTmr.Disable();
                    NeedUpdateTime = true;
                    NewAbsTime = MeshMsg.PktRx.CycleN + 1;
                    PriorityID = MeshMsg.PktRx.TimeOwnerID;
                    NextCycleStart = MeshMsg.Timestamp + (uint32_t)CYCLE_TIME - (SLOT_TIME * PriorityID);
                }
                uint8_t Lvl = (uint8_t)(MeshMsg.RSSI + 120);
                SnsTable.PutSnsInfo(MeshMsg.PktRx.ID, Lvl);   /* Put Information in SensTable */
            } while(PktBuf.GetFilledSlots() != 0);
            SnsTable.SendEvtReady();                        /* Send Evt to App Theread */
        }

        if(NeedUpdateTime) {
//            Uart.Printf("NewCycleUpdate=%u\r", NewAbsTime);
//            SetCurrCycleN(NewAbsTime); // FIXME: aligh NewAbsTime
//            uint32_t timeNow = chTimeNow();
//            do NextCycleStart += CYCLE_TIME;
//            while (NextCycleStart < timeNow);
//            Uart.Printf("Sleep from %u to %u\r", chTimeNow(), NextCycleStart);
//            chThdSleepUntil(NextCycleStart);
            CycleTmr.Enable();
            CycleTmr.SetCounter(0);
            NeedUpdateTime = false;
        }
    }
}

void Mesh_t::Init() {
    // Init Thread
    IPThread = chThdCreateStatic(waMeshLvlThread, sizeof(waMeshLvlThread), NORMALPRIO, (tfunc_t)MeshLvlThread, NULL);
    // Create RandomTable
    MsgBox.Init();
    for(uint8_t i=0; i<RND_TBL_BUFFER_SZ; i++) {
        RndTableBuf[i] = GET_RND_VALUE(COUNT_OF_CYCLES);
    }
//    Uart.Printf("RndTable: %A\r", RndTableBuf, RND_TBL_BUFFER_SZ, ' ');
    CycleTmr.Init();
    CycleTmr.SetupPrescaler(1000);
    CycleTmr.SetTopValue(CYCLE_TIME-1);
    CycleTmr.SetCounter(0);
    CycleTmr.IrqOnTriggerEnable();
    CycleTmr.Enable();
    nvicEnableVector(TIM5_IRQn, CORTEX_PRIORITY_MASK(IRQ_PRIO_HIGH));
}

void Mesh_t::IIrqHandler() {
    CycleTmr.ClearIrqPendingBit();
    if(IPThread != nullptr) chEvtSignalI(IPThread, EVTMSK_NEW_CYCLE);
}
