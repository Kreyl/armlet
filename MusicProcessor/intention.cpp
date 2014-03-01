#include "intention.h"
#include <stddef.h>
#include "cmd_uart.h"
//TODO move it right
int CurrentIntentionArraySize=2;
/*Intention intentionArray[INTENTIONS_ARRAY_SIZE]={
	{1000,"creation",0},			//0
	{1000,"destruction",1},		//1 negativ
	{1000,"horror_house",1},	//2 negativ
	{1000,"wasteland",0},		//3
	{1000,"reaper",4}		//4 positiv
};*/
struct IncomingIntentions ArrayOfIncomingIntentions[2]={
		{2,2},{6,3}
};
struct IntentionCalculationData SICD=
{
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		1,//	int Normalizer;
		0,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;
};
/*
int Intention_weight_cost;
int Signal_power_weight_cost;
int Normalizer;
int last_intention_power_winner;//NOT NORMALIZED
int last_intention_index_winner;
int winning_integral;//NORMALIZED
int winning_integral_top_limit_normalizer;*/

void PrintSCIDToUart()
{
	Uart.Printf("IWC %d,SPWC %d, Norm %d \r",SICD.Intention_weight_cost,SICD.Signal_power_weight_cost,SICD.Normalizer);
	Uart.Printf("last_id %d, last_pw %d,Win_int %d \r",SICD.last_intention_index_winner,SICD.last_intention_power_winner,SICD.winning_integral);
	}

//TODO
int GetNotNormalizedIntegral(int power, int reason_id)
{
    return SICD.Intention_weight_cost*reasons[reason_id].weight+power*SICD.Signal_power_weight_cost;
}

int CalcWinIntegral(int first_winner_power,int winner_id, int second_winner_power, int second_id)
{
    int int1=GetNotNormalizedIntegral(first_winner_power,winner_id);
    int int2=GetNotNormalizedIntegral(second_winner_power,second_id);
	int summ=(int1-int2)/SICD.Normalizer;
	if(summ>=0)
	{
	    Uart.Printf("win_int %d/%d \r",int1-int2,SICD.Normalizer);
	    return summ;
	}
	else
	{
	    Uart.Printf("CalcWinIntegral winner lower than 2nd place");
		return 0;//ERROR
	}
}

void CalculateIntentionsRadioChange()
{
	//formula: (IWC*Intention.weight1000+power256*SPWC)/Normalizer

    //если никого не слышим, сбросить суммы - TODO учесть интегрирование!
        if(CurrentIntentionArraySize==0)
        {
            Uart.Printf("CalculateIntentionsRadioChange array 0\r");
            SICD.winning_integral=0;
            SICD.last_intention_power_winner=0;
            return;
        }

        if(CurrentIntentionArraySize==1)
        {
            Uart.Printf("CalculateIntentionsRadioChange array 1\r");
            SICD.winning_integral+=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[0].power256,ArrayOfIncomingIntentions[0].reason_indx)/SICD.Normalizer;//CalcWinIntegral(ArrayOfIncomingIntentions[0].power256,ArrayOfIncomingIntentions[0].reason_indx,0,0);
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[0].power256;//get current power!
        }

        int maxnotnormval=-1;// временная переменная!
        int currnotnormval=0;
        int current_winner_indx=-1;
        for(int i=0;i<CurrentIntentionArraySize;i++)
        {
            currnotnormval=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power256,ArrayOfIncomingIntentions[i].reason_indx);
            if(currnotnormval>maxnotnormval)
                    {
                        maxnotnormval=currnotnormval;
                        current_winner_indx=ArrayOfIncomingIntentions[i].reason_indx;
                    }
        }
        if(current_winner_indx==SICD.last_intention_index_winner)
        {


        }


	//int winner_power_tmp=SICD.last_intention_power_winner;
/*
	//если никого не слышим, сбросить суммы - учесть интегрирование!
	if(CurrentIntentionArraySize==0)
	{
	    Uart.Printf("CalculateIntentionsRadioChange array 0\r");
		SICD.winning_integral=0;
		SICD.last_intention_power_winner=0;
		return;
	}
	//если массив из 1 элемента, забить на всю эту хрень - учесть интегрирование!
	if(CurrentIntentionArraySize==1)
	{
	    Uart.Printf("CalculateIntentionsRadioChange array 1\r");
		SICD.winning_integral+=CalcWinIntegral(ArrayOfIncomingIntentions[0].power256,0);
		SICD.last_intention_power_winner=ArrayOfIncomingIntentions[0].power256;//get current power!
	}


	int maxnotnormval=-1;// временная переменная!
	int currnotnormval=0;
	int current_winner_indx=-1;
	for(int i=0;i<CurrentIntentionArraySize;i++)
	{
		currnotnormval=SICD.Intention_weight_cost*reasons[ArrayOfIncomingIntentions[i].intention_indx].weight+
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
	    Uart.Printf("same winner\r");
		int secod_norm_val=-1;
		maxnotnormval=-1;
		int current_second_winner_indx=-1;
		//ищем максимум по всем, кроме победителя
		for(int i=0;i<CurrentIntentionArraySize;i++)
		if(i!=current_winner_indx)
		{
			secod_norm_val=SICD.Intention_weight_cost*reasons[ArrayOfIncomingIntentions[i].intention_indx].weight+
					SICD.Signal_power_weight_cost*ArrayOfIncomingIntentions[i].power256;
			if(secod_norm_val>maxnotnormval)
			{
				maxnotnormval=currnotnormval;
				current_second_winner_indx=ArrayOfIncomingIntentions[i].intention_indx;
			}
		}
		//добавляем интеграл из значений обоих
		SICD.winning_integral+=CalcWinIntegral(ArrayOfIncomingIntentions[current_winner_indx].power256,ArrayOfIncomingIntentions[current_second_winner_indx].power256);//f(currnotnormval-secod_norm_val)
		SICD.last_intention_power_winner=currnotnormval;
		return;
	}
	else
		//если новый - начинаем считать заново
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
*/
}
