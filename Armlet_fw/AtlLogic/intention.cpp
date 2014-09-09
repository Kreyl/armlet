#include "intention.h"
#include <stddef.h>
#include "cmd_uart.h"
#include "Sound.h"
#include "energy.h"
#include "gui.h"

//TODO move it right

#define AGE_WEIGHT_SCALE_REDUCE 1
#define AGE_MAX_WEIGHT_REDUCE 5
int CurrentIntentionArraySize=2;

#define DRAKA_MIN_SEC 10
#define DRAKA_MAX_SEC 60
#define DRAKA_STEP ((DRAKA_MAX_SEC-DRAKA_MIN_SEC)/10)
#define DRAKA_RANDOM_SEC_RANGE 7
int GetDrakaTime()
{

    int static_char_timing= ArrayOfUserIntentions[SI_FIGHT].time_on_plateau; //[16-60]
    int energy_bonus= DRAKA_STEP*(Energy.GetEnergy()-START_ENERGY)/(MAX_ENERGY_LVL-MIN_ENERGY_LVL);
    int random_bonus=Random(DRAKA_RANDOM_SEC_RANGE*2)-DRAKA_RANDOM_SEC_RANGE;
    int summ_fight_time=static_char_timing+energy_bonus+random_bonus;
    Uart.Printf("GetDrakaTime result uncropped:%d", summ_fight_time);
    if(summ_fight_time>DRAKA_MAX_SEC)
        return DRAKA_MAX_SEC;
    if(summ_fight_time<DRAKA_MIN_SEC)
        return DRAKA_MIN_SEC;

    return summ_fight_time;
}
void WriteDrakaTimeFromPower(int pwr_in)
{
    ArrayOfUserIntentions[SI_FIGHT].time_on_plateau=DRAKA_MIN_SEC+pwr_in*DRAKA_STEP;
}
void WriteFrontTime(int val_in,int array_indx)
{
    ArrayOfUserIntentions[array_indx].time_to_plateau=val_in;
}
void WriteMidTime(int val_in,int array_indx)
{
    ArrayOfUserIntentions[array_indx].time_on_plateau= val_in;
}
void WriteTailTime(int val_in,int array_indx)
{
    ArrayOfUserIntentions[array_indx].time_after_plateau=val_in;
}
void OnGetTumanMessage(int appid)
{
    Uart.Printf("\rTEST OnGetTumanMessage");
    //если леноран - забить болт !
    if(appid==REASON_RRAY)
        return;
    //если туман или страх активны - ничего не делать
    if(ArrayOfUserIntentions[SI_TUMAN].current_time>=0)// || ArrayOfUserIntentions[SI_STRAH].current_time>=0)
        return;
    else if( ArrayOfUserIntentions[SI_STRAH].current_time>=0)
    {
        //ArrayOfUserIntentions[SI_STRAH].TurnOn();
        //если туман уже прошел, а страх еще играет, включить страх на плато
        ArrayOfUserIntentions[SI_STRAH].was_winning=true;
        ArrayOfUserIntentions[SI_STRAH].current_time=Energy.GetEnergyScaleValLess(ArrayOfUserIntentions[SI_STRAH].time_to_plateau,SI_STRAH)+1;
                //Energy.GetEnergyScaleValMore(Energy.GetEnergyScaleValLessDefault(ArrayOfUserIntentions[SI_STRAH].time_to_plateau))+1;
        //Uart.Printf("\r TUMAN OFF STRAH ON , ctime= %d", ArrayOfUserIntentions[SI_STRAH].current_time);
        // Energy.GetEnergyScaleValMoreDefault(int1)
    }
    else
    {
        //Uart.Printf("\r TUMAN OFF STRAH OFF INITIAL START");
        //включить туман и страх
        ArrayOfUserIntentions[SI_TUMAN].TurnOn();
        ArrayOfUserIntentions[SI_STRAH].TurnOn();
    }
}
struct SeekRecentlyPlayedFilesEmo SRPFESingleton
{
    -1,-1,-1,{
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
struct IncomingIntentions ArrayOfIncomingIntentions[MAX_INCOMING_INTENTIONS_ARRAY_SIZE];
//                                                    ={
//		{1,2},{4,3},
//		{1,2},{4,3},
//		{1,2},{4,3},
//		{1,2},{4,3},
//		{1,2},{4,3}
//};

void UserIntentions::TurnOn()
{
    //включить с начала! плато не пройдено!
    this->current_time=0;
    this->was_winning=false;
}
//int CurrentUserIntentionsArraySize=6;
#if 0
#define RNAME_FIGHT  "\xc4\xf0\xe0\xea\xe0"
#define RNAME_SEX    "\xd1\xe5\xea\xf1"
#define RNAME_MURDER "\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee"
#define RNAME_DESTR  "\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5"
#define RNAME_CREATION "\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5"
#define RNAME_DEATH "\xd1\xec\xe5\xf0\xf2\xfc"

int reason_indx;    //индекс из стандартного массива
int power512_plateau; //[power512] сила сигнала наплато 0-256 если включено.
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

//ARRAY, inits inside, all external are in InitArrayOfUserIntentions
struct UserIntentions ArrayOfUserIntentions[MAX_USER_INTENTIONS_ARRAY_SIZE]={
        {-1,-1,-1,-1,-1,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_MURDER)},//murder 0     INITED
        {-1,-1,-1,-1,-1,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_CREATION)},//creation 1     INITED
        {-1,-1,-1,-1,-1,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_DESTR)},//destruction 2     INITED
        {-1,-1,-1,-1,-1,-1,false,0,PROCESS_NORMAL,const_cast<char *> (RNAME_SEX)},//sex 3     INITED
        {-1,-1,-1,-1,-1,-1,false,0,PROCESS_FIGHT,const_cast<char *> (RNAME_FIGHT)},//fight 4 generally used only time on plateau,others not used atexternal pipeline
        //TODO
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 5 weed     INITED
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 6 heroin     INITED
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 7 lsd     INITED
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_KRAYK,nullptr},//narco 8 krayk
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_DEATH,const_cast<char *> (RNAME_DEATH)},//death 9
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_MANIAC,nullptr},//maniac 10
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//tuman 11
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//strah 12
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//mSource 13
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_TUMAN,nullptr},//mProject 14
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_LOMKA,nullptr},//narco 15 lomka
        {-1,-1,1200,2000,400,-1,false,0,PROCESS_NARCO,nullptr},//narco 7 kakt     INITED
};
void InitArrayOfUserIntentions()
{
   // for(int i=0;i<NUMBER_OF_REASONS;i++)
    {

             ArrayOfUserIntentions[SI_MURDER].reason_indx=REASON_MURDER;
             ArrayOfUserIntentions[SI_CREATION].reason_indx=REASON_CREATION;
             ArrayOfUserIntentions[SI_DESTRUCTION].reason_indx=REASON_DESTRUCTION;
             ArrayOfUserIntentions[SI_SEX].reason_indx=REASON_SEX;
             ArrayOfUserIntentions[SI_FIGHT].reason_indx=REASON_FIGHT;


             ArrayOfUserIntentions[SI_WEED].reason_indx=REASON_WEED;
             ArrayOfUserIntentions[SI_HER].reason_indx=REASON_HEROIN;
             ArrayOfUserIntentions[SI_LSD].reason_indx=REASON_LSD;
             ArrayOfUserIntentions[SI_KAKT].reason_indx=REASON_PEYOTE;
         //маньяк и крайк слышат в ушах одно и то-же, если успешно делают своё дело - ничего, 0 силы.

             ArrayOfUserIntentions[SI_KRAYK].reason_indx=REASON_KRAYK;
             ArrayOfUserIntentions[SI_DEATH].reason_indx=REASON_DEATH;
             ArrayOfUserIntentions[SI_MANIAC].reason_indx=REASON_KRAYK;
             ArrayOfUserIntentions[SI_TUMAN].reason_indx=REASON_MIST;
             ArrayOfUserIntentions[SI_STRAH].reason_indx=REASON_FEAR;
             ArrayOfUserIntentions[SI_MSOURCE].reason_indx=REASON_MSOURCE;
             ArrayOfUserIntentions[SI_PROJECT].reason_indx=REASON_MPROJECT;
             ArrayOfUserIntentions[SI_WITHDRAWAL].reason_indx=REASON_ADDICTION;
    }
    //if any is not inited, panic!!
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
    {
        if(ArrayOfUserIntentions[i].reason_indx<0)
            Uart.Printf("CRITICAL ERROR User intention not inited %d , rn %d !!!!\r",i,NUMBER_OF_REASONS);
        else//переносим вес в мощность сигнала
        {
            ArrayOfUserIntentions[i].power512_plateau=reasons[ArrayOfUserIntentions[i].reason_indx].weight*SICD.Intention_weight_cost/SICD.Signal_power_weight_cost;
            reasons[ArrayOfUserIntentions[i].reason_indx].weight=0;
            //инициализируем хвост!
            if(i!=SI_MURDER);
            WriteTailTime(300,i);
        }
    }
    //now data!
    WriteFrontTime(2,SI_SEX);
    WriteMidTime(240,SI_SEX);

    WriteFrontTime(600,SI_CREATION);
    WriteMidTime(600,SI_CREATION);

    WriteFrontTime(600,SI_DESTRUCTION);
    WriteMidTime(600,SI_DESTRUCTION);

    WriteFrontTime(300,SI_WEED);
    WriteMidTime(600,SI_WEED);

    WriteFrontTime(300,SI_HER);
    WriteMidTime(600,SI_HER);

    WriteFrontTime(3600*5,SI_WITHDRAWAL);
    WriteFrontTime(3600+1800,SI_WITHDRAWAL);
    //TODO mid time!

    WriteFrontTime(7*60,SI_LSD);
    WriteMidTime(15*60,SI_LSD);

    WriteFrontTime(7*60,SI_KAKT);
    WriteMidTime(15*60,SI_KAKT);


    WriteFrontTime(0,SI_TUMAN);
    WriteFrontTime(0,SI_STRAH);

    WriteFrontTime(0,SI_DEATH);
    WriteMidTime(60000,SI_DEATH);

    WriteMidTime(60,SI_TUMAN);
    WriteMidTime(600,SI_STRAH);

    WriteTailTime(5,SI_TUMAN);
    WriteTailTime(5,SI_STRAH);

#ifdef BRACELET_TEST_MODE_VALS
    WriteFrontTime(5,SI_SEX);
    WriteMidTime(150,SI_SEX);
    WriteTailTime(20,SI_SEX);

    WriteFrontTime(5,SI_CREATION);
    WriteMidTime(150,SI_CREATION);
    WriteTailTime(20,SI_CREATION);

    WriteFrontTime(5,SI_DESTRUCTION);
    WriteMidTime(150,SI_DESTRUCTION);
    WriteTailTime(20,SI_DESTRUCTION);

    WriteFrontTime(30,SI_WITHDRAWAL);
    WriteMidTime(30,SI_WITHDRAWAL);
    WriteTailTime(30,SI_WITHDRAWAL);

    WriteFrontTime(5,SI_HER);
    WriteMidTime(20,SI_HER);
    WriteTailTime(20,SI_HER);

    WriteFrontTime(10,SI_LSD);
    WriteMidTime(40,SI_LSD);
    WriteTailTime(20,SI_LSD);

    WriteFrontTime(10,SI_WEED);
    WriteMidTime(40,SI_WEED);
    WriteTailTime(20,SI_WEED);

    WriteFrontTime(10,SI_KAKT);
    WriteMidTime(40,SI_KAKT);
    WriteTailTime(20,SI_KAKT);

    WriteFrontTime(0,SI_TUMAN);
    WriteFrontTime(21,SI_STRAH);

    WriteMidTime(25,SI_TUMAN);
    WriteMidTime(20,SI_STRAH);

    WriteFrontTime(0,SI_DEATH);
    WriteMidTime(200,SI_DEATH);
#endif
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
        if(i!=SI_FIGHT)
        ArrayOfUserIntentions[i].NormalizeToDefEnergy();
    Uart.Printf("\rInitArrayOfUserIntentions done");
}

