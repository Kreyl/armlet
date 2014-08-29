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
from itertools import chain
from os.path import dirname, isfile, join, realpath
#from urllib import urlopen
from traceback import format_exc
from sys import argv

from Settings import CHARACTER_ID_START, CHARACTER_ID_END, CHARACTER_IDS
from EmotionConverter import convertEmotion

GAME_ID = 584

MAIN_COLUMN_TITLES = (u'Имя на браслете', u'Имя и фамилия латиницей')
OTHER_COLUMN_TITLES = (u'Пол персонажа', u'Крутость в драке', u'Готовность убить', u'Длительность готовности убить', u'Наркомания', u'Эмоция', u'Метки')

ADDICTIONS = dict((s, i) for (i, s) in enumerate((u'отсутствует', u'марихуана', u'ЛСД', u'героин', u'убийца', u'Крайк')))

HAS_MUSIC = u'музыкаесть'
MUSIC_OK = u'музыкаок'

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
    assert tuple(sorted(number for (number, _longName, _otherFields) in characters.itervalues())) == tuple(xrange(CHARACTER_ID_START, CHARACTER_ID_START + len(characters))), "Damaged %s file" % CHARACTERS_CSV

def readCharacters(fileName = getFileName(CHARACTERS_CSV)):
    if not isfile(fileName):
        return {}
    data = tuple(tuple(s.strip() for s in d) for d in readCSV(fileName))
    numbers = set()
    shortNames = set()
    longNames = set()
    ret = {}
    for d in data:
        ((number, shortName, longName), (power, kill, killLength, addiction, emotion, hasMusic)) = (d[:3], d[3:])
        # Processing number
        number = int(number)
        assert CHARACTER_ID_START <= number <= CHARACTER_ID_END
        assert number not in numbers, "Duplicate character ID %s" % number
        numbers.add(number)
        # Processing short name
        assert shortName.lower() not in shortNames, "Duplicate character short name %s" % shortName
        shortNames.add(shortName.lower())
        # Processing long name
        assert longName.lower() not in longNames, "Duplicate character long name %s" % longName
        longNames.add(longName.lower())
        # Processing other fields
        power = int(power)
        kill = int(kill)
        killLength = int(killLength)
        addiction = int(addiction)
        assert 0 <= int(addiction) < len(ADDICTIONS)
        hasMusic = int(hasMusic)
        assert hasMusic in (0, 1, 2)
        ret[shortName] = (number, longName, (power, kill, killLength, addiction, emotion, hasMusic))
    verifyCharacters(ret)
    return ret

def writeCharacters(characters, fileName = getFileName(CHARACTERS_CSV), header = CHARACTERS_CSV_HEADER):
    with open(fileName, 'wb') as f:
        f.writelines(s + '\r\n' for s in (header % currentTime()).splitlines())
        CSVWriter(f).writerows((number, shortName, longName) + otherFields for (shortName, (number, longName, otherFields)) in sorted(characters.iteritems(), key = lambda (shortName, (number, longName, otherFields)): number))

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
        (mainColumnIDs, otherColumnIDs) = (tuple(header.index(title) for title in titles) for titles in (MAIN_COLUMN_TITLES, OTHER_COLUMN_TITLES))
        data = (tuple(chain((row[x] for x in mainColumnIDs), (tuple(row[x] for x in otherColumnIDs),))) for row in allRoles[1:])
        data = sorted(data, key = lambda (_shortName, longName, _otherFields): ((longName.split()[-1:] or ('',))[0].lower(), longName.lower()))
        ret = []
        shortNames = set()
        longNames = set()
        for (shortName, longName, (sex, power, kill, killLength, addiction, emotion, tags)) in data:
            # Processing short name
            if not shortName:
                continue
            try:
                shortName = str(shortName.strip())
            except UnicodeError:
                print "UNICODE ERROR:", repr(shortName)
                raise
            assert shortName.lower() not in shortNames, "Duplicate character short name %s" % shortName
            shortNames.add(shortName.lower())
            # Processing long name
            try:
                words = str(longName.strip()).split()
            except UnicodeError:
                print "UNICODE ERROR:", repr(longName)
                raise
            prefix = words[0]
            if prefix.lower() == 'no.':
                longName = ' '.join(words[1:])
            elif prefix.endswith('.') or len(prefix) <= 2:
                longName = ' '.join(words)
            else:
                longName = '%s. %s' % (SEXES[sex], ' '.join(words))
            assert longName.lower() not in longNames, "Duplicate character long name %s" % longName
            longNames.add(longName.lower())
            # Processing other fields
            sex = sex.strip()
            assert sex in SEXES, "%s: unknown sex: %s" % (shortName, repr(sex))
            power = int(power)
            kill = int(kill)
            killLength = int(killLength)
            addiction = ADDICTIONS[addiction]
            emotion = convertEmotion(emotion)
            tags = tags.lower()
            hasMusic = 2 if MUSIC_OK in tags else 1 if HAS_MUSIC in tags else 0
            ret.append((shortName, longName, (power, kill, killLength, addiction, emotion, hasMusic)))
        return tuple(ret)
    except Exception, e:
        print format_exc()
        print "ERROR fetching data, using current version: %s" % e
        return ()

def updateCharacters():
    print "Processing characters..."
    characters = readCharacters()
    changed = False
    for (shortName, longName, otherFields) in loadCharacters():
        (rid, oldLongName, oldOtherFields) = characters.get(shortName, (None, None, None))
        if not rid: # new character
            characterID = len(characters) + CHARACTER_ID_START
            assert characterID in CHARACTER_IDS
            characters[shortName] = (characterID, longName, otherFields)
            changed = True
        elif (longName, otherFields) != (oldLongName, oldOtherFields): # changed character
            characters[shortName] = (rid, longName, otherFields)
            changed = True
    verifyCharacters(characters)
    if changed:
        print "Updating %s..." % CHARACTERS_CSV
        writeCharacters(characters)
        # print "Updating tickets..."
        # from TicketProcessor import generateTickets
        # generateTickets(characters.iteritems())
    else:
        print "No changes detected"
    print "Done"
    return characters

def main():
    updateCharacters()

if __name__ == '__main__':
    main()
