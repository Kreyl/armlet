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
    from Levenshtein import distance
except ImportError, ex:
    distance = None
    print "%s: %s\nWARNING: Emotion guessing will not be available.\bPlease install Levenshtein v0.11.2 or later: https://pypi.python.org/pypi/python-Levenshtein\n" % (ex.__class__.__name__, ex)

from Settings import MASTER_ID_START, MASTER_ID_END, MASTER_IDS
from Settings import MOB_ID_START, MOB_ID_END, MOB_IDS
from Settings import PLACEHOLDER_ID_START, PLACEHOLDER_ID_END, PLACEHOLDER_IDS
from Settings import LOCATION_ID_START, LOCATION_ID_END, LOCATION_IDS
from Settings import FORESTA_ID_START, FORESTA_ID_END, FORESTA_IDS
from Settings import EMOTION_FIX_ID_START, EMOTION_FIX_ID_END, EMOTION_FIX_IDS
from Settings import LODGE_ID_START, LODGE_ID_END, LODGE_IDS
from Settings import CHARACTER_ID_START, CHARACTER_ID_END, CHARACTER_IDS
from Settings import FORESTBC_ID_START, FORESTBC_ID_END, FORESTBC_IDS
from Settings import MIST_ID_START, MIST_ID_END, MIST_IDS
from Settings import INTENTION_ID_START, INTENTION_ID_END, INTENTION_IDS
from Settings import MAX_ID

from Settings import RESERVED_WEIGHT, LOCATION_WEIGHT, CHARACTER_WEIGHT, FOREST_WEIGHT, DEATH_WEIGHT

from CharacterProcessor import CHARACTERS_CSV, currentTime, getFileName, updateCharacters

from EmotionConverter import convertEmotion

isWindows = system().lower().startswith('win')

CONSOLE_ENCODING = stdout.encoding or ('cp866' if isWindows else 'UTF-8')

def encodeForConsole(s):
    return s.encode(CONSOLE_ENCODING, 'replace')

SETTINGS_CSV = 'Settings.csv'

EMOTIONS_CSV = 'Emotions.csv'

MASTER_CSV = 'Master.csv'
LOCATIONS_CSV = 'Locations.csv'
LODGES_CSV = 'Lodges.csv'
INTENTIONS_CSV = 'Intentions.csv'

SOURCE_CSVS = (MASTER_CSV, LOCATIONS_CSV, LODGES_CSV, CHARACTERS_CSV, INTENTIONS_CSV)

C_PATH = '../Armlet_fw/SharedAtlLogic'

C_TARGET = join(C_PATH, 'emotions.cpp')

H_TARGET = join(C_PATH, 'emotions.h')

CSV_TARGET = 'Reasons.csv'

C_CONTENT = open('emotions_cpp.tpl').read() % ', '.join(SOURCE_CSVS)

H_CONTENT = open('emotions_h.tpl').read() % (', '.join(SOURCE_CSVS),
       MASTER_ID_START, MASTER_ID_END, MOB_ID_START, MOB_ID_END,
       PLACEHOLDER_ID_START, PLACEHOLDER_ID_END, LOCATION_ID_START, LOCATION_ID_END,
       FORESTA_ID_START, FORESTA_ID_END, EMOTION_FIX_ID_START, EMOTION_FIX_ID_END,
       LODGE_ID_START, LODGE_ID_END, CHARACTER_ID_START, CHARACTER_ID_END,
       FORESTBC_ID_START, FORESTBC_ID_END, MIST_ID_START, MIST_ID_END,
       INTENTION_ID_START, INTENTION_ID_END, MAX_ID)

REASONS_CSV_HEADER = open('reasons_csv.tpl').read() % ', '.join(SOURCE_CSVS)

EMOTION_C_NODE = ' /* %s */ %s{ %s, 1, %s, -1, -1 },'

REASON_C_NODE = ' /* %s */ { %s, %d, %d, %d },'

REASON_COMMENT = '/* %s */'

REASON_COMMENT_OFFSET = 40

INDENT = '    '

EMOTION_H_NODE = '#define EMOTION_%s%s %2d'

REASON_H_NODE = '#define REASON_%s%s %2d'

