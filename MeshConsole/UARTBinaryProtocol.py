#!/usr/bin/python
#
# Ostranna UART binary protocol
# See UARTBinaryCommands.py for particular values
#
# See https://docs.google.com/document/d/1J2z4WCSR-WekH4tEe7bfqvjPJ0Hx7Ww-26ECBoLwl4s
#
from binascii import hexlify, unhexlify
from itertools import chain
from re import sub

# Value types
TYPE_MIN = TYPE_STR = -1
TYPE_UNSIGNED_HEX_LE = 0
TYPE_SIGNED_HEX_LE = 1
TYPE_UNSIGNED_HEX_BE = 2
TYPE_SIGNED_HEX_BE = 3
TYPE_UNSIGNED_DEC = 4
TYPE_MAX = TYPE_SIGNED_DEC = 5

TYPES_SIGNED = (TYPE_SIGNED_HEX_LE, TYPE_SIGNED_HEX_BE, TYPE_SIGNED_DEC)
TYPES_HEX = (TYPE_SIGNED_HEX_LE, TYPE_UNSIGNED_HEX_LE, TYPE_SIGNED_HEX_BE, TYPE_UNSIGNED_HEX_BE)
TYPES_LITTLE_ENDIAN = (TYPE_SIGNED_HEX_LE, TYPE_UNSIGNED_HEX_LE)
TYPES_DEC = (TYPE_SIGNED_DEC, TYPE_UNSIGNED_DEC)

