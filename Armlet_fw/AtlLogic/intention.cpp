#include "intention.h"
#include <stddef.h>
#include "cmd_uart.h"
#include "Sound.h"
#include "energy.h"
#include "gui.h"
//TODO move it right
int CurrentIntentionArraySize=2;

struct SeekRecentlyPlayedFilesEmo SRPFESingleton
{
    -1,{
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},
    {0,-1,-1},}
};

//DYNAMIC ARRAY SIZE
struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE]={
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3},
		{1,2},{4,3}
};
//int CurrentUserIntentionsArraySize=6;
#if 0
#define RNAME_FIGHT  "\xc4\xf0\xe0\xea\xe0"
#define RNAME_SEX    "\xd1\xe5\xea\xf1"
#define RNAME_MURDER "\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee"
#define RNAME_DESTR  "\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5"
#define RNAME_CREATION "\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5"
#define RNAME_DEATH "\xd1\xec\xe5\xf0\xf2\xfc"

int reason_indx;    //индекс из стандартного массива
int power256_plateau; //[power256] сила сигнала наплато 0-256 если включено.
int time_to_plateau;//[sec]
int time_on_plateau;//sec
int time_after_plateau;//[sec]
int current_time;//[sec] -1 если не включено, 0,+int если включено
//если уже музыка играла, true. - нужно для поддержки переподключений
bool was_winning;
//storage for data from radiochannel, recalc every radio_in
int human_support_number;
int process_type;
char * p_int_name;//button name if available

#endif

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

