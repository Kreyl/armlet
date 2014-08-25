#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 10
#define MAX_USER_INTENTIONS_ARRAY_SIZE 15
#define INTENTIONS_ARRAY_SIZE 5
#define WINING_INTEGRAL_SWITCH_LIMIT 50
#define FON_RELAX_SPEED 50
#define MAX_RECENTLY_PLAYED_ARRAY 10

#define PROCESS_NORMAL 0
#define PROCESS_NARCO  1
#define PROCESS_FIGHT  2
#define PROCESS_DEATH  3
#define PROCESS_TUMAN  4
#define PROCESS_MANIAC  5
#define PROCESS_KRAYK  6

//ArrayOfUserIntentions id defines, not buttons, user intentions itself

#define SI_MURDER 0
#define SI_CREATION 1
#define SI_DESTRUCTION 2
#define SI_SEX 3
#define SI_FIGHT 4
#define SI_WEED 5
#define SI_HER 6
#define SI_LSD 7
#define SI_KRAYK 8
#define SI_DEATH 9
#define SI_MANIAC 10
#define SI_TUMAN 11
#define SI_STRAH 12
#define SI_MSOURCE 13
#define SI_PROJECT 14

/*typedef struct Intention {
	int weight1000;	//константа, определ€юща€ степень и врем€ роста
	//int last_measure_weight;//
	//int cast_time_sec;//
	//int legth_time_sec;// стандартна€ продолжительность на плато
	//int cooldown_time_sec;// врем€ спуска с плато
	char* name;		//им€ намерени€
	int emotion_tree_indx;//индекс из массива эмоций, соответствующей музыки намерени€.
} Intention;
*/
//REASON extern struct Intention intentionArray[INTENTIONS_ARRAY_SIZE]; - change to reasons[]

typedef struct IncomingIntentions {
	int reason_indx;	//индекс из стандартного массива
	int power256; //сила сигнала
} IncomingIntentions;


typedef struct UserIntentions {
    int reason_indx;    //индекс из стандартного массива
    int power256_plateau; //[power256] сила сигнала наплато 0-256 если включено.
    int time_to_plateau;//[sec]
    int time_on_plateau;//sec
    int time_after_plateau;//[sec]
    int current_time;//[sec] -1 если не включено, 0,+int если включено
    //если уже музыка играла, true. - нужно дл€ поддержки переподключений
    bool was_winning;
    //storage for data from radiochannel, recalc every radio_in
    int human_support_number;
    int process_type;
    char * p_int_name;//button name if available
    void TurnOff()
    {
        current_time=-1;
        was_winning=false;
    }
    void TurnOn();
} UserIntentions;

extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];

//user_reason_id - номер лемента в этом массиве
extern struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE];

typedef struct IntentionCalculationData
//структура дл€ рассчета изменений по вход€щим намерени€м,
//TODO тут же тики по времени дл€ намерений, если есть, потом сделать
{
	//formula: (IWC*Intention.weight1000+power256*SPWC)/Normalizer
	int Intention_weight_cost;
	int Signal_power_weight_cost;
	int Normalizer;
	int last_intention_power_winner;//NOT NORMALIZED
	int last_intention_index_winner;//reason_indx
	int winning_integral;//NORMALIZED
	int winning_integral_top_limit_normalizer;
	bool is_empty_fon;
    int last_played_emo;
    int last_played_file;
    bool is_last_played_id_recent;//???
    bool is_global_stop_active;
    bool is_everysec_calculation_active;//???
}IntentionCalculationData;

#define HEART_PLAYING_TIME_SEC 20
#define MAX_FIGHT_PLAY_TIME 100
enum GlobalStopType_t {gsDraka, gsNotInited};
typedef struct GlobalStopCalculationSupport
{
    GlobalStopType_t stop_reason_type; //0 - draka
    int timer; //-1 - inactive, else move by sec

    int draka_fight_length;
    int draka_heart_length;
    int GetFightTime();
    void FinishStopCalculation();
    void OnNewSec();
    void BeginStopCalculations(GlobalStopType_t stop_reason_type_in);
} GlobalStopCalculationSupport;

typedef struct IntentionReduceData
{
    int reduced_reason_id;
    int weight_reduced;
    bool is_reason_changed;
}
IntentionReduceData;
typedef struct SRPFEArrayEl
{
    int seek_pos;
    int emo_id;
    int file_indx;
 }SRPFEArrayEl;
typedef struct SeekRecentlyPlayedFilesEmo
{
    int last_array_id;
    int last_played_file_indx;
    int last_played_emo_imdx; //теоретически то дублирование данных, но тут они нужны в св€зке, и чтобы 100 раз везде не провер€ть, пусть лучше safe-load будет в одно изолированное место
    SRPFEArrayEl seek_array[MAX_RECENTLY_PLAYED_ARRAY];
    void OnCallStopPlay(int emo_id,int file_id, int pos);
    //return -1 if no, pos if yes
    int CheckIfRecent(int emo_id,int file_id);
    void IncrementArrayId();
    int GetNext(int current_array_id);
}SeekRecentlyPlayedFilesEmo;

extern struct SeekRecentlyPlayedFilesEmo SRPFESingleton;

void CalculateIntentionsRadioChange();
//returns -1 if winner does not over switch limit, else return reason id

//run through player recieved array of intentoins, and return its power if available, else -1;
//obsolete,not used
int GetPlayerReasonCurrentPower(int reason_id);

int CalculateCurrentPowerOfPlayerReason(int array_indx); //считаеттекущую мощность позаданному стандартному алгоритму
//Obsolete??
void SwitchPlayerReason(int reason_id,bool is_turn_on);  // игрок нажал накнопку резона, вклили выкл.

//UPDATE TO ALL USER REASONS
//в этой же функции обрабатываютс€ проходы наркоподобных при окончании резона!
void CallReasonSuccess(int user_reason_id); // вызоветс€ если игрок отключит резон, все id и проверки внутри
//UPDATE TO ALL USER REASONS
void CallReasonFalure(int user_reason_id); // вызоветс€ если игрок просрал по времени
void PushPlayerReasonToArrayOfIntentions();
//return true if any intention go fall, false otherwise
bool UpdateUserIntentionsTime(int add_time_sec);
int MainCalculateReasons();
extern struct IntentionCalculationData SICD;//SingletonIntentionCalculationData;
extern struct GlobalStopCalculationSupport GSCS;
extern struct IntentionReduceData SRD;
void PrintSCIDToUart();

//user intentions array funcs
void InitArrayOfUserIntentions();
void UserReasonFlagRecalc(int reason_id);

void ReasonAgeModifyChangeMelody();

//в структуре рассчета будет индекс текущего победител€ мощности,
//индекс предыдущего победител€ мощности,
//набранна€ дельта ( по определению принадлежит предыдущему победителю)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
