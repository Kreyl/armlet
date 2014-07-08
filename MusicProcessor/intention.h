#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 10
#define MAX_USER_INTENTIONS_ARRAY_SIZE 6
#define INTENTIONS_ARRAY_SIZE 5
#define WINING_INTEGRAL_SWITCH_LIMIT 50
#define FON_RELAX_SPEED 50
#define MAX_RECENTLY_PLAYED_ARRAY 10
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
    int current_time;//[sec] -1 ���� �� ��������, 0,+int ���� ��������
    //���� ��� ������ ������, true. - ����� ��� ��������� ���������������
    bool was_winning;
    //storage for data from radiochannel, recalc every radio_in
    int human_support_number;
    void TurnOff()
    {
        current_time=-1;
        was_winning=false;
    }
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
    bool is_last_played_id_recent;
}IntentionCalculationData;

typedef struct SRPFEArrayEl
{
    int seek_pos;
    int emo_id;
    int file_indx;
 }SRPFEArrayEl;
typedef struct SeekRecentlyPlayedFilesEmo
{
    int last_array_id;
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
void CallReasonSuccess(int user_reason_id); // ��������� ���� ����� �������� �����, ��� id � �������� ������
void CallReasonFalure(int user_reason_id); // ��������� ���� ����� ������� �� �������
void PushPlayerReasonToArrayOfIntentions();
//return true if any intention go fall, false otherwise
bool UpdateUserIntentionsTime(int add_time_sec);
int MainCalculateReasons();
extern struct IntentionCalculationData SICD;//SingletonIntentionCalculationData;
void PrintSCIDToUart();

//user intentions array funcs
void InitArrayOfUserIntentions();
void UserReasonFlagRecalc(int reason_id);

//� ��������� �������� ����� ������ �������� ���������� ��������,
//������ ����������� ���������� ��������,
//��������� ������ ( �� ����������� ����������� ����������� ����������)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
