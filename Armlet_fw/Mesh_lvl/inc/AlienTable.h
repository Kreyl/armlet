/*
 * AlienTable.h
 *
 *  Created on: 08 ���� 2014 �.
 *      Author: r.leonov
 */

#ifndef ALIENTABLE_H_
#define ALIENTABLE_H_

#include "mesh_lvl.h"
#include "console.h"
#ifdef STM32F2XX
#include "kl_lib_f2xx.h"
#else
#include "kl_lib_L15x.h"
#endif

#define ALIEN_BUF_SIZE      MAX_ABONENTS
#define FINDER_COUNTER      2 // How many times search Buf for valid info

#define ret_ID              uint16_t
#define ret_Err             uint8_t

struct AlienTable_t {
private:
    AlienInfo_t Buf[ALIEN_BUF_SIZE];

    /* Internal functionality */
    void write_data(uint16_t ID, AlienInfo_t *Ptr) { Ptr->Mesh.Hops += 1; Buf[ID] = *Ptr; }
public:
    AlienInfo_t *PNext;
    AlienTable_t(): PNext(Buf) {}

    /* External functionality */
    // Put Information to buffer
    ret_Err PutAlien(uint16_t ID, int32_t TimeStampDiff, AlienInfo_t *Ptr);
    ret_Err PutSender(uint32_t CurrentCycle, SenderInfo_t *Ptr);

    /* Get Info by ID
     * return pointer to string in Buf
     * */
    AlienInfo_t* GetInfo(uint16_t ID) { return (AlienInfo_t*)&Buf[ID]; }

    /* Get Next info ID
     * return ID of the next new available information, if correct
     * return 0 if failed
     */
    ret_ID GetID() {
        uint8_t Cnt = 0;
        do {
            PNext++;
            if(PNext >= (Buf + ALIEN_BUF_SIZE)) {
                if(Cnt++ > FINDER_COUNTER) {
                    return 0;
                }
                PNext = Buf;
            }
        } while(PNext->Mesh.Hops == 0);
        return (uint16_t)(PNext - Buf);
    }
    // UpdateState
    ret_Err UpdateState(uint16_t ID, state_t *Ptr) {
        Buf[ID].State = *Ptr;
        return OK;
    }

    void UpdateSelf(uint32_t NewCycle) {
        if(App.SelfID > ALIEN_BUF_SIZE) return;
        Buf[App.SelfID].Mesh.Timestamp = NewCycle;
        Buf[App.SelfID].Mesh.TimeDiff = 0;
        Buf[App.SelfID].State = App.CurrInfo;
//        Uart.Printf("ATab %u %u %u\r", Buf[App.SelfID].State.Location, Buf[App.SelfID].State.Neighbor, Buf[App.SelfID].State.Battery);
        Console.Send_Info(App.SelfID, &Buf[App.SelfID]);
    }

    void TimeCorrection(int32_t Corr);
};

extern AlienTable_t AlienTable;

#endif /* ALIENTABLE_H_ */
