#!/usr/bin/python
#
# Music Processor for Ticket to Atlantis LARP.
#
# - Uses CharacterProcessor.py to update character information.
# - Uses EmotionProcessor.py to update emotions information.
# - Updates and verifies information in Atlantis Music Directory
#   specified with ATLANTIS_MUSIC environment variable, with a parameter
#   or in current directory.
# - Atlantis Music directory must contain music_here file.
# - If -v|--verify option is used, actual processed files are verified for consistency.
#
# Usage:
# - python MusicProcessor.py [-v|--verify] [musicDirectory]
#
from codecs import open as codecsOpen
from datetime import datetime
from errno import ENOENT
from getopt import getopt
from itertools import chain, count
from os import getenv, listdir, makedirs, remove, rmdir, walk
from os.path import basename, expanduser, getmtime, getsize, isdir, isfile, islink, join
from re import compile as reCompile
from shutil import copy, copytree, rmtree
from sys import argv, platform, stdout
from time import mktime
from traceback import format_exc

try:
    from pydub import AudioSegment
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pydub v0.9.2 or later: https://pypi.python.org/pypi/pydub\n" % (ex.__class__.__name__, ex))

from EmotionConverter import convert, convertEmotion, convertTitle
from EmotionProcessor import guessEmotion, updateEmotions, verifyCharacter

MUSIC_LOCATION_VARIABLE = 'ATLANTIS_MUSIC'

SOURCE_DIR = 'src'
ARMLET_DIR = 'armlet'
MUSIC_DIR = 'music'
ERROR_DIR = 'errors'

SD_DIR = '_SD'
EXCLUDE_DIRS = (SD_DIR,)

INI_FILE = 'settings.ini'
INI_CONTENT = '''\
[radio]
id=%d
locationThreshold=60
forestTheshold=60
mistThreshold=0
characterThreshold=0
IRlevel=75

[character]
fightPower=%d
readyToKillInSeconds=%d
readyToKillForMinutes=%d
addiction=%d
'''.replace('\r\n', '\n').replace('\n', '\r\n')

CHARACTER_CSV = 'character.csv'

FILE_PATTERN = reCompile(r'.*\..*')

EMOTION = 'emotion'
ARTIST = 'artist'
TITLE = 'title'
TAIL = 'tail'

SEPARATOR = '-'

CHECK_PATTERN = reCompile(r'(?i)^(?P<%s>[^%s\s\d]+)\s*\d*\s*%s\s*(?P<%s>[^%s]*)(?:\s*%s\s*(?P<%s>[^%s]*?)(?:\s*%s\s*(?P<%s>.*))?)?\.[^.]*$' % (EMOTION, SEPARATOR, SEPARATOR, ARTIST, SEPARATOR, SEPARATOR, TITLE, SEPARATOR, SEPARATOR, TAIL))
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

RESULT_MARKS = { True: 'music_errors', False: 'music_ok' }

CRITICAL_EMOTIONS = ('fon', 'seks', 'draka', 'ubijstvo', 'sozidanie', 'razrushenie', 'zavisimost\'')

INVALID_FILENAME_CHARS = '<>:"/\\|?*' # for file names, to be replaced with _
def cleanupFileName(fileName):
    return ''.join('_' if c in INVALID_FILENAME_CHARS else c for c in fileName)

isWindows = platform.lower().startswith('win')
CONSOLE_ENCODING = stdout.encoding or ('cp866' if isWindows else 'UTF-8')
def encodeForConsole(s):
    return s.encode(CONSOLE_ENCODING, 'replace')

def getFileModificationTime(fileName):
    t = getmtime(fileName)
    dt = datetime.fromtimestamp(t)
    if dt.year > 2050:
        return mktime(dt.replace(year = dt.year - 100).timetuple()) # sorry, no easier way to do it
    return t

def silentRemove(filename):
    try:
        remove(filename)
    except OSError, e:
        if e.errno != ENOENT:
            raise

def createDir(dirName):
    if not isdir(dirName):
        makedirs(dirName)

def deepListDir(dirName):
    return tuple(chain.from_iterable(((dirPath, fileName) for fileName in fileNames) for (dirPath, dirNames, fileNames) in walk(dirName)))

def getFiles(dirName):
    return tuple(join(dirName, f) for f in listdir(dirName)) if isdir(dirName) else ()

def deepGetFiles(dirName):
    return tuple(join(d, f) for (d, f) in deepListDir(dirName)) if isdir(dirName) else ()

