 //* gui.c
#include "gui.h"
#include"stddef.h"

Screen_t screens[] = {
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
//        {"^^", 0, 130, 33, buttonIsPressable, bSoundUpCheck, bSoundUpChange},
//        {"vv", 0, 130, 0, buttonIsPressable, bSoundDownCheck, bSoundDownChange}
        {"lock", 0, 0, 114, buttonIsPressable, buttonGetState, buttonPress},
        {"\xfd\xea\xf0\xe0\xed", 0, 0, 114, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {"\xec\xf3\xe7\xfb\xea\xe0", 0, 95, 20, buttonIsPressable, buttonGetState, buttonPress},
        {"^^", 0, 95, 58, buttonIsPressable, bSoundUpCheck, bSoundUpChange},
        {"vv", 0, 95, 96, buttonIsPressable, bSoundDownCheck, bSoundDownChange}
    }},
    {"intentions",
     {-1,-1,-1,-1,-1,-1,-1,0,-1},
     {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
     {
        NO_BUTTON,
        {"\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee", 0, 0, 56, buttonIsPressable, buttonGetState, buttonPress},
        {"\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5", 0, 0, 17, buttonIsPressable, buttonGetState, bSoundUpCheck},
        {"lock", 0, 4, 0, buttonIsPressable, buttonGetState, buttonPress},
        {"\xfd\xea\xf0\xe0\xed", 0, 59, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {"\xd7\xf2\xee-\xf2\xee", 0, 109, 74, buttonIsPressable, buttonGetState, buttonPress},
        {"\xc5\xf9\xb8", 0, 125, 35, buttonIsPressable, buttonGetState, buttonPress},
        {"\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5", 0, 103, 1, buttonIsPressable, buttonGetState, buttonPress}
    }}
};

const int screens_number = countof(screens);

// End of gui.c
