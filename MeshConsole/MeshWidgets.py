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
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from SerialPort import SerialPort

DATE_FORMAT = 'MM.dd'
LONG_DATE_FORMAT = 'yyyy.MM.dd'
TIME_FORMAT = 'hh:mm:ss'
DATETIME_FORMAT = 'dd  %s' % TIME_FORMAT
LONG_DATETIME_FORMAT = '%s %s' % (LONG_DATE_FORMAT, TIME_FORMAT)

TIME_SET_INTERVAL = 10 # seconds
TIME_UPDATE_INTERVAL = 100 # milliseconds

ACCENT_PREFIX = 'deviceTableViewAccentSample'
ACCENT_PREFIX_LENGTH = len(ACCENT_PREFIX)

MAIN_UI_FILE_NAME = 'MeshConsole.ui'
ABOUT_UI_FILE_NAME = 'AboutMC.ui'
CONFIRMATION_UI_FILE_NAME = 'Confirmation.ui'

LOG_FILE_NAME = 'MeshConsole.log'

CYCLE_LENGTH = 1000

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
    def configure(self, dateFormat, callback, cycleLength = None):
        self.setDisplayFormat(dateFormat)
        self.setMaximumDate(QDate.currentDate())
        self.reConfigure(cycleLength)
        self.dateChanged.connect(callback)

    def reConfigure(self, cycleLength = None):
        self.setMinimumDate(QDate.currentDate().addDays(-(MAX_CYCLE_NUMBER * (cycleLength or CYCLE_LENGTH) // (1000 * 3600 * 24 * 2))))

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

class AboutDialog(QDialog):
    def __init__(self, trigger):
        QDialog.__init__(self)
        uic.loadUi(ABOUT_UI_FILE_NAME, self)
        trigger.connect(self.exec_)

class ConfirmationDialog(QDialog):
    def __init__(self, dateFormat):
        QDialog.__init__(self)
        uic.loadUi(CONFIRMATION_UI_FILE_NAME, self)
        self.dateFormat = dateFormat
        self.text = str(self.label.text())

    def popup(self, newDate, oldDate):
        self.label.setText(self.text % (oldDate.toString(self.dateFormat), newDate.toString(self.dateFormat)))
        self.buttonBox.button(self.buttonBox.No).setFocus()
        return self.exec_()