//STATIC ARRAY, inits inside, all external are in InitArrayOfUserIntentions
struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE]={
        {-1,25,120,300,120,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_MURDER)},//murder 0
        {-1,25,3,10,2,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_CREATION)},//creation 1
        {-1,25,3,10,2,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_DESTR)},//destruction 2
        {-1,250,1,300,120,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_SEX)},//sex 3
        {-1,250,1,60,120,-1,false,0,PROCESS_FIGHT,const_cast<char *> (RNAME_FIGHT)},//fight 4
        {-1,250,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 5 weed
        {-1,250,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 6 heroin
        {-1,250,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 7 lsd
        {-1,250,1200,2000,400,-1,false,0,PROCESS_KRAYK,nullptr},//narco 8 krayk
        {-1,250,1200,2000,400,-1,false,0,PROCESS_DEATH,const_cast<char *> (RNAME_DEATH)},//death 9
        {-1,250,1200,2000,400,-1,false,0,PROCESS_MANIAC,nullptr},//maniac 10
        {-1,250,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//tuman 11
        {-1,250,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//strah 12
        {-1,250,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//mSource 13
        {-1,250,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//mProject 14
};
void InitArrayOfUserIntentions()
{
    for(int i=0;i<reasons_number;i++)
    {
         if(strcmp(reasons[i].name,"murder")==0)
             ArrayOfUserIntentions[0].reason_indx=i;
         else if(strcmp(reasons[i].name,"creation")==0)
             ArrayOfUserIntentions[1].reason_indx=i;
         else if(strcmp(reasons[i].name,"destruction")==0)
             ArrayOfUserIntentions[2].reason_indx=i;
         else if(strcmp(reasons[i].name,"sex")==0)
             ArrayOfUserIntentions[3].reason_indx=i;
         else if(strcmp(reasons[i].name,"fight")==0)
             ArrayOfUserIntentions[4].reason_indx=i;
         else if(strcmp(reasons[i].name,"weed")==0)//grass
             ArrayOfUserIntentions[5].reason_indx=i;
         else if(strcmp(reasons[i].name,"heroin")==0) //hero
             ArrayOfUserIntentions[6].reason_indx=i;
         else if(strcmp(reasons[i].name,"lsd")==0)//lsd
             ArrayOfUserIntentions[7].reason_indx=i;
         else if(strcmp(reasons[i].name,"krayk")==0)//krayk
             ArrayOfUserIntentions[8].reason_indx=i;
         else if(strcmp(reasons[i].name,"death")==0)//death
             ArrayOfUserIntentions[9].reason_indx=i;
         else if(strcmp(reasons[i].name,"azart")==0)//maniac
             ArrayOfUserIntentions[10].reason_indx=i;
         else if(strcmp(reasons[i].name,"mist")==0)//tuman
             ArrayOfUserIntentions[11].reason_indx=i;
         else if(strcmp(reasons[i].name,"fear")==0)//strah
             ArrayOfUserIntentions[12].reason_indx=i;
         else if(strcmp(reasons[i].name,"mSource")==0)//mSource
             ArrayOfUserIntentions[13].reason_indx=i;
         else if(strcmp(reasons[i].name,"mProject")==0)//mProject
             ArrayOfUserIntentions[14].reason_indx=i;
    }
    //if any is not inited, panic!!
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
    {
        if(ArrayOfUserIntentions[i].reason_indx<0)
            Uart.Printf("CRITICAL ERROR User intention not inited %d , rn %d !!!!\r",i,reasons_number);
    }
    Uart.Printf("InitArrayOfUserIntentions done\r");
}


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
      -1,
      -1,
      false,
      false, //global stop
      false //is_everysec_calculation_active
        /* DO NOT DELETE
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		1,//	int Normalizer;
		-100,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;*/
};


void SeekRecentlyPlayedFilesEmo::OnCallStopPlay(int emo_id,int file_id, int pos)
{
    //this->last_array_id идет в минус по ходу пляски, проверяются в плюс начиная с этого
    //стартовый - ???
    this->IncrementArrayId();
    seek_array[this->last_array_id].emo_id=emo_id;
    seek_array[this->last_array_id].file_indx=file_id;
    seek_array[this->last_array_id].seek_pos=pos;
}
int SeekRecentlyPlayedFilesEmo::CheckIfRecent(int emo_id,int file_id)
{
    int curr_id=this->last_array_id;
    for(int i=0;i<MAX_RECENTLY_PLAYED_ARRAY;i++)
    {

        if(seek_array[this->last_array_id].emo_id==emo_id)
            if(seek_array[this->last_array_id].file_indx==file_id)
                return seek_array[this->last_array_id].seek_pos;

        curr_id=GetNext(curr_id);
    }
    return -1;
}

void SeekRecentlyPlayedFilesEmo::IncrementArrayId()
{
    this->last_array_id--;
    if(this->last_array_id<0)
        this->last_array_id=MAX_RECENTLY_PLAYED_ARRAY-1;
}
int SeekRecentlyPlayedFilesEmo::GetNext(int current_array_id)
{
    if(current_array_id==MAX_RECENTLY_PLAYED_ARRAY-1)
        return 0;
    else
        return current_array_id+1;

}

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
               ArrayOfUserIntentions[i].current_time+=add_time_sec;
               if(ArrayOfUserIntentions[i].current_time>ArrayOfUserIntentions[i].time_to_plateau+ArrayOfUserIntentions[i].time_on_plateau+ArrayOfUserIntentions[i].time_after_plateau)//после плато, на спуске
               {
                   ArrayOfUserIntentions[i].TurnOff();//current_time=-1;
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
void UserReasonFlagRecalc(int reason_id)
{
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
       {
           if(ArrayOfUserIntentions[i].reason_indx==reason_id)
               ArrayOfUserIntentions[i].was_winning=true;
       }
}
int MainCalculateReasons() {
    //return valie:
    //-1; low winner intergral to play new
    //-2??? still enough interalto play,dont switch??
    //????? why play fon now?; -3;
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

    Energy.human_support=ArrayOfUserIntentions[array_indx].human_support_number;
    //всё удлинняет
    int faster_time= Energy.GetEnergyScaleValMore(ArrayOfUserIntentions[array_indx].current_time);//>current_time
    int slower_time= Energy.GetEnergyScaleValLess(ArrayOfUserIntentions[array_indx].current_time);//<current_time


    if(ArrayOfUserIntentions[array_indx].current_time>=0)
    {
        if(faster_time<ArrayOfUserIntentions[array_indx].time_to_plateau)//перед плато
            return (ArrayOfUserIntentions[array_indx].power256_plateau*GetPersentage100(faster_time,ArrayOfUserIntentions[array_indx].time_to_plateau))/100;
        if(slower_time<ArrayOfUserIntentions[array_indx].time_to_plateau+ArrayOfUserIntentions[array_indx].time_on_plateau)//на плато
            return (ArrayOfUserIntentions[array_indx].power256_plateau);
        if(slower_time<ArrayOfUserIntentions[array_indx].time_to_plateau+ArrayOfUserIntentions[array_indx].time_on_plateau+ArrayOfUserIntentions[array_indx].time_after_plateau)//после плато, на спуске
            return (ArrayOfUserIntentions[array_indx].power256_plateau *
                    GetPersentage100(
                            slower_time-ArrayOfUserIntentions[array_indx].time_to_plateau-ArrayOfUserIntentions[array_indx].time_on_plateau
                            ,ArrayOfUserIntentions[array_indx].time_after_plateau
                            )
                    )/100;
        else    //вышли за плато
        {//выключить резон??
            ArrayOfUserIntentions[array_indx].TurnOff();//current_time=-1;
            CallReasonFalure(array_indx);
            return -1;
        }
    }
    else return -1;
}

void CallReasonFalure(int user_reason_id)
{
    if(user_reason_id==SI_SEX)
    {
        // не прерванный секс всегда успешен! ^_^
        CallReasonSuccess(user_reason_id);
        return;
    }
    Uart.Printf("CALL REASON FALURE reason %d\r",user_reason_id);
    Energy.AddEnergy(REASON_FAIL_ENERGY_CHANGE);

   // маньяки икрайк неизлечимо наркозависимы
    if(ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_MANIAC || ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_KRAYK)
    {
        ArrayOfUserIntentions[user_reason_id].current_time=0;
    }
    //если у маньяка что-то неполучилось - убвать не хочет??

    return;
}

void CallReasonSuccess(int user_reason_id)
{
    //если наркозависимость - перезапустить!
    //подумать над размерамиинтервалов! PROCESS_NARCO
    if(ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_NARCO)
        ArrayOfUserIntentions[user_reason_id].current_time=0;
    //если маньяк убил кого-то - вырубить ему намеряние заново. если не нашел - также вырубить, онять нергию.
    if(ArrayOfUserIntentions[SI_MANIAC].current_time>=0 && user_reason_id==SI_MURDER)
        ArrayOfUserIntentions[SI_MANIAC].current_time=0;
    //если крайк что-то сделал - перезапустить его зависимость
    //TODO заменить процессы на SI
    if(ArrayOfUserIntentions[SI_KRAYK].current_time>=0 &&
            (ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_NORMAL ||
             ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_FIGHT
            ))
        ArrayOfUserIntentions[SI_KRAYK].current_time=0;
    Energy.AddEnergy(5);

   return;
}
//not used!???
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
                   ArrayOfUserIntentions[i].TurnOff();//current_time=-1;
               }
               return;
           }
       }
}

