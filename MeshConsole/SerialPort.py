#!/usr/bin/python
#
# Mesh Console
# Serial port operation routines
#
from collections import deque
from re import sub
from threading import Thread
from time import sleep, time

try:
    from serial import Serial, SerialTimeoutException
    from serial.tools.list_ports import comports
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pySerial v2.6 or later: http://pypi.python.org/pypi/pyserial\n" % (ex.__class__.__name__, ex))

BAUD_RATE = 115200

TIMEOUT = 1
DT = 0.1

class SerialPort(object):
    TRYING = 0
    CONNECTED = 1
    VERIFIED = 2
    ERROR = 3
    NONE = 4

    def __init__(self, logger, ping = None, pong = '', connectCallBack = None, readCallBack = None, portTryCallBack = None, externalPort = None):
        self.logger = logger
        self.ping = ping
        self.pong = pong
        self.connectCallBack = connectCallBack
        self.readCallBack = readCallBack
        self.portTryCallBack = portTryCallBack
        self.externalPort = externalPort
        self.writeBuffer = deque()
        self.port = None
        self.ready = None
        self.expectTimeout = None
        self.expectPrefix = None
        self.expectResult = None
        self.startThread(self.reader, 'reader')
        self.startThread(self.writer, 'writer')
        self.startThread(self.connect, 'connect')

    def startThread(self, what, name):
        thread = Thread(target = what, name = '%s 0x%x %s' % (self.__class__.__name__, id(self), name))
        thread.setDaemon(True)
        thread.start()

    def statusUpdate(self, portName, portStatus):
        if self.portTryCallBack:
            self.portTryCallBack(portName, portStatus)

    def reader(self):
        while True:
            try:
                if self.port:
                    line = self.port.readline()
                    if line:
                        self.logger.info("< %s" % line)
                        if time() < self.expectTimeout and line.startswith(self.expectPrefix):
                            self.expectResult = line
                        elif self.ready and self.readCallBack:
                            self.readCallBack(line)
            except Exception:
                self.logger.warning("connection broken")
                self.reset()
            sleep(DT)

    def writer(self):
        while True:
            if self.writeBuffer:
                data = self.writeBuffer.popleft() + '\n'
                while True:
                    while not self.port:
                        sleep(DT)
                    try:
                        if self.port.write(data) == len(data):
                            break
                    except SerialTimeoutException:
                        pass
                    self.reset()
            else:
                sleep(DT)

    def connect(self):
        while True:
            while self.port:
                sleep(DT)
            self.statusUpdate("SCAN", self.TRYING)
            sleep(DT)
            portNames = tuple(portName for (portName, _description, _address) in comports())
            if portNames:
                for portName in portNames:
                    try:
                        displayPortName = sub('^/dev/', '', portName)
                        self.statusUpdate(displayPortName, self.TRYING)
                        self.port = self.externalPort or Serial(portName, baudrate = BAUD_RATE, timeout = TIMEOUT, writeTimeout = TIMEOUT)
                        self.statusUpdate(displayPortName, self.CONNECTED)
                        self.logger.info("connected to %s" % portName)
                        if self.ping:
                            pong = self.command(self.ping, self.pong, notReady = True)
                            if pong is not None:
                                if self.connectCallBack:
                                    self.connectCallBack(pong)
                                self.ready = True
                                break
                        else:
                            self.ready = True
                            break
                    except Exception:
                        self.statusUpdate(displayPortName, self.ERROR)
                    self.reset()
            else:
                self.statusUpdate("No COM", self.NONE)

    def reset(self):
        if self.port:
            self.port.close()
            self.port = None

    def write(self, data, notReady = False):
        if self.port and (self.ready or notReady):
            self.logger.info(" > %s" % data)
            self.writeBuffer.append(data)
        else:
            self.logger.info(" >! %s" % data)

    def expect(self, prefix, idle = None, notReady = False):
        if self.port and (self.ready or notReady):
            self.expectPrefix = prefix
            self.expectResult = None
            self.expectTimeout = time() + self.port.timeout
            while self.expectResult is None and time() < self.expectTimeout:
                if idle:
                    idle()
                else:
                    sleep(DT)
            if idle:
                idle()
            else:
                sleep(0)
            self.expectTimeout = None
            return self.expectResult

    def command(self, command, expectPrefix = None, idle = None, notReady = False):
        self.write(command, notReady)
        return self.expect(expectPrefix, idle, notReady) if expectPrefix is not None else None
