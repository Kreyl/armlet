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
    {"main", {
        NO_BUTTON,
        NO_BUTTON,
        NO_BUTTON,
        {'L', 4, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 59, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 130, 68, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 130, 33, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 130, 0, buttonIsPressable, buttonGetState, buttonPress}
    }},
    {"intentions", {
        NO_BUTTON,
        {'B', 0, 56, buttonIsPressable, buttonGetState, buttonPress},
        {'C', 0, 17, buttonIsPressable, buttonGetState, buttonPress},
        {'L', 4, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 59, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 109, 74, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 125, 35, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 103, 1, buttonIsPressable, buttonGetState, buttonPress}
    }}
};

const int screens_number = countof(screens);

// End of gui.c
