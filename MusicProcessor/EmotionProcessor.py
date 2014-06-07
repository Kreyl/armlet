#!/usr/bin/python
# -*- coding: utf-8 -*-
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
# - Run this script: python EmotionProcessor.py
# - The script would process Emotions.csv and produce emotions.c and emotions.h
# - Compile and link those two files with whatever code that needs emotions data.
#
from csv import reader as CSVReader
from itertools import chain
from platform import system
from subprocess import Popen, PIPE, STDOUT

try:
    from pytils.translit import translify
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pytils v0.2.3 or later: https://pypi.python.org/pypi/pytils\n" % (ex.__class__.__name__, ex))

from CharacterProcessor import CHARACTERS_CSV, CHARACTER_ID_START, getFileName, updateCharacters

isWindows = system().lower().startswith('win')

EMOTIONS_CSV = 'Emotions.csv'
LOCATIONS_CSV = 'Locations.csv'

C_TARGET = 'emotions.c'

C_CONTENT = '''\
/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py from %s, %s and %s
 *
 * !!! DO NOT EDIT !!!
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
%%s
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
\t// Locations
%%s
\t// Characters
%%s
};

const int reasons_number = countof(reasons);

// End of emotions.c
''' % (EMOTIONS_CSV, LOCATIONS_CSV, CHARACTERS_CSV)

C_NODE = ' /* %s */ %s{ %s, 1, %s, -1, -1 },'

REASON_NODE = ' /* %s */ { %s, %d, %d },'

REASON_COMMENT = '\t/* %s */'

INDENT = '    '

NO_PARENT = 'ROOT'

MAX_ID = 254

MAX_WEIGHT = 9

RESERVED_REASON = 'reserve'

TEST_COMMAND = 'gcc -o test %s test.c && ./test && rm test' % C_TARGET

def width(array):
    return len(str((len(array) or 1) - 1))

def convert(s):
    return translify(s.replace(u'ё', u'е').replace(u'Ё', u'Е'))

def convertTitle(s):
    return convert(s.strip())

def convertEmotion(s):
    return convertTitle(s).lower()

def readCSV(csv): # generator
    with open(csv) as f:
        for row in CSVReader(f):
            assert row, "Bad CSV file format"
            if not row[0].startswith('#'):
                yield row

def getEmotion(emotions, emotion):
    eid = emotions.get(emotion)
    assert eid != None, "Unknown emotion: %s" % emotion
    return eid

def getWeight(weight):
    ret = int(weight)
    assert ret in xrange(MAX_WEIGHT + 1), "Incorrect weight: %d" % ret
    return ret

def reserveReason(rid):
    return (rid, RESERVED_REASON, 0, 0, '')

def addReason(reasons, rid, reason, weight, eid, emotion):
    assert rid == int(rid) and rid >= 0 and rid < MAX_ID, "Bad ReasonID: %s" % rid
    assert reason.lower() not in (r[1].lower() for r in reasons), "Duplicate reason: %s" % reason
    reasons.append((rid, reason, weight, eid, emotion))

def processEmotions(fileName = getFileName(EMOTIONS_CSV)):
    parents = []
    emotionsIndexes = {}
    emotionsTree = []
    for (eid, row) in enumerate(readCSV(fileName)):
        emotion = convertEmotion(row[-1].decode('utf-8'))
        level = len(row) - 1
        if level > len(parents):
            assert level == len(parents) + 1
            parents.append(eid - 1)
        parents = parents[:level]
        emotionsIndexes[emotion] = eid
        emotionsTree.append((eid, level, emotion, parents[-1] if parents else None))
    assert len(emotionsIndexes) == len(emotionsTree), "Duplicate emotions detected"
    return (emotionsIndexes, tuple(emotionsTree))

def processLocations(emotions, fileName = getFileName(LOCATIONS_CSV)):
    reasons = []
    for (rid, row) in enumerate(readCSV(fileName), 1):
        assert len(row) == 3, "Bad locations file format"
        (reason, weight, emotion) = row
        eid = getEmotion(emotions, emotion)
        weight = getWeight(weight)
        addReason(reasons, rid, reason, weight, eid, emotion)
    return tuple(reasons)

def processCharacters(fileName = getFileName(CHARACTERS_CSV)):
    reasons = []
    for row in readCSV(fileName):
        assert len(row) == 2, "Bad characters file format"
        (rid, reason) = row
        addReason(reasons, int(rid), reason, 0, 0, '')
    return tuple(reasons)

def processReasons(emotions):
    locations = (reserveReason(0),) + processLocations(emotions)
    locationReasons = set(r[1].lower() for r in locations)
    characters = processCharacters()
    for r in characters:
        assert r[1].lower() not in locationReasons, "Duplicate reason: %s" % r[1]
    stuffing = tuple(reserveReason(rid) for rid in xrange(len(locations), CHARACTER_ID_START))
    assert tuple(r[0] for r in chain(locations, stuffing, characters)) == tuple(xrange(len(locations) + len(stuffing) + len(characters)))
    return (locations + stuffing, characters)

def verifyCharacter(emotions, fileName):
    (locations, characters) = processReasons(emotions)
    validReasons = tuple(r[1].lower() for r in locations + characters)
    characterReasons = processLocations(emotions, fileName)
    for r in characterReasons:
        assert r[1].lower() in validReasons, "Unknown reason: %s" % r[1]

def cChar(c):
    if c == r'\\':
        return r'\\'
    e = c.encode('unicode_escape')
    if e == c:
        return c
    return r'\x' + e[3:].zfill(4)

def cString(s):
    return '"%s"' % ''.join(cChar(c) for c in s.decode('utf-8'))

def cEmotion(eidWidth, eid, level, emotion, parent):
    return C_NODE % (str(eid).rjust(eidWidth), level * INDENT, cString(emotion), parent if parent != None else NO_PARENT)

def cReason(ridWidth, rid, reason, weight, eid, emotion):
    return REASON_NODE % (str(rid).rjust(ridWidth), cString(reason), weight, eid) + (REASON_COMMENT % emotion if emotion else '')

def writeC(emotions, locations, characters):
    emotions = tuple(emotions)
    eidWidth = width(emotions)
    ridWidth = len(str(max(r[0] for r in chain(locations, characters))))
    with open(getFileName(C_TARGET), 'wb') as f:
        f.write(C_CONTENT % ('\n'.join(cEmotion(eidWidth, *emotion) for emotion in emotions),
                             '\n'.join(cReason(ridWidth, *location) for location in locations),
                             '\n'.join(cReason(ridWidth, *character) for character in characters)))

def updateEmotions():
    charactersList = updateCharacters()
    print "\nProcessing emotions..."
    (emotionsIndexes, emotionsTree) = processEmotions()
    (locations, characters) = processReasons(emotionsIndexes)
    writeC(emotionsTree, locations, characters)
    if isWindows:
        print "Done"
    else:
        print "Running test: %s" % TEST_COMMAND
        subprocess = Popen(TEST_COMMAND, shell = True, stdout = PIPE, stderr = STDOUT)
        output = subprocess.communicate()[0]
        print "Done (%s): %s" % (subprocess.returncode, output),
    return (emotionsIndexes, charactersList)

def main():
    updateEmotions()

if __name__ == '__main__':
    main()
