#include "intention.h"
#include <stddef.h>
Intention intentionArray[INTENTIONS_ARRAY_SIZE]={
	{1000,"creation",0},			//0
	{1000,"destruction",1},		//1 negativ
	{1000,"horror_house",1},	//2 negativ
	{1000,"wasteland",0},		//3
	{1000,"reaper",4}		//4 positiv
};
struct IncomingIntentions ArrayOfIncomingIntentions[2]={
		{0,2,},{1,3}
};
struct IntentionCalculationData SingletonIntentionCalculationData=
{
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		10,//	int Normalizer;
		0,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;
		};
