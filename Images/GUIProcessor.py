#!/usr/bin/python
#
# GUI Processor for Ticket to Atlantis LARP.
#

from os.path import dirname, join, realpath
from re import match, split
from sys import argv

try:
    from bs4 import BeautifulSoup
except ImportError, ex:
    raise ImportError("%s: %s\nPlease install BeautifulSoup v4.3.2 or later: http://www.crummy.com/software/BeautifulSoup\n" % (ex.__class__.__name__, ex))

GUI_HTML = 'GUI.html'
C_TARGET = 'gui.c'

BUTTONS = 'ABCLERXYZ'

INDENT = '    '

C_CONTENT = '''\
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
%s
};

const int screens_number = countof(screens);

// End of gui.c
'''

def getFileName(name):
    return join(dirname(realpath(argv[0])), name)

def getInt(s):
    return int(match(r'(\d+)', s).groups()[0]) if s else 0

def cButton(node):
    name = str(node.attrs['alt']).upper()
    index = BUTTONS.index(name)
    styles = dict(split(' *: *', pair) for pair in split(' *; *', str(node.attrs['style'])))
    left = getInt(styles.get('left'))
    bottom = getInt(styles.get('bottom'))
    isPressable = node.attrs.get('isPressable')
    getState = node.attrs.get('getState')
    press = node.attrs.get('press')
    return (index, '{\'%s\', %d, %d, %s, %s, %s}' % (name, left, bottom, isPressable or 'buttonIsPressable', getState or 'buttonGetState', press or 'buttonPress'))

def cScreen(node, indent = ''):
    name = str(node.attrs['id'])
    buttons = dict(cButton(button) for button in node.find_all(class_ = 'button'))
    buttonsText = indent + INDENT + (',\n' + indent + INDENT).join(buttons.get(i, 'NO_BUTTON') for i in xrange(len(BUTTONS)))
    return indent + ('{\"%s\", {\n' % name) + buttonsText + '\n' + indent +'}}'

def main():
    soup = BeautifulSoup(open(GUI_HTML))
    screens = soup.find_all(class_ = 'screen')
    with open(getFileName(C_TARGET), 'w') as f:
        f.write(C_CONTENT % (',\n'.join(cScreen(screen, INDENT) for screen in screens)))

if __name__ == '__main__':
    main()
