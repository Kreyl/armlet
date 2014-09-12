#!/usr/bin/python
#
# Verifier for personal customization files for Ticket to Atlantis LARP.
#
from os import listdir
from os.path import join
from sys import argv

from EmotionProcessor import updateEmotions, verifyCharacter
from MusicProcessor import CHARACTER_CSV_PATTERN

def main():
    (emotions, _characters) = updateEmotions()
    path = argv[1]
    for f in listdir(path):
        if CHARACTER_CSV_PATTERN.match(f):
            print "Checking %s" % f
            verifyCharacter(emotions, join(path, f))
        else:
            print "Bad file name: %s" % f

if __name__ == '__main__':
    main()
