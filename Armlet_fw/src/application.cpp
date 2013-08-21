/*
 * application.cpp
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#include "application.h"
#include "ch.h"
#include "hal.h"

#include "lcd2630.h"
#include "peripheral.h"
#include "BeepSequences.h"
#include "VibroSequences.h"
#include "keys.h"
#include "pill.h"
#include "infrared.h"
#include "power.h"
#include "tiny_sprintf.h"

#include "lvl1_assym.h"
#include "evt_mask.h"
#include "kl_sd.h"
#include "sound.h"
//#include "ArmletApi.h"

//#define DEMONSTRATE

void LcdRedraw();

App_t App;

static EventListener EvtLstnrRadioRx, EvtListenerKeys, EvtListenerPill, EvtListenerIR, EvtListenerTmr;
static EventSource IEvtSrcTmr;
static rPktWithData_t<RRX_PKT_DATA_SZ> SRxPkt;

//static uint8_t AppState[APP_STATE_LEN];

// Lustra
#define LUSTRA_RADIO_RESET_CNT  40  // How many times to wait to reset lustra number
class Lustra_t {
public:
    uint8_t IDForApp, IDForRadio, Counter;
    void SetID(uint8_t AID) {
        IDForApp = AID;
        IDForRadio = AID;
        Counter = 0;
    }
    void PerformReset() {
//        IDForApp = UNKNOWN_ID;
//        if(Counter >= LUSTRA_RADIO_RESET_CNT) IDForRadio = UNKNOWN_ID;
//        else Counter++;
    }
//    Lustra_t() : IDForApp(UNKNOWN_ID), IDForRadio(UNKNOWN_ID), Counter(0) {}
};
static Lustra_t Lustra;

// Lcd
//static uint16_t Framebuf[LCD_W*LCD_H];
static bool LcdHasChanged;
#define LCD_REDRAW_MS   99

// ==== Timer ====
#define TMR_CNT     11
struct TmrData_t {
//    ArmletApi::TIMER_PROC* Callback;
    int32_t TimeFired, PreviousTimeFired, Period;
    VirtualTimer VirTmr;
    bool Fired;
};
class ITmr_t {
public:
    TmrData_t Tmrs[TMR_CNT];
    uint32_t Cnt;
//    bool AddTmr(uint32_t APeriod, ArmletApi::TIMER_PROC* PCallback);
    void Iterate();
    ITmr_t() : Cnt(0) {}
};
static ITmr_t STimers;
// Pill
struct Med_t {
    int CureID, Charges;
} __attribute__ ((__packed__));
static Med_t Med;

// =============================== Threads ==================================
static WORKING_AREA(waAppThread, 128);
__attribute__((noreturn))
static void AppThread(void *arg) {
    (void)arg;
    chRegSetThreadName("App");

    // Init shell
//    ArmletApi::InitializeShell();

    // Events
    uint32_t EvtMsk;
    KeysRegisterEvt(&EvtListenerKeys, EVTMASK_KEYS);
//    rLevel1.RegisterEvtRx(&EvtLstnrRadioRx, EVTMASK_RADIO_RX);
//    PillRegisterEvtChange(&EvtListenerPill, EVTMASK_PILL);
//    IR.RegisterEvt(&EvtListenerIR, EVTMASK_IR);
//    chEvtRegisterMask(&IEvtSrcTmr, &EvtListenerTmr, EVTMASK_TIMER);

    while(1) {
        EvtMsk = chEvtWaitAny(EVTMASK_RADIO_RX | EVTMASK_KEYS | EVTMASK_PILL | EVTMASK_IR | EVTMASK_TIMER);

        if(EvtMsk &EVTMASK_KEYS) {
            if(KeyStatus[0].HasChanged) {
                if(KeyStatus[0].State == ksPressed) {
                    Sound.VolumeIncrease();
                }
            }
            if(KeyStatus[1].HasChanged) {
                if(KeyStatus[1].State == ksPressed) {
                    Sound.VolumeDecrease();
                }
            }
            if(KeyStatus[2].HasChanged) {
                if(KeyStatus[2].State == ksPressed) {
                    Sound.Stop();
                }
            }
            if(KeyStatus[3].HasChanged) {
                if(KeyStatus[3].State == ksPressed) {
                    Sound.Play("sylvans.mp3");
                }
            }

            //            for(uint8_t i=0; i<KEYS_CNT; i++) {
//                if(KeyStatus[i].HasChanged) {
//                    if(KeyStatus[i].State == ksReleased) ArmletApi::OnButtonRelease(i);
//                    else {
//                        #ifdef DEMONSTRATE
//                        Beeper.Beep(ShortBeep);
//                        #endif
//                        Vibro.Vibrate(ShortBrr);
//                        ArmletApi::OnButtonPress(i);
//                    }
//                    KeyStatus[i].HasChanged = false;
//                }
//            } // for
        } // keys

//        if(EvtMsk & EVTMASK_RADIO_RX) {
//            while(rLevel1.GetReceivedPkt(&SRxPkt) == OK) {
//                #ifdef DEMONSTRATE
//                Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
//                #endif
//                //Uart.Printf("%A\r", SRxPkt.Data, SRxPkt.Length, ' ');
//                //rLevel1.AddPktToTx(0, SRxPkt.Data, SRxPkt.Length);
//                ArmletApi::OnRadioPacket(SRxPkt.Data, SRxPkt.Length);
//            }
//        } // if evtmsk

//        if(EvtMsk & EVTMASK_PILL) {
//            #ifdef DEMONSTRATE
//            Beeper.Beep(ShortBeep);
//            #endif
//            // Read med
//            if(Pill[0].Connected) {
//                Pill[0].Read((uint8_t*)&Med, sizeof(Med_t));
//                ArmletApi::OnPillConnect(Med.CureID, Med.Charges);
//            }
//        }

//        if(EvtMsk & EVTMASK_IR) {
//            uint16_t w = IR.RxWord;
//            Lustra.SetID(w >> 8);
//          //  #ifdef DEMONSTRATE
//            Uart.Printf("IR ID=%u\r", Lustra.IDForApp);
//           // Beeper.Beep(ShortBeep);
//           // #endif
//        }

//        if(EvtMsk & EVTMASK_TIMER) { STimers.Iterate(); } // if timer
    } // while 1
}

static WORKING_AREA(waLcdThread, 128);
static msg_t LcdThread(void *arg) {
    (void)arg;
    chRegSetThreadName("Lcd");
    while(1) {
        chThdSleepMilliseconds(LCD_REDRAW_MS);
        if(LcdHasChanged) LcdRedraw();
    } // while 1
    return 0;
}

//=========================== Command processing ===============================
void UartCmdCallback(uint8_t CmdCode, uint8_t *PData, uint32_t Length) {
    uint8_t b;
    FRESULT res;
    switch(CmdCode) {
        case 0x01:
            b = OK;
            Uart.Cmd(0x90, &b, 1);
            break;

        case 0x51:  // GetID
            Uart.Printf("ID=%u\r", rLevel1.GetID());
            break;

        case 0x52:  // SetID
            b = PData[0];
            res = f_open(&SD.File, "settings.ini", FA_CREATE_ALWAYS | FA_WRITE);
            if(res == FR_OK) {
                f_printf(&SD.File, "[Radio]\r\nID=%u\r\n", b);
                f_close(&SD.File);
                Uart.Printf("Written\r");
            }
            rLevel1.SetID(b);
            Uart.Printf("New ID=%u\r", rLevel1.GetID());
            break;

        default: break;
    } // switch
}

// =============================== App class ===================================
void App_t::Init() {
    chEvtInit(&IEvtSrcTmr);

//    chThdCreateStatic(waLcdThread, sizeof(waLcdThread), LOWPRIO, LcdThread, NULL);
    chThdCreateStatic(waAppThread, sizeof(waAppThread), NORMALPRIO, (tfunc_t)AppThread, NULL);
}

// Fill radiopkt with current data
void RFillPkt(uint8_t *Ptr, uint8_t *PLen) {
//    *Ptr++ = Lustra.IDForRadio;
//    memcpy(Ptr, AppState, APP_STATE_LEN);
//    *PLen = APP_STATE_LEN+1;    // state len and lustra
}

// Dummy
#if 0
bool ArmletApi::InitializeShell() { return false; }

void ArmletApi::OnButtonPress(int button_id) {
}
void ArmletApi::OnButtonRelease(int button_id) {
}

void ArmletApi::OnRadioPacket(unsigned char* packet, int len) {
}
void ArmletApi::OnPillConnect(int cure_id, int charges) {
}
#endif
// ================================== API ======================================
/*
unsigned char ArmletApi::GetLustraId() {
    uint8_t b = Lustra.IDForApp;
    Lustra.PerformReset();
    Uart.Printf("IR ID=%u %u\r", Lustra.IDForApp, b);
    return b;
}

int ArmletApi::snprintf(char* buf, int bufSz, char* fmt,...) {
    va_list args;
    va_start(args, fmt);
    uint32_t Cnt = tiny_vsprintf(buf, bufSz, fmt, args);
    va_end(args);
    return Cnt;
}

unsigned char ArmletApi::GetBatteryLevel() { return Power.RemainingPercent; }

// ==== Display ====
void LcdRedraw() {
    Lcd.PutBitmap(0, 0, LCD_W, LCD_H, Framebuf);
    LcdHasChanged = false;
}

void ArmletApi::DrawPixel(int x, int y, unsigned short c) {
    //Uart.Printf("%u %u %X\r", x, y, c);
    uint32_t N = x + y*LCD_W;
    if(N >= (LCD_W*LCD_H)) return;
    // Add to buffer
    Framebuf[N] = c;
    LcdHasChanged = true;
}
void ArmletApi::DrawArea(int x, int y, unsigned short *const pc, int len, int sx, int sy) {
    Lcd.PutBitmap(x, y, sx, sy, pc);
}
void ArmletApi::SetScreenBrightness(uint8_t Percent) {
    Lcd.Brightness(Percent);
}


void ArmletApi::DoVibroAndBeep(int msecs) {
    Vibro.Vibrate(msecs);
    Beeper.Beep(msecs);
}

// Pill
bool ArmletApi::WritePill(int cure_id, int charges) {
    if(!Pill[0].Connected) return false;
    Med.CureID = cure_id;
    Med.Charges = charges;
    return (Pill[0].Write((uint8_t*)&Med, sizeof(Med_t)) == OK);
}

// ==== Radio ====
void ArmletApi::SendRadioPacket(unsigned char* packet, int len) {
    rLevel1.AddPktToTx(0, packet, len);
}
unsigned short ArmletApi::GetArmletId() { return rLevel1.GetID(); }
void ArmletApi::SendAppState(unsigned char packet[APP_STATE_LEN]) {
    chSysLock();
    memcpy(AppState, packet, APP_STATE_LEN);
    chSysUnlock();
}
void ArmletApi::GetRadioStatus(int* gate_id, int* signal_level) {
    int8_t FId, FSl;
    rLevel1.GetGateParams(&FId, &FSl);
    if(FId == R_NO_ID) *gate_id = -1;
    else *gate_id = FId;
    *signal_level = FSl;
}
*/

