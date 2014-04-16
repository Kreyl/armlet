#!/usr/bin/python
#
# Mesh Console
# Serial port operation routines
#
from collections import deque
from random import randint
from re import sub
from threading import Thread
from time import sleep, time

try:
    from serial import Serial, SerialTimeoutException
    from serial.tools.list_ports import comports
except ImportError, ex:
    raise ImportError("%s: %s\n\nPlease install pySerial v2.6 or later: http://pypi.python.org/pypi/pyserial\n" % (ex.__class__.__name__, ex))

EMULATED = True

BAUD_RATE = 115200

TIMEOUT = 1
DT = 0.1

class EmulatedSerial(object):
    def __init__(self, timeout, pong):
        self.timeout = timeout
        self.pong = pong
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
                data = 'node %s %s %s %s %s %s %s' % (num, 0 if num == 1 else randint(1, 5), randint(0, 10000), randint(0, 200) - 100, randint(0, 20), randint(0, 20), randint(0, 20))
                break
            sleep(DT)
        return data

    def write(self, data):
        self.buffer.append(self.pong)
        self.ready = True
        return len(data)

class SerialPort(object):
    TRYING = 0
    CONNECTED = 1
    VERIFIED = 2
    ERROR = 3
    NONE = 4

    def __init__(self, logger, ping = None, pong = '', connectCallBack = None, readCallBack = None, portTryCallBack = None):
        self.logger = logger
        self.ping = ping
        self.pong = pong
        self.connectCallBack = connectCallBack
        self.readCallBack = readCallBack
        self.portTryCallBack = portTryCallBack
        self.writeBuffer = deque()
        self.port = None
        self.ready = None
        self.expectTimeout = None
        self.expectPrefix = None
        self.expectResult = None
        readThread = Thread(target = self.reader, name = '%s 0x%x reader' % (self.__class__.__name__, id(self)))
        readThread.setDaemon(True)
        readThread.start()
        writeThread = Thread(target = self.writer, name = '%s 0x%x writer' % (self.__class__.__name__, id(self)))
        writeThread.setDaemon(True)
        writeThread.start()

    def statusUpdate(self, portName, portStatus):
        if self.portTryCallBack:
            self.portTryCallBack(portName, portStatus)

    def reader(self):
        while True:
            if not self.port:
                self.connect()
            try:
                line = self.port.readline()
                if line:
                    self.logger.info("< %s" % line)
                    if time() < self.expectTimeout and line.startswith(self.expectPrefix):
                        self.expectResult = line
                    elif self.readCallBack:
                        self.readCallBack(line)
            except Exception:
                self.logger.warning("connection broken")
                self.port = None
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
                    self.port = None
            else:
                sleep(DT)

    def connect(self):
        self.port = None
        self.ready = None
        while True:
            self.statusUpdate("SCAN", self.TRYING)
            sleep(DT)
            portNames = tuple(portName for (portName, _description, _address) in comports())
            if portNames:
                for portName in portNames:
                    try:
                        displayPortName = sub('^/dev/', '', portName)
                        self.statusUpdate(displayPortName, self.TRYING)
                        if EMULATED:
                            self.port = EmulatedSerial(timeout = TIMEOUT, pong = self.pong)
                        else:
                            self.port = Serial(portName, baudrate = BAUD_RATE, timeout = TIMEOUT, writeTimeout = TIMEOUT)
                        self.statusUpdate(displayPortName, self.CONNECTED)
                        self.logger.info("connected to %s" % portName)
                        if self.ping:
                            self.logger.info(" > %s" % self.ping)
                            self.port.write('%s\n' % self.ping)
                            timeout = time() + self.port.timeout
                            while time() < timeout:
                                line = self.port.readline()
                                if line:
                                    self.logger.info("< %s" % line.rstrip())
                                    if line.startswith(self.pong):
                                        if self.connectCallBack:
                                            self.connectCallBack(line)
                                        self.ready = True
                                        return line
                            self.port.close()
                    except Exception:
                        self.statusUpdate(displayPortName, self.ERROR)
            else:
                self.statusUpdate("No COM", self.NONE)

    def write(self, data):
        if self.port and self.ready:
            self.logger.info(" > %s" % data)
            self.writeBuffer.append(data)
        else:
            self.logger.info(" >! %s" % data)

    def expect(self, prefix, idle = None):
        if self.port and self.ready:
            self.expectPrefix = prefix
            self.expectResult = None
            self.expectTimeout = time() + self.port.timeout
            while self.expectResult is None and time() < self.expectTimeout:
                idle()
            idle()
            self.expectTimeout = None
            return self.expectResult

    def command(self, command, expectPrefix = None, idle = None):
        self.write(command)
        return self.expect(expectPrefix, idle) if expectPrefix != None else None
