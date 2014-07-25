/*
 * sd_log.cpp
 *
 *  Created on: 24 июля 2014 г.
 *      Author: g.kruglov
 */

#include "sd_log.h"
#include "kl_sprintf.h"

SdLog_t Log;

static inline void FLogPutChar(char c) { f_putc(c, &Log.IFile); }

//static WORKING_AREA(waLogThread, 1024);
//static void LogThread(void *arg) {
//    chRegSetThreadName("Log");
//}

void SdLog_t::Init() {
    IError = f_open(&IFile, LOG_NAME, FA_WRITE+FA_OPEN_ALWAYS);
    if(IError != FR_OK) { Uart.Printf("\rLogfile open error: %u", IError); return; }
    IError = f_lseek(&IFile, f_size(&IFile));
    if(IError != FR_OK) { Uart.Printf("\rLogfile seek error: %u", IError); return; }
    IPrintf("\r\n%u ==== Log Start ==== ", chTimeNow());
    f_sync(&IFile);
//    Flush();
}

void SdLog_t::IPrintf(const char *S, ...) {
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLogPutChar, LOG_BUF_SZ, S, args);
    va_end(args);
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
//    Uart.Printf("L");
    f_sync(&IFile);
}
