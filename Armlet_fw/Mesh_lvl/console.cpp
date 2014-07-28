/*
 * console.cpp
 *
 *  Created on: 04 ��� 2014 �.
 *      Author: r.leonov
 */

#include "console.h"

#define PRINT

Console_t Console;

void Console_t::Send_Info(uint16_t ID, AlienInfo_t *Ptr) {
    Cnt++;
    if(Cnt == SEND_IN_COUNT) {
#ifdef PRINT
        Uart.Printf("\r\n#Node %u %u %u %d %u %u %u",
                ID,
                Ptr->Mesh.Hops,
                Ptr->Mesh.Timestamp,
                Ptr->Mesh.TimeDiff,
                Ptr->State.Location,
                Ptr->State.Reason,
                Ptr->State.Emotion);
        Cnt = 0;
#endif
    }
}

void Console_t::SetTime_Ack(int32_t NewCycDiff) {
#ifdef PRINT
    Uart.Printf("\r\n#MeshCycle %d", NewCycDiff);
#endif
}

void Console_t::GetMeshInfo_Ack(uint32_t Rslt) {
    if(Rslt == OK) {
#ifdef PRINT
        Uart.Printf("\r\n#MeshInfo %u %u", MAX_ABONENTS, CYCLE_TIME);
#endif
    }
    else Uart.Ack(Rslt);
}