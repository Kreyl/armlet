#!/usr/bin/python
#
# Mesh Console
# Constants and minor widgets
#
try:
    from PyQt4 import uic
    from PyQt4.QtCore import QDate, pyqtSignal
    from PyQt4.QtGui import QDateEdit, QDialog, QLabel, QLineEdit, QPushButton
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from SerialPort import SerialPort

DATE_FORMAT = 'MM.dd'
TIME_FORMAT = 'hh:mm:ss'
DATETIME_FORMAT = 'dd  %s' % TIME_FORMAT

COMMAND_PING = '#01'
COMMAND_PONG = '#90,00'
COMMAND_SET_TIME = '#71,%d'
COMMAND_NODE_INFO = 'node '

TIME_SET_INTERVAL = 10

ACCENT_PREFIX = 'deviceTableViewAccentSample'
ACCENT_PREFIX_LENGTH = len(ACCENT_PREFIX)

UI_FILE_NAME = 'MeshConsole.ui'
CONFIRMATION_UI_FILE_NAME = 'Confirmation.ui'
LOG_FILE_NAME = 'MeshConsole.log'
DUMP_FILE_NAME = 'MeshConsole.dmp'

CYCLE_LENGTH = 400

MAX_CYCLE_NUMBER = 256 ** 3 // 2 - 1 # 3 signed bytes

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

HIGHLIGHT_STYLE = 'color: red'

def fixWidgetSize(widget, adjustment = 1):
    widget.setFixedWidth(widget.fontMetrics().boundingRect(widget.text()).width() * adjustment) # This is a bad hack, but there's no better idea

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

class PauseButton(QPushButton):
    def configure(self, callback):
        self.callback = callback
        self.clicked.connect(self.processClick)

    def processClick(self):
        self.callback()

class ResetButton(QPushButton):
    def configure(self, callback):
        fixWidgetSize(self, 1.5)
        self.clicked.connect(callback)

class StartDateEdit(QDateEdit):
    def configure(self, dateFormat, callback):
        self.setDisplayFormat(dateFormat)
        now = QDate.currentDate()
        self.setMaximumDate(now)
        self.setMinimumDate(now.addDays(-(MAX_CYCLE_NUMBER * CYCLE_LENGTH // (1000 * 3600 * 24 * 2))))
        self.dateChanged.connect(callback)

class DateTimeValueLabel(QLabel):
    def configure(self, dateTimeFormat):
        self.dateTimeFormat = dateTimeFormat
        self.savedStyleSheet = self.styleSheet()

    def setValue(self, dateTime, cycle):
        self.setText(("%s  %s" % (dateTime.toString(self.dateTimeFormat), cycle)) if dateTime else 'not set')
        self.setStyleSheet(self.savedStyleSheet if dateTime else '%s; %s' % (self.savedStyleSheet, HIGHLIGHT_STYLE))

class ConsoleEdit(QLineEdit):
    def configure(self, callback):
        self.returnPressed.connect(callback)

    def getInput(self):
        ret = self.text()
        self.clear()
        return ret

class StartDateChangeConfirmationDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)
        uic.loadUi(CONFIRMATION_UI_FILE_NAME, self)

    def popup(self):
        self.buttonBox.button(self.buttonBox.No).setFocus()
        return self.exec_()