def processFile(fullName, newFullName, playerID, albumName, trackNumber, emotion, artist, title, tail):
    try:
        sourceAudio = AudioSegment.from_file(fullName)
        if sourceAudio.duration_seconds < 4:
            return "Audio too short: %d seconds" % sourceAudio.duration_seconds
        if sourceAudio.duration_seconds < 60:
            print "\nWARNING: %s: Audio too short: %d seconds" % (encodeForConsole(basename(fullName)), sourceAudio.duration_seconds)
        processedAudio = sourceAudio.normalize() # pylint: disable=E1103
        if processedAudio.duration_seconds != sourceAudio.duration_seconds:
            return "Normalized audio duration mismatch: %d seconds, expected %d seconds" % (processedAudio.duration_seconds, sourceAudio.duration_seconds)
        TAGS.update({'disc': playerID, 'album': albumName, 'track': trackNumber, 'artist': artist, 'title': title, 'genre': emotion, 'comment': tail, 'comments': tail})
        processedAudio.export(newFullName, format = NEW_FORMAT, bitrate = '256k', tags = TAGS)
        if not isfile(newFullName) or getsize(newFullName) < 0.1 * getsize(fullName):
            return "Processed file is too small: %d bytes, while original file was %d bytes" % (getsize(newFullName), getsize(fullName))
        return None
    except Exception, e:
        print format_exc()
        return e

def verifyFile(fullName):
    try:
        sourceAudio = AudioSegment.from_file(fullName)
        if sourceAudio.duration_seconds < 4:
            return "Audio too short: %d seconds" % sourceAudio.duration_seconds
        if sourceAudio.duration_seconds < 60:
            print "\nWARNING: %s: Audio too short: %d seconds" % (encodeForConsole(basename(fullName)), sourceAudio.duration_seconds)
        processedAudio = sourceAudio.normalize() # pylint: disable=E1103
        if processedAudio.duration_seconds != sourceAudio.duration_seconds:
            return "Normalized audio duration mismatch: %d seconds, expected %d seconds" % (processedAudio.duration_seconds, sourceAudio.duration_seconds)
        processedAudio.export(format = 'null')
        return None
    except Exception, e:
        return e

def resultMark(targetDir, result, okNum = None, okSize = None, errorText = None):
    for markName in RESULT_MARKS.itervalues():
        silentRemove(join(targetDir, markName))
    if okNum is not None:
        with open(join(targetDir, RESULT_MARKS[False]), 'wb') as f:
            f.write('%d,%d\r\n' % (okNum, okSize))
    if result:
        with codecsOpen(join(targetDir, RESULT_MARKS[True]), 'wb', 'utf-8') as f:
            f.write(errorText)

def checkResultMark(targetDir):
    okMark = join(targetDir, RESULT_MARKS[False])
    okDate = getFileModificationTime(okMark) if isfile(okMark) else 0
    okText = ''
    if okDate:
        with open(okMark) as f:
            okText = tuple(int(i) for i in f.read().split(','))
            okNum = okText[0]
            okSize = okText[1] if len(okText) > 1 else 0
    else:
        okNum = okSize = None
    errorMark = join(targetDir, RESULT_MARKS[True])
    errorDate = getFileModificationTime(errorMark) if isfile(errorMark) else 0
    errorText = ''
    if errorDate:
        with codecsOpen(errorMark, 'r', 'utf-8') as f:
            errorText = f.read()
    return (bool(errorDate), max(okDate, errorDate), okNum, okSize, errorText)

