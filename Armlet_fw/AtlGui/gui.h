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

#define BUTTON_ABSENT      0    // Button is not present on this screen
#define BUTTON_NORMAL      1    // Button is present and available (white)
#define BUTTON_ENABLED     2    // Button is present activated (red)
#define BUTTON_DISABLED    3    // Button is present on this screen but can't be used (gray)
#define BUTTON_PRESSED     4    // Button is pressed right now (yellow)
#define BUTTON_NOT_PRESSED 5    // Button is pressed but can't be used right now (gray)
#define BUTTON_ERROR       6    // Bad state for a button

#define BUTTON_PRESSABLE 1
#define BUTTON_LOCKED    3

#define BUTTONS "ABCXYZLER"
#define NUM_BUTTONS (sizeof(BUTTONS) - 1)

#define NO_BUTTON {nullptr, 0, 0, nullptr, nullptr, nullptr}

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
    const Button_t buttons[NUM_BUTTONS];
} Screen_t;

extern const char* buttons_names;

extern Screen_t screens[];
extern const int screens_number;

extern int buttonIsPressable(int num_screen, int num_button);
extern int buttonGetState(int num_screen, int num_button);
extern int buttonPress(int num_screen, int num_button, int event);

#endif

// End of gui.h
