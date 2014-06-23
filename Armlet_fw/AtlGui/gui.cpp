 //* gui.c
#include "gui.h"
#include"stddef.h"
//160*128
Screen_t screens[] = {
        {"intentions",
            {-1,-1,-1,-1,-1,-1,-1,0,-1},
            {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
            {
               NO_BUTTON,
               {"\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee", 0, 0, 39, bReasonCheck, bReasonGetState, bReasonChange}, //kill 1
               {"\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5", 0, 0, 68, bReasonCheck, bReasonGetState, bReasonChange}, //create 2
               {"\xd7\xf2\xee-\xf2\xee", 0, 97, 20, bReasonCheck, bReasonGetState, bReasonChange}, //что то fight
               {"\xc5\xf9\xb8", 0, 97, 58, bReasonCheck, bReasonGetState, bReasonChange},   //еще sex
               {"\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5", 0, 97, 96, bReasonCheck, bReasonGetState, bReasonChange},//разруш 5
               {"lock", 0, 0, 114, bLockCheck, bLockGetState, bLockChange},  //lock
               {"\xfd\xea\xf0\xe0\xed", 0, 30, 114, buttonIsPressable, buttonGetState, buttonPress},    //switch screens
               NO_BUTTON
           }}
        ,
        {"main",
     {-1,-1,-1,-1,-1,-1,-1,1,-1},
     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
     {
        NO_BUTTON,
        NO_BUTTON,
        NO_BUTTON,
//        {"lock", 0, 4, 0, buttonIsPressable, buttonGetState, buttonPress},
//        {"\xfd\xea\xf0\xe0\xed", 0, 59, 0, buttonIsPressable, buttonGetState, buttonPress},
//        NO_BUTTON,
//        {"\xec\xf3\xe7\xfb\xea\xe0", 0, 130, 68, buttonIsPressable, buttonGetState, buttonPress},
//        {"^^", 0, 130, 33, bSoundUpCheck, buttonGetState, bSoundUpChange},
//        {"vv", 0, 130, 0, bSoundDownCheck, buttonGetState, bSoundDownChange}
        {"\xec\xf3\xe7\xfb\xea\xe0", 0, 95, 20, bChangeMelodyCheck, buttonGetState, bChangeMelody}, // new track
        {"^^", 0, 95, 58, bSoundUpCheck, buttonGetState, bSoundUpChange},   //sound up
        {"vv", 0, 95, 96, bSoundDownCheck, buttonGetState, bSoundDownChange},   //sound down
        {"lock", 0, 0, 114, bLockCheck, bLockGetState, bLockChange},    //lock
        {"\xfd\xea\xf0\xe0\xed", 0, 30, 114, buttonIsPressable, buttonGetState, buttonPress},    //switch screens
        NO_BUTTON
    }}//,

};

const int screens_number = countof(screens);

// End of gui.c
