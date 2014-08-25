/*
 * gui.h
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-08-25 19:11:10
 */
#ifndef GUI_H
#define GUI_H

#ifndef nullptr
#define nullptr 0
#endif

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define BUTTON_ABSENT      0    // Button is not present on this screen
#define BUTTON_NORMAL      1    // Button is present and available (white)
#define BUTTON_ENABLED     2    // Button is present activated (red)
#define BUTTON_DISABLED    3    // Button is present on this screen but can't be used (gray)
#define BUTTON_PRESSED     4    // Button is pressed right now (yellow)
#define BUTTON_NOT_PRESSED 5    // Button is pressed but can't be used right now (gray)
#define BUTTON_ERROR       6    // Bad state for a button
#define BUTTON_NO_REDRAW   7

#define BUTTON_PRESSABLE 1
#define BUTTON_LOCKED    3

#define BUTTONS "ABCXYZLER"
#define NUM_BUTTONS (sizeof(BUTTONS) - 1)

#define RNAME_FIGHT	"\xc4\xf0\xe0\xea\xe0"
#define RNAME_SEX	"\xd1\xe5\xea\xf1"
#define RNAME_MURDER	"\xd3\xe1\xe8\xe9\xf1\xf2\xe2\xee"
#define RNAME_DESTR	"\xd0\xe0\xe7\xf0\xf3\xf8\xe5\xed\xe8\xe5"
#define RNAME_CREATION	"\xd1\xee\xe7\xe8\xe4\xe0\xed\xe8\xe5"
#define RNAME_DEATH	"\xd1\xec\xe5\xf0\xf2\xfc"

#define NO_BUTTON {nullptr, 0, 0, nullptr, nullptr, nullptr}

#define LOCK_X 62
#define LOCK_Y 44

typedef struct Button {
    const char* name; // Text to be displayed for a button if text interface is used.
    const int left;
    const int bottom;
    int (*isPressable)(int num_screen, int num_button);      // Called if pressed, must return non-zero if pressable, zero otherwise.
    int (*getState)(int num_screen, int num_button);         // Called at redrawing, must return button state.
    int (*press)(int num_screen, int num_button, int event); // Called after pressed, must process pressing and return new button state.
} Button_t;

typedef struct Screen {
    const char* name;                                            // Name of the subfolder containing respective imagery.
    const char* screen_switch[NUM_BUTTONS];                      // Screens to go to after the respective buttons are pressed.
    int (*fptr_gui_state_array_switch_functions[NUM_BUTTONS])(); // Functions to change screen contents after the respective buttons are pressed.
    Button_t buttons[NUM_BUTTONS];
} Screen_t;

extern const char* buttons;

extern Screen_t screens[];
extern const int screens_number;

extern int buttonIsPressable(int num_screen, int num_button);
extern int buttonGetState(int num_screen, int num_button);
extern int buttonPress(int num_screen, int num_button, int event);

#endif

// End of gui.h
