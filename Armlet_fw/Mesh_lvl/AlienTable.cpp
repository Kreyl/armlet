/*
 * AlienTable.cpp
 *
 *  Created on: 08 июля 2014 г.
 *      Author: r.leonov
 */


#include "AlienTable.h"


AlienTable_t AlienTable;


/* PutAlien
 * return OK
 *      1) if AlienInfo writedown correct;
 *      2) if AlienInfo is about recipient;
 *      3) if AlienInfo Timestamp is lower then recipient have.
 * return CMD_ERROR
 *      1) if ID is bigger than Buffer size.
 * return UNKNOWN
 *      1) if anything unknown happend.
 */
ret_Err AlienTable_t::PutAlien(uint16_t ID, int32_t TimeStampDiff, AlienInfo_t *Ptr) {
    if(ID > ALIEN_BUF_SIZE) return CMD_ERROR;
    else if(ID == App.ID) {
        Uart.Printf("[AlienTable.cpp] rcv SelfID\r\n");
        return OK;
    }
    else if(Ptr->Mesh.Timestamp <= Buf[ID].Mesh.Timestamp) return OK;
    else { // Need to share information to Buf
        AlienInfo_t iNewString;
        iNewString = *Ptr; // copy input struct to new one
        iNewString.Mesh.Hops         += 1; // increase Hops
        iNewString.Mesh.Timestamp    -= TimeStampDiff;
        iNewString.Mesh.TimeDiff     -= TimeStampDiff;
        write_data(ID, &iNewString);
        Console.Send_Info(ID, &iNewString);
        return OK;
    }
    return CMD_UNKNOWN;
}


/* PutSender
 * return OK            if SenderInfo correctly save;
 * retunr CMD_ERROR     if ID is bigger than Buffer size.
 */
ret_Err AlienTable_t::PutSender(uint32_t CurrentCycle, SenderInfo_t *Ptr) {
   if(Ptr->Mesh.SelfID > ALIEN_BUF_SIZE) return CMD_ERROR;
   AlienInfo_t iNewString;
   iNewString.Mesh.Hops         = 1; // cause recipient catched pkt straight from sender
   iNewString.Mesh.Timestamp    = CurrentCycle;
   iNewString.Mesh.TimeDiff     = (int32_t)(Ptr->Mesh.CycleN - CurrentCycle);
   iNewString.State             = Ptr->State; // copy State part
   write_data(Ptr->Mesh.SelfID, &iNewString);
   Console.Send_Info(Ptr->Mesh.SelfID, &iNewString);
   return OK;
}

/* TimeCorrection */
void AlienTable_t::TimeCorrection(uint32_t CorrValueMS) {
    uint32_t CorrValueCycle = CorrValueMS/CYCLE_TIME;
    if(CorrValueCycle == 0) return;
    for(uint16_t i=0; i<ALIEN_BUF_SIZE; i++) {
        if(Buf[i].Mesh.Timestamp != 0) {
            Buf[i].Mesh.Timestamp   -= CorrValueCycle;
            Buf[i].Mesh.TimeDiff    -= CorrValueCycle;
        } // if valid string
    } // for all Buf
}
