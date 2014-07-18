/*
 * gui.c
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-07-18 20:56:26
 */

#include "gui.h"

const char* buttons = BUTTONS;

Screen_t screens[] = {
    { "main",
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        {
        /* A */ NO_BUTTON,
        /* B */ { "\xc4\xf0\xe0\xea\xe0" /* Драка */, 0, 45, buttonIsPressable, buttonGetState, buttonPress },
        /* C */ { "\xd1\xe5\xea\xf1" /* Секс */, 0, 82, buttonIsPressable, buttonGetState, buttonPress },
        /* X */ { "M", 130, 19, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ { "^^", 130, 60, buttonIsPressable, buttonGetState, buttonPress },
        /* Z */ { "vv", 130, 95, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 110, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed" /* экран */, 59, 111, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }},
    { "intentions",
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        {
        /* A */ NO_BUTTON,
        /* B */ { "\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee" /* Убийство */, 0, 42, buttonIsPressable, buttonGetState, buttonPress },
        /* C */ { "\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5" /* Разрушение */, 0, 82, buttonIsPressable, buttonGetState, buttonPress },
        /* X */ { "\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5" /* Созидание */, 82, 26, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ NO_BUTTON,
        /* Z */ { "\xd1\xec\xe5\xf0\xf2\xfc" /* Смерть */, 105, 99, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 110, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed" /* экран */, 59, 111, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }}
};

const int screens_number = countof(screens);

// End of gui.c
