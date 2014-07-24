/*
 * sd_log.cpp
 *
 *  Created on: 24 июля 2014 г.
 *      Author: g.kruglov
 */

#include "sd_log.h"
#include "kl_sprintf.h"

SdLog_t Log;

static inline void FLogPutChar(char c) { Log.IPutchar(c); }

void SdLog_t::Init() {
    IError = f_open(&IFile, LOG_NAME, FA_WRITE+FA_OPEN_ALWAYS);
    if(IError != FR_OK) { Uart.Printf("\rLogfile open error: %u", IError); return; }
    IError = f_lseek(&IFile, f_size(&IFile));
    if(IError != FR_OK) { Uart.Printf("\rLogfile seek error: %u", IError); return; }
    IPrintf("\r\n%u ==== Log Start ==== ", chTimeNow());
    Flush();
}

void SdLog_t::IPrintf(const char *S, ...) {
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLogPutChar, LOG_BUF_SZ, S, args);
    va_end(args);
}

void SdLog_t::IPutchar(char c) {
    Data[Cnt++] = c;
    if(Cnt == LOG_BUF_SZ) Flush();
}

void SdLog_t::Printf(const char *S, ...) {
    if(IError != FR_OK) return;
    // Print systime
    IPrintf("\r\n%u ", chTimeNow());
    // Print to buf
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLogPutChar, LOG_BUF_SZ , S, args);
    va_end(args);
}
