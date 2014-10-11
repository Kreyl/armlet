#!/usr/bin/python
#
# Firefly Control widget definitions
#
from functools import partial
from itertools import chain, islice

try:
    from PyQt4.QtCore import Qt, QSize
    from PyQt4.QtGui import QColorDialog, QHBoxLayout, QScrollArea, QStackedWidget, QWidget
    from PyQt4.QtGui import QButtonGroup, QLabel, QLineEdit, QRadioButton, QToolButton
    from PyQt4.QtGui import QColor, QIcon, QIntValidator, QSizePolicy, qRgb
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextCommands import UART_FF_RGB, UART_FF_WAIT, UART_FF_GOTO, UART_FF_LIGHT

MAX_INT = 2 ** 31 - 1

MIN_COLOR = 0
MAX_COLOR = 255

def fixWidgetSize(widget, adjustment = 1):
    widget.setFixedWidth(widget.fontMetrics().boundingRect(widget.text()).width() * adjustment) # This is a bad hack, but there's no better idea

def setTip(widget, tip):
    widget.setToolTip(tip)
    widget.setStatusTip(tip)

def widgets(layout, headerSize = 0, tailSize = 0): # generator
    for i in xrange(headerSize, layout.count() - tailSize):
        yield layout.itemAt(i).widget()

class DeleteButton(QToolButton):
    def __init__(self, parent, callback):
        QToolButton.__init__(self, parent)
        self.setIcon(QIcon('images/delete.png'))
        setTip(self, "Delete command")
        self.clicked.connect(callback)

    def setCorrectSize(self, size):
        self.setFixedSize(size, size)

class ConsoleEdit(QLineEdit):
    def configure(self, callback):
        self.setStatusTip(self.placeholderText())
        self.returnPressed.connect(callback)

    def getInput(self):
        ret = self.text()
        self.clear()
        return ret

class VerticalScrollArea(QScrollArea):
    def __init__(self, parent = None):
        QScrollArea.__init__(self, parent)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.scrollBarSet = False

    def resizeEvent(self, event):
        if not self.scrollBarSet:
            self.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
            self.scrollBarSet = True
        self.setMinimumWidth(self.widget().sizeHint().width() + self.verticalScrollBar().width())
        QScrollArea.resizeEvent(self, event)

class TimeEdit(QLineEdit):
    MAX_VALUE = MAX_INT
    MAX_LENGTH = len(str(MAX_VALUE))

    def __init__(self, parent, callback = None, initialValue = None):
        QLineEdit.__init__(self, parent)
        self.setAlignment(Qt.AlignRight)
        self.setMaxLength(self.MAX_LENGTH)
        self.setValidator(QIntValidator(0, self.MAX_VALUE))
        self.setText(str(self.MAX_VALUE))
        fixWidgetSize(self, 1.4)
        self.setText(str(initialValue or 0))
        if callback:
            self.textEdited.connect(callback)