def processCharacter(name, number, otherFields, emotions, baseDir = '.', verifyFiles = False):
    class ProcessException(Exception):
        pass
    def log(error, fileName, message):
        s = '%s%s' % (('%s: ' % fileName) if fileName else '', message)
        if fileName:
            print
        print encodeForConsole(s)
        messages.append('%s\r\n' % s)
        hasErrors[0] = hasErrors[0] or error # pylint: disable=E0601
    print "\nProcessing character: %s (%d)" % (name, number)
    messages = []
    hasErrors = [False]
    sdDir = join(unicode(baseDir), SD_DIR)
    baseDir = join(unicode(baseDir), name)
    sourceDir = join(baseDir, SOURCE_DIR)
    errorDir = join(baseDir, ERROR_DIR)
    armletDir = join(baseDir, ARMLET_DIR)
    musicDir = join(armletDir, MUSIC_DIR)
    createDir(armletDir)
    sourceFiles = deepGetFiles(sourceDir)
    musicFiles = getFiles(musicDir)
    errorFiles = getFiles(errorDir)
    newFileNameSet = set()
    # Removing common files
    for fileName in (join(armletDir, f) for f in listdir(armletDir) if f not in (MUSIC_DIR, CHARACTER_CSV)):
        if isdir(fileName) and not islink(fileName):
            rmtree(fileName)
        else:
            remove(fileName)
    # Copying common files
    for fileName in listdir(sdDir):
        src = join(sdDir, fileName)
        dst = join(armletDir, fileName)
        if isdir(src):
            copytree(src, dst)
        else:
            copy(src, dst)
    # Creating settings.ini
    if number > 0:
        with open(join(armletDir, INI_FILE), 'wb') as f:
            f.write(INI_CONTENT % ((number,) + otherFields[:4]))
    # Processing character.csv
    characterFile = join(armletDir, CHARACTER_CSV)
    if isfile(characterFile):
        print "Character file found, verifying"
        verifyCharacter(emotions, characterFile)
    # Check music status
    (withErrors, markDate, okNum, okSize, errorText) = checkResultMark(baseDir)
    if markDate:
        try:
            # Verify that status mark is still actual
            if any(date > markDate for date in (getFileModificationTime(f) for f in chain((sourceDir, musicDir), sourceFiles, musicFiles, errorFiles))):
                raise ProcessException("Status mark obsolete, newer music files exist")
            if okNum is None:
                raise ProcessException("No music files found")
            if okNum != len(musicFiles):
                raise ProcessException("Existing record mentions %s files, but %d is actually found" % (okNum, len(musicFiles)))
            if okSize != sum(getsize(f) for f in musicFiles):
                raise ProcessException("Existing record mentions total file size %d bytes, while actual total size is %d bytes" % (okSize, sum(getsize(f) for f in musicFiles)))
            # Verify existing music files
            print "Veryfying files",
            foundEmotions = set()
            for fileName in listdir(musicDir):
                stdout.write('.')
                stdout.flush()
                fullName = join(musicDir, fileName)
                dumpToErrors = False
                match = CHECK_PATTERN.match(fileName)
                if match:
                    groups = match.groupdict()
                    emotion = convertEmotion(groups[EMOTION])
                    artist = convertTitle(groups[ARTIST])
                    title = convertTitle(groups[TITLE] or '')
                    tail = convert(groups[TAIL] or '')
                    if emotion not in emotions:
                        raise ProcessException("\nUnknown emotion: %d" % emotion)
                    foundEmotions.add(emotion)
                else:
                    raise ProcessException("\nBad file name: %s" % fileName)
                if verifyFiles:
                    e = verifyFile(join(musicDir, fileName))
                    if e:
                        raise ProcessException("\nError processing: %s" % e)
            print
            for emotion in (e for e in CRITICAL_EMOTIONS if e not in foundEmotions):
                print "WARNING: Critical emotion %s is missing" % emotion.upper()
        except ProcessException, e:
            print "%s, reprocessing" % e
            resultMark(baseDir, None)
            markDate = None
    if markDate:
        hasMusic = True
        print "Music already processed %s, %s files found (%d total size)" % ('OK' if not withErrors else 'with ERRORS', okNum, okSize)
        if errorText:
            print errorText.strip()
    elif not isdir(sourceDir):
        hasMusic = False
        log(True, None, "No music source directory found: %s" % sourceDir)
    else:
        # Process source music
        files = deepListDir(sourceDir)
        hasMusic = bool(files)
        foundEmotions = set()
        if not hasMusic:
            log(True, None, "No music files found in source directory: %s" % sourceDir)
        else:
            log(False, None, "Source music files found: %d" % len(files))
            createDir(musicDir)
            if isdir(errorDir):
                for f in chain(getFiles(musicDir), getFiles(errorDir)):
                    remove(f)
                rmdir(errorDir)
            for (trackNumber, (dirName, fileName)) in enumerate(files, 1):
                stdout.write('.')
                stdout.flush()
                fullName = join(dirName, fileName)
                dumpToErrors = False
                match = CHECK_PATTERN.match(fileName)
                if match:
                    groups = match.groupdict()
                    emotion = guessEmotion(emotions, groups[EMOTION])
                    artist = convertTitle(groups[ARTIST])
                    title = convertTitle(groups[TITLE] or '')
                    tail = convert(groups[TAIL] or '')
                    if not title:
                        title = artist
                        artist = ''
                    if emotion not in emotions:
                        log(True, fileName, "Unknown emotion")
                        dumpToErrors = True
                    foundEmotions.add(emotion)
                    newFileNamePrefix = SEPARATOR.join((emotion, name))
                    for s in (artist, title, tail):
                        if s:
                            newFileNamePrefix += SEPARATOR + s
                    newFileNamePrefix = cleanupFileName(newFileNamePrefix)[:MAX_FILE_NAME]
                    for i in count():
                        newFileName = '%s%s%s' % (newFileNamePrefix, i or '', NEW_EXTENSION)
                        if newFileName.lower() not in newFileNameSet:
                            break
                    newFileNameSet.add(newFileName.lower())
                    newFullName = join(musicDir, newFileName)
                else:
                    log(True, fileName, "Bad file name")
                    dumpToErrors = True
                    newFileName = fileName
                if dumpToErrors:
                    createDir(errorDir)
                    newFullName = join(errorDir, newFileName)
                if match:
                    e = processFile(fullName, newFullName, number, name, '%d/%d' % (trackNumber, len(files)), emotion, artist, title, tail)
                else:
                    e = True
                if e:
                    if e != True:
                        log(True, fileName, "Error processing: %s" % e)
                    createDir(errorDir)
                    copy(fullName, errorDir)
            print
            obsoleteFiles = tuple(f for f in listdir(musicDir) if f.lower() not in newFileNameSet)
            if obsoleteFiles:
                print "Obsolete files found (%d), removing:" % len(obsoleteFiles)
                for fileName in obsoleteFiles:
                    print encodeForConsole(fileName)
                    remove(join(musicDir, fileName))
            processedFiles = getFiles(musicDir)
            numProcessed = len(processedFiles)
            if numProcessed != len(files):
                log(True, None, "Processed file number mismatch: %d, expected %d" % (numProcessed, len(files)))
            for emotion in (e for e in CRITICAL_EMOTIONS if e not in foundEmotions):
                print "WARNING: Critical emotion %s is missing" % emotion.upper()
            processedSize = sum(getsize(f) for f in processedFiles)
            resultMark(baseDir, hasErrors[0], numProcessed if hasMusic else None, processedSize if hasMusic else None, ''.join(messages))
            if numProcessed:
                print "Files OK: %d (%d total size)" % (numProcessed, processedSize)
    return (hasMusic, hasErrors[0])

