#!/usr/bin/python
#
# Mesh Console
# Device implementation
#
from csv import reader as CSVReader
from functools import partial

from MeshView import CONST, RAW, PROC

NUM_DEVICES = 300

EMOTIONS_CSV = '../MusicProcessor/Emotions.csv'
REASONS_CSV = '../MusicProcessor/Reasons.csv'

EMOTIONS = tuple(row[-1].decode('utf-8') for row in CSVReader(open(EMOTIONS_CSV)) if row and not row[0].startswith('#'))
REASONS = tuple(row[1] for row in CSVReader(open(REASONS_CSV)) if row and not row[0].startswith('#'))

LONGEST_EMOTION = max((len(emotion), n) for (n, emotion) in enumerate(EMOTIONS))[1]
LONGEST_REASON = max((len(reason), n) for (n, reason) in enumerate(REASONS))[1]

def signedNumber(n):
    return ('%+d' % n) if n else '0'

def getItem(what, index):
    try:
        return what[index].lower()
    except IndexError:
        return index

def getColumnsData(processor):
    return ( # checked, changing, highlight, alignment, name, description, fieldName, longestValue, formatter, fmt
        (True, CONST, False, True, 'ID', 'Device ID', 'number', NUM_DEVICES),
        (True, CONST, False, False, 'Name', 'Device name', 'number', LONGEST_REASON, partial(getItem, REASONS)),
        (True, RAW, True, True, 'Hops', 'Number of hops from device', 'hops', NUM_DEVICES),
        (True, RAW, True, True, 'TimestampC', 'Information timestamp', 'time', 9999),
        (True, RAW, True, True, 'TimestampD', 'Information timestamp date', 'time', 0, processor.cycleDateStr),
        (True, PROC, False, True, 'AgeC', 'Age of information in cycles', 'time', 9999, processor.cycleAge),
        (True, PROC, False, True, 'AgeS', 'Age of information in seconds', 'time', 9999, processor.cycleAgeSeconds),
        (True, PROC, False, True, 'AgeT', 'Age of information as time', 'time', 0, processor.cycleAgeTimeStr),
        (True, RAW, True, True, 'TimeDiffC', 'Time difference in cycles', 'td', 9999, signedNumber),
        (True, RAW, True, True, 'TimeDiffS', 'Time difference in seconds', 'td', 9999, lambda x: signedNumber(processor.cycleSeconds(x))),
        (True, RAW, True, True, 'TimeDiffT', 'Time difference as time', 'td', 0, processor.cycleTimeStr),
        (True, PROC, False, True, 'LocalTimeC', 'Device local time in cycles', 'td', 9999, processor.tdTime),
        (True, PROC, False, True, 'LocalTimeD', 'Device local date', 'td', 0, processor.tdDateStr),
        (True, RAW, True, True, 'Location', 'Device location', 'location', len(REASONS)),
        (True, RAW, True, False, 'LocationN', 'Device location name', 'location', LONGEST_REASON, partial(getItem, REASONS)),
        (True, RAW, True, True, 'Reason', 'Actual reason', 'reason', len(REASONS)),
        (True, RAW, True, False, 'ReasonN', 'Actual reason', 'reason', LONGEST_REASON, partial(getItem, REASONS)),
        (True, RAW, True, True, 'Emotion', 'Emotion', 'emotion', len(EMOTIONS)),
        (True, RAW, True, False, 'EmotionN', 'Emotion', 'emotion', LONGEST_EMOTION, partial(getItem, EMOTIONS)),
        (True, RAW, True, False, 'Energy', 'Energy', 'energy', 100)
    )

class Device(object): # pylint: disable=R0902
    def __init__(self, number, parent):
        assert number in xrange(1, NUM_DEVICES + 1)
        self.number = number
        self.name = ('Device#%%0%dd' % len(str(NUM_DEVICES))) % number
        self.parent = parent
        self.reset()

    @classmethod
    def configure(cls, parent):
        cls.devices = tuple(cls(i, parent) for i in xrange(1, NUM_DEVICES + 1))

    def reset(self):
        self.hops = None
        self.time = None
        self.td = None
        self.location = None
        self.reason = None
        self.emotion = None
        self.energy = None

    def update(self, *args):
        (self.hops, self.time, self.td, self.location, self.reason, self.emotion, self.energy) = ((int(arg) if arg != 'None' else None) for arg in args)

    def settings(self):
        return ' '.join(str(x) for x in (self.hops, self.time, self.td, self.location, self.reason, self.emotion, self.energy)) if self.time else ''
