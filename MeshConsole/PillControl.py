#!/usr/bin/python
#
# Pill Control
#
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
    from PyQt4.QtCore import QCoreApplication, QDateTime, QObject, QSettings, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QComboBox, QDialog, QIntValidator, QLabel, QLineEdit, QMainWindow, QPushButton, QWidget
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextProtocol import Command, COMMAND_MARKER
from UARTTextCommands import pingCommand, ackResponse, pillWrite32Command, pillRead32Command, pillData32Response
from SerialPort import SerialPort, DT, TIMEOUT

# ToDo
# Change way of highlighting buttons: http://qt-project.org/doc/qt-4.8/stylesheet-examples.html#customizing-qpushbutton
# Provide some padding to command buttons
# Migrate advancements back to MeshConsole

LONG_DATETIME_FORMAT = 'yyyy.MM.dd hh:mm:ss'

MAIN_UI_FILE_NAME = 'PillControl.ui'
ABOUT_UI_FILE_NAME = 'AboutPC.ui'

PILL_COMMANDS_FILE_NAME = 'PillCommands.txt'

LOG_FILE_NAME = 'PillControl.log'

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

STATE_SETUP_OK = 0
STATE_SETUP_ERROR = 1
STATE_PILL_OK = 2
STATE_PILL_ERROR = 3

DEVICE_TYPES = (
    ('player', 1),
    ('radClear', 10),
    ('radLight', 11),
    ('radHeavy', 12),
    ('radDeath', 13),
    ('mobDetector', 21),
    ('statDetector', 22),
    ('empMech', 31),
    ('empGrenade', 32),
    ('finder', 41),
    ('pillControl', 51)
)

DEVICE_INDEXES = dict((i, v) for (i, (k, v)) in enumerate(DEVICE_TYPES))

def fixWidgetSize(widget, adjustment = 1):
    widget.setFixedWidth(widget.fontMetrics().boundingRect(widget.text()).width() * adjustment) # This is a bad hack, but there's no better idea

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

class EmulatedSerial(object):
    def __init__(self):
        self.name = 'EMUL'
        self.timeout = TIMEOUT
        self.buffer = deque()
        self.ready = False
        self.nextPill = None

    def readline(self):
        while True:
            if self.buffer:
                data = self.buffer.popleft()
                break
            now = time()
            if self.ready and now > self.nextPill:
                self.nextPill = now + float(randint(3000, 10000)) / 1000
                data = ackResponse.encode(max(0, randint(0, 13) - 10))
                break
            sleep(DT)
        return data

    def write(self, data):
        (tag, _args) = Command.decodeCommand(data)
        if tag:
            self.buffer.append(ackResponse.encode(max(0, randint(0, 13) - 10)))
        self.ready = True
        return len(data)

    def close(self):
        pass

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
    def configure(self, callback):
        fixWidgetSize(self, 1.5)
        self.clicked.connect(callback)

class CommandButton(QPushButton):
    COMMAND_SEPARATOR = ','

    def configure(self, styleSheets, comment, command, callback):
        self.styleSheets = styleSheets
        self.commandBase = command
        self.savedStyleSheet = str(self.styleSheet())
        if comment:
            self.setText('%s: %s' % (self.text(), comment))
        else:
            self.setText(self.text())
        self.setArguments()
        self.highlight()
        self.clicked.connect(partial(callback, self))

    def setArguments(self, values = ()):
        self.values = values
        self.command = self.commandBase + self.COMMAND_SEPARATOR.join(str(value) for value in values)
        self.setStatusTip('Command: %s' % self.command)

    def highlight(self, key = None):
        self.setStyleSheet(' ;'.join((self.savedStyleSheet, self.styleSheets.get(key, ''))))

class DoseTopEdit(QLineEdit):
    def configure(self, defaultButton, callback):
        self.setPlaceholderText(self.text())
        self.setValidator(QIntValidator(99, 10 ** 9 - 1, self))
        defaultButton.clicked.connect(lambda: self.setText(self.placeholderText()))
        self.callback = callback
        self.textChanged.connect(self.report)
        self.report()

    def report(self):
        self.callback(self.text() or self.placeholderText())

class DeviceTypeComboBox(QComboBox): # pylint: disable=R0924
    def configure(self, callback):
        self.addItems(tuple('%02d: %s' % (value, key) for (key, value) in sorted(DEVICE_TYPES, key = lambda kv: kv[1])))
        self.activated.connect(callback)
        callback(0)

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

