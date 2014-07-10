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

Screen_t screens[] = {
    { "main", {
        /* A */ NO_BUTTON,
        /* B */ NO_BUTTON,
        /* C */ NO_BUTTON,
        /* X */ { "\xec\xf3\xe7\xfb\xea\xe0", 130, 68, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ { "^^", 130, 33, buttonIsPressable, buttonGetState, buttonPress },
        /* Z */ { "vv", 130, 0, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 0, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed", 59, 0, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }},
    { "intentions", {
        /* A */ NO_BUTTON,
        /* B */ { "\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee", 0, 56, buttonIsPressable, buttonGetState, buttonPress },
        /* C */ { "\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5", 0, 17, buttonIsPressable, buttonGetState, buttonPress },
        /* X */ { "\xd7\xf2\xee-\xf2\xee", 109, 74, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ { "\xc5\xf9\xb8", 125, 35, buttonIsPressable, buttonGetState, buttonPress },
        /* Z */ { "\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5", 103, 1, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 0, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed", 59, 0, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }}
};

const int screens_number = countof(screens);

// End of gui.c
