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
        {'L', 0, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 28, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 98, 70, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 98, 32, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 98, 0, buttonIsPressable, buttonGetState, buttonPress}
    }},
    {"intentions", {
        NO_BUTTON,
        {'B', 0, 52, buttonIsPressable, buttonGetState, buttonPress},
        {'C', 0, 14, buttonIsPressable, buttonGetState, buttonPress},
        {'L', 0, 0, buttonIsPressable, buttonGetState, buttonPress},
        {'E', 28, 0, buttonIsPressable, buttonGetState, buttonPress},
        NO_BUTTON,
        {'X', 98, 70, buttonIsPressable, buttonGetState, buttonPress},
        {'Y', 98, 32, buttonIsPressable, buttonGetState, buttonPress},
        {'Z', 98, 0, buttonIsPressable, buttonGetState, buttonPress}
    }}
};

const int screens_number = countof(screens);

// End of gui.c
