#!/usr/bin/python
#
# Mesh Console
# Serial port operation routines
#
from collections import deque
from logging import DEBUG, INFO, WARNING
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

    def readline(self):
        data = None
        while data is None:
            if self.buffer:
                data = self.buffer.popleft()
            else:
                sleep(float(randint(0, 1000)) / 1000)
                if self.ready and not self.buffer:
                    num = randint(1, 100)
                    data = 'node %s %s %s %s %s %s %s' % (num, 0 if num == 1 else randint(1, 5), randint(0, 10000), randint(0, 200) - 100, randint(0, 20), randint(0, 20), randint(0, 20))
        return data

    def write(self, data):
        numLines = data.count('\n')
        if numLines:
            self.ready = True
            self.buffer.extend(self.pong for i in xrange(numLines))
        return len(data)

class SerialPort(object):
    TRYING = 0
    CONNECTED = 1
    VERIFIED = 2
    ERROR = 3
    NONE = 4

    def __init__(self, ping = None, pong = None, connectCallBack = None, readCallBack = None, portTryCallBack = None, loggingCallBack = None):
        self.ping = ping
        self.pong = pong
        self.connectCallBack = connectCallBack
        self.readCallBack = readCallBack
        self.portTryCallBack = portTryCallBack
        self.loggingCallBack = loggingCallBack
        self.writeBuffer = deque()
        self.port = None
        readThread = Thread(target = self.reader, name = '%s 0x%x' % (self.__class__.__name__, id(self)))
        readThread.setDaemon(True)
        readThread.start()
        writeThread = Thread(target = self.writer, name = '%s 0x%x' % (self.__class__.__name__, id(self)))
        writeThread.setDaemon(True)
        writeThread.start()

    def debug(self, message):
        self.loggingCallBack(DEBUG, message)

    def info(self, message):
        self.loggingCallBack(INFO, message)

    def warning(self, message):
        self.loggingCallBack(WARNING, message)

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
                    self.info("< %s" % line)
                    if self.readCallBack:
                        self.readCallBack(line)
            except Exception:
                self.warning("connection broken")

    def writer(self):
        while True:
            if self.writeBuffer:
                data = self.writeBuffer.popleft()
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
                sleep(1)

    def connect(self):
        self.port = None
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
                        self.info("connected to %s" % portName)
                        if self.ping:
                            pong = self.command(self.ping, self.pong or '')
                            if pong != None:
                                if self.connectCallBack:
                                    self.connectCallBack(pong)
                                return pong
                            self.port.close()
                    except Exception:
                        self.statusUpdate(displayPortName, self.ERROR)
            else:
                self.statusUpdate("No COM", self.NONE)

    def write(self, data):
        self.info(" > %s" % data)
        self.writeBuffer.extend('%s\n' % data)

    def expect(self, prefix):
        timeout = time() + self.port.timeout
        while time() < timeout:
            line = self.port.readline()
            if line:
                self.info("< %s" % line)
                if line.startswith(prefix):
                    return line

    def command(self, command, expectedPrefix = None):
        self.write(command)
        return self.expect(expectedPrefix) if expectedPrefix != None else None
