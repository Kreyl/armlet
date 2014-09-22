#!/usr/bin/python
#
# Mesh Console
# Main widget and startup
#
from collections import deque
from functools import partial
from getopt import getopt
from logging import getLogger, getLoggerClass, setLoggerClass, FileHandler, Formatter, Handler, INFO, NOTSET
from math import ceil
from random import randint
from sys import argv, exit # pylint: disable=W0622
from time import sleep, time
from traceback import format_exc

try:
    from PyQt4 import uic
    from PyQt4.QtCore import QCoreApplication, QDate, QDateTime, QObject, QSettings, QTimer, pyqtSignal
    from PyQt4.QtGui import QApplication, QDesktopWidget, QMainWindow
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install PyQt4 v4.10.4 or later: http://riverbankcomputing.com/software/pyqt/download\n" % (ex.__class__.__name__, ex))

from MeshDevice import Device, getColumnsData
from MeshWidgets import * # pylint: disable=W0401
from MeshView import DevicesModel, Column, ColumnAction
from UARTTextCommands import Command, meshNodeInfoResponse
from UARTTextCommands import meshGetSettingsCommand, meshGetSettingsResponse
from UARTTextCommands import meshSetCycleCommand, meshSetCycleResponse
from SerialPort import SerialPort, DT, TIMEOUT

# ToDo
# Avoid extra bold in table heading popups
# Get devices number from device OR make devices list dynamic, start with empty table

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
            now = time()
            if self.ready and now > self.nextNode:
                self.nextNode = now + float(randint(0, 1000)) / 1000
                num = randint(1, 100)
                data = meshNodeInfoResponse.encode(num, 0 if num == 1 else randint(1, 5), randint(0, 10000), randint(0, 200) - 100, randint(0, 20) + randint(0, 1) * 0x8000, randint(0, 200), randint(0, 100))
                break
            sleep(DT)
        return data

    def write(self, data):
        (tag, _args) = Command.decodeCommand(data)
        if tag == meshGetSettingsCommand.tag:
            self.buffer.append(meshGetSettingsResponse.encode(len(Device.devices), 400))
        elif tag == meshSetCycleCommand.tag:
            self.buffer.append(meshSetCycleResponse.encode(randint(-10, 10)))
        self.ready = True
        return len(data)

    def close(self):
        pass

