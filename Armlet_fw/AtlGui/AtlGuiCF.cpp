#include "AtlGuiCF.h"
#include "cmd_uart.h"
#include "gui.h"
#include "..\MusicProcessor\intention.h"
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

  //  int current_sound
    return BUTTON_PRESSABLE;
}
int bSoundDownCheck(int screen_id, int button_id)
{
    return BUTTON_PRESSABLE;
}
int bSoundUpChange(int screen_id, int button_id)
{
//    int max_vol= 250;
//    int min_vol=0;
    return 0;
}
int bSoundDownChange(int screen_id, int button_id)
{
   // buttonIsPressable(1,10);
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
//lock button
int bLockCheck(int screen_id, int button_id)
{
    return 0;
}
int bLockChange(int screen_id, int button_id)
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
