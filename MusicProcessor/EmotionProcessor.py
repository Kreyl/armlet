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
# - The script would process Emotions.csv, Locations.csv and Characters.csv
#   and produce Reasons.csv, emotions.c and emotions.h.
# - Compile and link emotions.c and emotions.h with whatever code that needs emotions data.
#
from csv import reader as CSVReader, writer as CSVWriter
from itertools import chain
from os.path import join
from platform import system
from subprocess import Popen, PIPE, STDOUT
from sys import stdout

try:
    from pytils.translit import translify
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pytils v0.2.3 or later: https://pypi.python.org/pypi/pytils\n" % (ex.__class__.__name__, ex))

try:
    from unidecode import unidecode
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install Unidecode v0.04.16 or later: https://pypi.python.org/pypi/Unidecode/\n" % (ex.__class__.__name__, ex))

try:
    from Levenshtein import distance
except ImportError, ex:
    distance = None
    print "%s: %s\nWARNING: Emotion guessing will not be available.\bPlease install Levenshtein v0.11.2 or later: https://pypi.python.org/pypi/python-Levenshtein\n" % (ex.__class__.__name__, ex)

from Settings import LOCATION_ID_START, LOCATIONS_ID_END, LOCATION_IDS
from Settings import MIST_ID_START, MIST_ID_END, MIST_IDS
from Settings import CHARACTER_ID_START, CHARACTER_ID_END, CHARACTER_IDS
from Settings import INTENTION_ID_START, INTENTION_ID_END, INTENTION_IDS
from Settings import EMOTION_FIX_ID_START, EMOTION_FIX_ID_END, EMOTION_FIX_IDS

from CharacterProcessor import CHARACTERS_CSV, currentTime, getFileName, updateCharacters

isWindows = system().lower().startswith('win')

CONSOLE_ENCODING = stdout.encoding or ('cp866' if isWindows else 'UTF-8')

def encodeForConsole(s):
    return s.encode(CONSOLE_ENCODING, 'replace')

EMOTIONS_CSV = 'Emotions.csv'
LOCATIONS_CSV = 'Locations.csv'
INTENTIONS_CSV = 'Intentions.csv'

C_PATH = '../Armlet_fw/AtlLogic'

C_TARGET = join(C_PATH, 'emotions.c')

CSV_TARGET = 'Reasons.csv'

C_CONTENT = '''\
/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from %s, %s, %s and %s
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at %%s
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
%%s
};

const int emotions_number = countof(emotions);

// RID ranges
const int first_location_id = %d;
const int last_location_id = %d;

const int first_mist_id = %d;
const int last_mist_id = %d;

const int first_character_id = %d;
const int last_character_id = %d;

const int first_intention_id = %d;
const int last_intention_id = %d;

const int first_emotion_fix_id = %d;
const int last_emotion_fix_id = %d;

// RIDs are indexes in this array.
Reason_t reasons[] = {
%%s
\t// Locations
%%s
\t// end of locations
%%s
\t// Mist sources
%%s
\t// end of mist sources
%%s
\t// Characters
%%s
\t// end of characters
%%s
\t// Intentions
%%s
\t// end of intentions
%%s
\t// Emotion fixes
%%s
\t// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
''' % (EMOTIONS_CSV, LOCATIONS_CSV, CHARACTERS_CSV, INTENTIONS_CSV,
       LOCATION_ID_START, LOCATIONS_ID_END, MIST_ID_START, MIST_ID_END, CHARACTER_ID_START, CHARACTER_ID_END, INTENTION_ID_START, INTENTION_ID_END, EMOTION_FIX_ID_START, EMOTION_FIX_ID_END)

REASONS_CSV_HEADER = '''\
#
# Reasons.csv
#
# RID -> Reason table for MeshConsole and other uses.
#
# Generated by EmotionProcessor.py
#
# !!! DO NOT EDIT !!!
#
# Generated at %s
#
'''

C_NODE = ' /* %s */ %s{ %s, 1, %s, -1, -1 },'

REASON_NODE = ' /* %s */ { %s, %d, %d, %d },'

REASON_COMMENT = '\t/* %s */'

INDENT = '    '

NO_PARENT = 'ROOT'

MAX_ID = 254

MAX_WEIGHT = 99

RESERVED_REASON = 'r%03d'

MIST_REASON = 'm%02d'

EMOTION_FIX_REASON = 'x%s'

EMOTION_FIX_WEIGHT = 5

TEST_COMMAND = 'gcc -I "%s" -o test "%s" test.c && ./test && rm test' % (C_PATH, C_TARGET)

TRANSLIFY_PATCHES = {
    u'и\u0306': u'й' # й on Mac
}

