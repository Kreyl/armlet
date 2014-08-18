#include "AtlGuiCF.h"
#include "atlgui.h"
#include "cmd_uart.h"
#include "gui.h"
#include "intention.h"
#include "sound.h"
#define SOUND_STEP 25
#define MAX_VOL_CONST 256
#define MIN_VOL_CONST 0

#define SCREENS_WITH_REASONS 2
typedef struct t_scrbtoarr
{   int scr_id;//TODO change to screen name later
    int BtoR[NUM_BUTTONS];
    const char * scr_name;
    //-1 if no, indx in ButtonsToUserReasons if yes
    bool is_this_screen_with_reasons(const char * screen_name)
    {
            if(strcmp(scr_name,screen_name)==0)
               return true;
        return false;
    }
    }t_scrbtoarr;

t_scrbtoarr ButtonsToUserReasons[SCREENS_WITH_REASONS]=

{
    {1,
        {-1,-1,-1,-1,-1,-1,-1,-1,-1},
        //{-1,0,1,3,4,2,-1,-1,-1},
        "intentions"
    },
    {0,
        {-1,-1,-1,-1,-1,-1,-1,-1,-1},
        "main"
    },
};
#if 0
#define RNAME_FIGHT  "\xc4\xf0\xe0\xea\xe0"
#define RNAME_SEX    "\xd1\xe5\xea\xf1"
#define RNAME_MURDER "\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee"
#define RNAME_DESTR  "\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5"
#define RNAME_CREATION "\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5"
#define RNAME_DEATH "\xd1\xec\xe5\xf0\xf2\xfc"
#endif
void InitButtonsToUserReasons()
{
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
    {
        for(int j=0;j<screens_number;j++)
        //if screen is with reasons
        if(strcmp(ButtonsToUserReasons[i].scr_name,screens[j].name)==0)
        {
//            Uart.Printf("InitButtonsToUserReasons found user reason button i %d, j %d\r",i,j);
            for(unsigned int k=0;k<NUM_BUTTONS;k++)
            {
                for(int lm=0;lm<MAX_USER_INTENTIONS_ARRAY_SIZE;lm++)
                    if(ArrayOfUserIntentions[lm].p_int_name!=nullptr)
                        if(strcmp(ArrayOfUserIntentions[lm].p_int_name,screens[j].buttons[k].name)==0)
                        {
                            //если строка названия в ArrayOfUserIntentions  совпадает со строкой названия на кнопке - привязываем!
                            ButtonsToUserReasons[i].BtoR[k]=lm;
//                            Uart.Printf("InitButtonsToUserReasons connected user_reason %s to screen %s button %d ",ArrayOfUserIntentions[lm].p_int_name,ButtonsToUserReasons[i].scr_name,k);
                        }
                 //if(strcmp(RNAME_FIGHT,
            }
        }

    }
}
int current_volume_lvl=START_VOL_CONST;
void CallMainToReason()
{
    Uart.Printf("MainToReason\r");
}
void CallReasonToMain()
{
    Uart.Printf("ReasonToMain\r");
}



