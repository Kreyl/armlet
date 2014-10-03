#!/usr/bin/python
#
# Music Linker for Ticket to Atlantis LARP.
#
# Was created and used after game to create player music-by-emotion repository.
#
from os import fdopen, listdir, mkdir
from os.path import isdir, isfile, islink, join
from sys import stdout, argv

stdout = fdopen(stdout.fileno(), 'w', 0)

try: # Filesystem symbolic links configuration
    from os import link as hardlink, symlink # UNIX # pylint: disable=E0611, W0611
except ImportError:
    try:
        from ctypes import windll
        dll = windll.LoadLibrary('kernel32.dll')
        def hardlink(source, linkName):
            if not dll.CreateHardLinkW(linkName, source, None):
                raise OSError("code %d" % dll.GetLastError())
        def symlink(source, linkName):
            if not dll.CreateSymbolicLinkW(linkName, source, 0):
                raise OSError("code %d" % dll.GetLastError())
    except Exception, ex:
        print "%s: %s\nWARNING: Filesystem links are unavailable.\nPlease run on UNIX or Windows Vista or later.\n" % (ex.__class__.__name__, ex)
        raise

from EmotionProcessor import EMOTIONS_CSV, encodeForConsole
from MusicProcessor import deepListDir

def main(*args):
    emotions = dict((s.strip(' ,\r\n').decode('utf-8'), set()) for s in open(EMOTIONS_CSV).readlines() if s.strip())
    (musicDir, targetDir) = args # pylint: disable=W0632
    for character in listdir(musicDir):
        characterDir = join(musicDir, character)
        if not isdir(characterDir):
            continue
        print character,
        for (dirName, fileName) in deepListDir(unicode(characterDir)):
            stdout.write('.')
            fullName = join(dirName, fileName)
            try:
                (emotion, tail) = fileName.split('-', 1)
            except:
                print '\n!', encodeForConsole(fileName)
                continue
            emotion = emotion.strip().capitalize()
            tail = tail.strip()
            if emotion not in emotions:
                print '\n!', encodeForConsole(fileName)
                continue
            emotionDir = join(targetDir, emotion)
            if not isdir(emotionDir):
                mkdir(emotionDir)
            linkName = '-'.join((emotion, character, tail))
            if linkName in emotions[emotion]:
                print '\n!', encodeForConsole(linkName)
                raise RuntimeError()
            emotions[emotion].add(linkName)
            fullLinkName = join(emotionDir, linkName)
            if not isfile(fullLinkName) and not islink(fullLinkName):
                hardlink(fullName, fullLinkName)
        print

if __name__ == '__main__':
    main(*argv[1:])
