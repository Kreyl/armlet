#include "intention.h"
#include <stddef.h>
//TODO move it right
int CurrentIntentionArraySize=2;
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
struct IntentionCalculationData SICD=
{
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		10,//	int Normalizer;
		0,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;
		};

//TODO
int CalcWinIntegral()
{
	return -1;
	}

void CalculateIntentionsRadioChange()
{
	//formula: (IWC*Intention.weight1000+power256*SPWC)/Normalizer
	//int winner_power_tmp=SICD.last_intention_power_winner;
	//TODO если массив из 1 элемента, забить на всю эту хрень
	if(CurrentIntentionArraySize==0)
	{
		SICD.winning_integral=0;
		SICD.last_intention_power_winner=0;
		return;
	}
	if(CurrentIntentionArraySize==1)
	{
		SICD.winning_integral+=CalcWinIntegral();
		SICD.last_intention_power_winner;//get current power!
	}


	int maxnotnormval=-1;// временная переменная!
	int currnotnormval=0;
	int current_winner_indx=-1;
	for(int i=0;i<CurrentIntentionArraySize;i++)
	{
		currnotnormval=SICD.Intention_weight_cost*intentionArray[ArrayOfIncomingIntentions[i].intention_indx].weight1000+
				SICD.Signal_power_weight_cost*ArrayOfIncomingIntentions[i].power256;
		if(currnotnormval>maxnotnormval)
		{
			maxnotnormval=currnotnormval;
			current_winner_indx=ArrayOfIncomingIntentions[i].intention_indx;
		}
	}

	//если тот же победитель, смотрим значение второго?
	if(current_winner_indx==SICD.last_intention_index_winner)
	{
		int secod_norm_val=-1;
		maxnotnormval=-1;
		int current_second_winner_indx=-1;
		//ищем максимум по всем, кроме победителя
		for(int i=0;i<CurrentIntentionArraySize;i++)
		if(i!=current_winner_indx)
		{
			secod_norm_val=SICD.Intention_weight_cost*intentionArray[ArrayOfIncomingIntentions[i].intention_indx].weight1000+
					SICD.Signal_power_weight_cost*ArrayOfIncomingIntentions[i].power256;
			if(secod_norm_val>maxnotnormval)
			{
				maxnotnormval=currnotnormval;
				current_second_winner_indx=ArrayOfIncomingIntentions[i].intention_indx;
			}
		}
		//добавляем интеграл из значений обоих
		SICD.winning_integral+=CalcWinIntegral();//f(currnotnormval-secod_norm_val)
		SICD.last_intention_power_winner=currnotnormval;
		return;
	}
	else
	{
		SICD.winning_integral=0;
		SICD.last_intention_power_winner=currnotnormval;
		SICD.last_intention_index_winner=current_winner_indx;
		return;
	}

	//get new winner, and 2nd
	//check if same, if not- write 0 to integrall

	//move last winner
	//write new winner

}