// ==== Timer ====
static void AppTmrCallback(void *p) {
    TmrData_t *PTmr = (TmrData_t*)p;
    if (!PTmr->Fired) {
        PTmr->PreviousTimeFired = PTmr->TimeFired;
        PTmr->TimeFired = chTimeNow();
        PTmr->Fired = true;
    }
    chSysLockFromIsr();
    chEvtBroadcastI(&IEvtSrcTmr);
    chVTSetI(&PTmr->VirTmr, MS2ST(PTmr->Period), AppTmrCallback, p);
    chSysUnlockFromIsr();
}

//bool ITmr_t::AddTmr(uint32_t APeriod, ArmletApi::TIMER_PROC* PCallback) {
//    if((Cnt >= TMR_CNT) or (PCallback == NULL)) return false;
//    Tmrs[Cnt].Fired = false;
//    Tmrs[Cnt].Callback = PCallback;
//    Tmrs[Cnt].Period = APeriod;
//    Tmrs[Cnt].TimeFired = chTimeNow();
//    chVTSet(&Tmrs[Cnt].VirTmr, MS2ST(APeriod), AppTmrCallback, (void*)&Tmrs[Cnt]);
//    Cnt++;
//    return true;
//}

void ITmr_t::Iterate() {
    for (uint32_t i=0; i<Cnt; i++) {
        if(Tmrs[i].Fired) {
            int Elapsed = chTimeNow() - Tmrs[i].PreviousTimeFired;
            //#ifdef DEMONSTRATE
            //Uart.Printf("Tmr%u p%u e%d\r", i, Tmrs[i].Period, Elapsed);
            //#endif
            // Run callback and stop timer if not needed
//            if(!Tmrs[i].Callback(Elapsed)) { chVTReset(&Tmrs[i].VirTmr); }
            Tmrs[i].Fired = false;
        } // if
    } // for
}
//
//bool ArmletApi::RequestTimer(TIMER_PROC* timerProc, int period) {
//    return STimers.AddTmr(period, timerProc);
//}
//
//int ArmletApi::GetUpTime() { return chTimeNow(); }
//void ArmletApi::Sleep(int msces) { chThdSleepMilliseconds(msces); }
//
//unsigned int ArmletApi::GetRandom(unsigned int max) { return Random(max); }

