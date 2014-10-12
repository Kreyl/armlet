#!/usr/bin/python
#
# Firefly Control main module
#
from collections import deque
from functools import partial
from getopt import getopt
from logging import getLogger, getLoggerClass, setLoggerClass, FileHandler, Formatter, Handler, INFO, NOTSET
from os import getcwd
from os.path import basename, dirname, join, relpath
from sys import argv, exit # pylint: disable=W0622
from time import sleep
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QByteArray, QCoreApplication, QDateTime, QObject, QSettings, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QDialog, QFileDialog, QMainWindow
    from PyQt4.QtGui import QAction, QColor, QKeySequence, QLabel, QMessageBox
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from UARTTextProtocol import Command, COMMAND_MARKER
from UARTTextCommands import ackResponse, ffGetCommand, ffSetCommand, ffResponse
from SerialPort import SerialPort, DT, TIMEOUT

from FireflyWidgets import CommandWidget, InsertCommandButton, SelectColorLabel

LONG_DATETIME_FORMAT = 'yyyy.MM.dd hh:mm:ss'

MAIN_UI_FILE_NAME = 'FireflyControl.ui'
ABOUT_UI_FILE_NAME = 'AboutFC.ui'

LOG_FILE_NAME = 'FireflyControl.log'

FILE_FILTER = 'Firefly programs (*.ff);; All files(*)'

FILE_TEMPLATE = '''\
#
# Ostranna CG http://ostranna.ru
# Firefly program
#

%s

# End of program
'''

WINDOW_TITLE = '%s :: %s%s'

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

#
# ToDo:
# - Optimize resize events with sizes updating
# - Remove program table flicker when opening files
# - Avoid sending too many commands on color edit
# - Handle set immediately after get, is it needed?
#

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
        self.program = 'rgb,255,0,0,0,rgb,255,255,255,0,wait,1,rgb,0,0,255,0,rgb,255,255,255,0,rgb,0,255,0,0,rgb,255,255,255,0,goto,3'

    def readline(self):
        while True:
            if self.buffer:
                return self.buffer.popleft()
            sleep(DT)

    def write(self, data):
        try:
            (tag, args) = Command.decodeCommand(data)
            if tag == ffSetCommand.tag:
                self.program = ','.join(args)
                ret = ackResponse.encode(0)
            elif tag == ffGetCommand.tag:
                ret = ffResponse.encode(self.program)
            else:
                raise ValueError("Unknown command")
        except ValueError, e:
            ret = str(e)
        self.buffer.append(ret)
        self.ready = True
        return len(data)

    def close(self):
        pass

class AboutDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        uic.loadUi(ABOUT_UI_FILE_NAME, self)

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

