#!/usr/bin/python
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
from errno import ENOENT
from itertools import count
from os import getenv, listdir, makedirs, remove
from os.path import getmtime, getsize, isdir, isfile, join
from re import compile as reCompile
from sys import argv, stdout

try:
    from pydub import AudioSegment
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pydub v0.9.2 or later: https://pypi.python.org/pypi/pydub\n" % (ex.__class__.__name__, ex))

from EmotionProcessor import convert, convertEmotion, convertTitle, updateEmotions, verifyCharacter

MUSIC_LOCATION_VARIABLE = 'ATLANTIS_MUSIC'

SOURCE_DIR = 'src'
ARMLET_DIR = 'armlet'
MUSIC_DIR = 'music'

INI_FILE = 'settings.ini'
INI_ID_LINE = 'id=%d\n'

CHARACTER_CSV = 'Character.csv'

FILE_PATTERN = reCompile(r'.*\..*')

EMOTION = 'emotion'
ARTIST = 'artist'
TITLE = 'title'
TAIL = 'tail'

SEPARATOR = '-'

CHECK_PATTERN = reCompile(r'(?i)^(?P<%s>[^%s\s\d]+)\s*\d*\s*%s\s*(?P<%s>[^%s]+?)(?:\s*%s\s*(?P<%s>[^%s]*?)(?:\s*%s\s*(?P<%s>.*))?)?\..*' % (EMOTION, SEPARATOR, SEPARATOR, ARTIST, SEPARATOR, SEPARATOR, TITLE, SEPARATOR, SEPARATOR, TAIL))

NEW_FORMAT = 'mp3'

MAX_FILE_NAME = 64

NEW_EXTENSION = '.' + NEW_FORMAT

DEFAULT_TARGET_DIR = 'processed'

TAGS = {
    'publisher': 'Ticket to Atlantis',
    'date': '2014',
    'encoder': 'Pydub/FFMPEG'
}

MUSIC_MARK = 'music_here'

RESULT_MARKS = { True: 'music_ok', False: 'music_errors' }

def silentRemove(filename):
    try:
        remove(filename)
    except OSError, e:
        if e.errno != ENOENT:
            raise

def processFile(fullName, newFullName, playerID, albumName, trackNumber, emotion, artist, title, tail):
    try:
        sourceAudio = AudioSegment.from_file(fullName)
        if sourceAudio.duration_seconds < 50:
            return "Audio too short: %d seconds" % sourceAudio.duration_seconds
        processedAudio = sourceAudio.normalize() # pylint: disable=E1103
        if processedAudio.duration_seconds != sourceAudio.duration_seconds:
            return "Normalized audio duration mismatch: %d seconds, expected %d seconds" % (processedAudio.duration_seconds, sourceAudio.duration_seconds)
        TAGS.update({'disc': playerID, 'album': albumName, 'track': trackNumber, 'artist': artist, 'title': title, 'genre': emotion, 'comment': tail, 'comments': tail})
        processedAudio.export(newFullName, format = NEW_FORMAT, bitrate= '256k', tags = TAGS)
        if not isfile(newFullName) or getsize(newFullName) < 0.2 * getsize(fullName):
            return "Processed file is too small: %d bytes, while original file was %d bytes" % (getsize(newFullName), getsize(fullName))
        return None
    except Exception, e:
        return e

def resultMark(targetDir, result):
    for markName in RESULT_MARKS.itervalues():
        silentRemove(join(targetDir, markName))
    if result is not None:
        with open(join(targetDir, RESULT_MARKS[bool(result)]), 'w'):
            pass

def checkResultMark(targetDir):
    okMark = join(targetDir, RESULT_MARKS[True])
    errorMark = join(targetDir, RESULT_MARKS[False])
    if isfile(okMark):
        if isfile(errorMark):
            remove(okMark)
            remove(errorMark)
        else:
            return getmtime(okMark)
    elif isfile(errorMark):
        return False
    return None

