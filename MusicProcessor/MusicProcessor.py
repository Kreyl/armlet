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
from os import chdir, getcwd, makedirs, walk
from os.path import basename, getsize, isdir, isfile, join
from re import compile as reCompile
from sys import argv

try:
    from pydub import AudioSegment
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pydub v0.9.2 or later: https://pypi.python.org/pypi/pydub\n" % (ex.__class__.__name__, ex))

from EmotionProcessor import convert, convertEmotion, convertTitle, processEmotions, processReasons

CHARACTER_CSV = 'Character.csv'

EXTENSIONS = ('mp3', 'wav', 'wma', 'm4a', 'mpeg', 'ogg')

EXTENSIONS_RE = r'\.(%s)$' % '|'.join(EXTENSIONS)

FILE_PATTERN = reCompile('(?i).*' + EXTENSIONS_RE)

EMOTION = 'emotion'
ARTIST = 'artist'
TITLE = 'title'
TAIL = 'tail'

SEPARATOR = '-'

CHECK_PATTERN = reCompile('(?i)^(?P<%s>[^%s]+)%s(?P<%s>[^%s]+)(%s(?P<%s>[^%s]*)(%s(?P<%s>.*))?)?%s' % (EMOTION, SEPARATOR, SEPARATOR, ARTIST, SEPARATOR, SEPARATOR, TITLE, SEPARATOR, SEPARATOR, TAIL, EXTENSIONS_RE))

NEW_FORMAT = 'mp3'

MAX_FILE_NAME = 64

NEW_EXTENSION = '.' + NEW_FORMAT

DEFAULT_TARGET_DIR = 'processed'

TAGS = {
    'publisher': 'Ticket to Atlantis',
    'date': '2014',
    'encoder': 'Pydub/FFMPEG'
}

def findFiles(directory, pattern, excludes = ()):
    for (root, _dirNames, fileNames) in walk(unicode(directory)):
        if root not in excludes:
            for fileName in fileNames:
                if pattern.match(fileName):
                    yield (root, fileName)

def processFile(fullName, newFullName, playerID, albumName, trackNumber, emotion, artist, title, tail):
    try:
        sourceAudio = AudioSegment.from_file(fullName)
        if sourceAudio.duration_seconds < 60:
            print "ERROR: Audio too short: %d seconds" % sourceAudio.duration_seconds
            return False
        processedAudio = sourceAudio.normalize() # pylint: disable=E1103
        if processedAudio.duration_seconds != sourceAudio.duration_seconds:
            print "ERROR: Normalized audio duration mismatch: %d seconds, expected %d seconds" % (processedAudio.duration_seconds, sourceAudio.duration_seconds)
            return False
        TAGS.update({'disc': playerID, 'album': albumName, 'track': trackNumber, 'artist': artist, 'title': title, 'genre': emotion, 'comment': tail, 'comments': tail})
        processedAudio.export(newFullName, format = NEW_FORMAT, bitrate= '256k', tags = TAGS)
        return isfile(newFullName) and getsize(newFullName) > 0.5 * getsize(fullName)
    except EOFError:
        print "ERROR: Error processing!"
        return False

def main(sourceDir = None, targetDir = None):
    if sourceDir:
        chdir(sourceDir)
    albumName = basename(getcwd()) # ToDo: We'd prefer to use full character name
    playerID = str(100) # ToDo: Provide proper Player ID
    (emotionsIndexes, _emotionsTree) = processEmotions()
    if isfile(CHARACTER_CSV):
        print "%s found, verifying" % CHARACTER_CSV
        processReasons(emotionsIndexes, CHARACTER_CSV)
    targetDir = targetDir or join('.', DEFAULT_TARGET_DIR)
    if not isdir(targetDir):
        makedirs(targetDir)
    error = False
    newFileNameSet = set()
    files = tuple(findFiles('.', FILE_PATTERN, (targetDir,)))
    for (trackNumber, (dirName, fileName)) in enumerate(files, 1):
        fullName = join(dirName, fileName)
        match = CHECK_PATTERN.match(fileName)
        if not match:
            print "ERROR: Bad file name: " + fullName
            error = True
            continue
        groups = match.groupdict()
        emotion = convertEmotion(groups[EMOTION])
        artist = convertTitle(groups[ARTIST])
        title = convertTitle(groups[TITLE] or '')
        tail = convert(groups[TAIL] or '')
        if not title:
            title = artist
            artist = ''
        if emotion not in emotionsIndexes:
            print "ERROR: Unknown emotion: " + fullName
            error = True
            continue
        newFileName = emotion
        for s in (artist, title, tail):
            if s:
                newFileName += SEPARATOR + s
        newFileName = newFileName[:MAX_FILE_NAME] + NEW_EXTENSION
        if newFileName in newFileNameSet:
            print "ERROR: Duplicate processed file name: " + newFileName
            error = True
            continue
        newFileNameSet.add(newFileName)
        newFullName = join(targetDir, newFileName)
        print "Processing: " + fullName
        # print "Emotion: " + emotion
        # print "Artist: " + artist
        # print "Title: " + title
        # print "Tail: " + tail
        # print "Processed name: " + newFullName
        if not processFile(fullName, newFullName, playerID, albumName, '%d/%d' % (trackNumber, len(files)), emotion, artist, title, tail):
            error = True
            continue
    print "\n!!! ERRORS !!!" if error else "\nOK"

if __name__ == '__main__':
    main(*argv[1:])
