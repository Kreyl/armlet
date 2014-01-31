#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Music Processor for Ticket to Atlantis LARP.
#
# Usage:
# - Export emotions mind map to FreeMind format (Emotions.mm).
# - Open Emotions.mm in FreeMind.
# - Click File - Export - Using XSLT...
# - In Choose XSL File, locate mm2csv.xsl template in FreeMind/accessories directory.
# - In Choose ExportFile, specify Emotions.csv located in the same directory as this script.
# - Perform Export.
# - Run this script: python MusicProcessor.py [sourceDirectory [targetDirectory]]
# - The script would:
# -- Verify all the files in source directory (recursive) for the correct file name format.
# -- Convert all the files to the format suitable for ArmLet3 to use.
# -- Put the converted files to target directory.
# - If source directory is not specified, current directory is used.
# - If target directory is not specified, source/processed subdirectory is used.
#
from os import chdir, makedirs, walk
from os.path import getsize, isdir, isfile, join
from re import compile as reCompile
from sys import argv

try:
    from pydub import AudioSegment
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pydub v0.8.3 or later: https://pypi.python.org/pypi/pydub\n" % (ex.__class__.__name__, ex))

from EmotionProcessor import convert, convertEmotion, convertTitle, processEmotions, processReasons

CHARACTER_CSV = 'Character.csv'

EXTENSIONS = ('mp3', 'wav', 'wma', 'm4a')

EXTENSIONS_RE = r'\.(%s)$' % '|'.join(EXTENSIONS)

FILE_PATTERN = reCompile('(?i).*' + EXTENSIONS_RE)

EMOTION = 'emotion'
ARTIST = 'artist'
TITLE = 'title'
TAIL = 'tail'

SEPARATOR = '-'

CHECK_PATTERN = reCompile('^(?P<%s>[^%s]+)%s(?P<%s>[^%s]+)%s(?P<%s>[^%s]+)(?P<%s>.*)%s' % (EMOTION, SEPARATOR, SEPARATOR, ARTIST, SEPARATOR, SEPARATOR, TITLE, SEPARATOR, TAIL, EXTENSIONS_RE))

NEW_FORMAT = 'wav'

NEW_EXTENSION = '.' + NEW_FORMAT

DEFAULT_TARGET_DIR = 'processed'

def findFiles(directory, pattern, excludes = ()):
    for (root, _dirNames, fileNames) in walk(directory):
        if root not in excludes:
            for fileName in fileNames:
                if pattern.match(fileName):
                    yield (root, fileName)

def processFile(fullName, newFullName):
    try:
        AudioSegment.from_file(fullName).export(newFullName, format = NEW_FORMAT)
        return isfile(newFullName) and getsize(newFullName) >= getsize(fullName)
    except EOFError:
        return False

def main(sourceDir = None, targetDir = None):
    if sourceDir:
        chdir(sourceDir)
    (emotionsIndexes, _emotionsTree) = processEmotions()
    if isfile(CHARACTER_CSV):
        print "%s found, verifying" % CHARACTER_CSV
        processReasons(emotionsIndexes, CHARACTER_CSV)
    targetDir = targetDir or join('.', DEFAULT_TARGET_DIR)
    if not isdir(targetDir):
        makedirs(targetDir)
    error = False
    newFileNameSet = set()
    for (dirName, fileName) in findFiles('.', FILE_PATTERN, (targetDir,)):
        fullName = join(dirName, fileName)
        match = CHECK_PATTERN.match(fileName)
        if not match:
            print "ERROR: Bad file name: " + fullName
            error = True
            continue
        groups = match.groupdict()
        emotion = convertEmotion(groups[EMOTION])
        artist = convertTitle(groups[ARTIST])
        title = convertTitle(groups[TITLE])
        tail = convert(groups[TAIL])
        if emotion not in emotionsIndexes:
            print "ERROR: Unknown emotion: " + fullName
            error = True
            continue
        newFileName = SEPARATOR.join((emotion, artist, title)) + tail + NEW_EXTENSION
        if newFileName in newFileNameSet:
            print "ERROR: Duplicate processed file name: " + newFileName
            error = True
            continue
        newFileNameSet.add(newFileName)
        newFullName = join(targetDir, newFileName)
        print
        print "Processing: " + fullName
        print "Emotion: " + emotion
        print "Artist: " + artist
        print "Title: " + title
        print "Tail: " + tail
        print "Processed name: " + newFullName
        if not processFile(fullName, newFullName):
            print "ERROR: Error processing!"
            error = True
            continue
    print "\n!!! ERRORS !!!" if error else "\nOK"

if __name__ == '__main__':
    main(*argv[1:])