class MeshConsole(QMainWindow):
    comConnect = pyqtSignal(str)
    comInput = pyqtSignal(str)

    def __init__(self, args):
        QMainWindow.__init__(self)
        uic.loadUi(MAIN_UI_FILE_NAME, self)
        self.emulated = False
        self.needLoadSettings = True
        self.slave = False
        (options, _parameters) = getopt(args, 'ers', ('emulated', 'reset', 'slave'))
        for (option, _value) in options:
            if option in ('-e', '--emulated'):
                self.emulated = True
            elif option in ('-r', '--reset'):
                self.needLoadSettings = False
            elif option in ('-s', '--slave'):
                self.slave = True

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
        self.aboutDialog = AboutDialog(self.aboutAction.triggered)
        self.confirmationDialog = ConfirmationDialog(DATE_FORMAT)
        # Setup logging
        formatter = Formatter('%(asctime)s %(levelname)s\t%(message)s', '%Y-%m-%d %H:%M:%S')
        handlers = (FileHandler(LOG_FILE_NAME), CallableHandler(self.logTextEdit.appendPlainText))
        rootLogger = getLogger('')
        for handler in handlers:
            handler.setFormatter(formatter)
            rootLogger.addHandler(handler)
        rootLogger.setLevel(INFO)
        setLoggerClass(EventLogger)
        self.logger = getLogger('MeshConsole')
        self.logger.configure(self) # pylint: disable=E1103
        self.logger.info("start")
        # Configure devices and columns
        self.startTime = None
        self.savedStartDate = None
        self.currentCycle = None
        self.previousTimeSet = None
        self.cycleLength = CYCLE_LENGTH
        self.timeSetInterval = TIME_SET_INTERVAL * 1000
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
        self.loadSettings()
        self.logger.info("Running in %s mode", 'SLAVE' if self.slave else 'MASTER')
        self.playing = False # will be toggled immediately by pause()
        self.comConnect.connect(self.processConnect)
        self.comInput.connect(self.processInput)
        self.port = SerialPort(self.logger, meshGetSettingsCommand.prefix, meshGetSettingsResponse.prefix,
                               self.comConnect.emit, self.comInput.emit, self.portLabel.setPortStatus.emit,
                               EmulatedSerial() if self.emulated else None)
        self.pause()
        self.updateTime()
        if self.savedMaximized:
            self.showMaximized()
        else:
            self.show()

    def cycleSeconds(self, cycle):
        return cycle * self.cycleLength // 1000 if cycle is not None else None

    def cycleTimeStr(self, cycle):
        return timeDeltaStr(self.cycleSeconds(cycle)) if cycle is not None else None

    def cycleDate(self, cycle):
        return self.startTime.addMSecs(cycle * self.cycleLength) if self.startTime and cycle is not None else None

    def cycleDateStr(self, cycle):
        return self.cycleDate(cycle).toString(DATETIME_FORMAT) if self.startTime and cycle is not None else None

    def cycleAge(self, age):
        return self.currentCycle - age if self.currentCycle is not None and age is not None else None

    def cycleAgeSeconds(self, age):
        return self.cycleSeconds(self.cycleAge(age)) if self.currentCycle is not None and age is not None else None

    def cycleAgeTimeStr(self, age):
        return self.cycleTimeStr(self.cycleAge(age)) if self.currentCycle is not None and age is not None else None

    def tdTime(self, td):
        return self.currentCycle + td if self.currentCycle is not None and td is not None else None

    def tdDateStr(self, td):
        return self.cycleDateStr(self.tdTime(td)) if self.startTime and self.currentCycle is not None and td is not None else None

    def setStartTime(self):
        date = self.startDateEdit.date()
        if date != self.savedStartDate:
            if not self.savedStartDate or self.confirmationDialog.popup(date, self.savedStartDate):
                self.savedStartDate = date
                self.startTime = QDateTime(date)
                self.logger.info("Start date set to %s", date.toString(DATE_FORMAT))
            else:
                QTimer.singleShot(0, partial(self.startDateEdit.setDate, self.savedStartDate)) # direct call works incorrectly
        self.pauseButton.setFocus()

    def updateTime(self):
        now = QDateTime.currentDateTime()
        self.currentCycle = self.startTime.msecsTo(now) // self.cycleLength if self.startTime else None
        if self.currentCycle >= MAX_CYCLE_NUMBER:
            self.error("Cycle number overflow, aborting")
        if self.playing:
            self.dateTimeValueLabel.setValue(now, self.currentCycle)
        if not self.slave and self.startTime and self.port.ready and (not self.previousTimeSet or self.previousTimeSet.msecsTo(now) >= self.timeSetInterval):
            self.previousTimeSet = now
            self.logger.info("Setting mesh time to %d", self.currentCycle)
            data = self.port.command(meshSetCycleCommand.encode(self.currentCycle), meshSetCycleResponse.prefix, QApplication.processEvents)
            if data:
                dt = meshSetCycleResponse.decode(data)[0]
                self.logger.info(("Time adjusted %+d" % dt) if dt else "Time matched")
            else:
                self.logger.warning("Time set failed")
        self.devicesModel.refresh()
        dt = QDateTime.currentDateTime().msecsTo(now.addMSecs(TIME_UPDATE_INTERVAL))
        QTimer.singleShot(max(0, dt), self.updateTime)

    def processConnect(self, pong):
        self.logger.info("connected device detected")
        (numDevices, self.cycleLength) = meshGetSettingsResponse.decode(pong)
        self.logger.info("Mesh settings: %d devices, cycle length %dms", numDevices, self.cycleLength)
        if numDevices != len(self.devices): # ToDo: Do something clever with it
            self.error("Number of devices mismatch, got %d, expected %d" % (numDevices, len(self.devices)))
        if self.cycleLength < 1:
            self.error("Bad cycle length %d" % self.cycleLength)
        self.timeSetInterval = int(ceil(float(TIME_SET_INTERVAL * 1000) / self.cycleLength)) * self.cycleLength
        self.logger.info("Time set interval set to %dms", self.timeSetInterval)

    def processInput(self, data):
        data = unicode(data).strip()
        (tag, args) = Command.decodeCommand(data)
        if tag == meshNodeInfoResponse.tag:
            try:
                assert 1 <= args[0] <= len(self.devices)
                self.devices[args[0] - 1].update(*args[1:])
                self.saveSettings()
                if self.playing:
                    self.devicesModel.refresh()
            except Exception:
                self.logger.exception("Error processing node info: %s", data)
        elif args is not None: # unexpected valid command
            self.logger.warning("Unexpected command: %s %s", tag, ' '.join(str(arg) for arg in args))
        elif tag: # unknown command
            self.logger.warning("Unknown command %s: %s", tag, data.rstrip())

    def consoleEnter(self):
        data = self.consoleEdit.getInput()
        if data:
            self.port.write(data)

    def closeEvent(self, _event):
        self.saveSettings()
        self.logger.info("close")

    def reset(self):
        self.logger.info("reset")
        self.pauseButton.setFocus()
        for device in self.devices:
            device.reset()
        self.devicesModel.refresh(True)
        self.port.reset()

    def pause(self):
        self.playing = not self.playing
        self.logger.info("play" if self.playing else "pause")
        scrollBar = self.logTextEdit.verticalScrollBar()
        scrollBar.setValue(max(0, scrollBar.maximum() - (0 if self.playing else 1)))
        if self.playing:
            self.devicesModel.refresh()

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
            settings.setValue('columnsVisible', ' '.join(str(int(action.isChecked())) for action in self.columnActions))
            settings.setValue('startDate', self.startTime.date().toString(LONG_DATE_FORMAT))
            settings.beginGroup('emulated' if self.emulated else 'devices')
            for device in self.devices:
                settings.setValue(str(device.number), device.settings())
            settings.endGroup()
        except:
            self.logger.exception("Error saving settings")
            settings.clear()
        settings.sync()

    def loadSettings(self):
        QCoreApplication.setOrganizationName('Ostranna')
        QCoreApplication.setOrganizationDomain('ostranna.ru')
        QCoreApplication.setApplicationName('MeshConsole')
        settings = QSettings()
        startDate = None
        self.savedMaximized = False
        if self.needLoadSettings:
            self.logger.info("Loading settings from %s", settings.fileName())
            try:
                timeStamp = settings.value('timeStamp').toString()
                if timeStamp:
                    settings.beginGroup('window')
                    self.resize(settings.value('width').toInt()[0], settings.value('height').toInt()[0])
                    self.move(settings.value('x').toInt()[0], settings.value('y').toInt()[0])
                    self.savedMaximized = settings.value('maximized', False).toBool()
                    self.restoreState(settings.value('state').toByteArray())
                    settings.endGroup()
                    columnsVisible = str(settings.value('columnsVisible').toString()).strip().split()
                    for (action, checked) in zip(self.columnActions, columnsVisible):
                        action.setChecked(int(checked))
                    startDate = QDate.fromString(settings.value('startDate').toString(), LONG_DATE_FORMAT)
                    settings.beginGroup('emulated' if self.emulated else 'devices')
                    for tag in settings.childKeys():
                        data = str(settings.value(tag).toString()).strip()
                        if data:
                            self.devices[int(tag) - 1].update(*data.split())
                    settings.endGroup()
                    self.logger.info("Loaded settings dated %s", timeStamp)
                else:
                    self.logger.info("No settings found")
            except:
                self.logger.exception("Error loading settings")
        self.startDateEdit.setDate(startDate if startDate else QDate.currentDate())
        self.devicesModel.refresh(True)

    def error(self, message):
        print "ERROR:", message
        self.logger.error(message)
        exit(-1)

def main():
    try:
        application = QApplication(argv)
        meshConsole = MeshConsole(argv[1:]) # retain reference
        meshConsole.configure()
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