NO_PARENT = 'ROOT'

RESERVED_REASON = 'R%03d'

MOB_REASON = 'Mob%02d'

PLACEHOLDER_REASON = 'PH%02d'

EMOTION_FIX_REASON = 'x%s'

FORESTA_REASON = 'FA%02d'

FORESTBC_REASON = 'F%s%03d'

MIST_REASON = 'M%03d'

TEST_COMMAND = 'gcc -I "%s" -o test "%s" test.c && ./test && rm test' % (C_PATH, C_TARGET)

def firstCapital(s):
    return '%s%s' % (s[0].upper(), s[1:])

def width(array):
    return len(str((len(array) or 1) - 1))

FON = convertEmotion(u'фон')
WRONG = convertEmotion(u'неверно')
MASTER = convertEmotion(u'мастерка')
SILENCE = convertEmotion(u'тишина')
TUMAN = convertEmotion(u'туман')
SMERT = convertEmotion(u'смерть')
MERTV = convertEmotion(u'мертвяк')
HEARTBEAT = convertEmotion(u'сердцебиение')
TRIP = convertEmotion(u'трип')
CACTUS = convertEmotion(u'кактус')
NARKOMAN = convertEmotion(u'наркоман')
LES = convertEmotion(u'лес')
ADDITIONAL_EMOTIONS = (WRONG, MASTER, SILENCE, TUMAN, SMERT, MERTV, HEARTBEAT, TRIP, CACTUS, NARKOMAN, LES)

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
            if row and not row[0].startswith('#'):
                yield row

def getEmotion(emotions, emotion):
    eid = emotions.get(emotion)
    assert eid != None, "Unknown emotion: %s" % emotion
    return eid

def getWeight(weight):
    ret = int(weight)
    assert 0 <= ret <= DEATH_WEIGHT, "Incorrect weight: %d" % ret
    return ret

def addReason(reasons, rid, reason, weight, age, eid, emotion):
    assert rid == int(rid) and 0 <= rid < MAX_ID, "Bad ReasonID: %s" % rid
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

def processReasonRange(emotions, fileName, tag, startID, maxLength):
    reasons = []
    for (rid, row) in enumerate(readCSV(fileName), startID):
        assert len(row) == 3, "Bad %s file format" % tag
        (reason, weight, emotion) = row
        eid = getEmotion(emotions, emotion)
        weight = getWeight(weight)
        addReason(reasons, rid, reason, weight, 0, eid, emotion)
    assert len(reasons) <= maxLength
    return tuple(reasons)

def processCharacters(emotions, fileName = getFileName(CHARACTERS_CSV)):
    reasons = []
    for row in readCSV(fileName):
        assert len(row) == 9, "Bad characters file format"
        (rid, reason, _longName, _power, _kill, _killLength, _addiction, emotion, _hasMusic) = row
        emotion = emotion.strip()
        addReason(reasons, int(rid), reason, CHARACTER_WEIGHT if emotion else 0, 0, getEmotion(emotions, emotion) if emotion else 0, emotion)
    assert len(reasons) <= len(CHARACTER_IDS)
    return tuple(reasons)

