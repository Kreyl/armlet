/*
 * atlgui.h
 *
 *  Created on: 23 марта 2014 г.
 *      Author: USER
 */

#ifndef ATLGUI_H_
#define ATLGUI_H_
#include "kl_lib_f2xx.h"
#include "AtlGuiCF.h"
#include "keys.h"
/*
#define KEY_A   Keys.Status[0]
#define KEY_B   Keys.Status[1]
#define KEY_C   Keys.Status[2]
#define KEY_X   Keys.Status[3]
#define KEY_Y   Keys.Status[4]
#define KEY_Z   Keys.Status[5]
#define KEY_L   Keys.Status[6]
#define KEY_E   Keys.Status[7]
#define KEY_R   Keys.Status[8]

*/
#define GSI 0
#define GUI_STATE_ABSENT 1
#define GUI_STATE_BASIC 2
#define GUI_STATE_ENABLED 3
#define GUI_STATE_DISABLED 4
#define GUI_STATE_PRESSED 5
#define GUI_STATE_NOT 6

#define GUI_BUTTON_NUMBER 9
#define SUSPEND_SCREEN_SEC 30

#define MAX_CHARNAME_LCD_SIZE 40

/* GSI - init this
 * GUI_STATE_ABSENT - no color
 * GUI_STATE_BASIC - white
 * GUI_STATE_ENABLED - blue
 * GUI_STATE_DISABLED - grey
 * GUI_STATE_PRESSED - green
 * GUI_STATE_NOT - red
 */

/*
 typedef struct button_state {
 int state_id;
 int x_pos;
 int y_pos;

 //ishere data checked??
 int * fptr_on_press();
 //call it if fptr_on_press result is green or red
 //is it change data???
 int * fptr_change_state_id();
} button_state;

typedef struct gui_state{
    int gui_state_array_switch[GUI_BUTTON_NUMBER]; //list of new states on sucessifull clicks on buttons
    int (* fptr_gui_state_array_switch_functions[GUI_BUTTON_NUMBER])(); // apply datachanges from gui on screenchanges
   struct button_state * sptr_button_state[GUI_BUTTON_NUMBER];// list of button states
}gui_state;
*/


//extern gui_state gui_states[];
//extern const int gui_states_number;

class AtlGui_t {
public:
    //bool UseQuartz;
  //  Thread *PThd;
    void Init();
    void ShowSplashscreen();
    void CallStateScreen(int screen_id);
    //основная очередь событий по клику кнопки
    bool ButtonIsClicked(int button_id);
    void ButtonIsReleased(int button_id ,KeyEvt_t Type);
    void RenderFullScreen(int screen_id);

    bool is_lock_redraw_active;
    void DrawBigLockMark();
    void DrawSondLvlMark();
    void RenderSingleButton(int screen_id,int button_id,int button_state);
    void RenderSingleButtonStateCheck(int screen_id,int button_id);

    void RenderNameTimeBat();
    void GetCharname();//getname by id
    char char_name[MAX_CHARNAME_LCD_SIZE+1];
    char timechar[5];
    int time1, time2;


    int on_run;
    int is_splash_screen_onrun;//0 - not showed, 1- on show now, 2 - show finished
    int current_state;
    bool is_gui_shown;
    bool is_locked;
    bool is_lock_now; //для разршения бесконечного цикла в отрисовке
    bool is_screen_suspended;
    bool is_suspend_timer_run;
    int screen_suspend_timer;
    void TurnOnScreen();
    void TurnOffScreen();
    void AddSuspendScreenTimer(int sec_to_add);
    //TODO fill reactpress func
    void ReactLockedPress();

    int GetScreenIndxFromName(char* name);


    bool ReasonFlag;
};

extern AtlGui_t AtlGui;
//button_state button_state_array[9];//store last button array on this screen

#endif /* ATLGUI_H_ */
