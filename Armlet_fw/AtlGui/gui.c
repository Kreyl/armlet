/*
 * gui.c
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-07-22 03:04:23
 */

#include "gui.h"

const char* buttons = BUTTONS;

Screen_t screens[] = {
    { "main",
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        {
        /* A */ NO_BUTTON,
        /* B */ { RNAME_FIGHT /* Драка */, 0, 45, buttonIsPressable, buttonGetState, buttonPress },
        /* C */ { RNAME_SEX /* Секс */, 0, 82, buttonIsPressable, buttonGetState, buttonPress },
        /* X */ { "M", 130, 19, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ { "^^", 127, 60, buttonIsPressable, buttonGetState, buttonPress },
        /* Z */ { "vv", 127, 95, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 110, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed" /* экран */, 59, 111, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }},
    { "intentions",
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
        {
        /* A */ NO_BUTTON,
        /* B */ { RNAME_MURDER /* Убийство */, 0, 42, buttonIsPressable, buttonGetState, buttonPress },
        /* C */ { RNAME_DESTR /* Разрушение */, 0, 82, buttonIsPressable, buttonGetState, buttonPress },
        /* X */ { RNAME_CREATION /* Созидание */, 82, 26, buttonIsPressable, buttonGetState, buttonPress },
        /* Y */ NO_BUTTON,
        /* Z */ { RNAME_DEATH /* Смерть */, 105, 99, buttonIsPressable, buttonGetState, buttonPress },
        /* L */ { "lock", 4, 110, buttonIsPressable, buttonGetState, buttonPress },
        /* E */ { "\xfd\xea\xf0\xe0\xed" /* экран */, 59, 111, buttonIsPressable, buttonGetState, buttonPress },
        /* R */ NO_BUTTON
    }}
};

const int screens_number = countof(screens);

// End of gui.c
