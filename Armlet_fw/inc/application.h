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
#include "atlantis_music_tree.h"
#include "kl_lib_f2xx.h"
#include "cmd_uart.h"
#include  "RxTable.h"
//state.ini defines
#define NARCO_IS_OFF_STATE 0
#define NARCO_IS_ON_STATE 1


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
enum AppState_t {asIdle, asCurrent};

class App_t {
public:
    int32_t ID=0;
    Pill_t Pill;
    Thread *PThd;
    state_t CurrInfo;
    // Timers
    VirtualTimer TmrPillCheck, TmrUartRx;
    void Init();
    void Task();
    void StopEverything();
    void LoadCharacterSettings();
    void LoadData();
    void DropData();//to default
    void SaveData();
    uint8_t ParseCsvFileToEmotions(const char* filename);
    void WriteInentionStringToData(char * int_name, int int_val, char * emo_name);

    void UpdateState();
    //void GetDataFileName();
   char DataFileBuff[33];

#if UART_RX_ENABLED
    void OnUartCmd(Cmd_t *PCmd);
#endif
    char BuffStr[SD_STRING_SZ];
    char reasonstr[SD_STRING_SZ];
    char emostr[SD_STRING_SZ];
    char toiintstr[SD_STRING_SZ];
    int on_run;

    int32_t locationThreshold;
    int32_t forestTheshold;
    int32_t mistThreshold;
    int32_t characterThreshold;
    int32_t IRlevel;
    int recalc_signal_pw_thr(int pw,int thr);
};

extern App_t App;

#endif /* APPLICATION_H_ */
