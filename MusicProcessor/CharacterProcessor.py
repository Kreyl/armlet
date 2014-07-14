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
#from urllib import urlopen
from traceback import format_exc
from sys import argv

from Settings import CHARACTER_ID_START, CHARACTER_IDS

GAME_ID = 584

SHORT_NAME_COLUMN_TITLE = u'Имя на браслете'
LONG_NAME_COLUMN_TITLE = u'Имя и фамилия латиницей'
SEX_COLUMN_TITLE = u'Пол персонажа'

SEXES = {u'мужчина': 'Mr', u'женщина': 'Ms'}

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
    assert characters
    assert len(characters) <= len(CHARACTER_IDS)
    assert tuple(sorted(number for (number, longName) in characters.itervalues())) == tuple(xrange(CHARACTER_ID_START, CHARACTER_ID_START + len(characters))), "Damaged %s file" % CHARACTERS_CSV

def readCharacters(fileName = getFileName(CHARACTERS_CSV)):
    if not isfile(fileName):
        return {}
    data = tuple(tuple(s.strip() for s in d) for d in readCSV(fileName))
    numbers = set()
    shortNames = set()
    longNames = set()
    ret = {}
    for (number, shortName, longName) in readCSV(fileName):
        assert number not in numbers, "Duplicate character ID %s" % number
        assert shortName.lower() not in shortNames, "Duplicate character short name %s" % shortName
        assert longName.lower() not in longNames, "Duplicate character long name %s" % longName
        numbers.add(number)
        shortNames.add(shortName.lower())
        longNames.add(longName.lower())
    ret = dict((shortName, (int(number), longName)) for (number, shortName, longName) in data)
    verifyCharacters(ret)
    return ret

def writeCharacters(characters, fileName = getFileName(CHARACTERS_CSV), header = CHARACTERS_CSV_HEADER):
    with open(fileName, 'wb') as f:
        f.writelines(s + '\r\n' for s in (header % currentTime()).splitlines())
        CSVWriter(f).writerows((number, shortName, longName) for (shortName, (number, longName)) in sorted(characters.iteritems(), key = lambda (shortName, (number, longName)): number))

def loadCharacters():
    #print "Fetching allrpg.info library..."
    #ALLRPG_MODULE_PATH = 'https://raw.githubusercontent.com/aestetique/ALLRPG/master/tools/excel-functions.py'
    #global getAllRoles # pylint: disable=W0602
    #try:
    #    raise Exception('tmp')
    #    exec urlopen(ALLRPG_MODULE_PATH).read() in globals() # Importing getAllRoles function from remote allrpg module # pylint: disable=W0122
    #    assert getAllRoles
    #except Exception, e:
    #    print format_exc()
    #    print "ERROR fetching library: %s, using local version" % e
    if True:
        from allrpg import getAllRoles # Importing getAllRoles function from local allrpg module
        assert getAllRoles
    print "Fetching data from allrpg.info..."
    try:
        # ToDo: Employ sorting by creation date when available
        allRoles = getAllRoles(GAME_ID)
        print "Processing data..."
        header = allRoles[0]
        shortNameColumnID = header.index(SHORT_NAME_COLUMN_TITLE)
        longNameColumnID = header.index(LONG_NAME_COLUMN_TITLE)
        sexColumnID = header.index(SEX_COLUMN_TITLE)
        allRoles = sorted(allRoles[1:])
        ret = []
        shortNames = set()
        longNames = set()
        for (shortName, longName, sex) in ((row[shortNameColumnID], row[longNameColumnID], row[sexColumnID]) for row in allRoles):
            if not shortName:
                continue
            shortName = str(shortName.strip())
            sex = sex.strip()
            assert sex in SEXES, "%s: unknown sex: %s" % (shortName, repr(sex))
            longName = '%s. %s' % (SEXES[sex], str(longName.strip()))
            assert shortName.lower() not in shortNames, "Duplicate character short name %s" % shortName
            assert longName.lower() not in longNames, "Duplicate character long name %s" % longName
            shortNames.add(shortName.lower())
            longNames.add(longName.lower())
            ret.append((shortName, longName))
        return tuple(ret)
    except Exception, e:
        print format_exc()
        print "ERROR fetching data, using current version: %s" % e
        return ()

def updateCharacters():
    print "Processing characters..."
    characters = readCharacters()
    changed = False
    for (shortName, longName) in loadCharacters():
        (rid, oldLongName) = characters.get(shortName, (None, None))
        if not rid: # new character
            characterID = len(characters) + CHARACTER_ID_START
            assert characterID in CHARACTER_IDS
            characters[shortName] = (characterID, longName)
            changed = True
        elif longName != oldLongName: # changed character
            characters[shortName] = (rid, longName)
            changed = True
    verifyCharacters(characters)
    if changed:
        print "Updating %s..." % CHARACTERS_CSV
        writeCharacters(characters)
        print "Updating tickets..."
        from TicketProcessor import generateTickets
        generateTickets(characters.iteritems())
    else:
        print "No changes detected"
    print "Done"
    return characters

def main():
    updateCharacters()

if __name__ == '__main__':
    main()
