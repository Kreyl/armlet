#!/usr/bin/python
#
# Reason Processor for Red Arrow Mesh Demonstration.
#
# Usage:
# - Run this script: python ReasonProcessor.py
# - The script would process Locations.csv and People.csv
#   and produce Reasons.csv, reasons.c and reasons.h.
# - Compile and link reasons.c and reasons.h with whatever code that needs reason data.
#
from csv import reader as CSVReader, writer as CSVWriter
from datetime import datetime
from itertools import chain
from os.path import dirname, join, realpath
from platform import system
from subprocess import Popen, PIPE, STDOUT
from sys import argv, stdout

from Settings import MASTER_ID_START, MASTER_ID_END, MASTER_IDS
from Settings import LOCATION_ID_START, LOCATION_ID_END, LOCATION_IDS
from Settings import PLACEHOLDER_ID_START, PLACEHOLDER_ID_END, PLACEHOLDER_IDS
from Settings import PERSON_ID_START, PERSON_ID_END, PERSON_IDS
from Settings import MAX_MESH_ID, MAX_ID

try:
    from pytils.translit import translify
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pytils v0.2.3 or later: https://pypi.python.org/pypi/pytils\n" % (ex.__class__.__name__, ex))

isWindows = system().lower().startswith('win')

CSV_ENCODING = 'Windows-1251'

CPP_ENCODING = 'Windows-1251'

CONSOLE_ENCODING = stdout.encoding or ('cp866' if isWindows else 'UTF-8')

def encodeForConsole(s):
    return s.encode(CONSOLE_ENCODING, 'replace')

def currentTime():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S')

def getFileName(name):
    return join(dirname(realpath(argv[0])), name)

SETTINGS_CSV = 'Settings.csv'

MASTER_CSV = 'Master.csv'
LOCATIONS_CSV = 'Locations.csv'
PEOPLE_CSV = 'People.csv'

SOURCE_CSVS = (LOCATIONS_CSV, PEOPLE_CSV)

C_PATH = '../Armlet_fw/SharedAtlLogic'

C_TARGET = join(C_PATH, 'reasons.cpp')

H_TARGET = join(C_PATH, 'reasons.h')

CSV_TARGET = 'Reasons.csv'

C_CONTENT = open('reasons_cpp.tpl').read() % ', '.join(SOURCE_CSVS)

H_CONTENT = open('reasons_h.tpl').read() % (', '.join(SOURCE_CSVS),
       MASTER_ID_START, MASTER_ID_END,
       PLACEHOLDER_ID_START, PLACEHOLDER_ID_END,
       LOCATION_ID_START, LOCATION_ID_END,
       PERSON_ID_START, PERSON_ID_END,
       MAX_MESH_ID, MAX_ID)

REASONS_CSV_HEADER = open('reasons_csv.tpl').read() % ', '.join(SOURCE_CSVS)

REASON_C_NODE = ' /* %s */ { %s },'

REASON_COMMENT = '/* %s */'

REASON_COMMENT_OFFSET = 40

INDENT = '    '

REASON_H_NODE = '#define REASON_%s%s %2d'

RESERVED_REASON = 'R%03d'

PLACEHOLDER_REASON = 'PH%02d'

TEST_COMMAND = 'gcc -I "%s" -o test "%s" test.c && ./test && rm test' % (C_PATH, C_TARGET)

def firstCapital(s):
    return '%s%s' % (s[0].upper(), s[1:])

def width(array):
    return len(str((len(array) or 1) - 1))

def readCSV(csv): # generator
    with open(csv) as f:
        for row in CSVReader(f):
            assert row, "Bad file format, empty line: %s" % csv
            if not row[0].startswith('#'):
                yield row

def addReason(reasons, rid, reason):
    assert rid == int(rid) and 0 <= rid < MAX_ID, "Bad ReasonID: %s" % rid
    assert reason.lower() not in (r[1].lower() for r in reasons), "Duplicate reason: %s" % reason
    reasons.append((rid, reason))