class FireflyControl(QMainWindow):
    comConnect = pyqtSignal(str)

    def __init__(self, args):
        QMainWindow.__init__(self)
        uic.loadUi(MAIN_UI_FILE_NAME, self)
        # Processing command line options
        self.advanced = False
        self.emulated = False
        self.needLoadSettings = True
        (options, _parameters) = getopt(args, 'aer', ('advanced', 'emulated', 'reset'))
        for (option, _value) in options:
            if option in ('-a', '--advanced'):
                self.advanced = True
            elif option in ('-e', '--emulated'):
                self.emulated = True
            elif option in ('-r', '--reset'):
                self.needLoadSettings = False
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
        # Setting variables
        self.title = self.windowTitle()
        self.fileName = None
        self.programChanged = False
        self.currentDirectory = getcwd()
        self.recentFiles = deque(maxlen = 9)
        self.port = None
        # Setting window size
        resolution = QDesktopWidget().screenGeometry()
        width = resolution.width()
        height = resolution.height()
        self.setGeometry(width * WINDOW_POSITION, height * WINDOW_POSITION, width * WINDOW_SIZE, height * WINDOW_SIZE)
        # Configuring widgets
        SelectColorLabel.configure(self, self.colorLabel)
        self.colorLabel.setCorrectSize(self.graphLabel.minimumHeight())
        self.colorLabel.callback = self.updateHardware
        self.portLabel.configure()
        self.resetButton.clicked.connect(self.reset)
        self.consoleEdit.configure(self.consoleEnter)
        self.newAction.triggered.connect(self.newFile)
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.saveAsAction.triggered.connect(self.saveFileAs)
        self.aboutDialog = AboutDialog()
        self.aboutAction.triggered.connect(self.aboutDialog.exec_)
        self.aboutQtAction.triggered.connect(partial(QMessageBox.aboutQt, self, "About Qt"))
        self.setColorButton.clicked.connect(self.hardwareSetColor)
        self.setProgramButton.clicked.connect(self.hardwareSetProgram)
        self.getProgramButton.clicked.connect(self.hardwareGetProgram)
        CommandWidget.configure(self.programStackedWidget, self.commandsWidget, self.updateProgram)
        InsertCommandButton.configure(self.insertCommandWidget)
        InsertCommandButton()
        # Starting up!
        self.loadSettings()
        self.comConnect.connect(self.processConnect)
        self.port = SerialPort(self.logger, ffGetCommand.prefix, ffResponse.prefix,
                               self.comConnect.emit, None, self.portLabel.setPortStatus.emit,
                               EmulatedSerial() if self.emulated else None, (115200,))
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    def askForSave(self):
        if not self.programChanged:
            return True
        messageBox = QMessageBox(QMessageBox.Question, "The program has unsaved changes",
                "Do you want to save it?", QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel, self)
        ret = messageBox.exec_()
        if ret == QMessageBox.Save:
            return self.saveFile()
        return ret == QMessageBox.Discard

    def newFile(self, program = None, force = False, markChanged = False):
        if not force and not self.askForSave():
            return False
        CommandWidget.setupProgram((program,) if program else None)
        self.fileName = None
        self.updateWindowTitle(markChanged)
        return True

    def openFile(self, fileName = None):
        if not self.askForSave():
            return False
        if not fileName:
            fileName = QFileDialog.getOpenFileName(self, "Open program", self.currentDirectory, FILE_FILTER)
            if not fileName:
                return False
        fileName = unicode(fileName)
        try:
            with open(fileName) as f:
                CommandWidget.setupProgram(f)
        except Exception, e:
            QMessageBox(QMessageBox.Warning, "Error opening file", unicode(e), QMessageBox.Ok, self).exec_()
            return False
        self.fileName = fileName
        self.currentDirectory = dirname(fileName)
        self.updateWindowTitle(False)
        self.updateRecentFiles(fileName)
        return True

    def saveFile(self, saveAs = False):
        if saveAs or not self.fileName:
            fileName = QFileDialog.getSaveFileName(self, "Save program as", self.fileName or join(self.currentDirectory, 'New.ff'), FILE_FILTER)
            if not fileName:
                return False
            fileName = unicode(fileName)
        else:
            fileName = self.fileName
        try:
            with open(fileName, 'w') as f:
                f.write(FILE_TEMPLATE % self.programEdit.text())
        except Exception, e:
            QMessageBox(QMessageBox.Warning, "Error saving file", unicode(e), QMessageBox.Ok, self).exec_()
            return False
        self.fileName = fileName
        self.currentDirectory = dirname(fileName)
        self.updateWindowTitle(False)
        self.updateRecentFiles(fileName)
        return True

    def saveFileAs(self):
        return self.saveFile(True)

    def updateRecentFiles(self, fileName = None):
        if fileName is not None:
            if fileName in self.recentFiles:
                self.recentFiles.remove(fileName)
            self.recentFiles.append(fileName)
        found = False
        insertBeforeSeparator = None
        for action in self.fileMenu.actions():
            if not found:
                if action.isSeparator():
                    found = True
            elif action.isSeparator():
                insertBeforeSeparator = action
                break
            else:
                action.setParent(None)
        for (i, fileName) in enumerate(reversed(self.recentFiles), 1):
            relPath = relpath(fileName, self.currentDirectory)
            action = QAction('%s\tCtrl+&%d' % (relPath if len(relPath) < len(fileName) else fileName, i), self.fileMenu)
            action.setShortcut(QKeySequence('CTRL+%d' % i))
            action.triggered.connect(partial(self.openFile, fileName))
            self.fileMenu.insertAction(insertBeforeSeparator, action)

    def updateHardware(self, color = None):
        if self.onChangeButtonGroup.checkedButton() is self.onChangeSetColorButton:
            self.hardwareSetColor()
        elif not color and self.onChangeButtonGroup.checkedButton() is self.onChangeSetProgramButton:
            self.hardwareSetProgram()

    def updateProgram(self, program, gradient, programChanged = True):
        if program != self.programEdit.text(): # ToDo: Wouldn't it block window title update when program contents is accidentially the same?
            self.programEdit.setText(program)
            self.graphLabel.setStyleSheet(gradient)
            self.updateWindowTitle(programChanged)
        self.updateHardware()

    def updateWindowTitle(self, programChanged = None):
        if programChanged is not None:
            self.programChanged = programChanged
        programName = basename(self.fileName or 'New')
        dotIndex = programName.rfind('.')
        if dotIndex > 0:
            programName = programName[:dotIndex]
        self.setWindowTitle(WINDOW_TITLE % (self.title, programName, '*' if self.programChanged else ''))

    def processConnect(self, pong):
        if self.onConnectButtonGroup.checkedButton() is self.onConnectSetColorButton:
            self.logger.info("connected device detected, setting color")
            self.hardwareSetColor()
        elif self.onConnectButtonGroup.checkedButton() is self.onConnectSetProgramButton:
            self.logger.info("connected device detected, setting program")
            self.hardwareSetProgram()
        elif self.onConnectButtonGroup.checkedButton() is self.onConnectGetProgramButton:
            self.logger.info("connected device detected, got program")
            self.hardwareGetProgram(Command.decodeCommand(pong)[1])
        elif self.onConnectButtonGroup.checkedButton() is self.onConnectForceGetProgramButton:
            self.logger.info("connected device detected, got program")
            self.hardwareGetProgram(Command.decodeCommand(pong)[1], True)
        else:
            self.logger.info("connected device detected")

    def processCommand(self, command, expect = COMMAND_MARKER):
        if not self.port:
            return
        data = self.port.command(command, expect, QApplication.processEvents)
        if data:
            data = str(data).strip()
            (tag, args) = Command.decodeCommand(data)
            if tag:
                self.logger.info("OK")
                return args
            else:
                self.logger.warning("Unexpected input: %s", data)
        else:
            self.logger.warning("command timed out")

    def consoleEnter(self):
        data = self.consoleEdit.getInput()
        if data:
            self.port.write(data)

    def closeEvent(self, event):
        if self.askForSave():
            self.saveSettings()
            self.logger.info("close")
        else:
            event.ignore()

    def reset(self):
        self.logger.info("reset")
        self.port.reset()

    def hardwareSetColor(self):
        self.processCommand(ffSetCommand.encode(self.colorLabel.getCommand()), ackResponse.prefix)

    def hardwareSetProgram(self):
        self.processCommand(ffSetCommand.encode(str(self.programEdit.text())), ackResponse.prefix)
        if self.programChanged and self.onWriteMarkSavedCheckBox.isChecked():
            self.updateWindowTitle(False)

    def hardwareGetProgram(self, args = None, force = False):
        if not args:
            args = self.processCommand(ffGetCommand.encode())
        if args:
            self.newFile(','.join(args), force, self.onReadMarkChangedCheckBox.isChecked())

    def saveSettings(self):
        settings = QSettings()
        try:
            settings.setValue('timeStamp', QDateTime.currentDateTime().toString(LONG_DATETIME_FORMAT))
            settings.setValue('currentDirectory', self.currentDirectory)
            settings.setValue('fileName', self.fileName or '')
            settings.setValue('recentFiles', tuple(self.recentFiles))
            settings.setValue('program', self.programEdit.text())
            settings.setValue('programChanged', self.programChanged)
            settings.setValue('onChangeButton', self.onChangeButtonGroup.checkedId())
            settings.setValue('onConnectButton', self.onConnectButtonGroup.checkedId())
            settings.setValue('onReadMarkChanged', self.onReadMarkChangedCheckBox.isChecked())
            settings.setValue('onWriteMarkSaved', self.onWriteMarkSavedCheckBox.isChecked())
            settings.setValue('color', self.colorLabel.color)
            settings.setValue('customColors', SelectColorLabel.getCustomColors())
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
        QCoreApplication.setApplicationName('Firefly Control')
        settings = QSettings()
        program = None
        self.savedMaximized = False
        if self.needLoadSettings:
            self.logger.info("Loading settings from %s", settings.fileName())
            try:
                timeStamp = settings.value('timeStamp', type = str)
                if timeStamp:
                    self.currentDirectory = unicode(settings.value('currentDirectory', type = unicode))
                    self.fileName = unicode(settings.value('fileName', type = unicode))
                    self.recentFiles.extend(settings.value('recentFiles', type = tuple))
                    self.updateRecentFiles()
                    program = str(settings.value('program', type = str))
                    CommandWidget.setupProgram((program,) if program else None)
                    self.updateWindowTitle(settings.value('programChanged', type = bool))
                    self.onChangeButtonGroup.button(settings.value('onChangeButton', type = int)).setChecked(True)
                    self.onConnectButtonGroup.button(settings.value('onConnectButton', type = int)).setChecked(True)
                    self.onReadMarkChangedCheckBox.setChecked(settings.value('onReadMarkChanged', type = bool))
                    self.onWriteMarkSavedCheckBox.setChecked(settings.value('onWriteMarkSaved', type = bool))
                    self.colorLabel.setColor(settings.value('color', type = QColor))
                    SelectColorLabel.setCustomColors(settings.value('customColors', type = tuple))
                    settings.beginGroup('window')
                    self.resize(settings.value('width', type = int), settings.value('height', type = int))
                    self.move(settings.value('x', type = int), settings.value('y', type = int))
                    self.savedMaximized = settings.value('maximized', False, type = bool)
                    self.restoreState(settings.value('state', type = QByteArray))
                    settings.endGroup()
                    self.logger.info("Loaded settings dated %s", timeStamp)
                else:
                    self.logger.info("No settings found")
            except:
                self.logger.exception("Error loading settings")
                CommandWidget.setupProgram()

    def error(self, message):
        print "ERROR:", message
        self.logger.error(message)
        exit(-1)

def main(args):
    try:
        application = QApplication(args)
        _fireflyControl = FireflyControl(args[1:]) # retain reference
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
