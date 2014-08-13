#!/usr/bin/python
#
# Ticket Processor for Ticket to Atlantis LARP.
#
# - Uses CharacterProcessor.py to acquire list of known character names.
# - Generates ticket images for all known character names.
# - Generates ready-for-print A3 sheets with tickets.
# - Stores all those images as PNG files in Tickets subfolder.
#
# Usage: python TicketProcessor.py
#
from os.path import join

try:
    from pygame import init as pyGameInit
    from pygame import Surface
    from pygame import NOFRAME
    from pygame import BLEND_RGB_MULT
    from pygame.display import set_mode
    from pygame.font import SysFont
    from pygame.image import load as loadImage, save as saveImage, get_extended
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyGame v1.9.1 or later: http://pygame.org\n" % (ex.__class__.__name__, ex))

assert get_extended()

TARGET_DIR = '../../Ticket'

TEMPLATE = '../../Ticket/TicketA.png'

EXTENSION = 'png'

TEXT_COLOR = (60, 70, 60) # typewriter

WHITE = (255, 255, 255)

TEXT_VERTICAL_POSITION = 390

MAX_TEXT_WIDTH = 560

FONT_NAME = 'B52'

MAX_FONT_SIZE = 40

NCOLS = 4
NROWS = 4

NUM_NAMED_TICKETS = 2
NUM_TICKETS_TO = 150
NUM_TICKETS_FROM = 100

NUM_SHEETS_TO = NUM_TICKETS_TO // (NCOLS * NROWS)
NUM_SHEETS_FROM = NUM_TICKETS_FROM // (NCOLS * NROWS)

def getFileName(dirName, fileName):
    return join(TARGET_DIR, dirName, '%s.%s' % (fileName, EXTENSION))

def createTicket(template, fileName, printName):
    print '%s: %s' % (fileName, printName)
    ticket = template.copy()
    fontSize = MAX_FONT_SIZE
    while True:
        assert fontSize
        font = SysFont(FONT_NAME, fontSize)
        text = font.render(printName, True, TEXT_COLOR, WHITE)
        (textWidth, textHeight) = text.get_size()
        if textWidth > MAX_TEXT_WIDTH:
            fontSize -= 1
        else:
            break
    x = (ticket.get_width() - textWidth) // 2
    y = TEXT_VERTICAL_POSITION - textHeight
    ticket.blit(text, (x, y), None, BLEND_RGB_MULT)
    #saveImage(ticket, getFileName('Characters', fileName))
    return ticket

def createSheets(tickets, template):
    (ticketWidth, ticketHeight) = template.get_size()
    sheetWidth = ticketWidth * NCOLS
    sheetHeight = ticketHeight * NROWS
    tickets = iter(tickets)
    nSheets = 0
    lastSheet = False
    while not lastSheet:
        nSheets += 1
        sheet = Surface((sheetWidth, sheetHeight)) # pylint: disable=E1121
        sheet.fill(WHITE)
        for y in xrange(0, sheetHeight, ticketHeight):
            for x in xrange(0, sheetWidth, ticketWidth):
                t = next(tickets, template)
                if t is template:
                    if x or y:
                        lastSheet = True
                    else:
                        return nSheets - 1
                sheet.blit(t, (x, y))
        targetName = 'TicketsA%d-%d' % (nSheets, NUM_NAMED_TICKETS)
        print "Sheet %s" % targetName
        saveImage(sheet, getFileName('A3', targetName))
    return nSheets

def createCopiedSheet(fileName, targetName):
    print "Sheet %s" % targetName
    template = loadImage(getFileName('.', fileName))
    (ticketWidth, ticketHeight) = template.get_size()
    sheetWidth = ticketWidth * NCOLS
    sheetHeight = ticketHeight * NROWS
    sheet = Surface((sheetWidth, sheetHeight)) # pylint: disable=E1121
    for y in xrange(0, sheetHeight, ticketHeight):
        for x in xrange(0, sheetWidth, ticketWidth):
            sheet.blit(template, (x, y))
    saveImage(sheet, getFileName('A3', targetName))

def generateTickets(characters):
    pyGameInit()
    set_mode((1, 1), NOFRAME)
    template = loadImage(TEMPLATE)
    names = sorted(characters, key = lambda (shortName, (rid, longName)): rid)
    tickets = (createTicket(template, shortName, longName) for (shortName, (_rid, longName)) in names)
    nNamedSheets = createSheets(tickets, template)
    createCopiedSheet('TicketA', 'TicketsA-%d' % NUM_SHEETS_TO)
    createCopiedSheet('TicketFromA', 'TicketsFromA-%d' % NUM_SHEETS_FROM)
    createCopiedSheet('TicketR', 'TicketsR-%d' % (NUM_SHEETS_TO + NUM_SHEETS_FROM + NUM_NAMED_TICKETS * nNamedSheets))

def main():
    from CharacterProcessor import readCharacters
    generateTickets(readCharacters().iteritems())

if __name__ == '__main__':
    main()
