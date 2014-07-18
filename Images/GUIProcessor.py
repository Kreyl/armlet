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

C_TARGET = join(C_PATH, 'gui.c')

ENCODING = "Windows-1251"

BUTTONS = 'ABCXYZLER'

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

def cButton(node):
    name = node.attrs['alt']
    key = str(node.attrs['key']).upper()
    index = BUTTONS.index(key)
    styles = dict(split(' *: *', pair) for pair in split(' *; *', str(node.attrs['style'])))
    left = getInt(styles.get('left'))
    top = getInt(styles.get('top'))
    isPressable = node.attrs.get('isPressable')
    getState = node.attrs.get('getState')
    press = node.attrs.get('press')
    cName = cString(name)
    comment = (" /* %s */" % name.encode(ENCODING)) if len(cName) > len(name) + 2 else ''
    return (index, '/* %s */ { %s%s, %d, %d, %s, %s, %s }' % (key, cName, comment, left, top, isPressable or 'buttonIsPressable', getState or 'buttonGetState', press or 'buttonPress'))

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
