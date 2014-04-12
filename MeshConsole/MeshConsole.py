#!/usr/bin/python
#
# Mesh Console
# Main widget and startup
#
from binascii import hexlify, unhexlify
from datetime import datetime
from functools import partial
from logging import getLogger, FileHandler, Formatter, Handler, INFO, NOTSET
from re import match, split
from sys import argv, exit # pylint: disable=W0622
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QDate, QDateTime, QTimer, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QMainWindow
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from MeshDevice import Device, getColumnsData
from MeshWidgets import * # pylint: disable=W0401
from MeshView import DevicesModel, Column, ColumnAction
from SerialPort import SerialPort

# ToDo
# Correctly process replies to commands
# Split updateTime in two, to update mesh time and view immediately
# Set cycleLength from device

def timeDeltaStr(seconds):
    negative = seconds < 0
    (minutes, seconds) = divmod(abs(seconds), 60)
    if minutes:
        (hours, minutes) = divmod(minutes, 60)
        if hours:
            (days, hours) = divmod(hours, 24)
            if days:
                ret = '%d %02d:%02d:%02d' % (days, hours, minutes, seconds)
            else:
                ret = '%d:%02d:%02d' % (hours, minutes, seconds)
        else:
            ret = '%d:%02d' % (minutes, seconds)
    else:
        ret = '%d' % seconds
    return ('-' if negative else '') + ret

class CallableHandler(Handler):
    def __init__(self, emitCallable, level = NOTSET):
        Handler.__init__(self, level)
        self.emitCallable = emitCallable

    def emit(self, record):
        self.emitCallable(self.format(record))

class MeshConsole(QMainWindow):
    comConnect = pyqtSignal(str)
    comInput = pyqtSignal(str)
    comLog = pyqtSignal(int, str)

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
        self.pauseButton.configure(self.pause)
        self.resetButton.configure(self.reset)
        self.startDateEdit.configure(DATE_FORMAT, self.setStartTime)
        self.dateTimeValueLabel.configure(DATETIME_FORMAT)
        self.consoleEdit.configure(self.consoleEnter)
        self.sampleWidget.hide()
        self.statusBar.hide()
        self.startDateChangeConfirmationDialog = StartDateChangeConfirmationDialog()
        # Setup logging
        formatter = Formatter('%(asctime)s %(levelname)s\t%(message)s', '%Y-%m-%d %H:%M:%S')
        handlers = (FileHandler(LOG_FILE_NAME), CallableHandler(self.logTextEdit.appendPlainText))
        rootLogger = getLogger('')
        for handler in handlers:
            handler.setFormatter(formatter)
            rootLogger.addHandler(handler)
        rootLogger.setLevel(INFO)
        self.logger = getLogger('MeshConsole')
        self.comLog.connect(getLogger('COM').log)
        self.logger.info("start")
        # Configure devices and columns
        self.startTime = None
        self.savedStartDate = None
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
        self.loadDump()
        self.playing = False # will be toggled immediately by pause()
        self.comConnect.connect(self.processConnect)
        self.comInput.connect(self.processInput)
        self.port = SerialPort(COMMAND_PING, COMMAND_PONG, self.comConnect.emit, self.comInput.emit, self.portLabel.setPortStatus.emit, self.comLog.emit)
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

    def setStartTime(self):
        date = self.startDateEdit.date()
        if date != self.savedStartDate:
            if not self.savedStartDate or self.startDateChangeConfirmationDialog.popup():
                self.savedStartDate = date
                self.startTime = QDateTime(date)
                self.logger.info("Start date set to %s" % date.toString(DATE_FORMAT))
            else:
                QTimer.singleShot(0, partial(self.startDateEdit.setDate, self.savedStartDate)) # direct call works incorrectly
        self.pauseButton.setFocus()

    def processConnect(self, _pong):
        self.logger.info("connected device found")

    def updateTime(self):
        now = QDateTime.currentDateTime()
        self.currentCycle = self.startTime.msecsTo(now) // CYCLE_LENGTH if self.startTime else None
        if self.currentCycle >= MAX_CYCLE_NUMBER:
            self.error("Cycle number overflow, aborting")
        if self.playing:
            self.dateTimeValueLabel.setValue(now, self.currentCycle)
        if self.startTime and self.port.port and (not self.previousTimeSet or self.previousTimeSet.secsTo(now) >= TIME_SET_INTERVAL):
            self.previousTimeSet = now
            self.logger.info("Setting mesh time to %d" % self.currentCycle)
            self.port.write(COMMAND_SET_TIME % self.currentCycle)
        dt = QDateTime.currentDateTime().msecsTo(QDateTime.fromMSecsSinceEpoch((now.toMSecsSinceEpoch() // 1000 + 1) * 1000))
        QTimer.singleShot(max(0, dt), self.updateTime)

    def processInput(self, inputLine):
        inputLine = str(inputLine)
        if inputLine.startswith(COMMAND_NODE_INFO):
            try:
                words = split(' *[, ] *', inputLine.strip())
                self.devices[int(words[1]) - 1].update(*words[2:])
                self.saveDump()
                if self.playing:
                    self.devicesModel.refresh()
            except Exception:
                self.logger.exception("Bad info command %s", inputLine)

    def consoleEnter(self):
        self.port.write(self.consoleEdit.getInput())

    def closeEvent(self, _event):
        self.saveDump()
        self.logger.info("close")

    def reset(self):
        self.logger.info("reset")
        self.pauseButton.setFocus()
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
                dump.write('startDate %d %d %d\n' % (date.year(), date.month(), date.day()))
            for device in self.devices:
                dumpStr = device.toDumpStr()
                if dumpStr:
                    dump.write('%s\n' % dumpStr)
            dump.write('# end\n')

    def loadDump(self):
        startDate = None
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
                    elif tag == 'startDate':
                        startDate = QDate(*(int(d) for d in data))
                    elif match(r'\d+', tag):
                        device = self.devices[int(tag) - 1]
                        if device.time != None:
                            self.error("Bad dump: duplicate entry for device %s" % device.number)
                        device.update(*data)
                self.devicesModel.refresh(True)
                self.logger.info("Loaded dump file")
        except IOError:
            self.logger.warning("Error loading dump file, resetting to defaults")
        self.startDateEdit.setDate(startDate if startDate else QDate.currentDate())

    def error(self, message):
        print "ERROR:", message
        self.logger.error(message)
        exit(-1)

def main():
    try:
        application = QApplication(argv)
        MeshConsole().configure()
        return application.exec_()
    except KeyboardInterrupt:
        pass
    except SystemExit:
        raise
    except BaseException:
        print format_exc()

if __name__ == '__main__':
    main()
