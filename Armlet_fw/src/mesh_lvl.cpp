/*
 * mesh_lvl.cpp
 *
 *  Created on: 28 февр. 2014 г.
 *      Author: r.leonov
 */


#include "mesh_lvl.h"
#include "radio_lvl1.h"
#include "SensorTable.h"
//pyton translation for db
//[22:19:36] Jolaf: str(tuple(1 + int(sqrt(float(i) / 65) * 99) for i in xrange(0, 65 + 1)))
const int DbTranslate[66] ={1, 13, 18, 22, 25, 28, 31, 33, 35, 37, 39, 41, 43, 45, 46, 48, 50, 51, 53, 54, 55, 57, 58, 59, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 86, 87, 88, 89, 90, 91, 92, 92, 93, 94, 95, 96, 96, 97, 98, 99, 100};


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
//        Uart.Printf("i,%u, t=%u\r", AbsCycle, chTimeNow());
        Beeper.Beep(ShortBeep);
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
        uint8_t PriorityID = SelfID;
        uint32_t NewAbsTime=0;
        uint32_t NextCycleStart=0;
        mshMsg_t MeshMsg;
        if(PktBuf.GetFilledSlots() != 0) {
            do {
                PktBuf.ReadPkt(&MeshMsg);
//                Uart.Printf("ID=%u %d\r", MeshMsg.PktRx.ID, MeshMsg.RSSI);
                if(PriorityID > MeshMsg.PktRx.ID) {                /* Priority time checking */
                    CycleTmr.Disable();
                    NeedUpdateTime = true;
                    NewAbsTime = MeshMsg.PktRx.CycleN + 1;
                    PriorityID = MeshMsg.PktRx.TimeOwnerID;
                    NextCycleStart = MeshMsg.Timestamp + (uint32_t)CYCLE_TIME - (SLOT_TIME * PriorityID);
                }

                if(MeshMsg.RSSI < -100) MeshMsg.RSSI = -100;
                else if(MeshMsg.RSSI > -35) MeshMsg.RSSI = -35;
                MeshMsg.RSSI += 100;    // 0...65
                uint32_t Lvl =DbTranslate[MeshMsg.RSSI]; //1 + (uint32_t)(((int32_t)MeshMsg.RSSI * 99) / 65);
                SnsTable.PutSnsInfo(MeshMsg.PktRx.ID, Lvl);   /* Put Information in SensTable */
            } while(PktBuf.GetFilledSlots() != 0);
        }

        NeedToSendTable++;
        if(NeedToSendTable == TABLE_SEND_N) {
            Uart.Printf("MESH TS,t=%u\r", chTimeNow());
            SnsTable.SendEvtReady();
            NeedToSendTable = 0;
        }

        if(NeedUpdateTime) {
            Uart.Printf("MESH CycUpdat=%u\r", NewAbsTime);
            SetCurrCycleN(NewAbsTime); // FIXME: aligh NewAbsTime
            uint32_t timeNow = chTimeNow();
            do NextCycleStart += CYCLE_TIME;
            while (NextCycleStart < timeNow);
            Uart.Printf("MESH Sl %u to %u\r", chTimeNow(), NextCycleStart);
            chThdSleepUntil(NextCycleStart);
            CycleTmr.Enable();
            CycleTmr.SetCounter(0);
            NeedUpdateTime = false;
        }
    }
}

void Mesh_t::Init(uint32_t ID) {
    // Init Thread
    IPThread = chThdCreateStatic(waMeshLvlThread, sizeof(waMeshLvlThread), NORMALPRIO, (tfunc_t)MeshLvlThread, NULL);
    // Create RandomTable
    MsgBox.Init();
    SelfID = (uint8_t)ID;
    SleepTime= ((SelfID-1)*SLOT_TIME);
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
