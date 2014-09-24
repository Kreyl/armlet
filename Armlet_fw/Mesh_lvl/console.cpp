/*
 * console.cpp
 *
 *  Created on: 04 мая 2014 г.
 *      Author: r.leonov
 */

#include "console.h"

#define PRINT

Console_t Console;

void Console_t::Send_Info(uint16_t ID, AlienInfo_t *Ptr) {
    Cnt++;
    if(Cnt == SEND_IN_COUNT) {
#ifdef PRINT
        Uart.Printf("#Node %u %u %u %d %u %u %u\r\n",
                ID,
                Ptr->Mesh.Hops,
                Ptr->Mesh.Timestamp,
                Ptr->Mesh.TimeDiff,
                (Ptr->State.Location & ~LOCATION_VALID),
                Ptr->State.Neighbour,
                Ptr->State.Battery);
        Cnt = 0;
#endif
    }
}

void Console_t::SetTime_Ack(int32_t NewCycDiff) {
#ifdef PRINT
    Uart.Printf("#MeshCycle %d\r\n", NewCycDiff);
#endif
}

void Console_t::GetMeshInfo_Ack(uint32_t Rslt) {
    if(Rslt == OK) {
#ifdef PRINT
        Uart.Printf("#MeshInfo %u %u\r\n", MAX_ABONENTS, CYCLE_TIME);
#endif
    }
    else Uart.Ack(Rslt);
}