def processReasonRange(fileName, tag, startID, maxLength):
    reasons = []
    for (rid, row) in enumerate(readCSV(fileName), startID):
        assert len(row) == 1, "Bad file format: %s" % tag
        (reason,) = row
        addReason(reasons, rid, reason.decode(CSV_ENCODING))
    assert maxLength is None or len(reasons) <= maxLength
    return tuple(reasons)

def processReasons():
    def reserveReason(pattern, rid):
        return (rid, pattern % rid)
    reasons = []
    num = 0
    r = tuple(reserveReason('ZER%d', rid) for rid in xrange(num, MASTER_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(getFileName(MASTER_CSV), 'master', MASTER_ID_START, len(MASTER_IDS))
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
    r = processReasonRange(getFileName(LOCATIONS_CSV), 'location', LOCATION_ID_START, len(LOCATION_IDS))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, PERSON_ID_START))
    reasons.append(r)
    num += len(r)
    r = processReasonRange(getFileName(PEOPLE_CSV), 'person', PERSON_ID_START, len(PERSON_IDS))
    reasons.append(r)
    num += len(r)
    r = tuple(reserveReason(RESERVED_REASON, rid) for rid in xrange(num, MAX_ID + 1))
    reasons.append(r)
    num += len(r)
    checkSets = tuple(set(r[1].lower() for r in reason) for reason in reasons)
    for i in xrange(0, len(checkSets)):
        for rs in checkSets[:i]:
            for r in checkSets[i]:
                assert r not in rs, "Duplicate reason: %s" % r
    assert tuple(r[0] for r in chain.from_iterable(reasons)) == tuple(xrange(sum(len(r) for r in reasons)))
    return reasons

def cString(s):
    return '"%s"' % s.encode(CPP_ENCODING)

def cReason(ridWidth, rid, reason):
    return REASON_C_NODE % (str(rid).rjust(ridWidth), cString(reason))

def hReason(rid, reason, padding):
    return REASON_H_NODE % (translify(unicode(reason)).upper().replace("'", '').replace('-', '_').replace(' ', '_'), ' ' * padding, rid)

def writeC(reasons):
    ridWidth = len(str(max(r[0] for r in chain.from_iterable(reasons))))
    reasonsTexts = tuple('\n'.join(cReason(ridWidth, *r) for r in reason) + ('\n' if reason else '') for reason in reasons)
    with open(getFileName(C_TARGET), 'wb') as f:
        f.write(C_CONTENT % ((currentTime(),) + reasonsTexts))

def writeH(reasons):
    maxReasonWidth = max(len(translify(unicode(reason)).replace("'", '')) for (_rid, reason) in chain(*reasons)) + len(str(MAX_ID))
    reasonsTexts = tuple('\n'.join(hReason(rid, reason, maxReasonWidth - len(translify(unicode(reason)).replace("'", '')) - len(str(rid)) - int(len(str(rid)) == 1)) for (rid, reason) in reason) + ('\n' if reason else '') for reason in reasons)
    with open(getFileName(H_TARGET), 'wb') as f:
        f.write(H_CONTENT % ((currentTime(), ' ' * max(1, maxReasonWidth - 9 - len(str(MAX_ID))), sum(len(r) for r in reasons)) + reasonsTexts))

def writeCSV(*reasons):
    with open(getFileName(CSV_TARGET), 'wb') as f:
        f.writelines(s + '\r\n' for s in (REASONS_CSV_HEADER % currentTime()).splitlines())
        CSVWriter(f).writerows((rid, reason.encode(CSV_ENCODING)) for (rid, reason) in chain.from_iterable(reasons))

def updateReasons():
    print "\nProcessing reasons..."
    reasons = processReasons()
    writeC(reasons)
    writeH(reasons)
    writeCSV(*reasons)
    if isWindows:
        print "Not running test on Windows\nDone"
    else:
        print "Running test: %s" % TEST_COMMAND
        subprocess = Popen(TEST_COMMAND, shell = True, stdout = PIPE, stderr = STDOUT)
        output = subprocess.communicate()[0]
        print "Done (%s): %s" % (subprocess.returncode, output),

def main():
    updateReasons()

if __name__ == '__main__':
    main()
