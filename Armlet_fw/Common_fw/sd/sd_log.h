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

#define LOG_BUF_SZ      512     // Multiple of 512
#define LOG_NAME        "log.txt"

class SdLog_t {
private:
    FRESULT IError = FR_NOT_ENABLED;
    FIL IFile;
    char Data[LOG_BUF_SZ];
    UINT Cnt = 0;
    void IPrintf(const char *S, ...);
public:
    void Init();
    void Printf(const char *S, ...);
    void Flush() {
        UINT dummy=0;
        IError = f_write(&IFile, Data, Cnt, &dummy);
        if(IError != FR_OK) Uart.Printf("\rLogfile w error: %u", IError);
        else {
            IError = f_sync(&IFile);
            if(IError != FR_OK) Uart.Printf("\rLogfile sync error: %u", IError);
        }
        Cnt = 0;
    }
    // Inner use
    void IPutchar(char c);
};


extern SdLog_t Log;

#endif /* SD_LOG_H_ */
