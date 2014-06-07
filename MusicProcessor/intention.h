#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 10
#define MAX_USER_INTENTIONS_ARRAY_SIZE 2
#define INTENTIONS_ARRAY_SIZE 5
#define WINING_INTEGRAL_SWITCH_LIMIT 50
#define FON_RELAX_SPEED 50
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
    int current_time;//[sec] -1 ���� �� ��������
} UserIntentions;

extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];
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
	int last_intention_index_winner;
	int winning_integral;//NORMALIZED
	int winning_integral_top_limit_normalizer;
	bool is_empty_fon;
    int last_played_emo;
}IntentionCalculationData;
void CalculateIntentionsRadioChange();
//returns -1 if winner does not over switch limit, else return reason id

//run through player recieved array of intentoins, and return its power if available, else -1;
int GetPlayerReasonCurrentPower(int reason_id);
int CalculateCurrentPowerOfPlayerReason(int array_indx);
void SwitchPlayerReason(int reason_id,bool is_turn_on);

int MainCalculateReasons();
extern struct IntentionCalculationData SICD;//SingletonIntentionCalculationData;
void PrintSCIDToUart();
//� ��������� �������� ����� ������ �������� ���������� ��������,
//������ ����������� ���������� ��������,
//��������� ������ ( �� ����������� ����������� ����������� ����������)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
