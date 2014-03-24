/*
 * atlgui.cpp
 *
 *  Created on: 23 марта 2014 г.
 *      Author: USER
 */
#include "atlgui.h"
#include"emotions.h"
#include"stddef.h"
#include "AtlGuiCF.h"
AtlGui_t AtlGui;
gui_state gui_states[]={
        {
                {-1,-1,-1,-1,-1,-1,-1,1,-1},
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
        },
        {
                {-1,-1,-1,-1,-1,-1,-1,0,-1},
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
        }
};




const int gui_states_number = countof(gui_states);


void AtlGui_t::Init()
{
    is_splash_screen_onrun=0;
    on_run=0;
    current_state=-1;
    is_gui_shown=false;
}
void AtlGui_t::ShowSplashscreen()
{

    is_splash_screen_onrun=1;

}
void AtlGui_t::CallStateScreen(int screen_id)
{

   if(current_state==screen_id)
       return;

   current_state=screen_id;

   //TODO render??
}
void AtlGui_t::RenderFullScreen(int screen_id)
{
    //get filename
    // render it

}

void AtlGui_t::ButtonIsClicked(int button_id)
{
    //вызываем геттер кнопки
    if(current_state>=0 && current_state<gui_states_number)
    {
       if( gui_states[current_state].sptr_button_state[button_id]->fptr_on_press()!=NULL)
       {
           int button_state_val=*(gui_states[current_state].sptr_button_state[button_id]->fptr_on_press());
           if(button_state_val>0)
           {
               //on red andgreenchange state
               //если геттер успешен,и можем что-то сделать,вызываем сеттер кнопки
               gui_states[current_state].sptr_button_state[button_id]->fptr_change_state_id();

           }
       }
       //вызываем смену крана,если есть
       if( gui_states[current_state].gui_state_array_switch[button_id]>=0)
       {
           if(gui_states[current_state].fptr_gui_state_array_switch_functions[button_id]!= NULL)
               gui_states[current_state].fptr_gui_state_array_switch_functions[button_id]();
       }



      // else return; // если нет действия на кнопке - ничего не делаем

    }
    else return; // если мы вне автомата состояний - ошибка!


    // если есть изменение экрана, меняем экран??

    //отрисовываем изменения??
}
