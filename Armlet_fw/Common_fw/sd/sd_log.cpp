/*
 * sd_log.cpp
 *
 *  Created on: 24 июля 2014 г.
 *      Author: g.kruglov
 */

#include "sd_log.h"
#include "kl_sprintf.h"
#include "evt_mask.h"
#include "usb_f2.h"

SdLog_t Log;

static inline void FLogPutChar(char c) { Log.IPutchar(c); }

static WORKING_AREA(waLogThread, 512);
__attribute__((__noreturn__))
static void LogThread(void *arg) {
    chRegSetThreadName("Log");
    Log.ITask();
}

__attribute__((__noreturn__))
void SdLog_t::ITask() {
    while(true) {
        uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
        if(EvtMsk & EVTMSK_LOGFILE) {
//            Uart.Printf("\rT");
            if(Usb.IsReady) continue;
            chSysLock();
            LogBuf_t *PBufToFlush = PBuf;
            if(PBuf == &Buf1) PBuf = &Buf2; // }
            else PBuf = &Buf1;              // } switch buf for printing
            chSysUnlock();
            Flush(PBufToFlush);
            if(IError == FR_OK) {
                IError = f_sync(&IFile);
                if(IError != FR_OK) Uart.Printf("\rLogfile sync error: %u", IError);
            }
        }
    } // while 1
}

void SdLog_t::Init() {
    IError = f_open(&IFile, LOG_NAME, FA_WRITE+FA_OPEN_ALWAYS);
    if(IError != FR_OK) { Uart.Printf("\rLogfile open error: %u", IError); return; }
    IError = f_lseek(&IFile, f_size(&IFile));
    if(IError != FR_OK) { Uart.Printf("\rLogfile seek error: %u", IError); return; }
    PThd = chThdCreateStatic(waLogThread, sizeof(waLogThread), LOWPRIO, (tfunc_t)LogThread, NULL);
    Printf("==== Log Start ====");
}

void SdLog_t::IPutchar(char c) {
    if(PBuf->Cnt < LOG_BUF_SZ)  // Write if buffer is not full
        PBuf->Buf[PBuf->Cnt++] = c;
}

void SdLog_t::IPrintf(const char *S, ...) {
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLogPutChar, LOG_BUF_SZ, S, args);
    va_end(args);
}

void SdLog_t::Printf(const char *S, ...) {
    if(IError != FR_OK) return;
    chSysLock();
    // Print systime
    IPrintf("\r\n%u ", chTimeNow());
    // Print to buf
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLogPutChar, LOG_BUF_SZ , S, args);
    va_end(args);
    chEvtSignalI(PThd, EVTMSK_LOGFILE);
    chSysUnlock();
//    Uart.Printf("L");
}
