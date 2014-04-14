/*
 * gui.c
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 */
#include "gui.h"
#include"stddef.h"
//#include "kl_lib_f2xx.h"
Screen_t screens[] = {
    {"main",
     {-1,-1,-1,-1,-1,-1,-1,1,-1},
     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
     {
        NO_BUTTON,
        NO_BUTTON,
        NO_BUTTON,
        {'L', 0, 0, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 0, 28, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 0, 98, 70, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 0, 98, 32, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 0, 98, 0, buttonIsPressable, buttonGetState, buttonPress}
    }},
    {"intentions",
     {-1,-1,-1,-1,-1,-1,-1,0,-1},
     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
     {
        NO_BUTTON,
        {'B', 0, 0, 52, buttonIsPressable, buttonGetState, buttonPress},
        {'C', 0, 0, 14, buttonIsPressable, buttonGetState, buttonPress},
        {'L', 0, 0, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 0, 28, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 0, 98, 70, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 0, 98, 32, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 0, 98, 0, buttonIsPressable, buttonGetState, buttonPress}
    }}
};

const int screens_number = countof(screens);
int buttonIsPressable(int screen_id, int button_id)
{
    //Uart.Printf("screen_id %d button_id %d\r",screen_id,button_id);
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
// End of gui.c
