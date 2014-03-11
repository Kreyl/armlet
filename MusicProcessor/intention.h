#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 10
#define INTENTIONS_ARRAY_SIZE 5
#define WINING_INTEGRAL_SWITCH_LIMIT 400
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
extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];

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
int MainCalculateReasons();
extern struct IntentionCalculationData SICD;//SingletonIntentionCalculationData;
void PrintSCIDToUart();
//� ��������� �������� ����� ������ �������� ���������� ��������,
//������ ����������� ���������� ��������,
//��������� ������ ( �� ����������� ����������� ����������� ����������)
extern int CurrentIntentionArraySize;
//extern IncomingIntentions ArrayOfIncomingIntentions[];
#endif // intention_h__