EMOTION_PATCHES = {
    'somneniya': 'somnenie',
    'sex': 'seks',
    'napryazhenie': 'napryazhennost\'',
    'nastol\'giya': 'nostal\'giya',
    'sostrodanie': 'sostradanie',
    'udovletvorennost': 'udovletvorenie'
}

def firstCapital(s):
    return '%s%s' % (s[0].upper(), s[1:])

def width(array):
    return len(str((len(array) or 1) - 1))

def convert(s):
    for (f, t) in TRANSLIFY_PATCHES.iteritems():
        s = s.replace(f, t)
    ret = ''
    for c in s:
        try:
            c = translify(c)
        except ValueError:
            c = unidecode(c)
        ret += c
    return ret

def convertTitle(s):
    return convert(s.strip())

def convertEmotion(s):
    e = convertTitle(s).lower()
    return EMOTION_PATCHES.get(e, e)

WRONG = convertEmotion(u'неверно')
MASTER = convertEmotion(u'мастерка')
SILENCE = convertEmotion(u'тишина')
TUMAN = convertEmotion(u'туман')
SMERT = convertEmotion(u'смерть')
MERTV = convertEmotion(u'мертвяк')
HEARTBEAT = convertEmotion(u'сердцебиение')
TRIP = convertEmotion(u'трип')
ADDITIONAL_EMOTIONS = (WRONG, MASTER, SILENCE, TUMAN, SMERT, MERTV, HEARTBEAT, TRIP)

EMOTION_ACCEPT_RANGE = 1
EMOTION_GUESS_RANGE = 3
EMOTION_GUESS_MAX_PROPOSALS = 9

def guessEmotion(emotions, emotion):
    emotion = convertEmotion(emotion)
    if not distance or emotion in emotions:
        return emotion
    distances = sorted((distance(e, emotion), e) for e in emotions)
    acceptable = tuple((d, e) for (d, e) in distances if d <= EMOTION_ACCEPT_RANGE)
    if len(acceptable) == 1:
        print "\nWARNING: Unknown emotion: %s, replaced with %s" % (emotion, acceptable[0][1])
        return acceptable[0][1]
    close = ((d, e) for (d, e) in distances if d <= EMOTION_GUESS_RANGE)
    proposals = (emotion,) + tuple(e for (d, e) in sorted(close))[:EMOTION_GUESS_MAX_PROPOSALS]
    ret = None
    while True:
        print "\nUnknown emotion: %s, possible matches are:" % emotion
        for (i, proposal) in enumerate(proposals):
            print "%d: %s%s" % (i, proposal, '' if i else ' (leave as is)')
        s = raw_input("Choose: ").strip()
        try:
            n = int(s)
            if 0 <= n < len(proposals):
                ret = proposals[n]
                break
        except Exception:
            if s in emotions:
                ret = s
                break
    print "Using: %s" % ret
    return ret

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

def addReason(reasons, rid, reason, weight, age, eid, emotion):
    assert rid == int(rid) and rid >= 0 and rid < MAX_ID, "Bad ReasonID: %s" % rid
    assert reason.lower() not in (r[1].lower() for r in reasons), "Duplicate reason: %s" % reason
    reasons.append((rid, reason, weight, age, eid, emotion))

def processEmotions(fileName = getFileName(EMOTIONS_CSV)):
    parents = []
    emotionsIndexes = {}
    emotionsTree = []
    rows = tuple(readCSV(fileName))
    for (eid, row) in enumerate(chain(rows[0:1], (('', e) for e in ADDITIONAL_EMOTIONS), rows[1:])):
        emotion = convertEmotion(row[-1].decode('utf-8'))
        level = len(row) - 1
        if level > len(parents):
            assert level == len(parents) + 1
            parents.append(eid - 1)
        parents = parents[:level]
        emotionsIndexes[emotion] = eid
        emotionsTree.append((eid, level, emotion, parents[-1] if parents else None))
    assert len(emotionsIndexes) == len(emotionsTree), "Duplicate emotions detected"
    assert len(emotionsIndexes) <= len(EMOTION_FIX_IDS)
    return (emotionsIndexes, tuple(emotionsTree))

def processLocations(emotions, fileName = getFileName(LOCATIONS_CSV), tag = 'locations', startID = LOCATION_ID_START, maxLength = len(LOCATION_IDS)):
    reasons = []
    for (rid, row) in enumerate(readCSV(fileName), startID):
        assert len(row) == 3, "Bad %s file format" % tag
        (reason, weight, emotion) = row
        eid = getEmotion(emotions, emotion)
        weight = getWeight(weight)
        addReason(reasons, rid, reason, weight, 0, eid, emotion)
    assert len(reasons) <= maxLength
    return tuple(reasons)