def processCharacter(name, number, emotions, baseDir = '.'):
    def error(s):
        print "\nERROR: %s" % s
        errors.append(s)
    errors = []
    print "\nProcessing character: %s (%d)" % (name, number)
    baseDir = join(unicode(baseDir), name)
    armletDir = join(baseDir, ARMLET_DIR)
    musicDir = join(armletDir, MUSIC_DIR)
    if not isdir(musicDir):
        makedirs(musicDir)
    sourceDir = join(baseDir, SOURCE_DIR)
    if number > 0:
        with open(join(armletDir, INI_FILE), 'w') as f:
            f.write(INI_ID_LINE % number)
    characterFile = join(armletDir, CHARACTER_CSV)
    if isfile(characterFile):
        print "Character file found, verifying"
        verifyCharacter(emotions, characterFile)
    okDate = checkResultMark(baseDir)
    if okDate:
        musicFiles = listdir(musicDir)
        if musicFiles and any(getmtime(f) > okDate for f in musicFiles):
            print "OK mark obsolete, newer source files exist"
            resultMark(baseDir, None)
            okDate = None
    if okDate:
        hasMusic = True
        print "Already OK"
    elif not isdir(sourceDir):
        print sourceDir
        hasMusic = False
        print "No music source found"
    else:
        files = listdir(sourceDir)
        hasMusic = bool(files)
        print "Music files found: %d" % len(files)
        newFileNameSet = set()
        for (trackNumber, fileName) in enumerate(files, 1):
            match = CHECK_PATTERN.match(fileName)
            if not match:
                error("Bad file name: " + fileName)
                continue
            groups = match.groupdict()
            emotion = convertEmotion(groups[EMOTION])
            artist = convertTitle(groups[ARTIST])
            title = convertTitle(groups[TITLE] or '')
            tail = convert(groups[TAIL] or '')
            if not title:
                title = artist
                artist = ''
            if emotion not in emotions:
                error("Unknown emotion: " + fileName)
                continue
            newFileNamePrefix = SEPARATOR.join((emotion, name))
            for s in (artist, title, tail):
                if s:
                    newFileNamePrefix += SEPARATOR + s
            newFileNamePrefix = newFileNamePrefix[:MAX_FILE_NAME]
            for i in count():
                newFileName = '%s%s%s' % (newFileNamePrefix, i or '', NEW_EXTENSION)
                if newFileName not in newFileNameSet:
                    break
            fullName = join(sourceDir, fileName)
            newFileNameSet.add(newFileName)
            newFullName = join(musicDir, newFileName)
            stdout.write('.') # "%s -> %s" % (fileName, newFileName)
            stdout.flush()
            e = processFile(fullName, newFullName, number, name, '%d/%d' % (trackNumber, len(files)), emotion, artist, title, tail)
            if e:
                error("Error processing: %s: %s" % (e, fullName))
        print
        obsoleteFiles = tuple(f for f in listdir(unicode(musicDir)) if f not in newFileNameSet)
        if obsoleteFiles:
            error("Obsolete files found (%d), removing" % len(obsoleteFiles))
            for fileName in obsoleteFiles:
                remove(join(musicDir, fileName))
        resultMark(baseDir, errors)
        if errors:
            print "Processed with ERRORS:"
            for e in errors:
                print e
        else:
            print "Processed OK"
    return (hasMusic, bool(errors))

def updateMusic(sourceDir = '.'):
    if not isdir(sourceDir):
        print "Music directory not found: %s" % sourceDir
        return
    if not isfile(join(sourceDir, MUSIC_MARK)):
        print "Not a music directory: %s" % sourceDir
        return
    characterDirs = tuple(sorted(str(d) for d in listdir(unicode(sourceDir)) if isdir(join(sourceDir, d))))
    if not characterDirs:
        print "No character directories found in music directory: %s" % sourceDir
        return
    (emotions, characters) = updateEmotions()
    unknownCharacters = tuple(sorted(d for d in characterDirs if d not in characters))
    noMusicCharacters = [name for name in characters if name not in characterDirs]
    print
    print "Processing music at %s" % sourceDir
    print "Known characters found: %d%s" % (len(characters), (' (%d have no music dir)' % len(noMusicCharacters)) if noMusicCharacters else '')
    print "Character directories found: %d%s" % (len(characterDirs), (' (%d unknown)' % len(unknownCharacters)) if unknownCharacters else '')
    errorCharacters = []
    for d in characterDirs:
        (hasMusic, hasErrors) = processCharacter(d, characters.get(d, -1), emotions, sourceDir)
        if not hasMusic:
            noMusicCharacters.append(d)
        if hasErrors:
            errorCharacters.append(d)
    if unknownCharacters:
        print "\nUnknown character directories found (%d): %s" % (len(unknownCharacters), ', '.join(unknownCharacters))
    if noMusicCharacters:
        print "\nNo music found for characters (%d): %s" % (len(noMusicCharacters), ', '.join(sorted(noMusicCharacters)))
    if errorCharacters:
        print "\nErrors detected with music for characters (%d): %s" % (len(errorCharacters), ', '.join(sorted(errorCharacters)))

def main(sourceDir = None):
    updateMusic(sourceDir or getenv(MUSIC_LOCATION_VARIABLE, '.'))

if __name__ == '__main__':
    main(*argv[1:])