class SelectColorLabel(QLabel):
    STANDARS_COLORS = ((255, 0, 0), (0, 255, 0), (0, 0, 255),
                       (255, 255, 0), (255, 0, 255), (0, 255, 255),
                       (255, 64, 64), (64, 255, 64), (64, 64, 255),
                       (255, 255, 64), (255, 64, 255), (64, 255, 255),
                       (255, 128, 128), (128, 255, 128), (128, 128, 255),
                       (255, 255, 128), (255, 128, 255), (128, 255, 255),
                       (255, 192, 192), (192, 255, 192), (192, 192, 255),
                       (255, 255, 192), (255, 192, 255), (192, 255, 255),
                       (255, 0, 128), (128, 255, 0), (0, 128, 255),
                       (255, 128, 0), (128, 0, 255), (0, 255, 128),
                       (128, 0, 0), (0, 128, 0), (0, 0, 128),
                       (128, 128, 0), (128, 0, 128), (0, 128, 128),
                       (64, 0, 0), (0, 64, 0), (0, 0, 64),
                       (64, 64, 0), (64, 0, 64), (0, 64, 64),
                       (128, 64, 64), (64, 128, 64), (64, 64, 128),
                       (255, 255, 255), (128, 128, 128), (0, 0, 0))

    def __init__(self, parent, callback = None, color = None):
        QLabel.__init__(self, parent)
        self.setFrameStyle(self.Box | self.Plain)
        setTip(self, "Select color")
        self.callback = callback
        self.setColor(color or QColor(Qt.white))
        self.mousePressEvent = self.editColor

    @classmethod
    def configure(cls, parent):
        cls.colorDialog = QColorDialog(parent)
        cls.setStandardColors(qRgb(*rgb) for rgb in cls.STANDARS_COLORS)

    @classmethod
    def getCustomColors(cls):
        return tuple(cls.colorDialog.customColor(i) for i in xrange(cls.colorDialog.customCount()))

    @classmethod
    def setCustomColors(cls, colors):
        for (i, color) in enumerate(colors):
            cls.colorDialog.setCustomColor(i, color)

    @classmethod
    def setStandardColors(cls, colors):
        for (i, color) in enumerate(colors):
            cls.colorDialog.setStandardColor(i, color)

    def setCorrectSize(self, size, adjustForBorder = False):
        if adjustForBorder:
            size -= 2 * self.lineWidth()
        if size > 0:
            self.setFixedSize(size, size)

    def setColor(self, color):
        self.color = color
        self.setStyleSheet('background-color: %s' % color.name())
        if self.callback:
            self.callback(color)

    def editColor(self, _event):
        try:
            self.colorDialog.currentColorChanged.disconnect()
        except TypeError:
            pass
        self.colorDialog.setCurrentColor(self.color)
        self.colorDialog.currentColorChanged.connect(self.setColor)
        previousColor = self.color
        if not self.colorDialog.exec_():
            self.setColor(previousColor)

    def getCommand(self):
        return ','.join((UART_FF_RGB, str(self.color.red()), str(self.color.green()), str(self.color.blue())))

