/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_
/*
void AppInit();
*/
#include "kl_lib_f2xx.h"
#include "cmd_uart.h"
#include  "RxTable.h"
//state.ini defines
#define NARCO_IS_OFF_STATE 0
#define NARCO_IS_ON_STATE 1

#define COPY_RX_TABLE_SZ 100
#define NUM_ROWS_COPY_RX_TABLE 3
#if 1 // ==== Timings ====
#define T_PILL_CHECK_MS         360  // Check if pill connected every TM_PILL_CHECK
#define T_MEASUREMENT_MS        5004 // Battery measurement
#endif

#if 1 // ============================== Pill ===================================
void TmrPillCheckCallback(void *p);
struct Pill_t {
    int32_t Type;
    int32_t ChargeCnt;
};
#define PILL_SZ     sizeof(Pill_t)
#define PILL_SZ32   (sizeof(Pill_t) / sizeof(int32_t))
// Inner Pill addresses
#define PILL_TYPE_ADDR          0
#define PILL_CHARGECNT_ADDR     4
#endif

#if UART_RX_ENABLED
void TmrUartRxCallback(void *p);
#endif
#define CSV_SEPARATOR_CHAR ','

class App_t {
private:
    void LocationValid() {
        CurrInfo.Location |= LOCATION_VALID;
    }
    void LocationInvalid() {
        CurrInfo.Location &= ~LOCATION_VALID;
    }
public:
    int32_t SelfID=0;
    Pill_t Pill;
    Thread *PThd;
    state_t CurrInfo;
    // Timers
    VirtualTimer TmrPillCheck, TmrUartRx;
    void Init();
    void Task();
    void StopEverything();
    void UpdateState();
    //void GetDataFileName();
   char DataFileBuff[33];

#if UART_RX_ENABLED
    void OnUartCmd(Cmd_t *PCmd);
#endif
    int on_run;
    int32_t IRlevel;
    //routine
    int recalc_signal_pw_thr(int pw,int thr);
    bool zero_signal_incoming_cut(int AOII_indx);
    void CallLustraReboot();
    void CallLustraReset();
};

extern App_t App;

#endif /* APPLICATION_H_ */
