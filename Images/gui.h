/*
 * gui.h
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Must be compatible with gui.c that is generated automatically by GUIProcessor.py
 */
#ifndef GUI_H
#define GUI_H

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define BUTTON_ABSENT 0
#define BUTTON_NORMAL 1
#define BUTTON_ENABLED 2
#define BUTTON_DISABLED 3

#define BUTTONS "ABCLERXYZ"

#define NO_BUTTON {0, 0, 0, 0, 0, 0}

typedef struct Button {
    char name;
    const int left;
    const int bottom;
    // All function arguments are screen number and button number, may be ignored.
    int (*isPressable)(int, int); // Called if pressed, must return non-zero if pressable, zero otherwise.
    int (*getState)(int, int); // Called at redrawing, must return button state.
    int (*press)(int, int); // Called after pressed, must process pressing and return new button state.
} Button_t;

typedef struct Screen {
    const char* name;
    const Button_t buttons[sizeof(BUTTONS)];
} Screen_t;

extern Screen_t screens[];
extern const int screens_number;

extern int buttonIsPressable(int, int);
extern int buttonGetState(int, int);
extern int buttonPress(int, int);

#endif

// End of gui.h
