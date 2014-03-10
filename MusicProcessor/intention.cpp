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
struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE]={
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3}
};
struct IntentionCalculationData SICD=
{
      10,//  Intention_weight_cost;
      4,//  Signal_power_weight_cost;
      1,//  Normalizer;
      -100,//  last_intention_power_winner;//NOT NORMALIZED
      0,//  last_intention_index_winner;
      0,//  winning_integral;//NORMALIZED
      10000//  winning_integral_top_limit_normalizer;

        /*
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		1,//	int Normalizer;
		-100,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;*/
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
	    Uart.Printf("CalcWinIntegral winner lower than 2nd place:\r");
	    Uart.Printf("pw1 %d, pw2 %d, int1 %d, INT2 %d, i1 %d, i2 %d\r",
	            first_winner_power,second_winner_power,winner_id,second_id,int1,int2
	            );
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
           // Uart.Printf("CalculateIntentionsRadioChange win_int %d, win_id %d \r",SICD.winning_integral,current_reason_arr_winner_indx);
            return;
        }

        if(CurrentIntentionArraySize==1)
        {
            Uart.Printf("CalculateIntentionsRadioChange array 1\r");
            //если не тот же, сбрасываем интеграл
            if(SICD.last_intention_index_winner!=ArrayOfIncomingIntentions[0].reason_indx)
                SICD.winning_integral=0;

            SICD.winning_integral+=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[0].power256,ArrayOfIncomingIntentions[0].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[0].power256;//get current power!
            SICD.last_intention_index_winner=ArrayOfIncomingIntentions[0].reason_indx;
            Uart.Printf("CalculateIntentionsRadioChange win_int %d, win_id %d \r",SICD.winning_integral, SICD.last_intention_index_winner);
            return;
        }

        int maxnotnormval=-1;// временная переменная!
        int currnotnormval=0;
        int current_reason_arr_winner_indx=-1;
        int current_incoming_winner_indx=-1;
        for(int i=0;i<CurrentIntentionArraySize;i++)
        {
          //  Uart.Printf("CalculateIntentionsRadioChange incoming i %d, id %d, pw %d\r",i,ArrayOfIncomingIntentions[i].reason_indx,ArrayOfIncomingIntentions[i].power256);
            currnotnormval=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power256,ArrayOfIncomingIntentions[i].reason_indx);
            if(currnotnormval>maxnotnormval)
                    {
                        maxnotnormval=currnotnormval;
                        current_reason_arr_winner_indx=ArrayOfIncomingIntentions[i].reason_indx;
                        current_incoming_winner_indx=i;
                    }
        }
       // Uart.Printf("CalculateIntentionsRadioChange  current_winner_indx %d\r",current_reason_arr_winner_indx);

        int maxnotnormval2=-1;// временная переменная!
        int currnotnormval2=0;
        int current_reason_arr_winner_indx2=-1;
        int current_incoming_winner_indx2=-1;
        if(current_reason_arr_winner_indx==SICD.last_intention_index_winner)
        {
            for(int i=0;i<CurrentIntentionArraySize;i++)
            {
                if(ArrayOfIncomingIntentions[i].reason_indx!=current_reason_arr_winner_indx)
                {
                    currnotnormval2=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power256,ArrayOfIncomingIntentions[i].reason_indx);
                    if(currnotnormval2>maxnotnormval2)
                            {
                                maxnotnormval2=currnotnormval2;
                                current_reason_arr_winner_indx2=ArrayOfIncomingIntentions[i].reason_indx;
                                current_incoming_winner_indx2=i;
                            }
                }
            }
            if(current_reason_arr_winner_indx2<0)
            {
                Uart.Printf("CalculateIntentionsRadioChange  ERROR current_winner_indx2<0\r");
                return;
            }
            //Uart.Printf("CalculateIntentionsRadioChange  second on tail reason 1stid= %d, 2ndid=%d \r",current_reason_arr_winner_indx,current_reason_arr_winner_indx2);
           // Uart.Printf("CalculateIntentionsRadioChange  second on tail, 1pwr %d, 1id %d, 2pwr %d, 2id %d\r",
           //         ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx,
           //         ArrayOfIncomingIntentions[current_incoming_winner_indx2].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx2].reason_indx
           // );
            //добавляем интеграл из значений обоих
            SICD.winning_integral+=CalcWinIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx,
                    ArrayOfIncomingIntentions[current_incoming_winner_indx2].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx2].reason_indx);
            //Uart.Printf("CalculateIntentionsRadioChange  second on tail, 1notnorm %d, 2notnorm %d\r",currnotnormval,currnotnormval2);
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[current_incoming_winner_indx].power256;
            SICD.last_intention_index_winner=ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }
        else//новый победитель, сбрасываем
        {
           // Uart.Printf("CalculateIntentionsRadioChange  new winner, 1pwr %d, 1id %d, last = %d\r",
           //         ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,current_incoming_winner_indx,SICD.last_intention_index_winner);//ArrayOfIncomingIntentions[current_winner_indx].reason_indx);
            SICD.winning_integral=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[current_incoming_winner_indx].power256;//get current power!
            SICD.last_intention_index_winner=ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }


        Uart.Printf("CalculateIntentionsRadioChange win_int %d, win_id %d \r",SICD.winning_integral,current_reason_arr_winner_indx);
}

int MainCalculateReasons()
{
    CalculateIntentionsRadioChange();

    if(SICD.winning_integral<WINING_INTEGRAL_SWITCH_LIMIT)
        return -1;
    else return SICD.last_intention_index_winner;
}
