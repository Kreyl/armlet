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
from time import sleep
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QCoreApplication, QDateTime, QObject, QSettings, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QDialog, QLabel, QLineEdit, QMainWindow, QPushButton, QWidget
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextProtocol import Command, COMMAND_MARKER
from UARTTextCommands import pingCommand, ackResponse
from SerialPort import SerialPort, DT, TIMEOUT

# ToDo
# Remove UARTCommands dependency OR treat config as parameters to commands
# Create reasonable button layout

LONG_DATETIME_FORMAT = 'yyyy.MM.dd hh:mm:ss'

MAIN_UI_FILE_NAME = 'PillControl.ui'
ABOUT_UI_FILE_NAME = 'AboutPC.ui'

PILL_COMMANDS_FILE_NAME = 'PillCommands.txt'

LOG_FILE_NAME = 'PillControl.log'

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

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
        self.nextNode = None

    def readline(self):
        while True:
            if self.buffer:
                data = self.buffer.popleft()
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

class ResetButton(QPushButton):
    def configure(self, callback):
        fixWidgetSize(self, 1.5)
        self.clicked.connect(callback)

class CommandButton(QPushButton):
    def __init__(self, parent):
        QPushButton.__init__(self, parent)

    def configure(self, styleSheets, name, command, callback):
        self.setObjectName(name)
        self.setText(name)
        self.styleSheets = styleSheets
        self.command = command
        self.clicked.connect(partial(callback, self))

    def highlight(self, key):
        self.setStyleSheet(self.styleSheets.get(bool(key) if key is not None else None, ''))

class CommandButtonWidget(QWidget):
    def configure(self, okButton, errorButton, fileName, callback):
        self.callback = callback
        layout = self.layout()
        while layout.count():
            layout.takeAt(0).widget().deleteLater()
        styleSheets = {
            False: okButton.styleSheet() if okButton else '',
            True: errorButton.styleSheet() if errorButton else ''
        }
        with open(fileName) as f:
            for line in f:
                (name, command) = line.split(',', 1)
                button = CommandButton(self)
                button.configure(styleSheets, name.strip(), command.strip(), self.highlight)
                layout.addWidget(button)

    def highlight(self, button = None, _checked = False):
        error = self.callback(button) if button else None
        for b in self.findChildren(CommandButton):
            b.highlight(error if b is button else None)

    def reset(self):
        self.highlight()

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
        self.commandButtonWidget.configure(self.okButton, self.errorButton, PILL_COMMANDS_FILE_NAME, self.command)
        self.consoleEdit.configure(self.consoleEnter)
        self.statusBar.hide()
        self.aboutDialog = AboutDialog(self.aboutAction.triggered)
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
        self.port = SerialPort(self.logger, pingCommand.prefix, ackResponse.prefix,
                               self.comConnect.emit, self.comInput.emit, self.portLabel.setPortStatus.emit,
                               EmulatedSerial() if self.emulated else None)
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    def processConnect(self, pong): # pylint: disable=W0613
        self.logger.info("connected device detected")

    def command(self, source):
        data = self.port.command(source.command, COMMAND_MARKER, QApplication.processEvents)
        if data:
            (tag, args) = Command.decodeCommand(data)
            if tag == ackResponse.tag:
                error = args[0]
                if not error:
                    self.logger.info("OK")
                else:
                    self.logger.warning("Error: %d", error)
                return error
            elif args is not None: # unexpected valid command
                self.logger.warning("Unexpected command: %s %s", hexlify(tag).upper(), ' '.join(str(arg) for arg in args))
            elif tag: # unknown command
                self.logger.warning("Unknown command %s: %s", tag, data)
            else: # not a command
                self.logger.warning("Unexpected input: %s", data)
        else:
            self.logger.warning("command timed out")

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
        self.logger.info("Loading settings from %s", settings.fileName())
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
