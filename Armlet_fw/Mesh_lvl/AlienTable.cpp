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
    Uart.Printf("\rPutAlien %u {h:%u, ts:%d, td:%d}", ID, Ptr->Mesh.Hops, Ptr->Mesh.Timestamp, Ptr->Mesh.TimeDiff);
    if(ID > ALIEN_BUF_SIZE) return CMD_ERROR;
    else if(ID == 0) { Uart.Printf("\rID Err\n"); return FAILURE; }
    else if(ID == App.SelfID) return OK;
    else { // Need to share information to Buf
        Ptr->Mesh.Timestamp    += TimeStampDiff;
        Ptr->Mesh.TimeDiff     += TimeStampDiff;
        Uart.Printf("\rBringIntoStep  %u {h:%u, ts:%d, td:%d}", ID, Ptr->Mesh.Hops, Ptr->Mesh.Timestamp, Ptr->Mesh.TimeDiff);
        Uart.Printf("\rInTable        %u {h:%u, ts:%d, td:%d}", ID, Buf[ID].Mesh.Hops, Buf[ID].Mesh.Timestamp, Buf[ID].Mesh.TimeDiff);
        if(Ptr->Mesh.Timestamp < Buf[ID].Mesh.Timestamp) {
            Uart.Printf("\r1");
            return OK;
        }
        if((Ptr->Mesh.Timestamp == Buf[ID].Mesh.Timestamp) && (Ptr->Mesh.Hops >= Buf[ID].Mesh.Hops)) {
            Uart.Printf("\r2");
            return OK;
        }
        Ptr->Mesh.Hops         += 1; // increase Hops
        write_data(ID, Ptr);
        Uart.Printf("\r(!) WriteToBuf %u {h:%u, ts:%d, td:%d}", ID, Ptr->Mesh.Hops, Ptr->Mesh.Timestamp, Ptr->Mesh.TimeDiff);
        Console.Send_Info(ID, Ptr);
    }
    return OK;
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
   iNewString.Mesh.TimeDiff     =(int32_t)((Ptr->Mesh.CycleN) - CurrentCycle);
   iNewString.State             = Ptr->State; // copy State part
   write_data(Ptr->Mesh.SelfID, &iNewString);
   Uart.Printf("\rPutSender %u {h:%u, ts:%d, td:%d}" ,Ptr->Mesh.SelfID,  iNewString.Mesh.Hops,  iNewString.Mesh.Timestamp,  iNewString.Mesh.TimeDiff);
   Console.Send_Info(Ptr->Mesh.SelfID, &iNewString);
   return OK;
}

/* TimeCorrection */
void AlienTable_t::TimeCorrection(int32_t Corr) {
//    if(Corr == 0) return;
//    for(uint16_t i=0; i<ALIEN_BUF_SIZE; i++) {
//        if(Buf[i].Mesh.Timestamp != 0) {
//            Uart.Printf("\rCycleCorr %u  {ts:%d, cr:%d}", i, Buf[i].Mesh.Timestamp, Corr);
//            Buf[i].Mesh.Timestamp   -= Corr;
//            Buf[i].Mesh.TimeDiff    -= Corr;
//            Uart.Printf("\rAfterCorr %u  {ts:%d, td:%d}\r", i, Buf[i].Mesh.Timestamp, Buf[i].Mesh.TimeDiff);
//        } // if valid string
//    } // for all Buf
}
