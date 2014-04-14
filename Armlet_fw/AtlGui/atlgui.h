/*
 * atlgui.h
 *
 *  Created on: 23 марта 2014 г.
 *      Author: USER
 */

#ifndef ATLGUI_H_
#define ATLGUI_H_
#include "kl_lib_f2xx.h"
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

/* GSI - init this
 * GUI_STATE_ABSENT - no color
 * GUI_STATE_BASIC - white
 * GUI_STATE_ENABLED - blue
 * GUI_STATE_DISABLED - grey
 * GUI_STATE_PRESSED - green
 * GUI_STATE_NOT - red
 */

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


extern gui_state gui_states[];
extern const int gui_states_number;

class AtlGui_t {
public:
    //bool UseQuartz;
  //  Thread *PThd;
    void Init();
    void ShowSplashscreen();
    void CallStateScreen(int screen_id);
    //основная очередьсобытийпоклику кнопки
    void ButtonIsClicked(int button_id);
    void RenderFullScreen(int screen_id);
    int on_run;
    int is_splash_screen_onrun;//0 - not showed, 1- on show now, 2 - show finished
    int current_state;
    bool is_gui_shown;
};

extern AtlGui_t AtlGui;
//button_state button_state_array[9];//store last button array on this screen

#endif /* ATLGUI_H_ */
