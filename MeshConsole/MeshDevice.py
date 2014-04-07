#!/usr/bin/python

from csv import reader as CSVReader
from functools import partial

from MeshView import CONST, RAW, PROC

NUM_DEVICES = 100
NUM_STATIC_DEVICES = 10

EMOTIONS_CSV = '../MusicProcessor/Emotions.csv'
REASONS_CSV = '../MusicProcessor/Reasons.csv'

EMOTIONS = tuple(row[-1].decode('utf-8') for row in CSVReader(open(EMOTIONS_CSV)) if row and not row[0].startswith('#'))
REASONS = tuple(row[0] for row in CSVReader(open(REASONS_CSV)) if row and not row[0].startswith('#'))

LONGEST_EMOTION = max((len(emotion), n) for (n, emotion) in enumerate(EMOTIONS))[1]
LONGEST_REASON = max((len(reason), n) for (n, reason) in enumerate(REASONS))[1]

def getItem(what, index):
    try:
        return what[index].lower()
    except IndexError:
        return index

def getColumnsData(processor):
    return ( # checked, changing, highlight, name, description, fieldName, longestValue, formatter, fmt
        (True, CONST, False, 'ID', 'Device ID', 'number', NUM_DEVICES),
        (True, RAW, True, 'Hops', 'Number of hops from device', 'hops', NUM_DEVICES),
        (True, RAW, True, 'TimestampC', 'Information timestamp', 'time', 9999),
        (True, RAW, True, 'TimestampD', 'Information timestamp date', 'time', 0, processor.cycleDateStr),
        (True, PROC, False, 'AgeC', 'Age of information in cycles', 'time', 9999, processor.cycleAge),
        (True, PROC, False, 'AgeS', 'Age of information in seconds', 'time', 9999, processor.cycleAgeSeconds),
        (True, PROC, False, 'AgeT', 'Age of information as time', 'time', 0, processor.cycleAgeTimeStr),
        (True, RAW, True, 'TimeDiffC', 'Time difference in cycles', 'td', 9999, None, '%+d'),
        (True, RAW, True, 'TimeDiffS', 'Time difference in seconds', 'td', 9999, processor.cycleSeconds, '%+d'),
        (True, RAW, True, 'TimeDiffT', 'Time difference as time', 'td', 0, processor.cycleTimeStr),
        (True, PROC, False, 'LocalTimeC', 'Device local time in cycles', 'td', 9999, processor.tdTime),
        (True, PROC, False, 'LocalTimeD', 'Device local date', 'td', 0, processor.tdDateStr),
        (True, RAW, True, 'Location', 'Device location', 'location', len(REASONS)),
        (True, RAW, True, 'LocationN', 'Device location name', 'location', LONGEST_REASON, partial(getItem, REASONS)),
        (True, RAW, True, 'Reason', 'Actual reason', 'reason', len(REASONS)),
        (True, RAW, True, 'ReasonN', 'Actual reason', 'reason', LONGEST_REASON, partial(getItem, REASONS)),
        (True, RAW, True, 'Emotion', 'Emotion', 'emotion', len(EMOTIONS)),
        (True, RAW, True, 'EmotionN', 'Emotion', 'emotion', LONGEST_EMOTION, partial(getItem, EMOTIONS))
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

    def update(self, *args):
        (self.hops, self.time, self.td, self.location, self.reason, self.emotion) = ((int(arg) if arg != 'None' else None) for arg in args)

    def toDumpStr(self):
        return ('%s %s %s %s %s %s %s' % (self.number, self.hops, self.time, self.td, self.location, self.reason, self.emotion)) if self.time else None
