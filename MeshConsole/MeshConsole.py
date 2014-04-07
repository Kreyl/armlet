#!/usr/bin/python

from binascii import hexlify, unhexlify
from datetime import datetime
from logging import getLogger, FileHandler, Formatter, Handler, INFO, NOTSET
from re import match
from sys import argv
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QDate, QDateTime, QTimer, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QLabel, QLineEdit, QMainWindow, QPushButton
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from MeshView import DevicesModel, Column, ColumnAction
from MeshDevice import Device, getColumnsData
from SerialPort import SerialPort

DATE_FORMAT = 'yyyy.MM.dd'
TIME_FORMAT = 'hh:mm:ss'
DATETIME_FORMAT = 'dd %s' % TIME_FORMAT

COMMAND_PING = 'getStartTime'
COMMAND_PONG = 'startTime'
COMMAND_SET_TIME = 'setTime %d'
COMMAND_NODE_INFO = 'node'

TIME_SET_INTERVAL = 10

HIGHLIGHT_STYLE = 'color: red'

ACCENT_PREFIX = 'deviceTableViewAccentSample'
ACCENT_PREFIX_LENGTH = len(ACCENT_PREFIX)

UI_FILE_NAME = 'MeshConsole.ui'
LOG_FILE_NAME = 'MeshConsole.log'
DUMP_FILE_NAME = 'MeshConsole.dmp'

CYCLE_LENGTH = 400

WINDOW_SIZE = 2.0 / 3
WINDOW_POSITION = (1 - WINDOW_SIZE) / 2

def timeDeltaStr(seconds):
    negative = seconds < 0
    (minutes, seconds) = divmod(abs(seconds), 60)
    if minutes:
        (hours, minutes) = divmod(minutes, 60)
        if hours:
            (days, hours) = divmod(minutes, 24)
            if days:
                ret = '%d %02d:%02d:%02d' % (days, hours, minutes, seconds)
            else:
                ret = '%d:%02d:%02d' % (hours, minutes, seconds)
        else:
            ret = '%d:%02d' % (minutes, seconds)
    else:
        ret = '%d' % seconds
    return ('-' if negative else '') + ret

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

class ResetButton(QPushButton):
    def configure(self):
        fixWidgetSize(self, 1.5)

class DateValueLabel(QLabel):
    def configure(self):
        self.savedStyleSheet = self.styleSheet()
        fixWidgetSize(self)

    def setValue(self, date):
        self.setText(date.toString(DATE_FORMAT) if date else 'not set')
        self.setStyleSheet(self.savedStyleSheet if date else '%s; %s' % (self.savedStyleSheet, HIGHLIGHT_STYLE))

class DateTimeValueLabel(QLabel):
    def configure(self):
        self.savedStyleSheet = self.styleSheet()
        fixWidgetSize(self)

    def setValue(self, dateTime):
        self.setText(dateTime.toString(DATETIME_FORMAT) if dateTime else 'not set')
        self.setStyleSheet(self.savedStyleSheet if dateTime else '%s; %s' % (self.savedStyleSheet, HIGHLIGHT_STYLE))

class CallableHandler(Handler):
    def __init__(self, emitCallable, level = NOTSET):
        Handler.__init__(self, level)
        self.emitCallable = emitCallable

    def emit(self, record):
        self.emitCallable(self.format(record))

class ConsoleEdit(QLineEdit):
    def getInput(self):
        ret = self.text()
        self.clear()
        return ret

