#!/usr/bin/python
#
# Ostranna UART text protocol
# See UARTTextCommands.py for particular values
#
# See https://docs.google.com/document/d/1J2z4WCSR-WekH4tEe7bfqvjPJ0Hx7Ww-26ECBoLwl4s
#
from binascii import hexlify, unhexlify
from functools import partial
from itertools import chain
from re import compile as reCompile

INT_LIMIT = 2 ** 32

COMMAND_MARKER = '#'

SEPARATOR = ','

SEPARATORS = reCompile(' *[, ] *')

REPEAT = 'repeat'

MAX_COMMAND_ARGS = 100

MAX_COMMAND_LENGTH = 384

def nop(s):
    return s

def checkStr(s):
    for c in s:
        if not 33 <= ord(c) <= 126:
            raise ValueError("Bad character in string: %r" % c)
    return s

def hexStr(s):
    return hexlify(s).upper()

def formatStr(fmt, value):
    return fmt % value

def hexInt(i):
    assert -INT_LIMIT // 2 <= i < INT_LIMIT // 2
    return '0x%X' % (i % INT_LIMIT)

def parseInt(s):
    if s.lower().startswith('0x'):
        value = int(s, 16)
        if value >= INT_LIMIT // 2:
            value -= INT_LIMIT
        return value
    return int(s)

FORMATS = { # 'format': (encoder, decoder)
    'd': (partial(formatStr, '%d'), parseInt),
    'i': (partial(formatStr, '%i'), parseInt),
    'x': (hexInt, parseInt),
    'f': (partial(formatStr, '%f'), float),
    's': (checkStr, nop),
    'h': (hexStr, unhexlify),
    '*': (REPEAT, REPEAT)
}

class Command(object):
    commands = {}

    def __init__(self, tag, args = None, reply = None):
        if not tag:
            raise ValueError("Bad tag: %r" % tag)
        if args and len(args) > MAX_COMMAND_ARGS:
            raise ValueError("Bad number of arguments for command %s, expected at most %d, found %d: %s" % (tag, MAX_COMMAND_ARGS, len(args), ' '.join(args)))
        self.tag = tag
        self.prefix = COMMAND_MARKER + self.tag
        self.encoders = []
        self.decoders = []
        self.varArgs = False
        if args:
            try:
                for (encoder, decoder) in (FORMATS[c.lower()] for c in args):
                    if self.varArgs:
                        raise ValueError("Repeat argument * may only be the last one")
                    if encoder == REPEAT:
                        if not self.encoders:
                            raise ValueError("Repeat argument * must be preceded by a conventional argument")
                        self.varArgs = True
                    else:
                        self.encoders.append(encoder)
                        self.decoders.append(decoder)
            except KeyError, e:
                raise ValueError("Unknown format tag: %s" % e.message)
        self.reply = reply
        if tag.lower() in self.commands:
            raise ValueError("Duplicate command tag: %s" % tag)
        self.commands[tag.lower()] = self

    @classmethod
    def linkReplies(cls):
        for (tag, command) in cls.commands.iteritems():
            if command.reply and not isinstance(command.reply, Command):
                reply = cls.commands.get(command.reply.lower())
                if reply:
                    command.reply = reply
                else:
                    raise ValueError("Unknown reply %s for command %s" % (command.reply, tag))

    @classmethod
    def getCommand(cls, tag):
        tag = tag.strip()
        if not tag:
            raise ValueError("Empty command tag")
        if tag.startswith(COMMAND_MARKER):
            tag = tag[len(COMMAND_MARKER):]
        command = cls.commands.get(tag.lower())
        if not command:
            raise ValueError("Unknown command tag: %s" % tag)
        return command

    def encode(self, *args):
        if self.varArgs:
            if len(args) < len(self.encoders):
                raise ValueError("Bad number of arguments for command %s, expected at least %d, found %d: %s" % (self.tag, len(self.encoders), len(args), ' '.join(str(arg) for arg in args)))
            if len(args) > MAX_COMMAND_ARGS:
                raise ValueError("Bad number of arguments for command %s, expected at most %d, found %d: %s" % (self.tag, MAX_COMMAND_ARGS, len(args), ' '.join(str(arg) for arg in args)))
        elif len(args) != len(self.encoders):
            raise ValueError("Bad number of arguments for command %s, expected %d, found %d: %s" % (self.tag, len(self.encoders), len(args), ' '.join(str(arg) for arg in args)))
        ret = COMMAND_MARKER + self.tag + SEPARATOR.join(chain(('',), (encoder(arg) for (encoder, arg) in zip(self.encoders, args)), (self.encoders[-1](arg) for arg in args[len(self.encoders):])))
        if len(ret) > MAX_COMMAND_LENGTH:
            raise ValueError("Encoded command length %d larger than maximum %d: %s" % (len(ret), MAX_COMMAND_LENGTH, ret))
        return ret

    @classmethod
    def encodeCommand(cls, tag, *args):
        return cls.getCommand(tag).encode(*args)

    def decodeArgs(self, args):
        if self.varArgs:
            if len(args) < len(self.encoders):
                raise ValueError("Bad number of arguments for command %s, expected at least %d, found %d: %s" % (self.tag, len(self.decoders), len(args), ' '.join(args)))
        elif len(args) != len(self.encoders):
            raise ValueError("Bad number of arguments for command %s, expected %d, found %d: %s" % (self.tag, len(self.decoders), len(args), ' '.join(args)))
        return tuple(chain((decoder(arg) for (decoder, arg) in zip(self.decoders, args)), (self.decoders[-1](arg) for arg in args[len(self.decoders):])))

    def decode(self, data):
        (tag, args) = self.decodeCommand(data)
        if tag.lower() != self.tag.lower():
            raise ValueError("Bad tag %s, expected %s: %s" % (tag, self.tag, data))
        return args

    @classmethod
    def decodeCommand(cls, data):
        data = unicode(data).strip()
        if not data.startswith(COMMAND_MARKER):
            return (None, None)
        words = SEPARATORS.split(data[len(COMMAND_MARKER):])
        tag = words[0].lower()
        command = cls.commands.get(tag.lower())
        if not command:
            return (tag, None)
        return (command.tag, command.decodeArgs(words[1:]))

    @classmethod
    def testCommand(cls, tag, data, *args):
        assert cls.encodeCommand(tag, *args) == data, "encodeCommand(%s, %s) is %s, not %s" % (tag, ', '.join(repr(arg) for arg in args), cls.encodeCommand(tag, *args), data)
        assert cls.decodeCommand(data) == (tag, args), "decodeCommand(%s, %s) is %r, not %r" % (tag, data, cls.decodeCommand(data), (tag, args))