class InsertCommandButton(QToolButton):
    insertCommandLayout = None
    correctHeight = None

    @classmethod
    def configure(cls, parentWidget):
        cls.parentWidget = parentWidget
        cls.insertCommandLayout = parentWidget.layout()
        cls.TAIL_SIZE = 1
        cls.HEADER_SIZE = cls.insertCommandLayout.count() - cls.TAIL_SIZE

    @classmethod
    def deleteExtraButtons(cls, count):
        for button in widgets(cls.insertCommandLayout, cls.HEADER_SIZE + count + 1, cls.TAIL_SIZE):
            button.setParent(None)

    @classmethod
    def adjustSizes(cls, lineSize, headerSize):
        if lineSize > cls.correctHeight:
            cls.correctHeight = lineSize
            cls.insertCommandLayout.setContentsMargins(0, headerSize - lineSize // 2, 0, 0)
            for button in widgets(cls.insertCommandLayout, cls.HEADER_SIZE, cls.TAIL_SIZE):
                button.setCorrectSize(lineSize)

    def __init__(self):
        QToolButton.__init__(self, self.parentWidget)
        self.setIcon(QIcon('images/add.png'))
        setTip(self, "Insert command")
        self.index = self.insertCommandLayout.count() - self.TAIL_SIZE
        self.clicked.connect(self.addCommand)
        self.insertCommandLayout.insertWidget(self.index, self)
        self.setCorrectSize()

    def setCorrectSize(self, size = None):
        size = size or self.correctHeight
        if size:
            self.setFixedHeight(size)

    def addCommand(self):
        CommandWidget(index = self.index)
        InsertCommandButton()
        CommandWidget.updateLoop()
        CommandWidget.updateDeleteButtons()

class CommandWidget(QWidget):
    BASIC_STYLESHEET = 'border: 1px solid'
    GRADIENT_TEMPLATE = 'background: qlineargradient(x1:0, x2:1, %s)'
    GRADIENT_STOP = 'stop: %f rgb(%d, %d, %d)'

    commandsLayout = None
    correctHeight = None
    sizeDidntChange = None

    @classmethod
    def configure(cls, stackedWidget, parentWidget, updateProgramCallback):
        cls.stackedWidget = stackedWidget
        cls.parentWidget = parentWidget
        cls.updateProgramCallback = updateProgramCallback
        cls.commandsLayout = parentWidget.layout()
        cls.headerWidget = widgets(cls.commandsLayout).next()
        cls.headerLayout = cls.headerWidget.layout()
        cls.buttonGroup = QButtonGroup(parentWidget)
        cls.TAIL_SIZE = 1
        cls.HEADER_SIZE = cls.commandsLayout.count() - cls.TAIL_SIZE

    @classmethod
    def updateDeleteButtons(cls):
        hiderIndexToSet = cls.numCommands() < 2
        for command in cls.commands():
            command.hider.setCurrentIndex(hiderIndexToSet)

    @classmethod
    def updateLoop(cls, programChanged = True):
        for command in cls.commands():
            command.setLoopIcon()
        cls.updateProgram(programChanged)

    @classmethod
    def updateProgram(cls, programChanged = True):
        commands = []
        programLength = 1.0
        for (rgb, morphLength, delayLength, _command) in (c.command() for c in chain(cls.commands(), cls.loopCommands())):
            commands.append((rgb, morphLength or 1, delayLength or 1))
            programLength += (morphLength or 1) + (delayLength or 1)
        dx = programLength / 1000
        for (rgb, morphLength, delayLength) in commands:
            programLength += max(0, dx - morphLength) + max(0, dx - delayLength)
        stops = [(0, 0, 0, 0),]
        position = 0.0
        for (rgb, morphLength, delayLength) in commands:
            position += max(morphLength, dx)
            stops.append((position / programLength,) + rgb)
            position += max(delayLength, dx)
            stops.append((position / programLength,) + rgb)
        cls.updateProgramCallback(','.join(c.command()[-1] for c in cls.commands()),
                '; '.join((cls.BASIC_STYLESHEET, cls.GRADIENT_TEMPLATE % ' '.join(cls.GRADIENT_STOP % stop for stop in stops))),
                programChanged)

    @classmethod
    def setupProgram(cls, source = None):
        def parseInt(words, index, minValue = 0, maxValue = MAX_INT):
            try:
                ret = float(words[index])
                if ret % 1:
                    raise ValueError()
                ret = int(ret)
                if not minValue <= ret <= maxValue:
                    raise ValueError()
            except ValueError:
                raise ValueError("Bad program data at word %d: expected a %d..%d integer" % (index, minValue, maxValue))
            return ret

        def parseColor(words, index):
            return parseInt(words, index, MIN_COLOR, MAX_COLOR)

        words = []
        if source is not None:
            for line in source:
                words.extend(w.lower() for w in (w.strip() for w in line.strip().split('#')[0].split(',')) if w)
            if not words:
                raise ValueError("No commands found in the file")
        commands = []
        index = 0
        numCommands = 0
        gotoPosition = None
        conditionsPresent = False
        positionsToVerify = []
        while index < len(words):
            if words[index] == UART_FF_RGB:
                if len(words) < index + 5:
                    raise ValueError("Abrupt program end")
                index += 1
                r = parseColor(words, index)
                index += 1
                g = parseColor(words, index)
                index += 1
                b = parseColor(words, index)
                index += 1
                morphLength = parseInt(words, index)
                commands.append([r, g, b, morphLength, 0, numCommands])
            elif words[index] == UART_FF_WAIT:
                if len(words) < index + 2:
                    raise ValueError("Abrupt program end")
                if not commands:
                    commands.append([0, 0, 0, 0, 0, 0])
                index += 1
                commands[-1][4] += parseInt(words, index)
            elif words[index] == UART_FF_GOTO:
                if len(words) < index + 2:
                    raise ValueError("Abrupt program end")
                if conditionsPresent:
                    index += 1
                    positionsToVerify.append(index)
                else:
                    if len(words) > index + 2:
                        raise ValueError("Unexpected program data at word %d after GOTO command" % (index + 2))
                    if not commands:
                        raise ValueError("Unexpected GOTO command at the start of the program")
                    index += 1
                    gotoPosition = parseInt(words, index, 0, numCommands - 1)
            elif words[index] == UART_FF_LIGHT:
                if len(words) < index + 3:
                    raise ValueError("Abrupt program end")
                index += 1
                _light = parseInt(words, index)
                index += 1
                positionsToVerify.append(index)
                conditionsPresent = True
            numCommands += 1
            index += 1
        for index in positionsToVerify:
            parseInt(words, index, 0, numCommands - 1) # verify jump targets
        if not conditionsPresent and gotoPosition:
            for (index, command) in enumerate(commands):
                if command[-1] == gotoPosition:
                    command[-1] = True
                    break
                if command[-1] > gotoPosition:
                    raise ValueError("GOTO command targeted to a non-color command")
        cls.stackedWidget.setCurrentIndex(conditionsPresent)
        if conditionsPresent:
            cls.updateProgramCallback(','.join(words), cls.BASIC_STYLESHEET, False)
        else:
            for command in tuple(cls.commands()):
                command.delete(False)
            if commands:
                gotoCommand = None
                for (r, g, b, morphLength, delayLength, gotoHere) in commands:
                    commandWidget = CommandWidget(QColor(r, g, b), morphLength, delayLength)
                    if gotoHere is True:
                        gotoCommand = commandWidget
                    InsertCommandButton()
                if gotoCommand:
                    gotoCommand.radioButton.setChecked(True)
            else:
                CommandWidget()
                InsertCommandButton()
            cls.updateLoop(False)
            cls.updateDeleteButtons()

    @classmethod
    def adjustSizes(cls, size):
        if cls.sizeDidntChange < 2: # This is a hack, but I couldn't find a better way to coordinate technically independent layouts
            cls.correctHeight = size
            sampleLayout = cls.commands().next().layout()
            cls.headerLayout.setContentsMargins(sampleLayout.contentsMargins())
            widthChanged = False
            for (h, w) in zip(widgets(cls.headerLayout), widgets(sampleLayout)):
                if h.width() != w.width():
                    widthChanged = True
                    h.setFixedWidth(w.width())
            if widthChanged:
                cls.sizeDidntChange = 0
            else:
                cls.sizeDidntChange += 1
        for command in cls.commands():
            command.setCorrectSize()
        InsertCommandButton.adjustSizes(size, cls.headerWidget.height() + cls.commandsLayout.spacing())

    def __init__(self, color = None, morphLength = None, delayLength = None, index = None):
        QWidget.__init__(self, self.parentWidget)
        self.setSizePolicy(QSizePolicy.Maximum, QSizePolicy.Maximum)
        self.colorLabel = SelectColorLabel(self, self.setColor, color)
        self.morphEdit = TimeEdit(self, self.updateProgram, morphLength)
        self.delayEdit = TimeEdit(self, self.updateProgram, delayLength)
        self.radioButton = QRadioButton(self)
        self.radioButton.clicked.connect(partial(self.updateLoop, True))
        self.radioButton.commandWidget = self
        self.buttonGroup.addButton(self.radioButton)
        self.deleteButton = DeleteButton(self, partial(self.delete, True))
        self.hider = QStackedWidget(self)
        self.hiderWidget = QWidget(self)
        hiderLayout = QHBoxLayout(self.hiderWidget)
        hiderLayout.setContentsMargins(0, 0, 0, 0)
        hiderLayout.addWidget(self.radioButton)
        hiderLayout.addWidget(self.deleteButton)
        self.hider.addWidget(self.hiderWidget)
        self.hider.addWidget(QWidget(self))
        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.colorLabel)
        layout.addWidget(self.morphEdit)
        layout.addWidget(self.delayEdit)
        layout.addWidget(self.hider)
        for (h, w) in zip(widgets(self.headerLayout), widgets(layout)):
            if not w.toolTip() and hasattr(h, 'text'):
                setTip(w, h.text())
        self.setCorrectSize()
        lastIndex = self.commandsLayout.count() - self.TAIL_SIZE
        insertIndex = index + self.HEADER_SIZE if index != None else lastIndex
        if not self.buttonGroup.checkedButton() or insertIndex == lastIndex and not self.hasLoop():
            self.radioButton.setChecked(True)
        self.commandsLayout.insertWidget(insertIndex, self)

    @classmethod
    def numCommands(cls):
        return cls.commandsLayout.count() - cls.HEADER_SIZE - cls.TAIL_SIZE

    @classmethod
    def commands(cls): # generator
        for w in widgets(cls.commandsLayout, cls.HEADER_SIZE, cls.TAIL_SIZE):
            yield w

    @classmethod
    def loopCommands(cls): # generator
        if not cls.hasLoop():
            return
        for w in widgets(cls.commandsLayout, cls.loopEndIndex(), cls.TAIL_SIZE):
            yield w

    @classmethod
    def loopEndIndex(cls):
        return cls.commandsLayout.indexOf(cls.buttonGroup.checkedButton().commandWidget)

    def isFirst(self):
        return self.commandsLayout.indexOf(self) == self.HEADER_SIZE

    def isLast(self):
        return self.commandsLayout.indexOf(self) == self.commandsLayout.count() - 1 - self.TAIL_SIZE

    def isLoopStart(self):
        return self.isLast()

    def isLoopEnd(self):
        return self.radioButton.isChecked()

    def isInLoop(self):
        return self.commandsLayout.indexOf(self) >= self.loopEndIndex()

    @classmethod
    def hasLoop(cls):
        return cls.buttonGroup.checkedButton() and not cls.buttonGroup.checkedButton().commandWidget.isLast()

    def setLoopIcon(self):
        self.radioButton.setIcon(QIcon('images/%s.png' % ( \
                ('noloop' if self.isLoopStart() else 'loopend') if self.isLoopEnd() else \
                'loopstart' if self.isLoopStart() else 'inloop' if self.isInLoop() else 'outofloop')))

    def resizeEvent(self, event):
        self.adjustSizes(self.setCorrectSize())
        QWidget.resizeEvent(self, event)

    def setCorrectSize(self):
        size = max(self.correctHeight, self.morphEdit.height())
        self.colorLabel.setCorrectSize(size, True)
        self.deleteButton.setCorrectSize(size)
        self.radioButton.setFixedWidth(size + self.radioButton.icon().actualSize(QSize(100, 100)).width())
        return size

    def setColor(self, color):
        self.color = color
        self.updateProgram()

    def delete(self, programChanged = True):
        if self.isLoopEnd():
            index = self.commandsLayout.indexOf(self)
            index = index - 1 if self.isLast() else index + 1
            if index >= self.HEADER_SIZE:
                self.commandsLayout.itemAt(index).widget().radioButton.setChecked(True)
        self.buttonGroup.removeButton(self.radioButton)
        self.setParent(None)
        self.updateLoop(programChanged)
        self.updateDeleteButtons()
        InsertCommandButton.deleteExtraButtons(self.numCommands())

    def command(self):
        fields = [UART_FF_RGB, self.color.red(), self.color.green(), self.color.blue(), self.morphEdit.text()]
        if int(self.delayEdit.text()):
            fields.extend((UART_FF_WAIT, self.delayEdit.text()))
        if self.isLoopStart() and not self.isLoopEnd():
            targetIndex = 0
            for command in islice(self.commands(), 0, self.loopEndIndex() - self.HEADER_SIZE):
                targetIndex += 1 + bool(int(command.delayEdit.text()))
            fields.extend((UART_FF_GOTO, targetIndex))
        return ((self.color.red(), self.color.green(), self.color.blue()),
                int(self.morphEdit.text()), int(self.delayEdit.text()), ','.join(str(f) for f in fields))
