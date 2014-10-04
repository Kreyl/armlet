#!/usr/bin/python
#
# Firefly Control
#
from codecs import open as codecsOpen
from collections import deque
from functools import partial
from getopt import getopt
from logging import getLogger, getLoggerClass, setLoggerClass, FileHandler, Formatter, Handler, INFO, NOTSET
from random import randint
from sys import argv, exit # pylint: disable=W0622
from time import sleep, time
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import Qt, QCoreApplication, QDateTime, QObject, QSettings, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QColorDialog, QDialog, QMainWindow
    from PyQt4.QtGui import QFrame, QHBoxLayout, QLayout, QScrollArea, QStackedWidget, QWidget
    from PyQt4.QtGui import QButtonGroup, QComboBox, QIcon, QLabel, QLineEdit, QPushButton, QRadioButton, QToolButton
    from PyQt4.QtGui import QColor, QIntValidator, QSizePolicy
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextProtocol import Command, COMMAND_MARKER
from UARTTextCommands import ackResponse, ffGetCommand, ffSetCommand, ffResponse
from SerialPort import SerialPort, DT, TIMEOUT

LONG_DATETIME_FORMAT = 'yyyy.MM.dd hh:mm:ss'

MAIN_UI_FILE_NAME = 'FireflyControl.ui'
ABOUT_UI_FILE_NAME = 'AboutFC.ui'

LOG_FILE_NAME = 'FireflyControl.log'

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

#
# ToDo:
# - Set LED color immediately when color is selected in GUI
#

def fixWidgetSize(widget, adjustment = 1):
    widget.setFixedWidth(widget.fontMetrics().boundingRect(widget.text()).width() * adjustment) # This is a bad hack, but there's no better idea

def setTip(widget, tip):
    widget.setToolTip(tip)
    widget.setStatusTip(tip)

class CallableHandler(Handler):
    def __init__(self, emitCallable, level = NOTSET):
        Handler.__init__(self, level)
        self.emitCallable = emitCallable

    def emit(self, record):
        self.emitCallable(self.format(record))

class EventLogger(getLoggerClass(), QObject):
    logSignal = pyqtSignal(tuple, dict)

    def configure(self, parent = None):
        QObject.__init__(self, parent)
        self.logSignal.connect(self.doLog)

    def doLog(self, args, kwargs):
        super(EventLogger, self)._log(*args, **kwargs)

    def _log(self, *args, **kwargs):
        self.logSignal.emit(args, kwargs)

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

class PortLabel(QLabel):
    STATUS_COLORS = {
        SerialPort.TRYING: 'black',
        SerialPort.CONNECTED: 'brown',
        SerialPort.VERIFIED: 'green',
        SerialPort.ERROR: 'red'
    }

    setPortStatus = pyqtSignal(str, int)

    def configure(self):
        self.savedStyleSheet = str(self.styleSheet())
        self.setPortStatus.connect(self.setValue)

    def setValue(self, portName, portStatus):
        self.setText(portName)
        self.setStyleSheet(self.savedStyleSheet + '; color: %s' % self.STATUS_COLORS.get(portStatus, 'gray'))

class CompactButton(QPushButton):
    def configure(self, callback, adjustment = 1.5):
        fixWidgetSize(self, adjustment)
        self.clicked.connect(callback)

class ConsoleEdit(QLineEdit):
    def configure(self, callback):
        self.setStatusTip(self.placeholderText())
        self.returnPressed.connect(callback)

    def getInput(self):
        ret = self.text()
        self.clear()
        return ret

class AboutDialog(QDialog):
    def __init__(self, trigger):
        QDialog.__init__(self)
        uic.loadUi(ABOUT_UI_FILE_NAME, self)
        trigger.connect(self.exec_)

class SelectColorLabel(QLabel):
    def __init__(self, parent, callback = None, color = None):
        QLabel.__init__(self, parent)
        self.setFrameStyle(self.Box | self.Plain)
        setTip(self, "Select color")
        self.callback = callback
        self.setColor(color or QColor('black'))
        self.mousePressEvent = self.editColor

    def setCorrectSize(self, size):
        size -= 2 * self.lineWidth()
        if size > 0:
            self.setFixedSize(size, size)

    def setColor(self, color):
        self.color = color
        self.setStyleSheet('background-color: %s' % color.name())
        if self.callback:
            self.callback(color)

    def editColor(self, _event):
        colorDialog = QColorDialog(self.color)
        if colorDialog.exec_():
            self.setColor(colorDialog.selectedColor())

class TimeEdit(QLineEdit):
    def __init__(self, parent):
        QLineEdit.__init__(self, parent)
        self.setText(str(0))