def testFormat(fmt, value, data):
    (encoder, decoder) = FORMATS[fmt]
    assert encoder(value) == data, "encoder(%r) is %s, not %s" % (value, encoder(value), data)
    assert decoder(data) == value, "decoder(%s) is %r, not %r" % (data, decoder(data), value)

def testFormats(): # ToDo: Add negative tests
    testFormat('d', 0, '0')
    testFormat('d', -1, '-1')
    testFormat('d', 2 ** 32, '4294967296')
    testFormat('d', 2 ** 31, '2147483648')
    testFormat('d', 2 ** 31 - 1, '2147483647')
    testFormat('d', -2 ** 31, '-2147483648')
    testFormat('i', 0, '0')
    testFormat('i', -1, '-1')
    testFormat('i', 2 ** 32, '4294967296')
    testFormat('i', 2 ** 31, '2147483648')
    testFormat('i', 2 ** 31 - 1, '2147483647')
    testFormat('i', -2 ** 31, '-2147483648')
    testFormat('x', 0, '0x0')
    testFormat('x', -1, '0xFFFFFFFF')
    testFormat('x', 2 ** 30, '0x40000000')
    testFormat('x', 2 ** 31 - 1, '0x7FFFFFFF')
    testFormat('x', -2 ** 31, '0x80000000')
    testFormat('f', 0, '0.000000')
    testFormat('f', 1, '1.000000')
    testFormat('f', -1, '-1.000000')
    testFormat('f', -0.000001, '-0.000001')
    testFormat('s', '', '')
    testFormat('s', 'a', 'a')
    testFormat('s', 'aBc', 'aBc')
    testFormat('h', '', '')
    testFormat('h', 'a', '61')
    testFormat('h', '-aBc\xa8\xff', '2D614263A8FF')

def testCommands(): # ToDo: Add negative tests
    Command('0', 'dixfsh', '1')
    Command('1', 'xdsfih*', '2')
    Command('2', 'fdhisx', '3')
    Command('3', 'hsxidf', '4')
    Command('4', 'i*', '5')
    Command('5', '')
    Command.linkReplies()
    Command.testCommand('0', '#0,0,-1,0x40000000,1.000000,a,61', 0, -1, 2 ** 30, 1, 'a', 'a')
    Command.testCommand('1', '#1,0x0,-1,,0.000000,0,614263,446546,674869', 0, -1, '', 0, 0, 'aBc', 'DeF', 'gHi')
    Command.testCommand('2', '#2,-1.000000,2147483648,,-2147483648,aBc,0x80000000', -1, 2 ** 31, '', -2 ** 31, 'aBc', -2 ** 31)
    Command.testCommand('3', '#3,,,0x80000000,-1,255,-0.000001', '', '', -2 ** 31, -1, 255, -0.000001)
    Command.testCommand('4', '#4,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15', 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 11, 12, 13, 14, 15)
    Command.testCommand('4', '#4,0', 0)
    Command.testCommand('5', '#5')
    Command.commands.clear()

if __name__ == '__main__':
    testFormats()
    testCommands()
