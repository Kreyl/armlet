#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 120
#define MAX_USER_INTENTIONS_ARRAY_SIZE 17
#define INTENTIONS_ARRAY_SIZE 5
#define WINING_INTEGRAL_SWITCH_LIMIT 500
#define FON_RELAX_SPEED 500
#define MAX_RECENTLY_PLAYED_ARRAY 10

#define PROCESS_NORMAL 0
#define PROCESS_NARCO  1
#define PROCESS_FIGHT  2
#define PROCESS_DEATH  3
#define PROCESS_TUMAN  4
#define PROCESS_MANIAC  5
#define PROCESS_KRAYK  6
#define PROCESS_LOMKA  6

#define BRACELET_TEST_MODE_VALS
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
#define SI_WITHDRAWAL 15
#define SI_KAKT 16

/*typedef struct Intention {
	int weight1000;	//константа, определяющая степень и время роста
	//int last_measure_weight;//
	//int cast_time_sec;//
	//int legth_time_sec;// стандартная продолжительность на плато
	//int cooldown_time_sec;// время спуска с плато
	char* name;		//имя намерения
	int emotion_tree_indx;//индекс из массива эмоций, соответствующей музыки намерения.
} Intention;
*/
//REASON extern struct Intention intentionArray[INTENTIONS_ARRAY_SIZE]; - change to reasons[]

typedef struct IncomingIntentions {
	int reason_indx;	//индекс из стандартного массива
	int power512; //сила сигнала
} IncomingIntentions;


typedef struct UserIntentions {
    int reason_indx;    //индекс из стандартного массива
    int power512_plateau; //[power512] сила сигнала наплато 0-512 если включено.

    //ВНИМАНИЕ! на вводе - это интервалы состояния. в коде - интервалы от 0 времени! иначе не приводится :(
    int time_to_plateau;//[sec]
    int time_on_plateau;//sec
    int time_after_plateau;//[sec]

    int current_time;//[sec] -1 если не включено, 0,+int если включено -2 если является носителем зависимости
    //если уже музыка играла, true. - нужно для поддержки переподключений
    bool was_winning;
    //storage for data from radiochannel, recalc every radio_in
    int human_support_number;
    int process_type;
    char * p_int_name;//button name if available
    void TurnOff();
    void TurnOn();
    void NormalizeToDefEnergy();
    void OnChangedEmo(int UI_indx);// if changed after winning - change time to end of plateau
    void PrintTimingsForEnergy(int en_val);
    //return new visual state of button
    int OnTurnOffManually(bool short_or_long, int SI_indx); //true - short, false - long
    //bool is_on_tail();
} UserIntentions;

extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];

//user_reason_id - номер лемента в этом массиве
extern struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE];

typedef struct IntentionCalculationData
//структура для рассчета изменений по входящим намерениям,
//TODO тут же тики по времени для намерений, если есть, потом сделать
{
	//formula: (IWC*Intention.weight1000+power512*SPWC)/Normalizer
	int Intention_weight_cost;
	int Signal_power_weight_cost;
	int Normalizer;
	int last_intention_power_winner;//NOT NORMALIZED
	int last_intention_index_winner;//reason_indx
	int winning_integral;//NORMALIZED
	int winning_integral_top_limit_normalizer;
	bool is_empty_fon; //TODO removefonkostil and check ifit works
    int last_played_emo;
    int last_played_file;
    bool is_global_stop_active;
    int last_reason_active_armlet;// последний активный резон с точки зрения игрока нужен для выдачи на базу.
}IntentionCalculationData;

#define HEART_PLAYING_TIME_SEC 20

//альтернативная ветка рассчетов того, что играть, со своим таймером для каждого отдельного включения. пока используется только в драке
enum GlobalStopType_t {gsDraka, gsNotInited, gsHerInfo};
typedef struct GlobalStopCalculationSupport
{
    GlobalStopType_t stop_reason_type; //0 - draka
    int timer; //-1 - inactive, else move by sec

//herinfo support
    bool EmoHerInfoIsOnTheRun;
    // filename!! "common/narkoman-.mp3"NARKOMAN
//herinfo support end


//draka support
    int draka_fight_length;
    int draka_heart_length;
    int TryDrakaShutdown();
//draka support end
    void OnMusicStopHerInfo();
    int last_reason_active_armlet_backup;//этой штукой заводим заново музыку после конца остановки!

    //return visual on button!
    int FinishStopCalculation(GlobalStopType_t stop_reason_type_in);

    //false if no need redraw, true if redraw needed
    bool OnNewSec();
    void BeginStopCalculations(GlobalStopType_t stop_reason_type_in);
} GlobalStopCalculationSupport;

typedef struct IntentionReduceData
{
    int reduced_reason_id;
    int overthrower_reason_id;
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
    int last_played_emo_imdx; //теоретически то дублирование данных, но тут они нужны в связке, и чтобы 100 раз везде не проверять, пусть лучше safe-load будет в одно изолированное место
    SRPFEArrayEl seek_array[MAX_RECENTLY_PLAYED_ARRAY];
    void OnCallStopPlay(int emo_id,int file_id, int pos);
    //return -1 if no, pos if yes
    int CheckIfRecent(int emo_id,int file_id);
    void IncrementArrayId();
    int GetNext(int current_array_id);
}SeekRecentlyPlayedFilesEmo;

extern struct SeekRecentlyPlayedFilesEmo SRPFESingleton;
//extern struct GlobalStopCalculationSupport GSCS;

void CalculateIntentionsRadioChange();
//returns -1 if winner does not over switch limit, else return reason id

bool UIIsONTail(int array_indx);
bool UIIsOnPlateau(int array_indx);


int CalculateCurrentPowerOfPlayerReason(int array_indx, bool is_change=true); //считаеттекущую мощность позаданному стандартному алгоритму, if(is_change) - меняет состояние
//Obsolete??
void SwitchPlayerReason(int reason_id,bool is_turn_on);  // игрок нажал накнопку резона, вклили выкл.

//UPDATE TO ALL USER REASONS
//в этой же функции обрабатываются проходы наркоподобных при окончании резона!
void CallReasonSuccess(int user_reason_id); // вызовется если игрок отключит резон, все id и проверки внутри
//UPDATE TO ALL USER REASONS
void CallReasonFalure(int user_reason_id); // вызовется если игрок просрал по времени
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

void WriteDrakaTimeFromPower(int pwr_in);
void WriteFrontTime(int val_in,int array_indx);
void WriteMidTime(int val_in,int array_indx);
void WriteTailTime(int val_in,int array_indx);

void OnGetTumanMessage(int appid);

//в структуре рассчета будет индекс текущего победителя мощности,
//индекс предыдущего победителя мощности,
//набранная дельта ( по определению принадлежит предыдущему победителю)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
