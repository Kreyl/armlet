#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 10
#define MAX_USER_INTENTIONS_ARRAY_SIZE 16
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
#define PROCESS_LOMKA  6


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

/*typedef struct Intention {
	int weight1000;	//���������, ������������ ������� � ����� �����
	//int last_measure_weight;//
	//int cast_time_sec;//
	//int legth_time_sec;// ����������� ����������������� �� �����
	//int cooldown_time_sec;// ����� ������ � �����
	char* name;		//��� ���������
	int emotion_tree_indx;//������ �� ������� ������, ��������������� ������ ���������.
} Intention;
*/
//REASON extern struct Intention intentionArray[INTENTIONS_ARRAY_SIZE]; - change to reasons[]

typedef struct IncomingIntentions {
	int reason_indx;	//������ �� ������������ �������
	int power256; //���� �������
} IncomingIntentions;


typedef struct UserIntentions {
    int reason_indx;    //������ �� ������������ �������
    int power256_plateau; //[power256] ���� ������� ������� 0-256 ���� ��������.
    int time_to_plateau;//[sec]
    int time_on_plateau;//sec
    int time_after_plateau;//[sec]
    int current_time;//[sec] -1 ���� �� ��������, 0,+int ���� �������� -2 ���� �������� ��������� �����������
    //���� ��� ������ ������, true. - ����� ��� ��������� ���������������
    bool was_winning;
    //storage for data from radiochannel, recalc every radio_in
    int human_support_number;
    int process_type;
    char * p_int_name;//button name if available
    void TurnOff();
    void TurnOn();
} UserIntentions;

extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];

//user_reason_id - ����� ������� � ���� �������
extern struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE];

typedef struct IntentionCalculationData
//��������� ��� �������� ��������� �� �������� ����������,
//TODO ��� �� ���� �� ������� ��� ���������, ���� ����, ����� �������
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

//�������������� ����� ��������� ����, ��� ������, �� ����� �������� ��� ������� ���������� ���������. ���� ������������ ������ � �����
enum GlobalStopType_t {gsDraka, gsNotInited};
typedef struct GlobalStopCalculationSupport
{
    GlobalStopType_t stop_reason_type; //0 - draka
    int timer; //-1 - inactive, else move by sec

    int draka_fight_length;
    int draka_heart_length;

    void FinishStopCalculation();
    void OnNewSec();
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
    int last_played_emo_imdx; //������������ �� ������������ ������, �� ��� ��� ����� � ������, � ����� 100 ��� ����� �� ���������, ����� ����� safe-load ����� � ���� ������������� �����
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

int CalculateCurrentPowerOfPlayerReason(int array_indx); //�������������� �������� ����������� ������������ ���������
//Obsolete??
void SwitchPlayerReason(int reason_id,bool is_turn_on);  // ����� ����� �������� ������, ������ ����.

//UPDATE TO ALL USER REASONS
//� ���� �� ������� �������������� ������� ������������� ��� ��������� ������!
void CallReasonSuccess(int user_reason_id); // ��������� ���� ����� �������� �����, ��� id � �������� ������
//UPDATE TO ALL USER REASONS
void CallReasonFalure(int user_reason_id); // ��������� ���� ����� ������� �� �������
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
void WriteRadyToKill(int val_in);
void WriteReadyToKillTimer(int val_in);

void OnGetTumanMessage();

//� ��������� �������� ����� ������ �������� ���������� ��������,
//������ ����������� ���������� ��������,
//��������� ������ ( �� ����������� ����������� ����������� ����������)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
