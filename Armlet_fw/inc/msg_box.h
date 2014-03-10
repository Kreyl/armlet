/*
 * msg_box.h
 *
 *  Created on: 06 марта 2014 г.
 *      Author: r.leonov
 */

#ifndef MSG_BOX_H_
#define MSG_BOX_H_

#include "kl_lib_f2xx.h"
#include "radio_lvl1.h"

#if 1 // =============================== Message Box =================================
struct mshMsg_t {
    uint32_t Timestamp;
    rPkt_t PktRx;
    int8_t RSSI;
};

template <typename T, int32_t Sz>
class MsgBox_t {
private:
    Mailbox imailbox;
    msg_t IMsgBuf[Sz];
    T IBuf[Sz], *pw;
public:
    void Init() {
        pw = IBuf;
        chMBInit(&imailbox, IMsgBuf, Sz);
    }

    uint8_t Post(T Obj) {
        uint8_t rslt=OK;
        chSysLock();
        if(chMBGetUsedCountI(&imailbox) >= Sz) rslt = FAILURE; // No empty slots
        chSysUnlock();
        if(rslt != OK) return rslt;
        memcpy(pw, &Obj, sizeof(T));
        rslt = (chMBPost(&imailbox, (msg_t)pw, TIME_INFINITE) == RDY_OK)? OK : FAILURE;
//        Uart.Printf("MsgBox %d\r", rslt);
        if(pw == &IBuf[Sz-1]) pw = IBuf;    // Circulate buffer
        else pw++;
        return rslt;
    }

    void FetchMsg(T *Ptr) {
        msg_t msg;
        while(chMBFetch(&imailbox, &msg, TIME_INFINITE) != RDY_OK);
        memcpy(Ptr, (T*)msg, sizeof(T));

    }

    uint8_t TryFetchMsg(T *Ptr) {
        msg_t msg;
        if(chMBFetch(&imailbox, &msg, 1) == RDY_OK) {
            memcpy(Ptr, (T*)msg, sizeof(T));
            return OK;
        }
        else return FAILURE;
    }

    uint8_t TryFetchMsgTmt(uint32_t time, T *Ptr) {
        msg_t msg;
        if(chMBFetch(&imailbox, &msg, time) == RDY_OK) {
            memcpy(Ptr, (T*)msg, sizeof(T));
            return OK;
        }
        else return FAILURE;
    }
};
#endif



#endif /* MSG_BOX_H_ */
