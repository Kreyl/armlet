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
from shutil import move
from sys import argv, stdout

try:
    from pydub import AudioSegment
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pydub v0.9.2 or later: https://pypi.python.org/pypi/pydub\n" % (ex.__class__.__name__, ex))

try: # Filesystem symbolic links configuration
    from os import symlink # UNIX # pylint: disable=E0611
    from os.path import islink # UNIX # pylint: disable=E0611
except ImportError:
    global symlink # pylint: disable=W0604
    try:
        import ctypes
        FILE_ATTRIBUTE_REPARSE_POINT = 1024
        INVALID_FILE_ATTRIBUTES = -1
        dll = ctypes.windll.LoadLibrary('kernel32.dll')
        def symlink(source, linkName):
            if not dll.CreateSymbolicLinkW(linkName, source, 0):
                raise OSError("code %d" % dll.GetLastError())
        def islink(path):
            ret = dll.GetFileAttributesW(path)
            if ret == INVALID_FILE_ATTRIBUTES:
                raise OSError("code %d" % dll.GetLastError())
            return bool(ret & FILE_ATTRIBUTE_REPARSE_POINT)
    except Exception, ex:
        symlink = None
        print "%s: %s\nWARNING: Filesystem links will not be available.\nPlease run on UNIX or Windows Vista or later.\n" % (ex.__class__.__name__, ex)

from EmotionProcessor import convert, convertEmotion, convertTitle, updateEmotions, verifyCharacter

MUSIC_LOCATION_VARIABLE = 'ATLANTIS_MUSIC'

SOURCE_DIR = 'src'
ARMLET_DIR = 'armlet'
MUSIC_DIR = 'music'
ERROR_DIR = 'errors'

SD_DIR = 'SD'
EXCLUDE_DIRS = (SD_DIR,)

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

RESULT_MARKS = { True: 'music_errors', False: 'music_ok' }

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
        with open(join(targetDir, RESULT_MARKS[bool(result)]), 'wb'):
            pass

def checkResultMark(targetDir):
    okMark = join(targetDir, RESULT_MARKS[False])
    errorMark = join(targetDir, RESULT_MARKS[True])
    return max(getmtime(okMark) if isfile(okMark) else 0, getmtime(errorMark) if isfile(errorMark) else 0)

def processCharacter(name, number, emotions, baseDir = '.'):
    def error(fullName, s):
        if fullName:
            if not isdir(errorDir):
                makedirs(errorDir)
            move(fullName, errorDir)
        errors.append(s)
    errors = []
    print "\nProcessing character: %s (%d)" % (name, number)
    sdDir = join(unicode(baseDir), SD_DIR)
    baseDir = join(unicode(baseDir), name)
    sourceDir = join(baseDir, SOURCE_DIR)
    errorDir = join(baseDir, ERROR_DIR)
    armletDir = join(baseDir, ARMLET_DIR)
    musicDir = join(armletDir, MUSIC_DIR)
    if not isdir(musicDir):
        makedirs(musicDir)
    musicFiles = tuple(join(musicDir, f) for f in listdir(musicDir))
    if number > 0:
        with open(join(armletDir, INI_FILE), 'wb') as f:
            f.write(INI_ID_LINE % number)
    if symlink:
        for fileName in (f for f in (join(armletDir, f) for f in listdir(armletDir)) if islink(f)):
            remove(fileName)
        for fileName in listdir(sdDir):
            print join(sdDir, fileName), join(armletDir, fileName)
            symlink(join(sdDir, fileName), join(armletDir, fileName))
    characterFile = join(armletDir, CHARACTER_CSV)
    if isfile(characterFile):
        print "Character file found, verifying"
        verifyCharacter(emotions, characterFile)
    okDate = checkResultMark(baseDir)
    if okDate and any(date > okDate for date in (getmtime(f) for f in musicFiles)):
        print "OK mark obsolete, newer source files exist, reprocessing"
        resultMark(baseDir, None)
        okDate = None
    if okDate:
        hasMusic = True
        print "Music OK, %d files found" % len(musicFiles)
    elif not isdir(sourceDir):
        hasMusic = False
        print "No music source directory found: %s" % sourceDir
    else:
        files = listdir(sourceDir)
        hasMusic = bool(files)
        print "Source music files found: %d" % len(files)
        newFileNameSet = set()
        for (trackNumber, fileName) in enumerate(files, 1):
            fullName = join(sourceDir, fileName)
            match = CHECK_PATTERN.match(fileName)
            if not match:
                error(fullName, "Bad file name: " + fileName)
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
                error(fullName, "Unknown emotion: " + fileName)
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
            newFileNameSet.add(newFileName)
            newFullName = join(musicDir, newFileName)
            stdout.write('.') # "%s -> %s" % (fileName, newFileName)
            stdout.flush()
            e = None # processFile(fullName, newFullName, number, name, '%d/%d' % (trackNumber, len(files)), emotion, artist, title, tail)
            if e:
                error(fullName, "Error processing: %s: %s" % (e, fullName))
        print
        obsoleteFiles = tuple(f for f in listdir(unicode(musicDir)) if f not in newFileNameSet)
        if obsoleteFiles:
            error(None, "Obsolete files found (%d), removing" % len(obsoleteFiles))
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
    characterDirs = tuple(sorted(str(d) for d in listdir(unicode(sourceDir)) if d not in EXCLUDE_DIRS and isdir(join(sourceDir, d))))
    if not characterDirs:
        print "No character directories found in music directory: %s" % sourceDir
        return
    (emotions, characters) = updateEmotions()
    okCharacters = []
    unknownCharacters = tuple(sorted(d for d in characterDirs if d not in characters))
    noMusicCharacters = [name for name in characters if name not in characterDirs]
    print
    print "Processing music at %s" % sourceDir
    print "Known characters found: %d%s" % (len(characters), (' (%d have no music dir)' % len(noMusicCharacters)) if noMusicCharacters else '')
    print "Character directories found: %d%s" % (len(characterDirs), (' (%d unknown)' % len(unknownCharacters)) if unknownCharacters else '')
    errorCharacters = []
    for d in characterDirs:
        (hasMusic, hasErrors) = processCharacter(d, characters.get(d, -1), emotions, sourceDir)
        if hasMusic:
            if not hasErrors:
                okCharacters.append(d)
        else:
            noMusicCharacters.append(d)
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

def main(sourceDir = None):
    updateMusic(sourceDir or getenv(MUSIC_LOCATION_VARIABLE, '.'))

if __name__ == '__main__':
    main(*argv[1:])
