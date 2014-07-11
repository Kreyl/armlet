/*
 * gui.cpp
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 */
#include "gui.h"
#include"stddef.h"
//160*128

const char* buttons = BUTTONS;

Screen_t screens[] = {
        {"main",
     {-1,-1,-1,-1,-1,-1,-1,1,-1},
     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
     {
        NO_BUTTON,
        NO_BUTTON,
        NO_BUTTON,
        {"\xec\xf3\xe7\xfb\xea\xe0", 95, 20, bChangeMelodyCheck, buttonGetState, bChangeMelody}, // new track
        {"^^", 95, 58, bSoundUpCheck, buttonGetState, bSoundUpChange},   //sound up
        {"vv", 95, 96, bSoundDownCheck, buttonGetState, bSoundDownChange},   //sound down
        {"lock", 0, 114, bLockCheck, bLockGetState, bLockChange},    //lock
        {"\xfd\xea\xf0\xe0\xed", 30, 114, buttonIsPressable, buttonGetState, buttonPress},    //switch screens
        NO_BUTTON
    }},
        {"intentions",
            {-1,-1,-1,-1,-1,-1,-1,0,-1},
            {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
            {
               NO_BUTTON,
               {"\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee", 0, 39, bReasonCheck, bReasonGetState, bReasonChange}, //kill 1
               {"\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5", 0, 68, bReasonCheck, bReasonGetState, bReasonChange}, //create 2
               {"\xd7\xf2\xee-\xf2\xee", 97, 20, bReasonCheck, bReasonGetState, bReasonChange}, //что то fight
               {"\xc5\xf9\xb8", 97, 58, bReasonCheck, bReasonGetState, bReasonChange},   //еще sex
               {"\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5", 97, 96, bReasonCheck, bReasonGetState, bReasonChange},//разруш 5
               {"lock", 0, 114, bLockCheck, bLockGetState, bLockChange},  //lock
               {"\xfd\xea\xf0\xe0\xed", 30, 114, buttonIsPressable, buttonGetState, buttonPress},    //switch screens
               NO_BUTTON
           }}
        //,


};

const int screens_number = countof(screens);

// End of gui.c
