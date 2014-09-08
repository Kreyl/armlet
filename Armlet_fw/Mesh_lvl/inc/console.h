/*
 * console.h
 *
 *  Created on: 04 мая 2014 г.
 *      Author: r.leonov
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_


#include "cmd_uart.h"
#include "application.h"
#include "mesh_params.h"

/*
 * SEND_IN_COUNT
 * 1 - Every cycle
 * 2 - Every second cycle
 * .
 * .
 * .
 * n - Every n cycle
 */
#define SEND_IN_COUNT   1

class Console_t {
private:
    uint8_t Cnt;
public:
    Console_t() : Cnt(0) {}
    void Send_Info(uint16_t ID, AlienInfo_t *Ptr);
    void SetTime_Ack(int32_t NewCycDiff);
    void GetMeshInfo_Ack(uint32_t Rslt);
};

extern Console_t Console;
#endif /* CONSOLE_H_ */
