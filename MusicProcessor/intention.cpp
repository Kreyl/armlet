#include "intention.h"
#include <stddef.h>
#include "cmd_uart.h"
#include "Sound.h"
//TODO move it right
int CurrentIntentionArraySize=2;
/*Intention intentionArray[INTENTIONS_ARRAY_SIZE]={

	{1000,"creation",0},			//0
	{1000,"destruction",1},		//1 negativ
	{1000,"horror_house",1},	//2 negativ
	{1000,"wasteland",0},		//3
	{1000,"reaper",4}		//4 positiv
};*/

//DYNAMIC ARRAY SIZE
struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE]={
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3}
};
//int CurrentUserIntentionsArraySize=6;
//STATIC ARRAY
struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE]={
        {-1,25,120,300,120,-1},//murder 0
        {-1,25,3,10,2,-1},//creation 1
        {-1,25,3,10,2,-1},//destruction 2
        {-1,250,1,300,120,-1},//sex 3
        {-1,250,1,60,120,-1},//fight 4
        {-1,250,1200,2000,400,-1},//narco 5
};

void InitArrayOfUserIntentions()
{
    for(int i=0;i<reasons_number;i++)
    {
         if(strcmp(reasons[i].name,"murder")==0)
             ArrayOfUserIntentions[0].reason_indx=i;
         if(strcmp(reasons[i].name,"creation")==0)
             ArrayOfUserIntentions[1].reason_indx=i;
         if(strcmp(reasons[i].name,"destruction")==0)
             ArrayOfUserIntentions[2].reason_indx=i;
         if(strcmp(reasons[i].name,"sex")==0)
             ArrayOfUserIntentions[3].reason_indx=i;
         if(strcmp(reasons[i].name,"fight")==0)
             ArrayOfUserIntentions[4].reason_indx=i;
         if(strcmp(reasons[i].name,"narco")==0)
             ArrayOfUserIntentions[5].reason_indx=i;
    }
    //if any is not inited, panic!!
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
    {
        if(ArrayOfUserIntentions[i].reason_indx<0)
            Uart.Printf("CRITICAL ERROR User intention not inited %d , rn %d !!!!\r",i,reasons_number);
    }
    Uart.Printf("InitArrayOfUserIntentions done\r");
}
//typedef struct UserIntentions {
//    int reason_indx;    //индекс из стандартного массива
//    int power256_plateau; //[power256] сила сигнала наплато -1 если не включено, 0-256 если включено.
//    int time_to_plateau;//[sec]
//    int time_after_plateau;//[sec]
//    int current_time;//[sec]
//} UserIntentions;

struct IntentionCalculationData SICD=
{
      10,//  Intention_weight_cost;
      4,//  Signal_power_weight_cost;
      1,//  Normalizer;
      -100,//  last_intention_power_winner;//NOT NORMALIZED
      0,//  last_intention_index_winner;
      0,//  winning_integral;//NORMALIZED
      10000,//  winning_integral_top_limit_normalizer;
      false,
      -1
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
//	Uart.Printf("IWC %d,SPWC %d, Norm %d \r",SICD.Intention_weight_cost,SICD.Signal_power_weight_cost,SICD.Normalizer);
//	Uart.Printf("last_id %d, last_pw %d,Win_int %d \r",SICD.last_intention_index_winner,SICD.last_intention_power_winner,SICD.winning_integral);
	}

int GetNotNormalizedIntegral(int power, int reason_id) {
    Uart.Printf("reas_id=%d, power=%d\r", reason_id, power);
    int res = SICD.Intention_weight_cost*reasons[reason_id].weight+power*SICD.Signal_power_weight_cost;
    Uart.Printf("rRslt=%d\r", res);
    return res;
}

int CalcWinIntegral(int first_winner_power,int winner_id, int second_winner_power, int second_id)
{
    int int1=GetNotNormalizedIntegral(first_winner_power,winner_id);
    int int2=GetNotNormalizedIntegral(second_winner_power,second_id);
	int summ=(int1-int2)/SICD.Normalizer;
	if(summ>=0)
	{
//	    Uart.Printf("win_int %d/%d \r",int1-int2,SICD.Normalizer);
	    return summ;
	}
	else
	{
//	    Uart.Printf("CalcWinIntegral winner lower than 2nd place:\r");
//	    Uart.Printf("pw1 %d, pw2 %d, int1 %d, INT2 %d, i1 %d, i2 %d\r", first_winner_power,second_winner_power,winner_id,second_id,int1,int2 );
		return 0;//ERROR
	}
}

