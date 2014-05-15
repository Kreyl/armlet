#!/usr/bin/python
#
# Pill Control
#
from binascii import hexlify
from collections import deque
from functools import partial
from logging import getLogger, getLoggerClass, setLoggerClass, FileHandler, Formatter, Handler, INFO, NOTSET
from random import randint
from sys import argv, exit # pylint: disable=W0622
from time import sleep, time
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QCoreApplication, QDateTime, QObject, QSettings, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QDialog, QIntValidator, QLabel, QLineEdit, QMainWindow, QPushButton
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextProtocol import Command, COMMAND_MARKER
from UARTTextCommands import pingCommand, ackResponse
from SerialPort import SerialPort, DT, TIMEOUT

# ToDo
# Configure and link custom controls
# Cross-link command buttons
# Invalidate current button by any other output
# Verify pill response diagnostics
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
        self.savedStyleSheet = self.styleSheet()
        fixWidgetSize(self)
        self.setPortStatus.connect(self.setValue)

    def setValue(self, portName, portStatus):
        self.setText(portName)
        self.setStyleSheet(self.savedStyleSheet + '; color: %s' % self.STATUS_COLORS.get(portStatus, 'gray'))

class CompactButton(QPushButton):
    def configure(self, callback):
        fixWidgetSize(self, 1.5)
        self.clicked.connect(callback)

class CommandButton(QPushButton):
    PADDING = '  '

    def __init__(self, parent):
        QPushButton.__init__(self, parent)

    def configure(self, styleSheets, comment, command, callback):
        self.styleSheets = styleSheets
        self.command = command
        if comment:
            self.setText('%s%s: %s%s' % (self.PADDING, self.text(), comment, self.PADDING))
        else:
            self.setText('%s%s%s' % (self.PADDING, self.text(), self.PADDING))
        self.setStatusTip('Command: %s' % command)
        self.highlight()
        self.clicked.connect(partial(callback, self))

    def highlight(self, key = None):
        self.setStyleSheet(self.styleSheets.get(key, ''))

class ConsoleEdit(QLineEdit):
    def configure(self, callback):
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
        self.emulated = len(args) > 1 and args[1].lower() in ('-e', '--emulated')

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
        self.doseTopEdit.setValidator(QIntValidator(0, 10 ** 9 - 1, self.doseTopEdit))
        # Configuring command buttons
        buttonStyleSheets = {
            STATE_SETUP_OK: self.setupOKLabel.styleSheet(),
            STATE_SETUP_ERROR: self.setupErrorLabel.styleSheet(),
            STATE_PILL_OK: self.pillOKLabel.styleSheet(),
            STATE_PILL_ERROR: self.pillErrorLabel.styleSheet()
        }
        with open(PILL_COMMANDS_FILE_NAME) as f:
            for line in (line for line in (line.strip() for line in f) if line and not line.strip().startswith('#')):
                (objectName, comment, command) = line.split(',', 2)
                button = self.findChild(QPushButton, objectName)
                if not button:
                    raise ValueError("Unknown button name: %s" % objectName)
                button.configure(buttonStyleSheets, comment.strip(), command.strip(), self.processCommand)
        self.lastCommandButton = None
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

    def processConnect(self, pong): # pylint: disable=W0613
        self.logger.info("connected device detected")

    def parseCommand(self, data):
        (tag, args) = Command.decodeCommand(data)
        if tag == ackResponse.tag:
            return args[0]
        if args is not None: # unexpected valid command
            self.logger.warning("Unexpected command: %s %s", hexlify(tag).upper(), ' '.join(str(arg) for arg in args))
        elif tag: # unknown command
            self.logger.warning("Unknown command %s: %s", tag, data)
        else: # not a command
            self.logger.warning("Unexpected input: %s", data)
        return None # error

    def processCommand(self, source, _checked = False):
        error = True
        data = self.port.command(source.command, COMMAND_MARKER, QApplication.processEvents)
        self.lastCommandButton = source
        if data:
            error = self.parseCommand(data)
            if error != None:
                if error:
                    self.logger.warning("Setup ERROR: %d", error)
                else:
                    self.logger.info("Setup OK")
        else:
            error = True
            self.logger.warning("command timed out")
        source.highlight(STATE_SETUP_ERROR if error else STATE_SETUP_OK)

    def processInput(self, data):
        error = self.parseCommand(data)
        if error != None:
            if error:
                self.logger.warning("Pill ERROR: %d", error)
            else:
                self.logger.info("Pill OK")
        if self.lastCommandButton:
            self.lastCommandButton.highlight(STATE_PILL_ERROR if error else STATE_PILL_OK)

    def consoleEnter(self):
        self.port.write(self.consoleEdit.getInput())

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

def main():
    try:
        application = QApplication(argv)
        pillControl = PillControl(argv) # retain reference
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
    main()
