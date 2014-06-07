#include "AtlGuiCF.h"
#include "atlgui.h"
#include "cmd_uart.h"
#include "gui.h"
#include "..\MusicProcessor\intention.h"
#include "sound.h"
#define SOUND_STEP 25
#define MAX_VOL_CONST 256
#define MIN_VOL_CONST 0
#define SCREENS_WITH_REASONS 1
typedef struct t_scrbtoarr
{   int scr_id;
    int BtoR[9];
    }t_scrbtoarr;

t_scrbtoarr ButtonsToReasons[SCREENS_WITH_REASONS]=
    {1,
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,}
    };
int current_volume_lvl=220;
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
        Uart.Printf("UP vol %d ss%d mvc%d \r", current_volume_lvl,SOUND_STEP,MAX_VOL_CONST);
        return BUTTON_PRESSABLE;
    }
    else
    {
        Uart.Printf("sound up LOCKED \r", current_volume_lvl);
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
int bSoundUpChange(int screen_id, int button_id)
{
    //ничего не чекает - все проверки в соседнем вызове
    current_volume_lvl+=SOUND_STEP;
    Sound.SetVolume(current_volume_lvl);
    Uart.Printf("sound up to %d \r", current_volume_lvl);
    return 0;
}
int bSoundDownChange(int screen_id, int button_id)
{
   // buttonIsPressable(1,10);
    current_volume_lvl-=SOUND_STEP;
    Sound.SetVolume(current_volume_lvl);
    Uart.Printf("sound down to %d\r", current_volume_lvl);
    return 0;
}

int buttonIsPressable(int screen_id, int button_id)
{
    return BUTTON_PRESSABLE;
}
int buttonGetState(int screen_id, int button_id)
{
    return 0;
}
int buttonPress(int screen_id, int button_id)
{
   return 0;
}
//reason button
int bReasonCheck(int screen_id, int button_id)
{
    //если есть соответствие кнопке и резону, то можно нажимать?
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
    {
        if(screen_id==ButtonsToReasons[i].scr_id)
        {
            if(ButtonsToReasons[i].BtoR[button_id]>=0)
            {
                //TODO сюда надо вставлять проверку на неотключаемые резоны???
                return BUTTON_PRESSABLE;
            }
        }

    }
    return BUTTON_LOCKED;
}
int bReasonChange(int screen_id, int button_id)
{
    //button is available, so just get it
    for(int i=0;i<SCREENS_WITH_REASONS;i++)
       {
           if(screen_id==ButtonsToReasons[i].scr_id)
           {
               int reason_id=ButtonsToReasons[i].BtoR[button_id];
               if(reason_id<0)
                   return BUTTON_ERROR;
               if(GetPlayerReasonCurrentPower(reason_id)<0)
               {
                   //setup reason

                   //return red
               }
               else
               {
                   //turn off??

                   //retrn normal??
               }

           }
       }
    //get reason state

    //return state depends on reason state- if pseudo lamp is presented -
    return BUTTON_ERROR;
}

int bChangeMelodyCheck(int screen_id, int button_id)
{
    //get file number, if no, set it disabled
    //get current emo id
    int emonum=GetFileNumerForEmoToPlay(SICD.last_played_emo);
    if(emonum<=1)
        return BUTTON_LOCKED;
    else
        return BUTTON_PRESSABLE;
    //get this emo file numbers
}
int bChangeMelody(int screen_id, int button_id)
{
    Sound.Stop();
    return 0;
    //тут кодзавязан на события в application - там вызывается играть ту же эмоцию, если старая кончилась
}

//lock button
int bLockCheck(int screen_id, int button_id)
{
    return BUTTON_PRESSABLE;
}
int bLockChange(int screen_id, int button_id)
{
    AtlGui.is_locked=!AtlGui.is_locked;
}
int bLockState(int screen_id, int button_id)
{
    if(AtlGui.is_locked)
        return BUTTON_ENABLED;
    else
        return BUTTON_NORMAL;
}
