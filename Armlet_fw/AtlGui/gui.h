/*
 * gui.h
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Must be compatible with gui.c that is generated automatically by GUIProcessor.py
 */
#ifndef GUI_H
#define GUI_H

#ifndef nullptr
#define nullptr 0
#endif

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define BUTTON_ABSENT      0
#define BUTTON_NORMAL      1  // белый
#define BUTTON_ENABLED     2  // красный
#define BUTTON_DISABLED    3  // коричнево-желтый
#define BUTTON_PRESSED     4  // желтый
#define BUTTON_NOT_PRESSED 5  // коричнево-желтый
#define BUTTON_ERROR       6

#define BUTTON_PRESSABLE 1
#define BUTTON_LOCKED    3

#define BUTTONS "ABCXYZLER"
#define NUM_BUTTONS sizeof(BUTTONS)

#define NO_BUTTON {nullptr, 0, 0, nullptr, nullptr, nullptr}

typedef struct Button {
    const char* name; // Text to be displayed for a button if text interface is used.
    const int left;
    const int bottom;
    int (*isPressable)(int nScreen, int nButton);      // Called if pressed, must return non-zero if pressable, zero otherwise.
    int (*getState)(int nScreen, int nButton);         // Called at redrawing, must return button state.
    int (*press)(int nScreen, int nButton, int event); // Called after pressed, must process pressing and return new button state.
} Button_t;

typedef struct Screen {
    const char* name; // Name of the subfolder containing respective imagery.
    const char* screen_switch[NUM_BUTTONS]; // New states on successful clicks on buttons
    int (* fptr_gui_state_array_switch_functions[NUM_BUTTONS])(); // apply datachanges from gui on screenchanges
    const Button_t buttons[NUM_BUTTONS];
} Screen_t;

extern const char* buttons;
extern Screen_t screens[];
extern const int screens_number;

extern int buttonIsPressable(int nScreen, int nButton);
extern int buttonGetState(int nScreen, int nButton);
extern int buttonPress(int nScreen, int nButton, int event);

#endif

// End of gui.h