// ============================= File operations ===============================
// return false in case of failure, true in case of success
// Try to open. In case of failure, create new file if bCreate is true, otherwise return error.
/*
bool ArmletApi::OpenFile(FILE* file, const char* filename, bool bCreate) {
    FRESULT Rslt;
    Rslt = f_open((FIL*)file, filename, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
    if(Rslt == FR_OK) return true;
    else {                  // File does not exist
        if(bCreate) {       // Create file if needed
            Rslt = f_open((FIL*)file, filename, FA_CREATE_NEW | FA_READ | FA_WRITE);
            return (Rslt == FR_OK);
        }
        else return false;  // No need to create, return failure
    }
}

// returns length read
int ArmletApi::ReadFile(FILE* file, char* buf, int len) {
    f_lseek((FIL*)file, 0);   // move to beginning
    UINT FLen=0;
    f_read((FIL*)file, buf, len, &FLen);
    return FLen;
}

// returns length written, rewrites file from beginning
int ArmletApi::WriteFile (FILE* file, char* buf, int len) {
    f_lseek((FIL*)file, 0);   // move to beginning
    UINT FLen=0;
    f_write((FIL*)file, buf, len, &FLen);
    return FLen;
}

// return length written, appends to end of file
int ArmletApi::AppendFile(FILE* file, char* buf, int len) {
    if(file == NULL) return 0;
    f_lseek((FIL*)file, f_size((FIL*)file));
    UINT FLen=0;
    f_write((FIL*)file, buf, len, &FLen);
    return FLen;
}
*/