void CalculateIntentionsRadioChange() {
        if(CurrentIntentionArraySize == 0) {
            if(SICD.is_empty_fon == false) {
                SICD.winning_integral=0;
                SICD.last_intention_power_winner=0;
                SICD.is_empty_fon=true;
            }
            else SICD.winning_integral+=FON_RELAX_SPEED;
            return;
        }
        SICD.is_empty_fon=false;
        Uart.Printf("input reas_id=%d, power=%d\r", ArrayOfIncomingIntentions[0].reason_indx,  ArrayOfIncomingIntentions[0].power256);

        if(CurrentIntentionArraySize == 1) {
            if(SICD.last_intention_index_winner != ArrayOfIncomingIntentions[0].reason_indx) SICD.winning_integral=0;

            SICD.winning_integral+=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[0].power256,ArrayOfIncomingIntentions[0].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[0].power256;//get current power!
            SICD.last_intention_index_winner=ArrayOfIncomingIntentions[0].reason_indx;
            return;
        }
        int maxnotnormval=-1;// временная переменная!
        int currnotnormval=0;
        int current_reason_arr_winner_indx=-1;
        int current_incoming_winner_indx=-1;
        for(int i=0; i<CurrentIntentionArraySize; i++) {
            currnotnormval = GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power256,ArrayOfIncomingIntentions[i].reason_indx);
            if(currnotnormval > maxnotnormval) {
                maxnotnormval=currnotnormval;
                current_reason_arr_winner_indx=ArrayOfIncomingIntentions[i].reason_indx;
                current_incoming_winner_indx=i;
            }
        }

        int maxnotnormval2 = -1;// временная переменная!
        int currnotnormval2 = 0;
        int current_reason_arr_winner_indx2 = -1;
        int current_incoming_winner_indx2 = -1;
        if(current_reason_arr_winner_indx == SICD.last_intention_index_winner) {
            for(int i=0; i<CurrentIntentionArraySize; i++) {
                if(ArrayOfIncomingIntentions[i].reason_indx!=current_reason_arr_winner_indx) {
                    currnotnormval2=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power256,ArrayOfIncomingIntentions[i].reason_indx);
                    if(currnotnormval2>maxnotnormval2)
                            {
                                maxnotnormval2=currnotnormval2;
                                current_reason_arr_winner_indx2=ArrayOfIncomingIntentions[i].reason_indx;
                                current_incoming_winner_indx2=i;
                            }
                }
            }
            if(current_reason_arr_winner_indx2<0) return;
            SICD.winning_integral+=CalcWinIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx,
                    ArrayOfIncomingIntentions[current_incoming_winner_indx2].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx2].reason_indx);
            SICD.last_intention_power_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].power256;
            SICD.last_intention_index_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }
        else {
            SICD.winning_integral = GetNotNormalizedIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power256,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].power256;//get current power!
            SICD.last_intention_index_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }
}
bool UpdateUserIntentionsTime(int add_time_sec)
{
    int return_value=false;

    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
       {
           if(ArrayOfUserIntentions[i].current_time>=0)
           {
               Uart.Printf("CALL ADD TIME SUCESS reason %d\r",i);
               ArrayOfUserIntentions[i].current_time+=add_time_sec;
               if(ArrayOfUserIntentions[i].current_time>ArrayOfUserIntentions[i].time_to_plateau+ArrayOfUserIntentions[i].time_on_plateau+ArrayOfUserIntentions[i].time_after_plateau)//после плато, на спуске
               {
                   Uart.Printf("ct%d, summ %d\r", ArrayOfUserIntentions[i].current_time,ArrayOfUserIntentions[i].time_to_plateau+ArrayOfUserIntentions[i].time_on_plateau+ArrayOfUserIntentions[i].time_after_plateau);
                   ArrayOfUserIntentions[i].current_time=-1;
                   CallReasonFalure(i);
                   return_value=true;
               }
           }
       }
    return return_value;
}
void PushPlayerReasonToArrayOfIntentions()
{
    //для каждого пользовательского резона должен быть шаблонный резон, обязательно! если нет- отметиться ошибкой!
    //для каждого резона пересчитать силу, если сила >0 - добавить его в массив ArrayOfIntentions
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
    {
        if(ArrayOfUserIntentions[i].current_time>=0)
        {
            int curr_power=CalculateCurrentPowerOfPlayerReason(i);
            if(curr_power>=0)
            {
                if(curr_power>256)
                    curr_power=256;
                //перезаписываем силу, если есть место в массиве!
                //иначе предупреждение о переполнении
                   if(CurrentIntentionArraySize < MAX_INCOMING_INTENTIONS_ARRAY_SIZE)
                   {
                       //добавляем во входящие
                       ArrayOfIncomingIntentions[CurrentIntentionArraySize].power256=curr_power;
                       ArrayOfIncomingIntentions[CurrentIntentionArraySize].reason_indx=ArrayOfUserIntentions[i].reason_indx;
                       CurrentIntentionArraySize++;
                   }
                   else
                       Uart.Printf("WARNING PushPlayerReasonToArrayOfIntentions ArrayOfIncomingIntentions is to small for total incoming intentions\r");
            }
        }
    }

}