def processReasons(emotions):
    def reserveReason(pattern, rid):
        return (rid, pattern % rid, RESERVED_WEIGHT, 0, emotions[FON], FON)
    def forestReason(rid):
        return (rid, (FORESTA_REASON % rid) if rid <= FORESTA_ID_END else FORESTBC_REASON % ('B' if rid % 2 == 0 else 'C', rid), FOREST_WEIGHT, 0, emotions[LES], LES)
    def emotionFixReason(eid, emotion):
        return (eid + EMOTION_FIX_ID_START, EMOTION_FIX_REASON % firstCapital(emotion), LOCATION_WEIGHT, 0, eid, emotion)
    reasons = []
    num = 0
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, MASTER_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(emotions, getFileName(MASTER_CSV), 'master', MASTER_ID_START, len(MASTER_IDS))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, MOB_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(MOB_REASON, rid) for rid in MOB_IDS)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, PLACEHOLDER_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(PLACEHOLDER_REASON, rid) for rid in PLACEHOLDER_IDS)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, LOCATION_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(emotions, getFileName(LOCATIONS_CSV), 'location', LOCATION_ID_START, len(LOCATION_IDS))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, FORESTA_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(forestReason(rid) for rid in FORESTA_IDS)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, EMOTION_FIX_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(emotionFixReason(eid, emotion) for (eid, emotion) in sorted((eid, emotion) for (emotion, eid) in emotions.iteritems()))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, LODGE_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(emotions, getFileName(LODGES_CSV), 'lodge', LODGE_ID_START, len(LODGE_IDS))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, CHARACTER_ID_START))
    reasons.append(r)
    num += len(r)
    r = processCharacters(emotions)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, FORESTBC_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(forestReason(rid) for rid in FORESTBC_IDS)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, MIST_ID_START))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(MIST_REASON, rid) for rid in MIST_IDS)
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, INTENTION_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(emotions, getFileName(INTENTIONS_CSV), 'intentions', INTENTION_ID_START, len(INTENTION_IDS))
    reasons.append(r)
    num += len(r)
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
    characterReasons = processReasonRange(emotions, fileName, None, 0, 0)
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
    return EMOTION_C_NODE % (str(eid).rjust(eidWidth), level * INDENT, cString(emotion), parent if parent != None else NO_PARENT)

def hEmotion(eid, emotion, padding):
    return EMOTION_H_NODE % (emotion.upper().replace("'", ''), ' ' * padding, eid)

def cReason(ridWidth, rid, reason, weight, age, eid, emotion):
    ret = REASON_C_NODE % (str(rid).rjust(ridWidth), cString(reason), weight, age, eid)
    if emotion:
        ret += ' ' * max(1, (REASON_COMMENT_OFFSET - len(ret))) + (REASON_COMMENT % emotion)
    return ret

def hReason(rid, reason, padding):
    return REASON_H_NODE % (reason.upper().replace("'", ''), ' ' * padding, rid)

def writeC(emotions, reasons):
    eidWidth = width(emotions)
    emotionsText = '\n'.join(cEmotion(eidWidth, *emotion) for emotion in emotions)
    ridWidth = len(str(max(r[0] for r in chain.from_iterable(reasons))))
    reasonsTexts = tuple('\n'.join(cReason(ridWidth, *r) for r in reason) + ('\n' if reason else '') for reason in reasons)
    with open(getFileName(C_TARGET), 'wb') as f:
        f.write(C_CONTENT % ((currentTime(), emotionsText) + reasonsTexts))

def writeH(emotions, reasons):
    maxEmotionWidth = max(len(emotion.replace("'", '')) for (_eid, _level, emotion, _parent) in emotions)
    emotionsText = '\n'.join(hEmotion(eid, emotion.replace("'", ''), maxEmotionWidth - len(emotion.replace("'", ''))) for (eid, _level, emotion, _parent) in emotions)
    maxReasonWidth = max(len(reason.replace("'", '')) for (_rid, reason, _weight, _age, _eid, _emotion) in chain(*reasons))
    reasonsTexts = tuple('\n'.join(hReason(rid, reason, maxReasonWidth - len(reason.replace("'", ''))) for (rid, reason, _weight, _age, _eid, _emotion) in reason) + ('\n' if reason else '') for reason in reasons)
    with open(getFileName(H_TARGET), 'wb') as f:
        f.write(H_CONTENT % ((currentTime(), ' ' * max(1, maxEmotionWidth - 9), len(emotions), emotionsText, ' ' * max(1, maxReasonWidth - 10), sum(len(r) for r in reasons)) + reasonsTexts))

def writeCSV(*reasons):
    with open(getFileName(CSV_TARGET), 'wb') as f:
        f.writelines(s + '\r\n' for s in (REASONS_CSV_HEADER % currentTime()).splitlines())
        CSVWriter(f).writerows((rid, reason) for (rid, reason, _weight, _age, _eid, _emotion) in chain.from_iterable(reasons))

def updateEmotions():
    charactersList = updateCharacters()
    print "\nProcessing emotions..."
    (emotionsIndexes, emotionsTree) = processEmotions()
    reasons = processReasons(emotionsIndexes)
    writeC(emotionsTree, reasons)
    writeH(emotionsTree, reasons)
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
