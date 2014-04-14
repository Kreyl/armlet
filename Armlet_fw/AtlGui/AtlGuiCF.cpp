#include "AtlGuiCF.h"
#include "cmd_uart.h"
#define SOUND_STEP 25
void CallMainToReason()
{
    Uart.Printf("MainToReason\r");
}
void CallReasonToMain()
{
    Uart.Printf("ReasonToMain\r");
}

void bSoundUpCheck(int screen_id, int button_id)
{
    int max_vol= 250;
  //  int current_sound

}
void bSoundDownCheck(int screen_id, int button_id)
{

}
void bSoundUpChange(int screen_id, int button_id)
{
    int max_vol= 250;
    int min_vol=0;

}
void bSoundDownChange(int screen_id, int button_id)
{

}