class MeshConsole(QMainWindow):
    comLogSignal = pyqtSignal(int, str)

    def __init__(self, *args, **kwargs):
        QMainWindow.__init__(self, *args, **kwargs)
        uic.loadUi(UI_FILE_NAME, self)

    def configure(self):
        # Setting window size
        resolution = QDesktopWidget().screenGeometry()
        width = resolution.width()
        height = resolution.height()
        self.setGeometry(width * WINDOW_POSITION, height * WINDOW_POSITION, width * WINDOW_SIZE, height * WINDOW_SIZE)
        # Configuring widgets
        self.portLabel.configure()
        self.pauseButton.clicked.connect(self.pause)
        self.pauseButton.setFocus()
        self.resetButton.configure()
        self.resetButton.clicked.connect(self.reset)
        self.startTimeValueLabel.configure()
        self.timeValueLabel.configure()
        self.consoleEdit.returnPressed.connect(self.consoleEnter)
        self.sampleWidget.hide()
        self.statusBar.hide()
        # Setup logging
        formatter = Formatter('%(asctime)s %(levelname)s\t%(message)s', '%Y-%m-%d %H:%M:%S')
        handlers = (FileHandler(LOG_FILE_NAME), CallableHandler(self.logTextEdit.appendPlainText))
        rootLogger = getLogger('')
        for handler in handlers:
            handler.setFormatter(formatter)
            rootLogger.addHandler(handler)
        rootLogger.setLevel(INFO)
        self.logger = getLogger('MeshConsole')
        self.comLogSignal.connect(getLogger('COM').log)
        self.logger.info("start")
        # Configure devices and columns
        self.startTime = None
        self.currentCycle = None
        self.previousTimeSet = None
        Device.configure(self)
        self.devices = Device.devices
        self.columns = tuple(Column(nColumn, *args) for (nColumn, args) in enumerate(getColumnsData(self)))
        self.devicesModel = DevicesModel(self.devices, self.columns, self)
        samples = (self.sampleLayout.itemAt(i).widget() for i in xrange(self.sampleLayout.count()))
        samples = ((str(sample.objectName()), sample) for sample in samples)
        accentSamples = dict((int(name[ACCENT_PREFIX_LENGTH:]), sample) for (name, sample) in samples if name.startswith(ACCENT_PREFIX))
        self.devicesTableView.configure(self.devicesModel, self.deviceTableViewChangedSample, accentSamples)
        self.columnActions = []
        for column in self.columns:
            action = ColumnAction(column, self.devicesTableView.setColumnHidden, self.columnsMenu)
            self.columnsMenu.addAction(action)
            self.columnActions.append(action)
        # Starting up!
        self.setStartTime()
        self.loadDump()
        self.playing = False # will be toggled immediately by pause()
        self.port = SerialPort(COMMAND_PING, COMMAND_PONG, self.processConnect, self.processInput, self.portLabel.setPortStatus.emit, self.comLogSignal.emit)
        self.pause()
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    @staticmethod
    def cycleSeconds(cycle):
        return cycle * CYCLE_LENGTH // 1000 if cycle != None else None

    def cycleTimeStr(self, cycle):
        return timeDeltaStr(self.cycleSeconds(cycle)) if cycle != None else None

    def cycleDate(self, cycle):
        return self.startTime.addMSecs(cycle * CYCLE_LENGTH) if self.startTime and cycle != None else None

    def cycleDateStr(self, cycle):
        return self.cycleDate(cycle).toString(DATETIME_FORMAT) if self.startTime and cycle != None else None

    def cycleAge(self, age):
        return self.currentCycle - age if self.currentCycle != None and age != None else None

    def cycleAgeSeconds(self, age):
        return self.cycleSeconds(self.cycleAge(age)) if self.currentCycle != None and age != None else None

    def cycleAgeTimeStr(self, age):
        return self.cycleTimeStr(self.cycleAge(age)) if self.currentCycle != None and age != None else None

    def tdTime(self, td):
        return self.currentCycle + td if self.currentCycle != None and td != None else None

    def tdDateStr(self, td):
        return self.cycleDateStr(self.tdTime(td)) if self.startTime and self.currentCycle != None and td != None else None

    def setStartTime(self, date = None):
        self.startTime = QDateTime(date) if date else None
        self.startTimeValueLabel.setValue(date)

    def processConnect(self, pong): # ToDo: make a special signal for this to avoid threading problems?
        try:
            self.setStartTime(QDate(*(int(d) for d in pong.split()[1:])))
        except Exception:
            self.logger.exception("Can't set start time from pong %s", pong) # ToDo: Called from other thread, must be wrapped

    def updateTime(self):
        now = QDateTime.currentDateTime()
        if self.playing:
            self.timeValueLabel.setValue(now)
        self.currentCycle = self.startTime.msecsTo(now) // CYCLE_LENGTH if self.startTime else None
        if self.startTime and self.port.port and (not self.previousTimeSet or self.previousTimeSet.secsTo(now) >= TIME_SET_INTERVAL):
            self.previousTimeSet = now
            self.logger.info("Setting network time to %d" % self.currentCycle)
            self.port.write(COMMAND_SET_TIME % self.currentCycle)
        dt = QDateTime.currentDateTime().msecsTo(QDateTime.fromMSecsSinceEpoch((now.toMSecsSinceEpoch() // 1000 + 1) * 1000))
        QTimer.singleShot(max(0, dt), self.updateTime)

    def processInput(self, inputLine): # ToDo: make a special signal for this to avoid threading problems?
        if inputLine.startswith(COMMAND_NODE_INFO):
            try:
                words = inputLine.split()
                self.devices[int(words[1]) - 1].update(*words[2:])
                self.saveDump()
                if self.playing:
                    self.devicesModel.refresh()
            except: # ToDo: catch exceptions carefully
                self.logger.exception("Bad info command %s", inputLine) # ToDo: Called from other thread, must be wrapped

    def consoleEnter(self):
        self.port.write(self.consoleEdit.getInput())

    def closeEvent(self, _event):
        self.saveDump()
        self.logger.info("close")

    def reset(self):
        self.logger.info("reset")
        self.setStartTime()
        for device in self.devices:
            device.reset()
        self.devicesModel.refresh(True)
        self.port.connect()

    def pause(self):
        self.playing = not self.playing
        self.logger.info("play" if self.playing else "pause")
        v = self.logTextEdit.verticalScrollBar()
        v.setValue(max(0, v.maximum() - (0 if self.playing else 1)))
        self.updateTime()
        if self.playing:
            self.devicesModel.refresh()

    def saveDump(self):
        with open(DUMP_FILE_NAME, 'w') as dump:
            dump.write('# Mesh Console dump at %s\n' % datetime.now())
            dump.write('window %d %d %d %d %s\n' % (max(0, self.pos().x()), max(0, self.pos().y()), self.size().width(), self.size().height(), self.isMaximized()))
            dump.write('state %s\n' % hexlify(self.saveState()))
            dump.write('columns %s\n' % ' '.join(str(action.isChecked()) for action in self.columnActions))
            if self.startTime:
                date = self.startTime.date()
                dump.write('startTime %d %d %d\n' % (date.year(), date.month(), date.day()))
            for device in self.devices:
                dumpStr = device.toDumpStr()
                if dumpStr:
                    dump.write('%s\n' % dumpStr)
            dump.write('# end\n')

    def loadDump(self):
        self.savedMaximized = None
        try:
            with open(DUMP_FILE_NAME) as dump:
                for line in dump:
                    line = line.strip()
                    if line.startswith('#'):
                        continue
                    words = line.split()
                    (tag, data) = (words[0], words[1:])
                    if tag == 'window':
                        (x, y, width, height, isMaximized) = data
                        self.move(int(x), int(y))
                        self.resize(int(width), int(height))
                        self.savedMaximized = isMaximized == 'True'
                    elif tag == 'state':
                        self.restoreState(unhexlify(data[0]))
                    elif tag == 'columns':
                        for (action, checked) in zip(self.columnActions, data):
                            action.setChecked(checked == 'True')
                    elif tag == 'startTime':
                        self.setStartTime(QDate(*(int(d) for d in data)))
                    elif match(r'\d+', tag):
                        device = self.devices[int(tag) - 1]
                        if device.time != None:
                            self.error("Bad dump: duplicate entry for device %s" % device.number)
                        device.update(*data)
                self.devicesModel.refresh(True)
                self.logger.info("Loaded dump file")
        except IOError:
            self.logger.warning("Error loading dump file, resetting to defaults")

    def error(self, message):
        self.logger.error(message)
        raise Exception(message)

def main():
    try:
        application = QApplication(argv)
        MeshConsole().configure()
        return application.exec_()
    except KeyboardInterrupt:
        pass
    except BaseException:
        print format_exc()

if __name__ == '__main__':
    main()