def processIntentions(emotions, fileName = getFileName(INTENTIONS_CSV)):
    return processLocations(emotions, fileName, 'intentions', INTENTION_ID_START, len(INTENTION_IDS))

def processCharacters(fileName = getFileName(CHARACTERS_CSV)):
    reasons = []
    for row in readCSV(fileName):
        assert len(row) == 4, "Bad characters file format"
        (rid, reason, _longName, _power) = row
        addReason(reasons, int(rid), reason, 0, 0, 0, '')
    assert len(reasons) <= len(CHARACTER_IDS)
    return tuple(reasons)

def processReasons(emotions):
    def reserveReason(rid):
        return (rid, RESERVED_REASON % rid, MAX_WEIGHT, 0, emotions[WRONG], WRONG) # ToDo: Default reserve to fon?
    def mistReason(rid):
        return (rid, MIST_REASON % rid, 0, 0, emotions['fon'], 'fon')
    def emotionFixReason(eid, emotion):
        return (eid + EMOTION_FIX_ID_START, EMOTION_FIX_REASON % firstCapital(emotion), EMOTION_FIX_WEIGHT, 0, eid, emotion)
    stuffing0 = (reserveReason(0),)
    locations =  processLocations(emotions)
    stuffing1 = tuple(reserveReason(rid) for rid in xrange(len(stuffing0) + len(locations), MIST_ID_START))
    mists = tuple(mistReason(rid) for rid in MIST_IDS)
    stuffing2 = tuple(reserveReason(rid) for rid in xrange(len(stuffing0) + len(locations) + len(stuffing1) + len(mists), CHARACTER_ID_START))
    characters = processCharacters()
    stuffing3 = tuple(reserveReason(rid) for rid in xrange(len(stuffing0) + len(locations) + len(stuffing1) + len(mists) + len(stuffing2) + len(characters), INTENTION_ID_START))
    intentions =  processIntentions(emotions)
    stuffing4 = tuple(reserveReason(rid) for rid in xrange(len(stuffing0) + len(locations) + len(stuffing1) + len(mists) + len(stuffing2) + len(characters) + len(stuffing3) + len(intentions), EMOTION_FIX_ID_START))
    emotionFixes = tuple(emotionFixReason(eid, emotion) for (eid, emotion) in sorted((eid, emotion) for (emotion, eid) in emotions.iteritems()))
    reasons = (stuffing0, locations, stuffing1, mists, stuffing2, characters, stuffing3, intentions, stuffing4, emotionFixes)
    checkSets = (emotions,) + tuple(set(r[1].lower() for r in reason) for reason in reasons)
    for i in xrange(0, len(checkSets)):
        for rs in checkSets[:i]:
            for r in checkSets[i]:
                assert r not in rs, "Duplicate reason: %s" % r
    assert tuple(r[0] for r in chain.from_iterable(reasons)) == tuple(xrange(sum(len(r) for r in reasons)))
    return reasons

def verifyCharacter(emotions, fileName):
    reasons = processReasons(emotions)
    validReasons = tuple(r[1].lower() for r in chain.from_iterable(reasons))
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

def cReason(ridWidth, rid, reason, weight, age, eid, emotion):
    return REASON_NODE % (str(rid).rjust(ridWidth), cString(reason), weight, age, eid) + (REASON_COMMENT % emotion if emotion else '')

def writeC(emotions, *reasons):
    eidWidth = width(emotions)
    ridWidth = len(str(max(r[0] for r in chain.from_iterable(reasons))))
    emotionsText = '\n'.join(cEmotion(eidWidth, *emotion) for emotion in emotions)
    reasonsTexts = tuple('\n'.join(cReason(ridWidth, *r) for r in reason) for reason in reasons)
    with open(getFileName(C_TARGET), 'wb') as f:
        f.write(C_CONTENT % ((currentTime(), emotionsText) + reasonsTexts))

def writeCSV(*reasons):
    with open(getFileName(CSV_TARGET), 'wb') as f:
        f.writelines(s + '\r\n' for s in (REASONS_CSV_HEADER % currentTime()).splitlines())
        CSVWriter(f).writerows((rid, reason) for (rid, reason, _weight, _age, _eid, _emotion) in chain.from_iterable(reasons))

def updateEmotions():
    charactersList = updateCharacters()
    print "\nProcessing emotions..."
    (emotionsIndexes, emotionsTree) = processEmotions()
    reasons = processReasons(emotionsIndexes)
    writeC(emotionsTree, *reasons)
    writeCSV(*reasons)
    if isWindows:
        print "Not running test on Windows\nDone"
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
