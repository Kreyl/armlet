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
struct RowSt_t
{
    int ID;
    int Level;
    int Reason;
};
struct TableSt_t {
    RowSt_t Row[COPY_RX_TABLE_SZ];
    int current_row_size;
    void ParseTableToStorage();
    //RxTableRow_t * ptr_curr_row;
};
struct RxTableSt_t{
    TableSt_t RxStorage[NUM_ROWS_COPY_RX_TABLE];
    void Recalc_storage();//get new and get new result!
    void Init();
    TableSt_t RxStorageResult; //сюдарезультаты
    int current_table_id;// сюда парсим приходящее
    TableSt_t * PTable; //для простоты копипасты, мб встроим это дело в основное, чтобы небыло n+1
};
#define CSV_SEPARATOR_CHAR ','
enum AppState_t {asIdle, asCurrent};

class App_t {
public:
    int32_t ID=0;
    Pill_t Pill;
    Thread *PThd;
    state_t CurrInfo;
    RxTableSt_t Table_buff;
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
    void CheckUserIntentionsOnSwitchToTail(int reason_id);//проверяем, не выперли ли кого с музыки, причина - музыка играет не та, что должна на этот резон!
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
    int32_t lodgeTheshold;
    int32_t emotionFixTheshold;
    int32_t mobThreshold;
    //masterThreshold=60
//    mobThreshold=60+
//    placeholderThreshold=60
//    locationThreshold=60+
//    forestTheshold=60+
//    lodgeTheshold=60+
//    emotionFixTheshold=60+
//    mistThreshold=0+
//    characterThreshold=0+
//    IRlevel=75

    //routine
    int recalc_signal_pw_thr(int pw,int thr);
    bool zero_signal_incoming_cut(int AOII_indx);
    void CallLustraReboot();
    void CallLustraReset();
};

extern App_t App;

#endif /* APPLICATION_H_ */
