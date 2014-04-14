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
#include "lcd2630.h"
#include "..\Images\gui.h"
AtlGui_t AtlGui;
//gui_state gui_states[]={
//        {
//                {-1,-1,-1,-1,-1,-1,-1,1,-1},
//                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
//        },
//        {
//                {-1,-1,-1,-1,-1,-1,-1,0,-1},
//                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//                {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
//        }
//};




//const int gui_states_number = countof(gui_states);


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
    Lcd.Printf(11, 11, clGreen, clBlack, "SPLASH SCREEN");
}
void AtlGui_t::CallStateScreen(int screen_id)
{
    Lcd.Printf(11, 21, clGreen, clBlack, "STATESCREEN %u", screen_id);
   if(current_state==screen_id)
   {
       //если вернулись на тотже экран, не перерисовываем его
       Lcd.Printf(11, 31, clGreen, clBlack, "C stscr %u same", screen_id);
       return;
   }

   current_state=screen_id;
   RenderFullScreen(current_state);
   Lcd.Printf(11, 31, clGreen, clBlack, "C stscr %u new", screen_id);
   //TODO render??
}
void AtlGui_t::RenderFullScreen(int screen_id)
{
    //get filename
    // render it

}
void AtlGui_t::ButtonIsReleased(int button_id)
{
    if(current_state>=0 && current_state<screens_number)
    {
        if( screens[current_state].buttons[button_id].isPressable!= 0)
        {
            int button_state_val=screens[current_state].buttons[button_id].isPressable(current_state,button_id);//sptr_button_state[button_id]->fptr_on_press());
            Uart.Printf("button_state_val %d\r",button_state_val);
            if(button_state_val==BUTTON_PRESSABLE)
            {


        //вызываем смену крана,если есть
                Uart.Printf("CallStateScreen %d \r",screens[current_state].screen_switch[button_id]);
                  if( screens[current_state].screen_switch[button_id]>=0  )
                  {
                      Uart.Printf("!!!!CallStateScreen %d \r",screens[current_state].screen_switch[button_id]);
                      // вызываем новое состояние экрана, отрисовываем
                      CallStateScreen(screens[current_state].screen_switch[button_id]);
                      //вызываем фуркцию логики смены крана
                      if(screens[current_state].fptr_gui_state_array_switch_functions[button_id]!= NULL)
                          screens[current_state].fptr_gui_state_array_switch_functions[button_id]();

                  }
            }
        }
        else return;// нельзя вызвать смену крана кнопкой, которой нет! хотя логика позволяет
     }

}
void AtlGui_t::ButtonIsClicked(int button_id)
{
    //вызываем геттер кнопки
    Uart.Printf("current_state %d screens_number %d button id %d \r",current_state,screens_number,button_id);
    if(current_state>=0 && current_state<screens_number)
    {
        if( screens[current_state].buttons[button_id].isPressable!= 0)
        {
           int button_state_val=screens[current_state].buttons[button_id].isPressable(current_state,button_id);//sptr_button_state[button_id]->fptr_on_press());
           Uart.Printf("button_state_val %d\r",button_state_val);
           if(button_state_val==BUTTON_PRESSABLE)
           {
               Uart.Printf("button%d on screen %d is pressable %d\r",button_id,current_state );
               //on red andgreenchange state
               //если геттер успешен,и можем что-то сделать,вызываем сеттер кнопки
               if( screens[current_state].buttons[button_id].press!= 0)
               {
                  int new_b_state= screens[current_state].buttons[button_id].press(current_state,button_id);
                  RenderSingleButton(current_state,button_id,new_b_state);
               }
               else
               {
                   Uart.Printf("ButtonIsClicked render func absent\r");
               }
           }
        }
        else return;
      // else return; // если нет действия на кнопке - ничего не делаем

    }
    else return; // если мы вне автомата состояний - ошибка!


    // если есть изменение экрана, меняем экран??

    //отрисовываем изменения??

//
//    //вызываем геттер кнопки
//    if(current_state>=0 && current_state<screens_number)
//    {
//       if( gui_states[current_state].sptr_button_state[button_id]->fptr_on_press()!=NULL)
//       {
//           int button_state_val=*(gui_states[current_state].sptr_button_state[button_id]->fptr_on_press());
//           if(button_state_val>0)
//           {
//               //on red andgreenchange state
//               //если геттер успешен,и можем что-то сделать,вызываем сеттер кнопки
//               gui_states[current_state].sptr_button_state[button_id]->fptr_change_state_id();
//
//           }
//       }
//       //вызываем смену крана,если есть
//       if( gui_states[current_state].gui_state_array_switch[button_id]>=0)
//       {
//           if(gui_states[current_state].fptr_gui_state_array_switch_functions[button_id]!= NULL)
//               gui_states[current_state].fptr_gui_state_array_switch_functions[button_id]();
//       }
//
//
//
//      // else return; // если нет действия на кнопке - ничего не делаем
//
//    }
//    else return; // если мы вне автомата состояний - ошибка!
//
//
//    // если есть изменение экрана, меняем экран??
//
//    //отрисовываем изменения??
}
void AtlGui_t::RenderSingleButton(int screen_id,int button_id,int button_state)
{
    Lcd.Printf(11, 41, clGreen, clBlack, "RB sc %d b %d st %d", screen_id,button_id,button_state);
}