class DeleteButton(QToolButton):
    def __init__(self, parent, callback):
        QToolButton.__init__(self, parent)
        self.setIcon(QIcon('images/delete.png'))
        setTip(self, "Delete command")
        self.clicked.connect(callback)

    def setCorrectSize(self, size):
        self.setFixedSize(size, size)

class CommandWidget(QWidget):
    commandsLayout = None
    correctHeight = None
    sizeDidntChange = None

    @classmethod
    def configure(cls, parentWidget):
        cls.parentWidget = parentWidget
        cls.commandsLayout = parentWidget.layout()
        cls.headerWidget = cls.commandsLayout.itemAt(0).widget()
        cls.headerLayout = cls.headerWidget.layout()
        cls.buttonGroup = QButtonGroup(parentWidget)
        cls.TAIL_SIZE = 1
        cls.HEADER_SIZE = cls.commandsLayout.count() - cls.TAIL_SIZE

    @classmethod
    def setDeleteButtons(cls):
        deleteButtonHiderIndexToSet = cls.numCommands() < 2
        for command in cls.commands():
            command.deleteButtonHider.setCurrentIndex(deleteButtonHiderIndexToSet)

    @classmethod
    def updateLoop(cls):
        for command in cls.commands():
            command.setLoopIcon()

    @classmethod
    def adjustSizes(cls, size):
        if cls.sizeDidntChange < 2: # This is a hack, but I couldn't find a better way to coordinate technically independent layouts
            cls.correctHeight = size
            sampleLayout = cls.commandsLayout.itemAt(1).widget().layout()
            count = sampleLayout.count()
            assert cls.headerLayout.count() == count
            cls.headerLayout.setContentsMargins(sampleLayout.contentsMargins())
            widthChanged = False
            for (h, w) in (tuple(layout.itemAt(i).widget() for layout in (cls.headerLayout, sampleLayout)) for i in xrange(count)):
                if h.width() != w.width():
                    widthChanged = True
                    h.setFixedWidth(w.width())
            if widthChanged:
                cls.sizeDidntChange = 0
            else:
                cls.sizeDidntChange += 1
            for i in xrange(1, cls.commandsLayout.count() - 1):
                cls.commandsLayout.itemAt(i).widget().setCorrectSize()
            InsertCommandButton.adjustSizes(size)

    def __init__(self, index = None):
        QWidget.__init__(self, self.parentWidget)
        self.setSizePolicy(QSizePolicy.Maximum, QSizePolicy.Maximum)
        self.colorLabel = SelectColorLabel(self, self.setColor)
        self.morphEdit = TimeEdit(self)
        self.delayEdit = TimeEdit(self)
        self.radioButton = QRadioButton(self)
        self.radioButton.clicked.connect(self.updateLoop)
        self.buttonGroup.addButton(self.radioButton)
        self.deleteButton = DeleteButton(self, self.delete)
        self.deleteButtonHider = QStackedWidget(self)
        self.deleteButtonHider.addWidget(self.deleteButton)
        self.deleteButtonHider.addWidget(QWidget(self))
        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 3, 5, 3)
        layout.addWidget(self.colorLabel)
        layout.addWidget(self.morphEdit)
        layout.addWidget(self.delayEdit)
        layout.addWidget(self.radioButton)
        layout.addWidget(self.deleteButtonHider)
        for (h, w) in (tuple(layout.itemAt(i).widget() for layout in (self.headerLayout, layout)) for i in xrange(layout.count())):
            if not w.toolTip() and hasattr(h, 'text'):
                setTip(w, h.text())
        self.setCorrectSize()
        lastIndex = self.commandsLayout.count() - self.TAIL_SIZE
        insertIndex = index + self.HEADER_SIZE if index != None else lastIndex
        if not self.buttonGroup.checkedButton() or insertIndex == lastIndex and not self.hasLoop():
            self.radioButton.setChecked(True)
        self.commandsLayout.insertWidget(insertIndex, self)
        self.updateLoop()
        self.setDeleteButtons()

    @classmethod
    def numCommands(cls):
        return cls.commandsLayout.count() - cls.HEADER_SIZE - cls.TAIL_SIZE

    @classmethod
    def commands(cls): # generator
        for i in xrange(cls.HEADER_SIZE, cls.HEADER_SIZE + cls.numCommands()):
            yield cls.commandsLayout.itemAt(i).widget()

    def isFirst(self):
        return self.commandsLayout.indexOf(self) == self.HEADER_SIZE

    def isLast(self):
        return self.commandsLayout.indexOf(self) == self.commandsLayout.count() - 1 - self.TAIL_SIZE

    def isLoopStart(self):
        return self.isLast()

    def isLoopEnd(self):
        return self.radioButton.isChecked()

    def isInLoop(self):
        return self.commandsLayout.indexOf(self) >= self.commandsLayout.indexOf(self.buttonGroup.checkedButton().parentWidget())

    @classmethod
    def hasLoop(cls):
        return not cls.buttonGroup.checkedButton().parentWidget().isLast()

    def setLoopIcon(self):
        self.radioButton.setIcon(QIcon('images/%s.png' % ( \
                ('noloop' if self.isLoopStart() else 'loopend') if self.isLoopEnd() else \
                'loopstart' if self.isLoopStart() else 'inloop' if self.isInLoop() else 'outofloop')))

    def resizeEvent(self, event):
        self.adjustSizes(self.setCorrectSize())
        QWidget.resizeEvent(self, event)

    def setCorrectSize(self):
        size = max(self.correctHeight, self.morphEdit.height())
        self.colorLabel.setCorrectSize(size)
        self.deleteButton.setCorrectSize(size)
        return size

    def setColor(self, color):
        self.color = color

    def delete(self):
        if self.radioButton.isChecked():
            index = self.commandsLayout.indexOf(self)
            index = index - 1 if self.isLast() else index + 1
            self.commandsLayout.itemAt(index).widget().radioButton.setChecked(True)
        self.buttonGroup.removeButton(self.radioButton)
        self.setParent(None)
        self.updateLoop()
        self.setDeleteButtons()
        InsertCommandButton.deleteExtraButtons(self.numCommands())

