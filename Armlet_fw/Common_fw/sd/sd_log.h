/*
 * sd_log.h
 *
 *  Created on: 24 июля 2014 г.
 *      Author: g.kruglov
 */

#ifndef SD_LOG_H_
#define SD_LOG_H_

#include "kl_lib_f2xx.h"
#include "ff.h"
#include "cmd_uart.h"

#define LOG_BUF_SZ      256
#define LOG_NAME        "log.txt"

struct LogBuf_t {
    char Buf[LOG_BUF_SZ];
    int Cnt=0;
};

class SdLog_t {
private:
    FRESULT IError = FR_NOT_ENABLED;
    LogBuf_t Buf1, Buf2, *PBuf=&Buf1;
    void IPrintf(const char *S, ...);
    Thread *PThd;
    void Flush(LogBuf_t *p) {
        if(p->Cnt == 0) return;
        UINT dummy=0;
        IError = f_write(&IFile, p->Buf, p->Cnt, &dummy);
        if(IError != FR_OK) Uart.Printf("\rLogfile w error: %u", IError);
        p->Cnt = 0;
    }
public:
    void Init();
    void Printf(const char *S, ...);
    // Inner use
    void ITask();
    void IPutchar(char c);
    FIL IFile;
};


extern SdLog_t Log;

#endif /* SD_LOG_H_ */
