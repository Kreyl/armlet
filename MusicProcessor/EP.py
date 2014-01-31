#!/usr/bin/python
#
# Emotions Processor for Ticket to Atlantis LARP.
#
# Usage:
# - Export emotions mind map to FreeMind format (Emotions.mm).
# - Open Emotions.mm in FreeMind.
# - Click File - Export - Using XSLT...
# - In Choose XSL File, locate mm2csv.xsl template in FreeMind/accessories directory.
# - In Choose ExportFile, specify Emotions.csv located in the same directory as this script.
# - Perform Export.
# - Run this script: python EP.py
# - The script would process Emotions.csv and produce emotions.c and emotions.h
# - Compile and link those two files with whatever code that needs emotions data.
#
from csv import reader as CSVReader
from os.path import dirname, join, realpath
from sys import argv

try:
    from pytils.translit import translify
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pytils v0.2.3 or later: https://pypi.python.org/pypi/pytils\n" % (ex.__class__.__name__, ex))

EMOTIONS_CSV = 'Emotions.csv'
REASONS_CSV = 'Reasons.csv'
C_TARGET = 'emotions.c'

C_CONTENT = '''\
/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EP.py from Emotions.csv and Reasons.csv
 *
 * !!! DO NOT EDIT !!!
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
%s
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
%s
};

const int reasons_number = countof(reasons);

// End of emotions.c
'''

C_NODE = ' /* %s */ %s{ %s, 1, %s, 0 },'

REASON_NODE = ' /* %s */ { %s, %d, %d }, \t/* %s */'

INDENT = '    '

NO_PARENT = 'ROOT'

def width(array):
    return len(str((len(array) or 1) - 1))

def convert(s):
    return translify(s.decode('utf-8'))

def convertTitle(s):
    return convert(s.strip())

def convertEmotion(s):
    return convertTitle(s).lower()

def getFileName(name):
    return join(dirname(realpath(argv[0])), name)

def readCSV(csv):
    with open(csv) as f:
        for row in CSVReader(f):
            assert row, "bad CSV file format"
            if not row[0].startswith('#'):
                yield row

def processEmotions():
    parents = []
    emotionsIndexes = {}
    emotionsTree = []
    for (eid, row) in enumerate(readCSV(getFileName(EMOTIONS_CSV))):
        emotion = convertEmotion(row[-1])
        level = len(row) - 1
        if level > len(parents):
            assert level == len(parents) + 1
            parents.append(eid - 1)
        parents = parents[:level]
        emotionsIndexes[emotion] = eid
        emotionsTree.append((eid, level, emotion, parents[-1] if parents else None))
    assert len(emotionsIndexes) == len(emotionsTree), "Duplicate emotions detected"
    return (emotionsIndexes, tuple(emotionsTree))

def processReasons(emotions):
    reasons = []
    for (rid, row) in enumerate(readCSV(getFileName(REASONS_CSV))):
        assert len(row) == 3, "bad reasons file format"
        (reason, weight, emotion) = row
        eid = emotions.get(emotion)
        assert eid != None, "unknown emotion: %s" % emotion
        weight = int(weight)
        reasons.append((rid, reason, weight, eid, emotion))
    return tuple(reasons)

def cChar(c):
    if c == r'\\':
        return '\\\\'
    e = c.encode('unicode_escape')
    if e == c:
        return c
    return '\\x' + e[3:].zfill(4)

def cString(s):
    return '"%s"' % ''.join(cChar(c) for c in s.decode('utf-8'))

def cEmotion(eidWidth, eid, level, emotion, parent):
    return C_NODE % (str(eid).rjust(eidWidth), level * INDENT, cString(emotion), parent if parent != None else NO_PARENT)

def cReason(ridWidth, rid, reason, weight, eid, emotion):
    return REASON_NODE % (str(rid).rjust(ridWidth), cString(reason), weight, eid, emotion)

def writeC(emotions, reasons):
    emotions = tuple(emotions)
    eidWidth = width(emotions)
    ridWidth = width(reasons)
    with open(getFileName(C_TARGET), 'w') as f:
        f.write(C_CONTENT % ('\n'.join(cEmotion(eidWidth, *emotion) for emotion in emotions), '\n'.join(cReason(ridWidth, *reason) for reason in reasons)))

def main():
    (emotionsIndexes, emotionsTree) = processEmotions()
    reasons = processReasons(emotionsIndexes)
    writeC(emotionsTree, reasons)

if __name__ == '__main__':
    main()