class InsertCommandButton(QToolButton):
    insertCommandLayout = None
    correctHeight = None

    @classmethod
    def configure(cls, parentWidget):
        cls.parentWidget = parentWidget
        cls.insertCommandLayout = parentWidget.layout()

    @classmethod
    def deleteExtraButtons(cls, count):
        for _i in xrange(count + 1, cls.insertCommandLayout.count() - 1):
            cls.insertCommandLayout.itemAt(cls.insertCommandLayout.count() - 2).widget().setParent(None)

    @classmethod
    def adjustSizes(cls, size):
        if size > cls.correctHeight:
            cls.correctHeight = size
            cls.insertCommandLayout.setContentsMargins(0, size // 2, 0, 0)
            for i in xrange(cls.insertCommandLayout.count() - 1):
                cls.insertCommandLayout.itemAt(i).widget().setCorrectSize(size)

    def __init__(self):
        QToolButton.__init__(self, self.parentWidget)
        self.setIcon(QIcon('images/add.png'))
        setTip(self, "Insert command")
        self.index = self.insertCommandLayout.count() - 1
        self.clicked.connect(self.addCommand)
        self.insertCommandLayout.insertWidget(self.index, self)
        self.setCorrectSize()

    def setCorrectSize(self, size = None):
        size = size or self.correctHeight
        if size:
            self.setFixedHeight(size)

    def addCommand(self):
        CommandWidget(self.index)
        InsertCommandButton()

class FireflyControl(QMainWindow):
    comConnect = pyqtSignal(str)
    comInput = pyqtSignal(str)

    def __init__(self, args):
        QMainWindow.__init__(self)
        uic.loadUi(MAIN_UI_FILE_NAME, self)
        self.emulated = False
        self.advanced = False
        (options, _parameters) = getopt(args, 'ae', ('advanced', 'emulated'))
        for (option, _value) in options:
            if option in ('-a', '--advanced'):
                self.advanced = True
            elif option in ('-e', '--emulated'):
                self.emulated = True

    def configure(self):
        # Setting window size
        resolution = QDesktopWidget().screenGeometry()
        width = resolution.width()
        height = resolution.height()
        self.setGeometry(width * WINDOW_POSITION, height * WINDOW_POSITION, width * WINDOW_SIZE, height * WINDOW_SIZE)
        # Configuring widgets
        self.portLabel.configure()
        self.resetButton.configure(self.reset)
        self.consoleEdit.configure(self.consoleEnter)
        self.aboutDialog = AboutDialog(self.aboutAction.triggered)
        CommandWidget.configure(self.commandsWidget)
        InsertCommandButton.configure(self.insertCommandWidget)
        InsertCommandButton().addCommand()
        # Setup logging
        formatter = Formatter('%(asctime)s %(levelname)s\t%(message)s', '%Y-%m-%d %H:%M:%S')
        handlers = (FileHandler(LOG_FILE_NAME), CallableHandler(self.logTextEdit.appendPlainText))
        rootLogger = getLogger('')
        for handler in handlers:
            handler.setFormatter(formatter)
            rootLogger.addHandler(handler)
        rootLogger.setLevel(INFO)
        setLoggerClass(EventLogger)
        self.logger = getLogger('FireflyControl')
        self.logger.configure(self) # pylint: disable=E1103
        self.logger.info("start")
        # Starting up!
        self.loadSettings()
        self.comConnect.connect(self.processConnect)
        self.comInput.connect(self.processInput)
        self.port = SerialPort(self.logger, ffGetCommand.prefix, ffResponse.prefix,
                               self.comConnect.emit, self.comInput.emit, self.portLabel.setPortStatus.emit,
                               None, (115200,))
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    def processConnect(self, pong): # pylint: disable=W0613
        self.logger.info("connected device detected")

    def processCommand(self, source, _checked = False):
        error = True
        data = self.port.command(source.command, COMMAND_MARKER, QApplication.processEvents)
        if data:
            data = str(data).strip()
            (tag, args) = Command.decodeCommand(data)
            if tag == ackResponse.tag:
                error = args[0]
                if error:
                    self.lastCommandButton = None
                    self.logger.warning("Setup ERROR: %d", error)
                else:
                    self.lastCommandButton = source
                    self.logger.info("Setup OK")
            elif args is not None: # unexpected valid command
                self.logger.warning("Unexpected command: %s %s", tag, ' '.join(str(arg) for arg in args))
            elif tag: # unknown command
                self.logger.warning("Unknown command %s: %s", tag, data)
            else: # not a command
                self.logger.warning("Unexpected input: %s", data)
        else:
            self.logger.warning("command timed out")

    def processInput(self, data):
        error = True
        data = unicode(data).strip()
        (tag, args) = Command.decodeCommand(data)
        if tag == ackResponse.tag:
            error = args[0]
            if error:
                self.logger.warning("ERROR: %d", error)
            else:
                self.logger.info("OK")
        elif args is not None: # unexpected valid command
            self.logger.warning("Unexpected command: %s %s", tag, ' '.join(str(arg) for arg in args))
        elif tag: # unknown command
            self.logger.warning("Unknown command %s: %s", tag, data.rstrip())
        else: # not a command
            self.logger.warning("Unexpected input: %s", data.rstrip())

    def consoleEnter(self):
        data = self.consoleEdit.getInput()
        if data:
            self.lastCommandButton = None
            self.highlightCommandButton()
            self.port.write(data)

    def closeEvent(self, _event):
        self.saveSettings()
        self.logger.info("close")

    def reset(self):
        self.logger.info("reset")
        self.port.reset()
        self.lastCommandButton = None
        self.highlightCommandButton()

    def saveSettings(self):
        settings = QSettings()
        try:
            settings.setValue('timeStamp', QDateTime.currentDateTime().toString(LONG_DATETIME_FORMAT))
            settings.beginGroup('window')
            settings.setValue('width', self.size().width())
            settings.setValue('height', self.size().height())
            settings.setValue('x', max(0, self.pos().x()))
            settings.setValue('y', max(0, self.pos().y()))
            settings.setValue('maximized', self.isMaximized())
            settings.setValue('state', self.saveState())
            settings.endGroup()
        except:
            self.logger.exception("Error saving settings")
            settings.clear()
        settings.sync()

    def loadSettings(self):
        QCoreApplication.setOrganizationName('Ostranna')
        QCoreApplication.setOrganizationDomain('ostranna.ru')
        QCoreApplication.setApplicationName('Firefly')
        settings = QSettings()
        self.logger.info(settings.fileName())
        self.savedMaximized = False
        try:
            timeStamp = settings.value('timeStamp').toString()
            if timeStamp:
                settings.beginGroup('window')
                self.resize(settings.value('width').toInt()[0], settings.value('height').toInt()[0])
                self.move(settings.value('x').toInt()[0], settings.value('y').toInt()[0])
                self.savedMaximized = settings.value('maximized', False).toBool()
                self.restoreState(settings.value('state').toByteArray())
                settings.endGroup()
                self.logger.info("Loaded settings dated %s", timeStamp)
            else:
                self.logger.info("No settings found")
        except:
            self.logger.exception("Error loading settings")

    def error(self, message):
        print "ERROR:", message
        self.logger.error(message)
        exit(-1)

def main(args):
    try:
        application = QApplication(args)
        fireflyControl = FireflyControl(args[1:]) # retain reference
        fireflyControl.configure()
        return application.exec_()
    except KeyboardInterrupt:
        pass
    except SystemExit:
        raise
    except BaseException:
        print format_exc()
        return -1

if __name__ == '__main__':
    main(argv)