class PillControl(QMainWindow):
    comConnect = pyqtSignal(str)
    comInput = pyqtSignal(str)

    def __init__(self, args):
        QMainWindow.__init__(self)
        uic.loadUi(MAIN_UI_FILE_NAME, self)
        self.emulated = False
        self.advanced = False
        (options, _parameters) = getopt(args, 'ae', ('advanced, emulate'))
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
        self.dontWritePillButton.setFocus()
        # Configuring command buttons
        buttonStyleSheets = {
            STATE_SETUP_OK: str(self.setupOKLabel.styleSheet()),
            STATE_SETUP_ERROR: str(self.setupErrorLabel.styleSheet()),
            STATE_PILL_OK: str(self.pillOKLabel.styleSheet()),
            STATE_PILL_ERROR: str(self.pillErrorLabel.styleSheet())
        }
        with open(PILL_COMMANDS_FILE_NAME) as f:
            for line in (line for line in (line.strip() for line in f) if line and not line.strip().startswith('#')):
                (objectName, comment, command) = line.split(',', 2)
                button = self.findChild(QPushButton, objectName)
                if not button:
                    raise ValueError("Unknown button name: %s" % objectName)
                button.configure(buttonStyleSheets, comment.strip(), command.strip(), self.processCommand)
        self.doseTopEdit.configure(self.doseTopDefaultButton, lambda value: self.doseTopButton.setArguments((value,)))
        self.deviceTypeComboBox.configure(lambda index: self.deviceTypeButton.setArguments((DEVICE_INDEXES[index],)))
        if not self.advanced:
            for w in self.commandWidget.findChildren(QWidget):
                if str(w.objectName()).startswith('advanced'):
                    w.hide()
        self.lastCommandButton = None
        self.writingPill = None
        # Setup logging
        formatter = Formatter('%(asctime)s %(levelname)s\t%(message)s', '%Y-%m-%d %H:%M:%S')
        handlers = (FileHandler(LOG_FILE_NAME), CallableHandler(self.logTextEdit.appendPlainText))
        rootLogger = getLogger('')
        for handler in handlers:
            handler.setFormatter(formatter)
            rootLogger.addHandler(handler)
        rootLogger.setLevel(INFO)
        setLoggerClass(EventLogger)
        self.logger = getLogger('PillControl')
        self.logger.configure(self) # pylint: disable=E1103
        self.logger.info("start")
        # Starting up!
        self.loadSettings()
        self.comConnect.connect(self.processConnect)
        self.comInput.connect(self.processInput)
        self.port = SerialPort(self.logger, pingCommand.prefix, ackResponse.prefix,
                               self.comConnect.emit, self.comInput.emit, self.portLabel.setPortStatus.emit,
                               EmulatedSerial() if self.emulated else None)
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    def highlightCommandButton(self, button = None, key = None):
        for b in self.commandWidget.findChildren(QPushButton):
            if b.__class__.__name__ == CommandButton.__name__: # isinstance doesn't work cross-module
                b.highlight(key if b is button else None)

    def processConnect(self, pong): # pylint: disable=W0613
        self.logger.info("connected device detected")

    def parseCommand(self, data):
        data = str(data)
        (tag, args) = Command.decodeCommand(data)
        if tag == ackResponse.tag:
            return args[0]
        if args is not None: # unexpected valid command
            self.logger.warning("Unexpected command: %s %s", tag, ' '.join(str(arg) for arg in args))
        elif tag: # unknown command
            self.logger.warning("Unknown command %s: %s", tag, data.rstrip())
        else: # not a command
            self.logger.warning("Unexpected input: %s", data.rstrip())
        return None # error

    def processCommand(self, source, _checked = False):
        error = True
        data = self.port.command(source.command, COMMAND_MARKER, QApplication.processEvents)
        if data:
            data = str(data).strip()
            (tag, args) = Command.decodeCommand(data)
            if tag == ackResponse.tag:
                error = args[0]
                if error:
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
        self.highlightCommandButton(source, STATE_SETUP_ERROR if error else STATE_SETUP_OK)

    def processInput(self, data):
        error = True
        data = str(data).strip()
        (tag, args) = Command.decodeCommand(data)
        if tag == ackResponse.tag:
            prefix = 'Pill ' if self.writingPill else ''
            self.writingPill = False
            error = args[0]
            if error:
                self.logger.warning("%sERROR: %d", prefix, error)
            else:
                self.logger.info("%sOK", prefix)
        elif tag == pillWrite32Command.tag:
            self.writingPill = True
        elif tag in (pillRead32Command.tag, pillData32Response.tag):
            pass
        elif args is not None: # unexpected valid command
            self.logger.warning("Unexpected command: %s %s", tag, ' '.join(str(arg) for arg in args))
        elif tag: # unknown command
            self.logger.warning("Unknown command %s: %s", tag, data.rstrip())
        else: # not a command
            self.logger.warning("Unexpected input: %s", data.rstrip())
        self.highlightCommandButton(self.lastCommandButton, STATE_PILL_ERROR if error else STATE_PILL_OK)

    def consoleEnter(self):
        data = self.consoleEdit.getInput()
        if data:
            self.lastCommandButton = None
            self.port.write(data)

    def closeEvent(self, _event):
        self.saveSettings()
        self.logger.info("close")

    def reset(self):
        self.logger.info("reset")
        self.port.reset()
        self.commandButtonWidget.reset()

    def saveSettings(self):
        settings = QSettings()
        try:
            settings.setValue('timeStamp', QDateTime.currentDateTime().toString(LONG_DATETIME_FORMAT))
            settings.setValue('doseTop', int(self.doseTopEdit.text() or self.doseTopEdit.placeholderText()))
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
        QCoreApplication.setApplicationName('PillControl')
        settings = QSettings()
        self.logger.info(settings.fileName())
        self.savedMaximized = False
        try:
            timeStamp = settings.value('timeStamp').toString()
            if timeStamp:
                self.doseTopEdit.setText(settings.value('doseTop').toString())
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
        pillControl = PillControl(args[1:]) # retain reference
        pillControl.configure()
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