def updateMusic(sourceDir = '.', verifyFiles = False):
    sourceDir = expanduser(sourceDir)
    if not isdir(sourceDir):
        print "Music directory not found: %s" % sourceDir
        return
    if not isfile(join(sourceDir, MUSIC_MARK)):
        print "Not a music directory: %s" % sourceDir
        return
    characterDirs = [str(d) for d in listdir(unicode(sourceDir)) if d not in EXCLUDE_DIRS and isdir(join(sourceDir, d))]
    (emotions, characters) = updateEmotions()
    okCharacters = []
    unknownCharacters = tuple(sorted(d for d in characterDirs if d not in characters))
    for name in (name for name in characters if name not in characterDirs):
        characterDirs.append(name)
        createDir(join(sourceDir, name, SOURCE_DIR))
    characterDirs = tuple(sorted(characterDirs))
    print
    print "Processing music at %s" % sourceDir
    print "Known characters found: %d" % len(characters)
    print "Character directories found: %d%s" % (len(characterDirs), (' (%d unknown)' % len(unknownCharacters)) if unknownCharacters else '')
    noMusicCharacters = []
    errorCharacters = []
    for d in characterDirs:
        (number, _longName, otherFields) = characters.get(d, (-1, None, None))
        markedHasMusic = otherFields[-1] if otherFields else None
        (hasMusic, hasErrors) = processCharacter(d, number, otherFields, emotions, sourceDir, verifyFiles)
        if hasMusic:
            if not hasErrors:
                okCharacters.append(d)
            if not markedHasMusic:
                print "WARNING: Character has music but is not marked so in allrpg"
        else:
            noMusicCharacters.append(d)
            if markedHasMusic:
                print "WARNING: Character does not have music but is marked so in allrpg"
        if hasErrors:
            errorCharacters.append(d)
    if okCharacters:
        print "\nOK character directories found (%d): %s" % (len(okCharacters), ', '.join(okCharacters))
    if unknownCharacters:
        print "\nUnknown character directories found (%d): %s" % (len(unknownCharacters), ', '.join(unknownCharacters))
    if noMusicCharacters:
        print "\nNo music found for characters (%d): %s" % (len(noMusicCharacters), ', '.join(sorted(noMusicCharacters)))
    if errorCharacters:
        print "\nErrors detected with music for characters (%d): %s" % (len(errorCharacters), ', '.join(sorted(errorCharacters)))
    print

def main(*args):
    verifyFiles = False
    (options, parameters) = getopt(args, 'v', ('verify',))
    for (option, _value) in options:
        if option in ('-v', '--verify'):
            verifyFiles = True
    updateMusic(parameters[0] if parameters else getenv(MUSIC_LOCATION_VARIABLE, '.'), verifyFiles)

if __name__ == '__main__':
    main(*argv[1:])
