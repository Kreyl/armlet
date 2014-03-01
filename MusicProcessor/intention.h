#ifndef intention_h__
#define intention_h__
//#include "emotions.h"
#include "atlantis_music_tree.h"
#define MAX_INCOMING_INTENTIONS_ARRAY_SIZE 2
#define INTENTIONS_ARRAY_SIZE 5
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
	int intention_indx;	//индекс из стандартного массива
	int power256; //сила сигнала
} IncomingIntentions;
extern struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];

typedef struct IntentionCalculationData
//структура для рассчета изменений по входящим намерениям,
//TODO тут же тики по времени для намерений, если есть, потом сделать
{
	//formula: (IWC*Intention.weight1000+power256*SPWC)/Normalizer
	int Intention_weight_cost;
	int Signal_power_weight_cost;
	int Normalizer;
	int last_intention_power_winner;//NOT NORMALIZED
	int last_intention_index_winner;
	int winning_integral;//NORMALIZED
	int winning_integral_top_limit_normalizer;
}IntentionCalculationData;
void CalculateIntentionsRadioChange();

extern struct IntentionCalculationData SICD;//SingletonIntentionCalculationData;
void PrintSCIDToUart();
//в структуре рассчета будет индекс текущего победителя мощности,
//индекс предыдущего победителя мощности,
//набранная дельта ( по определению принадлежит предыдущему победителю)

#endif // intention_h__
