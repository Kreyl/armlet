#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Character Processor for Ticket to Atlantis LARP.
#
# - Downloads allrpg.py library from allrpg.info source code repository
# - Uses to download Excel table with all roles for a game
# - Extracts character names from Excel table
# - Loads, verifies and updates Characters.csv file
#
# Usage: python CharacterProcessor.py
#
from csv import reader as CSVReader, writer as CSVWriter
from datetime import datetime
from os.path import dirname, isfile, join, realpath
from urllib import urlopen
from traceback import format_exc
from sys import argv

GAME_ID = 584

NAME_COLUMN_TITLE = u'Имя на браслете'

CHARACTER_ID_START = 101

CHARACTERS_CSV = 'Characters.csv'

CHARACTERS_CSV_HEADER = '''\
#
# Characters.csv
#
# Character name -> RID table for ArmLet initialization.
#
# Generated, updated and used by CharacterProcessor.py
# to track persistent unique character ArmLet IDs.
#
# !!! DO NOT EDIT !!!
#
# Generated at %s
#
'''

def currentTime():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S')

def getFileName(name):
    return join(dirname(realpath(argv[0])), name)

def readCSV(csv): # generator
    with open(csv) as f:
        for row in CSVReader(f):
            assert row, "bad CSV file format"
            if not row[0].startswith('#'):
                yield row

def verifyCharacters(characters):
    assert tuple(sorted(characters.itervalues())) == tuple(xrange(CHARACTER_ID_START, CHARACTER_ID_START + len(characters))), "Damaged %s file" % CHARACTERS_CSV

def readCharacters(fileName = getFileName(CHARACTERS_CSV)):
    if not isfile(fileName):
        return {}
    ret = dict((name, int(number)) for (number, name) in readCSV(fileName))
    verifyCharacters(ret)
    return ret

def writeCharacters(characters, fileName = getFileName(CHARACTERS_CSV), header = CHARACTERS_CSV_HEADER):
    with open(fileName, 'wb') as f:
        f.writelines(s + '\r\n' for s in (header % currentTime()).splitlines())
        CSVWriter(f).writerows((number, name) for (name, number) in sorted(characters.iteritems(), key = lambda (name, number): number))

def loadCharacters():
    print "Fetching allrpg.info library..."
    ALLRPG_MODULE_PATH = 'https://raw.githubusercontent.com/aestetique/ALLRPG/master/tools/excel-functions.py'
    global getAllRoles # pylint: disable=W0602
    try:
        exec urlopen(ALLRPG_MODULE_PATH).read() in globals() # Importing getAllRoles function from remote allrpg module # pylint: disable=W0122
        assert getAllRoles
    except Exception, e:
        print format_exc()
        print "ERROR fetching library: %s, using local version" % e
        from allrpg import getAllRoles # Importing getAllRoles function from local allrpg module
        assert getAllRoles
    print "Fetching data from allrpg.info..."
    try:
        # ToDo: Employ sorting by creation date when available
        allRoles = getAllRoles(GAME_ID)
        print "Processing data..."
        nameColumnID = allRoles[0].index(NAME_COLUMN_TITLE)
        allRoles = sorted(allRoles[1:])
        ret = tuple(str(name) for name in (row[nameColumnID] for row in allRoles) if name)
        print "Done"
        return ret
    except Exception, e:
        print format_exc()
        print "ERROR fetching data, using current version: %s" % e
        return ()

def updateCharacters():
    print "Processing characters..."
    characters = readCharacters()
    for name in loadCharacters():
        if name not in characters:
            characters[name] = len(characters) + CHARACTER_ID_START
    verifyCharacters(characters)
    writeCharacters(characters)
    return characters

def main():
    updateCharacters()

if __name__ == '__main__':
    main()
