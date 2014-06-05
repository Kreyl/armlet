#!/usr/bin/python
#
# Character Processor for Ticket to Atlantis LARP.
#
# Usage:
# -
#
from csv import reader as CSVReader, writer as CSVWriter
from os.path import isfile
from urllib import urlopen

# Import allrpg access module
print "Accessing allrpg.info library..."
ALLRPG_MODULE_PATH = 'https://raw.githubusercontent.com/aestetique/ALLRPG/master/tools/excel-functions.py'
exec urlopen(ALLRPG_MODULE_PATH).read() in globals() # Importing getAllRoles function # pylint: disable=W0122

GAME_ID = 584

NAME_COLUMN = 29

CHARACTER_ID_START = 101

CHARACTERS_CSV = 'Characters.csv'

CHARACTERS_CSV_HEADER = '''\
#
# Characters.csv
#
# Character name -> RID table for ArmLet initialization.
#
# Generated, updated and used by CharacterProcessor.py to track persistent and unique character ArmLet IDs.
#
# !!! DO NOT EDIT !!!
#
'''

def readCSV(csv): # generator
    with open(csv) as f:
        for row in CSVReader(f):
            assert row, "bad CSV file format"
            if not row[0].startswith('#'):
                yield row

def verifyCharacters(characters):
    assert tuple(sorted(characters.itervalues())) == tuple(xrange(CHARACTER_ID_START, CHARACTER_ID_START + len(characters))), "Damaged %s file" % CHARACTERS_CSV

def readCharacters(fileName = CHARACTERS_CSV):
    if not isfile(fileName):
        return {}
    ret = dict((name, int(number)) for (number, name) in readCSV(fileName))
    verifyCharacters(ret)
    return ret

def writeCharacters(characters, fileName = CHARACTERS_CSV, header = CHARACTERS_CSV_HEADER):
    with open(fileName, 'wb') as f:
        f.writelines(s + '\r\n' for s in header.splitlines())
        CSVWriter(f).writerows((number, name) for (name, number) in sorted(characters.iteritems(), key = lambda (name, number): number))

def loadCharacters():
    print "Fetching data from allrpg.info..."
    allRoles = getAllRoles(GAME_ID) # pylint: disable=E0602
    print "Done"
    # ToDo: Employ some sorting? By creation date?
    return tuple(str(name) for name in (allRoles[row][NAME_COLUMN] for row in xrange(1, len(allRoles))) if name)

def updateCharacters():
    characters = readCharacters()
    for name in loadCharacters():
        if name not in characters:
            characters[name] = len(characters) + CHARACTER_ID_START
    verifyCharacters(characters)
    writeCharacters(characters)

def main():
    updateCharacters()

if __name__ == '__main__':
    main()