struct GlobalStopCalculationSupport GSCS=
{
        gsNotInited,
        -1, //timer
       false, //EmoHerInfoIsOnTheRun
        -1,-1 //draka

};
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
      false, //global stop
      -1
        /* DO NOT DELETE
		10,//int Intention_weight_cost;
		4,//	int Signal_power_weight_cost;
		1,//	int Normalizer;
		-100,//	int last_intention_power_winner;
		1,//	int new_intention_power_winner;
		0,//	int winning_integral;
		10//int winning_integral_top_limit_normalizer;*/
};
struct IntentionReduceData SRD=
{
        -1,
        -1,
        -1,
        false
};

void SeekRecentlyPlayedFilesEmo::OnCallStopPlay(int emo_id,int file_id, uint32_t pos)
{
    //this->last_array_id идет в минус по ходу пляски, проверяются в плюс начиная с этого
    //стартовый - ???

    // проверяем на намеряния - если у корня и не эмоции!
    if(emotions[emo_id].parent==0 && strcmp(emotions[emo_id].name,"pozitiv")!=0 && strcmp(emotions[emo_id].name,"negativ")!=0 &&
            strcmp(emotions[emo_id].name,"duhovnoe")!=0 && strcmp(emotions[emo_id].name,"zhelanie")!=0)
        return;
    //Uart.Printf("\r STOPPLAY SEEK emo %d file %d pos %u,",emo_id,file_id,pos);
    //если уже есть этот файл с этой позицией - значит это конец трека, сбросить ему позицию на ноль и не запоминать!
    for(int i=0;i<MAX_RECENTLY_PLAYED_ARRAY;i++)
    {
        if(seek_array[i].emo_id==emo_id && seek_array[i].file_indx==file_id )//&&seek_array[i].seek_pos==pos)
        {
            Uart.Printf("\r STOPPLAY SEEK SAME FILE  emo %d %d file %d pos %u, indx %d,seek_pos %u,",emo_id,file_id,pos,i,seek_array[i].seek_pos);
           // pos=0;
            seek_array[i].seek_pos=0;
            return;
        }
    }
    this->IncrementArrayId();
    seek_array[this->last_array_id].emo_id=emo_id;
    seek_array[this->last_array_id].file_indx=file_id;
    seek_array[this->last_array_id].seek_pos=pos;
    //Uart.Printf("\r STOPPLAY SEEK SAME FILE INCREMENTED  emo %d %d file %d pos %u, indx %d,seek_pos %u,",emo_id,file_id,pos,this->last_array_id,seek_array[this->last_array_id].seek_pos);
}
int SeekRecentlyPlayedFilesEmo::CheckIfRecent(int emo_id,int file_id)
{
    int curr_id=this->last_array_id;
    for(int i=0;i<MAX_RECENTLY_PLAYED_ARRAY;i++)
    {

        if(seek_array[curr_id].emo_id==emo_id)
            if(seek_array[curr_id].file_indx==file_id)
            {
                int rval=seek_array[curr_id].seek_pos;
                seek_array[curr_id].seek_pos=0;
                return rval;
            }
        curr_id=GetNext(curr_id);
    }
    return 0;
}
void GlobalStopCalculationSupport::BeginStopCalculations(GlobalStopType_t stop_reason_type_in)
{
    if(stop_reason_type_in==gsHerInfo)
        EmoHerInfoIsOnTheRun=true;
    this->stop_reason_type=stop_reason_type_in;
    this->timer=0;
    SICD.is_global_stop_active=true;
    Uart.Printf("\rGlobalStopCalculationSupport::BeginStopCalculations()");
}
//если пользователь пытается вырубить драку, возвращяет
int GlobalStopCalculationSupport::TryDrakaShutdown()
{
    if(timer<draka_fight_length)
    {
        FinishStopCalculation(gsDraka);
        return BUTTON_NORMAL;
    }
    return BUTTON_ENABLED;
}
void GlobalStopCalculationSupport::OnMusicStopHerInfo()
{
    if(EmoHerInfoIsOnTheRun==true && timer>3)
    {
        EmoHerInfoIsOnTheRun=false;
        //turnoff!
        FinishStopCalculation(gsHerInfo);
    }
    else
        EmoHerInfoIsOnTheRun=false;
    Uart.Printf("\rOnMusicStopHerInfo CALLED");

}
int GlobalStopCalculationSupport::FinishStopCalculation(GlobalStopType_t stop_reason_type_in)
{
    Uart.Printf("\rGlobalStopCalculationSupport::FinishStopCalculation() START");
    int return_val= BUTTON_NORMAL;
    if(stop_reason_type_in==gsDraka)
    {
        Uart.Printf("\rGlobalStopCalculationSupport::FinishStopCalculation DRAKA TURN OFF");
        Energy.AddEnergy(REASON_SUCESS_ENERGY_CHANGE);
            ArrayOfUserIntentions[SI_FIGHT].TurnOff();
    }
    if(stop_reason_type_in==gsHerInfo)
    {
        Uart.Printf("\rGlobalStopCalculationSupport::FinishStopCalculation HERINFO TURN OFF");
        ArrayOfUserIntentions[SI_HER].TurnOn();
    }

    this->timer=-1;
    SICD.is_global_stop_active=false;

    SICD.last_reason_active_armlet=last_reason_active_armlet_backup;
    PlayNewEmo(reasons[SICD.last_reason_active_armlet].eID,8,false);//уже остановились!!
    //NEW EMO????
    //поменять last_indx_winner???
    //PlayNewEmo(0,8,true);// ТУТ подумать!! //TODO
    Uart.Printf("\rGlobalStopCalculationSupport::FinishStopCalculation()");

    return return_val;
}
bool GlobalStopCalculationSupport::OnNewSec()
{
    bool is_redraw=false;
    if(this->stop_reason_type==gsDraka)
    {
        if(timer==0)
        {
            draka_fight_length=GetDrakaTime();
            draka_heart_length= HEART_PLAYING_TIME_SEC;
            PlayNewEmo(reasons[ArrayOfUserIntentions[SI_FIGHT].reason_indx].eID,6,true);
            last_reason_active_armlet_backup=SICD.last_reason_active_armlet;
            SICD.last_reason_active_armlet=ArrayOfUserIntentions[SI_FIGHT].reason_indx;
            //TODO проверить что сюда пишется!!

            //turn offotheruserintentions,without energy changes
            if(ArrayOfUserIntentions[SI_MURDER].current_time>=0)
                ArrayOfUserIntentions[SI_MURDER].TurnOff();
            if(ArrayOfUserIntentions[SI_CREATION].current_time>=0)
                ArrayOfUserIntentions[SI_CREATION].TurnOff();
            if(ArrayOfUserIntentions[SI_DESTRUCTION].current_time>=0)
                ArrayOfUserIntentions[SI_DESTRUCTION].TurnOff();
            if(ArrayOfUserIntentions[SI_SEX].current_time>=0)
                ArrayOfUserIntentions[SI_SEX].TurnOff();
            is_redraw=true;
        }
        if(timer==draka_fight_length)
        {
            PlayNewEmo(reasons[REASON_HEARTBEAT].eID,7,true);
            SICD.last_reason_active_armlet=REASON_HEARTBEAT;
        }

        if(timer==draka_fight_length+draka_heart_length)
        {
            FinishStopCalculation(gsDraka);
            return true;
        }
    }

    if(this->stop_reason_type==gsHerInfo)
    {
       // Uart.Printf("\rGlobalStopCalculationSupport EmoHerInfoIsOnTheRun TIMER=%d",timer);
        if(timer==0)
        {
            PlayNewEmo(EMOTION_NARKOMAN,12,true);
            last_reason_active_armlet_backup=SICD.last_reason_active_armlet;
            SICD.last_reason_active_armlet=ArrayOfUserIntentions[SI_FIGHT].reason_indx;
        }
        else if(EmoHerInfoIsOnTheRun==false) //backup code,ifemostoppedandfail
        {
            Uart.Printf("\rGlobalStopCalculationSupport EmoHerInfoIsOnTheRun==false");
            FinishStopCalculation(gsHerInfo);
        }
    }

    this->timer++;
    Uart.Printf("\rGlobalStopCalculationSupport::OnNewSec() timer %d",timer);
    if(is_redraw)
        return true;
    return false;
}
void SeekRecentlyPlayedFilesEmo::IncrementArrayId()
{
    //на самом деле строчка ненужна, но надо понимать как оплучается стартовая позиция
    if(last_array_id==-1)
        last_array_id=0;
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
    //Uart.Printf("\rGetNotNormalizedIntegral reas_id=%d, power=%d\r", reason_id, power);
    int res = SICD.Intention_weight_cost*reasons[reason_id].weight+power*SICD.Signal_power_weight_cost;
    //Uart.Printf("rRslt=%d\r", res);
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
        //Uart.Printf("\r\ninput reas_id=%d, power=%d", ArrayOfIncomingIntentions[0].reason_indx,  ArrayOfIncomingIntentions[0].power512);

        if(CurrentIntentionArraySize == 1) {
            if(SICD.last_intention_index_winner != ArrayOfIncomingIntentions[0].reason_indx) SICD.winning_integral=0;

            SICD.winning_integral+=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[0].power512,ArrayOfIncomingIntentions[0].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner=ArrayOfIncomingIntentions[0].power512;//get current power!
            SICD.last_intention_index_winner=ArrayOfIncomingIntentions[0].reason_indx;
            return;
        }

        //ЛОКАЛЬНО СНИЖАЕМ ВЕС
        int wr=0;
        if(SRD.reduced_reason_id>=0)
        {
            wr=MIN(SRD.weight_reduced,reasons[SRD.reduced_reason_id].weight);
            reasons[SRD.reduced_reason_id].weight-=wr;
        }

        int maxnotnormval=-1;// временная переменная!
        int currnotnormval=0;
        int current_reason_arr_winner_indx=-1;
        int current_incoming_winner_indx=-1;
        for(int i=0; i<CurrentIntentionArraySize; i++) {
            currnotnormval = GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power512,ArrayOfIncomingIntentions[i].reason_indx);
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
                    currnotnormval2=GetNotNormalizedIntegral(ArrayOfIncomingIntentions[i].power512,ArrayOfIncomingIntentions[i].reason_indx);
                    if(currnotnormval2>maxnotnormval2)
                            {
                                maxnotnormval2=currnotnormval2;
                                current_reason_arr_winner_indx2=ArrayOfIncomingIntentions[i].reason_indx;
                                current_incoming_winner_indx2=i;
                            }
                }
            }
            if(current_reason_arr_winner_indx2<0) return;
            SICD.winning_integral+=CalcWinIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power512,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx,
                    ArrayOfIncomingIntentions[current_incoming_winner_indx2].power512,ArrayOfIncomingIntentions[current_incoming_winner_indx2].reason_indx);
            SICD.last_intention_power_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].power512;
            SICD.last_intention_index_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }
        else {
            SICD.winning_integral = GetNotNormalizedIntegral(ArrayOfIncomingIntentions[current_incoming_winner_indx].power512,ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx)/SICD.Normalizer;
            SICD.last_intention_power_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].power512;//get current power!
            SICD.last_intention_index_winner = ArrayOfIncomingIntentions[current_incoming_winner_indx].reason_indx;
        }
        Uart.Printf("\r SICD.winning_integral %d, iw %d",SICD.winning_integral,SICD.last_intention_index_winner);
        //ЛОКАЛЬНО ПОДНИМАЕМ ВЕС
        if(SRD.reduced_reason_id>=0)
            reasons[SRD.reduced_reason_id].weight+=wr;
}
bool UpdateUserIntentionsTime(int add_time_sec)
{
    //перерисовки выше! в app
    int return_value=false;
    for(int i=0;i<MAX_USER_INTENTIONS_ARRAY_SIZE;i++)
       {
           if(ArrayOfUserIntentions[i].current_time>=0)
           {
               bool is_on_plateau_last_sec=UIIsOnPlateau(i);
               ArrayOfUserIntentions[i].current_time+=add_time_sec;
               bool is_after_plateau_last_sec=UIIsONTail(i);
               //TODO тут должна быть перерисовка отдельной кнопки, но поскольку я не знаю отсюда какая то должна быть кнопка - рисуем всё
               if(is_on_plateau_last_sec==true &&is_after_plateau_last_sec==true)
                   return_value=true;
               //пересчитываем для отрисовки
               //в момент перехода с плато на суффикс - перерисовываем экран.

               //выключаем намерение если оно пролетело
               if(CalculateCurrentPowerOfPlayerReason(i,true)==-1)
                   return_value=true;

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
            //Uart.Printf("\rPushPlayerReasonToArrayOfIntentions Int_ indx%d",i);
            int curr_power=CalculateCurrentPowerOfPlayerReason(i);
            if(curr_power>=0)
            {
                if(curr_power>512*SICD.Intention_weight_cost/SICD.Signal_power_weight_cost)
                    curr_power=512*SICD.Intention_weight_cost/SICD.Signal_power_weight_cost;
                //перезаписываем силу, если есть место в массиве!
                //иначе предупреждение о переполнении
                   if(CurrentIntentionArraySize < MAX_INCOMING_INTENTIONS_ARRAY_SIZE)
                   {
                       //добавляем во входящие
                       Uart.Printf("\rPushPlayerReasonToArrayOfIntentions PW%d RI %d",curr_power,ArrayOfUserIntentions[i].reason_indx);
                       ArrayOfIncomingIntentions[CurrentIntentionArraySize].power512=curr_power;
                       ArrayOfIncomingIntentions[CurrentIntentionArraySize].reason_indx=ArrayOfUserIntentions[i].reason_indx;
                       CurrentIntentionArraySize++;
                   }
                   else
                       Uart.Printf("WARNING PushPlayerReasonToArrayOfIntentions ArrayOfIncomingIntentions is to small for total incoming intentions\r");
            }
            //else
                //Uart.Printf("\rPushPlayerReasonToArrayOfIntentions EMPTY");//
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

int GetPersentage100(int curval,int maxval)
{
    return (curval*100)/maxval;
}
void PrintTimingsForEnergy(int en_val)
{
    int energy_backup=Energy.GetEnergy();
    Energy.SetEnergy(en_val);


    Energy.SetEnergy(energy_backup);
}
void UserIntentions::OnChangedEmo(int UI_indx)
{
    if(was_winning)
    {
        this->current_time=Energy.GetEnergyScaleValMore(time_on_plateau,UI_indx)+1;
    }
}
int UserIntentions::OnTurnOffManually(bool short_or_long,int SI_indx)
{
    if(short_or_long==false)
    {
        CallReasonSuccess(SI_indx);
        return BUTTON_NORMAL;

    }
    else
    {
        if(UIIsONTail(SI_indx))
        {
            this->was_winning=false;
            this->current_time=Energy.GetEnergyScaleValLess(time_to_plateau,SI_indx)/2;
            return BUTTON_ENABLED;
        }
        else
        {
            CallReasonFalure(SI_indx);
            return BUTTON_NORMAL;
        }
    }

}
void UserIntentions::NormalizeToDefEnergy()
{
    int etp=this->time_to_plateau;
    int eop= this->time_on_plateau;
    int eap=this->time_after_plateau;
    int int1=etp;
    int int2=etp+eop;
    int int3=etp+eop+eap;
    int tp= Energy.GetEnergyScaleValMoreDefault(int1);
    int op=Energy.GetEnergyScaleValLessDefault(int2);
    int ap=Energy.GetEnergyScaleValLessDefault(int3);
    this->time_to_plateau=tp;
    this->time_on_plateau=op;
    this->time_after_plateau=ap;
   // Uart.Printf("\r NORMALIZE %d %d %d, in data %d,%d,%d en %d", tp,op,ap,etp,eop,eap, Energy.GetEnergy());
}
bool UIIsONTail(int array_indx)
{
    if(ArrayOfUserIntentions[array_indx].current_time<0)//не активировано
        return false;
    int faster_time= Energy.GetEnergyScaleValMore(ArrayOfUserIntentions[array_indx].current_time,array_indx);
    int slower_time= Energy.GetEnergyScaleValLess(ArrayOfUserIntentions[array_indx].current_time,array_indx);//<current_time
    if(slower_time<ArrayOfUserIntentions[array_indx].time_after_plateau)// хвост не кончился [op-ap] notpassed
        if(slower_time>=ArrayOfUserIntentions[array_indx].time_on_plateau)//уже были на плато [tp-op]pass
            if(faster_time>ArrayOfUserIntentions[array_indx].time_to_plateau) //[0-tp] pass
            return true;
    return false;
}
bool UIIsOnPlateau(int array_indx)
{
    if(ArrayOfUserIntentions[array_indx].current_time<0)//не активировано
        return false;
    int faster_time= Energy.GetEnergyScaleValMore(ArrayOfUserIntentions[array_indx].current_time,array_indx);
    int slower_time= Energy.GetEnergyScaleValLess(ArrayOfUserIntentions[array_indx].current_time,array_indx);
    if(slower_time<ArrayOfUserIntentions[array_indx].time_on_plateau)// на плато [tp-op] not passed
                if(faster_time>ArrayOfUserIntentions[array_indx].time_to_plateau) //[0-tp] passed
                return true;
        return false;
}

int CalculateCurrentPowerOfPlayerReason(int array_indx, bool is_change)
{//TODO normal func

   // Energy.SetEnergy(50);
    Energy.human_support=ArrayOfUserIntentions[array_indx].human_support_number;
    //всё удлинняет
    int faster_time= Energy.GetEnergyScaleValMore(ArrayOfUserIntentions[array_indx].current_time,array_indx);//>current_time
    int slower_time= Energy.GetEnergyScaleValLess(ArrayOfUserIntentions[array_indx].current_time,array_indx);//<current_time

    Uart.Printf("\r EN %d tp%d OP%d AP%d",Energy.GetEnergy(),ArrayOfUserIntentions[array_indx].time_to_plateau,ArrayOfUserIntentions[array_indx].time_on_plateau,ArrayOfUserIntentions[array_indx].time_after_plateau);
    Uart.Printf("\r FT %d LT %d,CT%d FLT %d, LFT%d",faster_time,slower_time, ArrayOfUserIntentions[array_indx].current_time,Energy.GetEnergyScaleValMore(slower_time,array_indx),Energy.GetEnergyScaleValLess(faster_time,array_indx));
    if(ArrayOfUserIntentions[array_indx].current_time>=0)
    {
        if(faster_time<ArrayOfUserIntentions[array_indx].time_to_plateau)//перед плато
            return 0;
        if(slower_time<ArrayOfUserIntentions[array_indx].time_on_plateau)//на плато
            return (ArrayOfUserIntentions[array_indx].power512_plateau);
        if(slower_time<ArrayOfUserIntentions[array_indx].time_after_plateau)//после плато, на спуске
            return 0;
        else    //вышли за плато
        {//выключить резон??
            if(!is_change)
                return -1;
            Uart.Printf("\rTURN_OFF_REASON tp%d op %d ap %d curr %d" ,ArrayOfUserIntentions[array_indx].time_to_plateau ,ArrayOfUserIntentions[array_indx].time_on_plateau ,ArrayOfUserIntentions[array_indx].time_after_plateau,slower_time );
            ArrayOfUserIntentions[array_indx].TurnOff();//current_time=-1;
            CallReasonFalure(array_indx);
            return -1;
        }
    }
    else return -1;
}
void UserIntentions::TurnOff()
{
    current_time=-1;
    was_winning=false;
}
void CallReasonFalure(int user_reason_id)
{
    Uart.Printf("\rCALL REASON FALURE RID %d",user_reason_id);
    ArrayOfUserIntentions[user_reason_id].was_winning=false;
    if(user_reason_id==SI_SEX) //здесь еше должна быть драка, но она в другом pipeline???
    {
        Uart.Printf("\rCALL REASON FALURE SEX %d\r",user_reason_id);
        // не прерванный секс всегда успешен! ^_^
        //CallReasonSuccess сам обрабатывает выключения!
        CallReasonSuccess(user_reason_id);
        return;
    }
    if(user_reason_id==SI_FIGHT) //здесь еше должна быть драка, но она в другом pipeline???
       {
           Uart.Printf("\rCALL REASON FALURE FIGHT %d\r",user_reason_id);
           CallReasonSuccess(user_reason_id);
           return;
       }
    Uart.Printf("\rCALL REASON FALURE reason %d\r",user_reason_id);
    //если это одно из включаемых намеряний - сфейлить.
    if( ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_NORMAL && ArrayOfUserIntentions[user_reason_id].current_time>ENERGY_SEC_IGNORE_SHUTDOWN)
    Energy.AddEnergy(REASON_FAIL_ENERGY_CHANGE);

   // если кайф перестает действовать. включается ломка кайф выключается!
    if(user_reason_id==SI_KRAYK || user_reason_id==SI_MANIAC ||user_reason_id==SI_HER)
    {
        Uart.Printf("\rKAIFOFF LOMKA ON reason %d\r",user_reason_id);
        ArrayOfUserIntentions[user_reason_id].current_time=-2;
        ArrayOfUserIntentions[SI_WITHDRAWAL].TurnOn();
    }
    // маньяки и крайк неизлечимо наркозависимы - при окончании ломки включить её еще. а героин просто проходит
    if( user_reason_id==SI_WITHDRAWAL)
    {
        if(ArrayOfUserIntentions[SI_KRAYK].current_time==-2 || ArrayOfUserIntentions[SI_KRAYK].current_time>=0)
            ArrayOfUserIntentions[user_reason_id].TurnOn();
        if(ArrayOfUserIntentions[SI_MANIAC].current_time==-2 || ArrayOfUserIntentions[SI_MANIAC].current_time>=0)
            ArrayOfUserIntentions[user_reason_id].TurnOn();
        return;
    }
    //если это смерть - сбросить нергию в 10
    if(user_reason_id==SI_DEATH && ArrayOfUserIntentions[user_reason_id].current_time>ENERGY_SEC_SAFE_ON_DEATH_SHUTDOWN)
        Energy.SetEnergy(ENERGY_VALUE_ON_DEATH_SHUTDOWN);

    //для всех нормальных - выключиться.
    ArrayOfUserIntentions[user_reason_id].TurnOff();

    return;
}
//вызывается только пользовательскими резонами!!!! при ручном отключении/ после секса/драки
void CallReasonSuccess(int user_reason_id)
{
    ArrayOfUserIntentions[user_reason_id].was_winning=false;
    //если наркозависимость - перезапустить!
    //подумать над размерамиинтервалов! PROCESS_NARCO

    //ok
    //маньяк и крайк неизлечимы - при окончании кайфа перезапускается ломка
        if(user_reason_id==SI_KRAYK || user_reason_id==SI_MANIAC)
        {
            ArrayOfUserIntentions[SI_WITHDRAWAL].TurnOn();
        }
    //ok маньяк получает за убийство.
    //если маньяк убил кого-то - вырубить ему намеряние заново. если не нашел - также вырубить, онять нергию.
    if((ArrayOfUserIntentions[SI_MANIAC].current_time>=0 || ArrayOfUserIntentions[SI_MANIAC].current_time==-2) && user_reason_id==SI_MURDER)
    {
        ArrayOfUserIntentions[SI_MANIAC].TurnOn();//включить кайф
        ArrayOfUserIntentions[SI_WITHDRAWAL].TurnOn();
    }
    //если крайк что-то сделал - перезапустить его зависимость
    //ok крайкполучает за обычне как и все
    if((ArrayOfUserIntentions[SI_KRAYK].current_time>=0 || ArrayOfUserIntentions[SI_KRAYK].current_time==-2) &&
            (ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_NORMAL ||
             ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_FIGHT)
       )
    {
        ArrayOfUserIntentions[SI_KRAYK].TurnOn(); //включить кайф
        ArrayOfUserIntentions[SI_WITHDRAWAL].TurnOn();
    }
    if( ArrayOfUserIntentions[user_reason_id].process_type==PROCESS_NORMAL && ArrayOfUserIntentions[user_reason_id].current_time>ENERGY_SEC_IGNORE_SHUTDOWN)
        Energy.AddEnergy(REASON_SUCESS_ENERGY_CHANGE);
    else
        Uart.Printf("\r CallReasonSuccess ENERGY_NOT_ADDED, rid %d",user_reason_id);

    //если это смерть - сбросить нергию в 10
    if(user_reason_id==SI_DEATH && ArrayOfUserIntentions[user_reason_id].current_time>ENERGY_SEC_SAFE_ON_DEATH_SHUTDOWN)
        Energy.SetEnergy(ENERGY_VALUE_ON_DEATH_SHUTDOWN);
    ArrayOfUserIntentions[user_reason_id].TurnOff();
   return;
}
void ReasonAgeModifyChangeMelody()
{
    //если нет резона уменьшеного - задать этот и уменьшить.
    if(SRD.reduced_reason_id==-1)
    {
        SRD.reduced_reason_id=SICD.last_intention_index_winner;
        SRD.weight_reduced=1;
        SRD.is_reason_changed=false;
    }
    else
    {
        if(SRD.reduced_reason_id==SICD.last_intention_index_winner) //если есть резон, этот - еще уменьшить
            SRD.weight_reduced+=AGE_WEIGHT_SCALE_REDUCE;
        else//если есть резон, но уже другой - задать этот заново
        {
            SRD.reduced_reason_id=SICD.last_intention_index_winner;
            SRD.overthrower_reason_id=-1;
            SRD.weight_reduced=AGE_WEIGHT_SCALE_REDUCE;
            SRD.is_reason_changed=false;
        }
    }
    if(SRD.weight_reduced>AGE_MAX_WEIGHT_REDUCE)
        SRD.weight_reduced=AGE_MAX_WEIGHT_REDUCE;
}
