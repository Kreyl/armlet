#!/usr/bin/python
#
# GUI Processor for Ticket to Atlantis LARP.
#
from datetime import datetime
from os.path import dirname, join, realpath
from platform import system
from re import match, split
from subprocess import Popen, PIPE, STDOUT
from sys import argv

try:
    from bs4 import BeautifulSoup
except ImportError, ex:
    raise ImportError("%s: %s\nPlease install BeautifulSoup v4.3.2 or later: http://www.crummy.com/software/BeautifulSoup\n" % (ex.__class__.__name__, ex))

isWindows = system().lower().startswith('win')

GUI_HTML = 'GUI.html'

C_PATH = '../Armlet_fw/AtlGui'

H_TARGET = join(C_PATH, 'gui.h')

C_TARGET = join(C_PATH, 'gui.c')

ENCODING = "Windows-1251"

BUTTONS = 'ABCXYZLER'

INDENT = '    '

H_CONTENT = '''\
/*
 * gui.h
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at %s
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

%s

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
'''

C_CONTENT = '''\
/*
 * gui.c
 *
 * Part of "Ticket to Atlantis" ArmLet firmware.
 *
 * Generated automatically by GUIProcessor.py from GUI.html
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at %s
 */

#include "gui.h"

const char* buttons = BUTTONS;

Screen_t screens[] = {
%s
};

const int screens_number = countof(screens);

// End of gui.c
'''

TEST_COMMAND = 'gcc -I "%s" -o test "%s" test.c && ./test && rm test' % (C_PATH, C_TARGET)

def currentTime():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S')

def getFileName(fileName):
    return join(dirname(realpath(argv[0])), fileName)

def getInt(s):
    return int(match(r'(\d+)', s).groups()[0]) if s else 0

def cChar(c):
    if c == r'\\':
        return '\\\\'
    if ord(c) < 128:
        return c
    return '\\x%2x' % ord(c)

def cString(s):
    return '"%s"' % ''.join(cChar(c) for c in s.encode(ENCODING))

def tagString(s):
    return 'RNAME_%s' % str(s).upper()

def hButton(tag, name):
    return "#define %s\t%s" % (tagString(tag), cString(name))

def cButton(node):
    name = node.attrs['alt']
    tag = node.attrs.get('tag')
    key = str(node.attrs['key']).upper()
    index = BUTTONS.index(key)
    styles = dict(split(' *: *', pair) for pair in split(' *; *', str(node.attrs['style'])))
    left = getInt(styles.get('left'))
    top = getInt(styles.get('top'))
    isPressable = node.attrs.get('isPressable')
    getState = node.attrs.get('getState')
    press = node.attrs.get('press')
    tagName = tagString(tag) if tag else cString(name)
    comment = (" /* %s */" % name.encode(ENCODING)) if tag or len(tagName) > len(name) + 2 else ''
    return (index, '/* %s */ { %s%s, %d, %d, %s, %s, %s }' % (key, tagName, comment, left, top, isPressable or 'buttonIsPressable', getState or 'buttonGetState', press or 'buttonPress'))

def cScreen(node, indent = ''):
    name = str(node.attrs['id'])
    buttons = dict(cButton(button) for button in node.find_all(class_ = 'button'))
    buttonsText = indent + INDENT + (',\n' + indent + INDENT).join(buttons.get(i, '/* %s */ NO_BUTTON' % BUTTONS[i]) for i in xrange(len(BUTTONS)))
    nulls = '%s{ %s },\n' % (indent + INDENT, ', '.join(('nullptr',) * len(BUTTONS)))
    return indent + ('{ \"%s\",\n' % name) + nulls + nulls + indent + INDENT + '{\n' + buttonsText + '\n' + indent +'}}'

def main():
    print "Processing %s..." % GUI_HTML
    soup = BeautifulSoup(open(GUI_HTML))
    screens = soup.find_all(class_ = 'screen')
    with open(getFileName(H_TARGET), 'wb') as f:
        f.write(H_CONTENT % (currentTime(), '\n'.join(hButton(node.attrs['tag'], node.attrs['alt']) for node in soup.find_all(tag = True))))
    with open(getFileName(C_TARGET), 'wb') as f:
        f.write(C_CONTENT % (currentTime(), ',\n'.join(cScreen(screen, INDENT) for screen in screens)))
    if isWindows:
        print "Not running test on Windows\nDone"
    else:
        print "Running test: %s" % TEST_COMMAND
        subprocess = Popen(TEST_COMMAND, shell = True, stdout = PIPE, stderr = STDOUT)
        output = subprocess.communicate()[0]
        print "Done (%s): %s" % (subprocess.returncode, output),

if __name__ == '__main__':
    main()