int bSoundUpCheck(int screen_id, int button_id)
{
    //int max_vol= 250;
    if(current_volume_lvl+SOUND_STEP<MAX_VOL_CONST)
    {
        Uart.Printf("\rUP vol %d ss%d mvc%d", current_volume_lvl,SOUND_STEP,MAX_VOL_CONST);
        return BUTTON_PRESSABLE;
    }
    else
    {
        Uart.Printf("\rsound up LOCKED", current_volume_lvl);
        return BUTTON_LOCKED;
    }
}
int bSoundDownCheck(int screen_id, int button_id)
{
    if(current_volume_lvl-SOUND_STEP>=MIN_VOL_CONST)
    return BUTTON_PRESSABLE;
    else
        return BUTTON_LOCKED;
}
int bSoundUpChange(int screen_id, int button_id ,int press_mode)
{
    if(press_mode!=0)
        return BUTTON_NO_REDRAW;
    //ничего не чекает - все проверки в соседнем вызове
    current_volume_lvl+=SOUND_STEP;
    Sound.SetVolume(current_volume_lvl);
    Uart.Printf("sound up to %d \r", current_volume_lvl);
    //если рядом есть звук , проверить его и перерисовать!
    for(int i=0;i<9;i++)
    if(screens[screen_id].buttons[i].press==bSoundDownChange)
    {
        AtlGui.RenderSingleButton(screen_id,i,BUTTON_NORMAL);
        break;//только 2 кнопкизвука на экране!
    }
    return bSoundUpCheck(screen_id,button_id);
}
int bSoundDownChange(int screen_id, int button_id ,int press_mode)
{
    if(press_mode!=0)
        return BUTTON_NO_REDRAW;
   // buttonIsPressable(1,10);
    current_volume_lvl-=SOUND_STEP;
    Sound.SetVolume(current_volume_lvl);
    Uart.Printf("sound down to %d\r", current_volume_lvl);
    //если рядом есть звук , проверить его и перерисовать!
    for(int i=0;i<9;i++)
    if(screens[screen_id].buttons[i].press==bSoundUpChange)
    {
        AtlGui.RenderSingleButton(screen_id,i,BUTTON_NORMAL);
        break;//только 2 кнопкизвука на экране!
    }
    return bSoundDownCheck(screen_id,button_id);
}
#ifdef __cplusplus
extern "C" {
#endif

    int buttonIsPressable(int screen_id, int button_id)
    {
        return BUTTON_PRESSABLE;
    }
    int buttonGetState(int screen_id, int button_id)
    {
        return 0;
    }
    int buttonPress(int screen_id, int button_id, int press_mode)
    {
        if(press_mode!=0)
            return BUTTON_NO_REDRAW;
       return BUTTON_NORMAL;
    }
#ifdef __cplusplus
}
#endif
//reason button
int bReasonCheck(int screen_id, int button_id)
{
    Uart.Printf("\rCALL bReasonCheck");
    //если есть соответствие кнопке и резону, то можно нажимать?
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
    {
        for(int i=0;i<SCREENS_WITH_REASONS;i++)
            if(ButtonsToUserReasons[i].is_this_screen_with_reasons(screens[screen_id].name))
            {
                if(ButtonsToUserReasons[i].BtoR[button_id]>=0)
                {
                   // if(ArrayOfUserIntentions[ButtonsToUserReasons[i].BtoR[button_id]].current_time>=0)
                   //     return BUTTON_ENABLED; //on the run!
                    //TODO сюда надо вставлять проверку на неотключаемые резоны???
                    Uart.Printf("\rCALL bReasonCheck return %d",BUTTON_PRESSABLE);
                    return BUTTON_PRESSABLE;
                }
            }

    }
    Uart.Printf("\rCALL bReasonCheck return %d",BUTTON_LOCKED);
    return BUTTON_LOCKED; //error??
}
int bReasonGetState(int screen_id, int button_id)
{
    Uart.Printf("\rCALL bReasonGetState");
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
       {
           if(ButtonsToUserReasons[i].is_this_screen_with_reasons(screens[screen_id].name))
           {
               if(ButtonsToUserReasons[i].BtoR[button_id]>=0)
               {
                  if(ArrayOfUserIntentions[ButtonsToUserReasons[i].BtoR[button_id]].current_time>=0)
                  {
                      Uart.Printf("\rbReasonGetState ENABLED, time %d , B%d",ArrayOfUserIntentions[ButtonsToUserReasons[i].BtoR[button_id]].current_time,button_id);
                      return BUTTON_ENABLED;
                  }
                   //TODO сюда надо вставлять проверку на неотключаемые резоны???
                   return BUTTON_NORMAL;
               }
           }

       }
       return BUTTON_ERROR; //error??
}
int bReasonChange(int screen_id, int button_id ,int press_mode)
{
    if(press_mode!=0)
        return BUTTON_NO_REDRAW;
   // Uart.Printf("CALL bReasonChange \r");
    //button is available, so just get it
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
       {
           if(ButtonsToUserReasons[i].is_this_screen_with_reasons(screens[screen_id].name))
          // if(screen_id==ButtonsToUserReasons[i].scr_id)
           {
               int reason_id=ButtonsToUserReasons[i].BtoR[button_id];
               if(reason_id<0)
               {
                   Uart.Printf("\rCALL bReasonChange REASON ERROR");
                   return BUTTON_ERROR;
               }
               int user_reason_indx=ButtonsToUserReasons[i].BtoR[button_id];
              // if(GetPlayerReasonCurrentPower(reason_id)<0) true
               if(ArrayOfUserIntentions[user_reason_indx].current_time==-1)
               {
                   //setup reason
                   ArrayOfUserIntentions[user_reason_indx].current_time=0;
                   Uart.Printf("\rCALL bReasonChange REASON IS ON THE RUN");
                   //return red
                   return BUTTON_ENABLED;
               }
               else
               {
                   //turn off??
                   //if we were playing, current reason to play is not our, and pressed
                   if(ArrayOfUserIntentions[user_reason_indx].was_winning &&
                           SICD.last_intention_index_winner==ArrayOfUserIntentions[user_reason_indx].reason_indx)
                   {
                       Uart.Printf("\rCALL bReasonChange RESTART");
                       ArrayOfUserIntentions[user_reason_indx].current_time=0;
                       //теперь продолжение можно выключить!
                       ArrayOfUserIntentions[user_reason_indx].was_winning=false;
                       return BUTTON_ENABLED;
                   }

                   // if we are on the tail, and it's not wining reason, restart it.
                   ArrayOfUserIntentions[user_reason_indx].TurnOff();
                   CallReasonSuccess(user_reason_indx);
                   ArrayOfUserIntentions[user_reason_indx].was_winning=false;
                   //retrn normal??
                   Uart.Printf("\rCALL bReasonChange TURNED OFF");
                   return BUTTON_NORMAL;
               }
           }
       }
    //get reason state
    //return state depends on reason state- if pseudo lamp is presented -
    return BUTTON_ERROR;
}
void CheckAndRedrawFinishedReasons()
{
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
    {
        if(ButtonsToUserReasons[i].is_this_screen_with_reasons(screens[AtlGui.current_state].name))
        {
            int scr_id=AtlGui.current_state;
       // if(AtlGui.current_state==ButtonsToUserReasons[i].scr_id)
            for(int j=0;j<9;j++)
            {
                Uart.Printf("\rCheckAndRedrawFinishedReasons CURRENT STATE%d, bttr %d",AtlGui.current_state,ButtonsToUserReasons[i].BtoR[j]);
                if(ButtonsToUserReasons[i].BtoR[j]>=0)
                    if(bReasonGetState(scr_id,j)!=BUTTON_ENABLED)
                        AtlGui.RenderSingleButton(scr_id,j,BUTTON_NORMAL);
            }
        }
    }

}