class Param(object):
    HEX_FORMAT = '%%0%dX'

    def __init__(self, length = 1, typ = TYPE_UNSIGNED_HEX_LE):
        if not TYPE_MIN <= typ <= TYPE_MAX:
            raise ValueError("Unknown parameter type %r" % typ)
        if length < 0:
            raise ValueError("Bad length %r" % length)
        self.typ = typ
        self.isStr = self.typ == TYPE_STR
        self.isSigned = self.typ in TYPES_SIGNED
        self.isDecCoded = self.typ in TYPES_DEC
        self.isLittleEndian = self.typ in TYPES_LITTLE_ENDIAN
        self.length = length
        self.encodedLength = self.length or 1
        if self.length:
            if self.isDecCoded:
                limit = 100 ** length - 1
                if self.isSigned:
                    (self.limitMin, self.limitMax) = (-(limit // 10), limit)
                else:
                    (self.limitMin, self.limitMax) = (-limit, limit)
            else:
                limit = 0x100 ** length
                if self.isSigned:
                    (self.limitMin, self.limitMax) = (-limit // 2, limit // 2 - 1)
                else:
                    (self.limitMin, self.limitMax) = (0, limit - 1)
                self.ret = self.HEX_FORMAT % (2 * self.length)
        if not self.isStr:
            self.zero = None
            self.zero = self.encode(0)

    @staticmethod
    def encodeDecHex(value):
        assert value >= 0
        if value == 0:
            return ''
        (hundreds, ones) = divmod(value, 100)
        return '%s%02d' % (Param.encodeDecHex(hundreds), ones)

    def encode(self, value):
        if self.isStr:
            if not value:
                raise ValueError("Empty value")
            try:
                value = str(value)
            except UnicodeError:
                raise ValueError("Unsupported unicode value: %s" % value)
            if self.length and len(value) != self.length:
                raise ValueError("Bad value length %d, expected %d: %s" % (len(value), self.length, value))
            ret = hexlify(value)
        elif not self.isSigned and value < 0:
            raise ValueError("Negative value for unsigned parameter: %r" % value)
        elif self.length and not self.limitMin <= value <= self.limitMax:
            raise ValueError("Value is out of range [%d..%d]: %r" % (self.limitMin, self.limitMax, value))
        elif self.isDecCoded:
            if value:
                ret = self.encodeDecHex(abs(value)).rjust(2 * self.length, '0')
                if value < 0:
                    ret = 'F' + ret[1:] if ret[0] == '0' else 'F0' + ret
            else: # value is 0
                ret = self.zero or '0' * 2 * (self.length or 1)
        else: # pure hex
            if self.length:
                if self.isSigned and value < 0:
                    value += 2 * (self.limitMax + 1)
                ret = self.ret % value
            else: # length not specified
                length = (len('%x' % (value if not self.isSigned else 2 * (value if value >= 0 else -value - 1))) + 1) // 2
                if value < 0:
                    value += 0x100 ** length
                ret = self.HEX_FORMAT % (2 * length) % value
            if self.isLittleEndian:
                assert len(ret) % 2 == 0
                ret = ''.join(ret[i - 2: i] for i in xrange(len(ret), 0, -2))
        assert len(ret) % 2 == 0
        return ret

    @staticmethod
    def decodeDecHex(data):
        assert data
        return reduce(lambda h, c: 100 * h + c, (t * 10 + c for (t, c) in (divmod(ord(d), 16) for d in data)))

    def decode(self, data):
        if len(data) % 2:
            raise ValueError("Odd data length %d: %s" % (len(data), data))
        try:
            data = unhexlify(data)
        except TypeError:
            raise ValueError("Bad hexlified format data: %s" % data)
        if self.length:
            if len(data) != self.encodedLength:
                raise ValueError("Got %d bytes of data, expected %d: %s" % (len(data), self.encodedLength, data))
        elif len(data) < self.encodedLength:
            raise ValueError("Got %d bytes of data, expected at least %d bytes: %s" % (len(data), self.encodedLength, data))
        if self.isStr:
            return data
        if self.isDecCoded:
            if self.isSigned:
                sign = data[0]
                if sign >= '\xA0':
                    return -self.decodeDecHex(chr(ord(sign) % 16) + data[1:])
                return self.decodeDecHex(data)
            else:
                return self.decodeDecHex(data)
        else: # pure hex
            if self.isLittleEndian:
                data = data[::-1]
            value = reduce(lambda h, c: 0x100 * h + c, (ord(c) for c in data), 0)
            if self.isSigned:
                limit = self.limitMax + 1 if self.length else 0x100 ** len(data) // 2
                if value >= limit:
                    value -= 2 * limit
            return value

    def test(self, value, data):
        assert self.encode(value) == data.upper(), "encode(%s) is %s, not %s" % (value, self.encode(value), data)
        assert self.decode(data) == value, "decode(%s) is %s, not %s" % (data, self.decode(data), value)

class Command(object):
    MARKER = '#'
    MAX_TOTAL_LENGTH = 252
    commands = {}

    def __init__(self, tag, params = None, reply = None):
        if len(tag) != 1:
            raise ValueError("Bad tag length %d, expected 1: %r" % (len(tag), tag))
        self.tag = tag
        self.encodedTag = hexlify(tag).upper()
        self.prefix = self.MARKER + self.encodedTag
        self.params = tuple(params or ())
        self.minLength = self.maxLength = 0
        for (i, param) in enumerate(self.params):
            self.minLength += param.encodedLength
            if not param.length:
                if i < len(params) - 1:
                    raise ValueError("Zero length on non-last parameter")
                self.maxLength = self.MAX_TOTAL_LENGTH
        if self.minLength > self.MAX_TOTAL_LENGTH:
            raise ValueError("Too long encoded length %d, must be no more than %d" % (self.minLength, self.MAX_TOTAL_LENGTH))
        self.maxLength = self.maxLength or self.minLength
        self.reply = reply
        self.commands[tag] = self

    @classmethod
    def checkReplies(cls):
        for (tag, command) in cls.commands.iteritems():
            if command.reply and command.reply not in cls.commands:
                raise ValueError("Unknown reply %s for command %s" % (hexlify(command.reply).upper(), hexlify(tag).upper()))

    @classmethod
    def getCommand(cls, tag):
        tag = tag.strip()
        if not tag:
            raise ValueError("Empty command tag")
        if len(tag) > 1:
            if tag.startswith(cls.MARKER):
                tag = tag[len(cls.MARKER):]
            try:
                tag = unhexlify(tag)
            except TypeError:
                pass
        command = cls.commands.get(tag)
        if not command:
            raise ValueError("Unknown command tag: %s" % tag)
        return command

    def encode(self, *args):
        if len(args) != len(self.params):
            raise ValueError("Bad number of arguments for command %s: %d, expected %s" % (self.encodedTag, len(args), len(self.params)))
        return ','.join(chain((self.prefix,), (param.encode(arg) for (param, arg) in zip(self.params, args))))

    @classmethod
    def encodeCommand(cls, tag, *args):
        return cls.getCommand(tag).encode(*args)

    def decodeParams(self, data): # generator
        index = 0
        for param in self.params:
            if param.length:
                yield param.decode(data[index : index + 2 * param.encodedLength])
                index += 2 * param.encodedLength
            else:
                yield param.decode(data[index:])
                break # not in fact needed, zero length may only occur in last parameter

    def decode(self, data):
        data = sub(' *[, ] *', '', unicode(data).strip())
        if data.startswith(self.MARKER):
            tag = data[len(self.MARKER) : len(self.MARKER) + 2].upper()
            if tag != self.encodedTag:
                raise ValueError("Wrong command tag %s, expected %s" % (tag, self.encodedTag))
            data = data[len(self.MARKER) + 2:]
        if len(data) < 2 * self.minLength:
            raise ValueError("Data too short, got %d bytes, expected at least %d bytes: %s" % (len(data), self.minLength, data))
        if len(data) > 2 * self.maxLength:
            raise ValueError("Data too long, got %d bytes, expected at most %d bytes: %s" % (len(data), self.maxLength, data))
        return tuple(self.decodeParams(data))

    @classmethod
    def decodeCommand(cls, data):
        data = unicode(data).strip()
        if len(data) < 2 + len(cls.MARKER) or not data.startswith(cls.MARKER):
            return (None, None)
        data = data[len(cls.MARKER):]
        hexTag = data[:2].upper()
        try:
            tag = unhexlify(hexTag)
        except TypeError:
            return (None, None)
        command = cls.commands.get(tag)
        if not command:
            return (tag, None)
        return (tag, command.decode(data[2:]))

    @classmethod
    def testCommand(cls, tag, data, *args):
        assert cls.encodeCommand(tag, *args) == data.upper(), "encodeCommand(%s, %s) is %s, not %s" % (hexlify(tag).upper(), ', '.join(repr(arg) for arg in args), cls.encodeCommand(tag, *args), data)
        assert cls.decodeCommand(data) == (tag, args), "decodeCommand(%s, %s) is %s, not %s" % (hexlify(tag).upper(), data, cls.decodeCommand(data), (tag, args))

def testParam():
    p = Param(0)
    p.test(0, '00')
    p.test(1, '01')
    p.test(0xff, 'FF')
    p = Param(4)
    p.test(0, '00000000')
    p.test(1, '01000000')
    p.test(0xff00ff00, '00FF00FF')
    p = Param(0, TYPE_SIGNED_HEX_LE)
    p.test(0, '00')
    p.test(-0xffff, '0100FF')
    p.test(-0xfffffffffffffffffff, '010000000000000000F0')
    p.test(1, '01')
    p.test(-1, 'FF')
    p.test(127, '7F')
    p.test(-127, '81')
    p.test(-128, '80')
    p.test(128, '8000')
    p.test(0xff, 'FF00')
    p.test(0xff00ff00, '00FF00FF00')
    p = Param(1, TYPE_SIGNED_HEX_LE)
    p.test(0, '00')
    p.test(1, '01')
    p.test(-1, 'FF')
    p.test(127, '7F')
    p.test(-127, '81')
    p.test(-128, '80')
    p = Param(4, TYPE_SIGNED_HEX_LE)
    p.test(0, '00000000')
    p.test(1, '01000000')
    p.test(0xff00ff, 'FF00FF00')
    p = Param(0, TYPE_UNSIGNED_HEX_BE)
    p.test(0, '00')
    p.test(1, '01')
    p.test(0xff, 'FF')
    p = Param(4, TYPE_UNSIGNED_HEX_BE)
    p.test(0, '00000000')
    p.test(1, '00000001')
    p.test(0xff00ff00, 'FF00FF00')
    p = Param(0, TYPE_SIGNED_HEX_BE)
    p.test(0, '00')
    p.test(-0xffff, 'FF0001')
    p.test(-0xfffffffffffffffffff, 'F0000000000000000001')
    p.test(1, '01')
    p.test(-1, 'FF')
    p.test(127, '7F')
    p.test(-127, '81')
    p.test(-128, '80')
    p.test(128, '0080')
    p.test(0xff, '00FF')
    p.test(0xff00ff00, '00FF00FF00')
    p = Param(1, TYPE_SIGNED_HEX_BE)
    p.test(0, '00')
    p.test(1, '01')
    p.test(-1, 'FF')
    p.test(127, '7F')
    p.test(-127, '81')
    p.test(-128, '80')
    p = Param(4, TYPE_SIGNED_HEX_BE)
    p.test(0, '00000000')
    p.test(1, '00000001')
    p.test(0xff00ff, '00FF00FF')
    p = Param(0, TYPE_UNSIGNED_DEC)
    p.test(0, '00')
    p.test(1, '01')
    p.test(99, '99')
    p.test(100, '0100')
    p.test(127, '0127')
    p.test(128, '0128')
    p.test(0xff00ff00, '4278255360')
    p.test(999999999, '0999999999')
    p = Param(1, TYPE_UNSIGNED_DEC)
    p.test(0, '00')
    p.test(1, '01')
    p.test(99, '99')
    p = Param(4, TYPE_UNSIGNED_DEC)
    p.test(0, '00000000')
    p.test(1, '00000001')
    p.test(0xff00ff, '16711935')
    p = Param(0, TYPE_SIGNED_DEC)
    p.test(0, '00')
    p.test(1, '01')
    p.test(-1, 'F1')
    p.test(99, '99')
    p.test(-99, 'F099')
    p.test(100, '0100')
    p.test(127, '0127')
    p.test(-127, 'F127')
    p.test(-128, 'F128')
    p.test(128, '0128')
    p.test(0xff00ff00, '4278255360')
    p.test(-0xff00ff00, 'F04278255360')
    p = Param(1, TYPE_SIGNED_DEC)
    p.test(0, '00')
    p.test(1, '01')
    p.test(-1, 'F1')
    p.test(99, '99')
    p = Param(4, TYPE_SIGNED_DEC)
    p.test(0, '00000000')
    p.test(1, '00000001')
    p.test(-1, 'F0000001')
    p.test(0xff00ff, '16711935')
    p.test(-9999999, 'F9999999')
    p = Param(0, TYPE_STR)
    p.test('a', '61')
    p.test('abcd', '61626364')
    p = Param(1, TYPE_STR)
    p.test('a', '61')
    p = Param(2, TYPE_STR)
    p.test('aa', '6161')

def testCommand():
    Command('\x00', (Param(), Param(4, TYPE_SIGNED_HEX_BE), Param(1, TYPE_SIGNED_DEC), Param(0, TYPE_STR)), '\x01')
    Command('\x01', (Param(2), Param(1, TYPE_UNSIGNED_DEC), Param(4, TYPE_SIGNED_DEC), Param(0, TYPE_SIGNED_HEX_BE)), '\x02')
    Command('\x02', (Param(1, TYPE_SIGNED_HEX_BE), Param(4, TYPE_UNSIGNED_DEC), Param(1, TYPE_STR), Param(0, TYPE_SIGNED_DEC)), '\x03')
    Command('\x03', (Param(0, TYPE_UNSIGNED_HEX_BE),), '\x04')
    Command('\x04', (Param(0, TYPE_UNSIGNED_DEC),), '\x05')
    Command('\x05', ())
    Command.checkReplies()
    Command.testCommand('\x00', '#00,FF,7FFFFFFF,F9,3939393939393939', 0xff, 0x7fffffff, -9, '99999999')
    Command.testCommand('\x01', '#01,0010,99,99999999,FF00000000000000000001', 0x1000, 99, 99999999, -0xffffffffffffffffffff)
    Command.testCommand('\x02', '#02,80,99999999,61,F999999999999999', -0x80, 99999999, 'a', -999999999999999)
    Command.testCommand('\x03', '#03,FFFFFFFFFFFFFFFFFFFF', 0xffffffffffffffffffff)
    Command.testCommand('\x04', '#04,0999999999999999', 999999999999999)
    Command.testCommand('\x05', '#05')
    Command.commands.clear()

if __name__ == '__main__':
    testParam()
    testCommand()