int MainCalculateReasons() {

    bool is_more_than_9000 = false;
//    int old_int = SICD.winning_integral;
    int old_winner = SICD.last_intention_index_winner;
    if(SICD.winning_integral>WINING_INTEGRAL_SWITCH_LIMIT && SICD.is_empty_fon == false) is_more_than_9000 = true;
    CalculateIntentionsRadioChange();
    if(is_more_than_9000 && old_winner == SICD.last_intention_index_winner && SICD.winning_integral>WINING_INTEGRAL_SWITCH_LIMIT && SICD.is_empty_fon == false) return -2;
    if(SICD.winning_integral<WINING_INTEGRAL_SWITCH_LIMIT) return -1;
    if(SICD.is_empty_fon == false) return SICD.last_intention_index_winner;
    if(SICD.last_played_emo!=0)  return -3; // play fon now!
    return -1;
}


int GetPlayerReasonCurrentPower(int reason_id)
{
    for(int i =0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
    {
        if(reason_id==ArrayOfUserIntentions[i].reason_indx)
            return CalculateCurrentPowerOfPlayerReason(i);
    }
    return -1;
    //search in last incoming array, where it's based??
}
int GetPersentage100(int curval,int maxval)
{
    return (curval*100)/maxval;
}
int CalculateCurrentPowerOfPlayerReason(int array_indx)
{//TODO normal func
    if(ArrayOfUserIntentions[array_indx].current_time>=0)
    {
        if(ArrayOfUserIntentions[array_indx].current_time<ArrayOfUserIntentions[array_indx].time_to_plateau)//перед плато
            return (ArrayOfUserIntentions[array_indx].power256_plateau*GetPersentage100(ArrayOfUserIntentions[array_indx].current_time,ArrayOfUserIntentions[array_indx].time_to_plateau))/100;
        if(ArrayOfUserIntentions[array_indx].current_time<ArrayOfUserIntentions[array_indx].time_to_plateau+ArrayOfUserIntentions[array_indx].time_on_plateau)//на плато
            return (ArrayOfUserIntentions[array_indx].power256_plateau);
        if(ArrayOfUserIntentions[array_indx].current_time<ArrayOfUserIntentions[array_indx].time_to_plateau+ArrayOfUserIntentions[array_indx].time_on_plateau+ArrayOfUserIntentions[array_indx].time_after_plateau)//после плато, на спуске
            return (ArrayOfUserIntentions[array_indx].power256_plateau *
                    GetPersentage100(
                            ArrayOfUserIntentions[array_indx].current_time-ArrayOfUserIntentions[array_indx].time_to_plateau-ArrayOfUserIntentions[array_indx].time_on_plateau
                            ,ArrayOfUserIntentions[array_indx].time_after_plateau
                            )
                    )/100;
        else    //вышли за плато
        {//выключить резон??
            ArrayOfUserIntentions[array_indx].current_time=-1;
            CallReasonFalure(array_indx);
            return -1;

        }
    }
    else return -1;
}
void CallReasonFalure(int user_reason_id)
{
    Uart.Printf("CALL REASON FALURE reason %d\r",user_reason_id);
    return;
}

void CallReasonSuccess(int user_reason_id)
{
   return;
}

void SwitchPlayerReason(int reason_id,bool is_turn_on)
{
    for(int i =0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
       {
           if(reason_id==ArrayOfUserIntentions[i].reason_indx)
           {
               if(is_turn_on)
               {
                   ArrayOfUserIntentions[i].current_time=0;
               }
               else //turn off
               {
                   ArrayOfUserIntentions[i].current_time=-1;
               }
               return;
           }
       }
}