int bChangeMelodyCheck(int screen_id, int button_id)
{
    //get file number, if no, set it disabled
    //get current emo id
    if(SICD.last_played_emo<0) //ничего еще не играло, тогдаявнобудет фон - можно не париться
        return BUTTON_PRESSABLE;
    int emonum=GetFileNumerForEmoToPlay(SICD.last_played_emo);
    Uart.Printf("bChangeMelodyCheck emonum%d \r",emonum);
    if(emonum<=1)
        return BUTTON_LOCKED;
    else
        return BUTTON_PRESSABLE;
    //get this emo file numbers
}
int bChangeMelody(int screen_id, int button_id,int press_mode)
{
    Uart.Printf("bChangeMelodyCheck press_mode%d \r",press_mode);
    if(press_mode!=0)

        return BUTTON_NO_REDRAW;
    Sound.Stop();
    return bChangeMelodyCheck(screen_id,button_id);
    //тут кодзавязан на события в application - там вызывается играть ту же эмоцию, если старая кончилась
}

//lock button
int bLockCheck(int screen_id, int button_id)
{
    return BUTTON_PRESSABLE;
}
int bLockChange(int screen_id, int button_id ,int press_mode)
{
    if(press_mode==2)
    AtlGui.is_locked=!AtlGui.is_locked;

    Uart.Printf("bLockChange press_mode%d islocked%d \r",press_mode,AtlGui.is_locked);
    if(AtlGui.is_locked)
        return BUTTON_ENABLED;
    else
        return BUTTON_NORMAL;
}
int bLockGetState(int screen_id, int button_id)
{
    if(AtlGui.is_locked)
        return BUTTON_ENABLED;
    else
        return BUTTON_NORMAL;
}
