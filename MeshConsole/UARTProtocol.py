#!/usr/bin/python
#
# Ostranna UART protocol
# See UARTCommands.py for particular values
#
# See https://docs.google.com/document/d/1J2z4WCSR-WekH4tEe7bfqvjPJ0Hx7Ww-26ECBoLwl4s
#
from binascii import hexlify, unhexlify
from re import sub

# Value types
TYPE_MIN = TYPE_STR = -1
TYPE_UNSIGNED_HEX = 0
TYPE_SIGNED_HEX = 1
TYPE_UNSIGNED_DEC = 2
TYPE_MAX = TYPE_SIGNED_DEC = 3

TYPES_SIGNED = (TYPE_SIGNED_HEX, TYPE_SIGNED_DEC)
TYPES_HEX = (TYPE_SIGNED_HEX, TYPE_UNSIGNED_HEX)
TYPES_DEC = (TYPE_SIGNED_DEC, TYPE_UNSIGNED_DEC)

class Param(object):
    HEX_FORMAT = '%%0%dX'
    SIGN_PLUS = '\x00' # ToDo: Not use a special byte for positive values
    SIGN_MINUS = '\xff' # ToDo: Use only half-byte F to indicate negative value
    ENCODE_SIGNS = {True: hexlify(SIGN_PLUS).upper(), False: hexlify(SIGN_MINUS).upper()}
    DECODE_SIGNS = {SIGN_PLUS: 1, SIGN_MINUS: -1}

    def __init__(self, length = 1, typ = TYPE_UNSIGNED_HEX):
        if not TYPE_MIN <= typ <= TYPE_MAX:
            raise ValueError("Unknown parameter type %r" % typ)
        if length < 0:
            raise ValueError("Bad length %r" % length)
        self.typ = typ
        self.isStr = self.typ == TYPE_STR
        self.isSigned = self.typ in TYPES_SIGNED
        self.isDecCoded = self.typ in TYPES_DEC
        self.length = length
        self.encodedLength = (self.length or 1) + int(self.isDecCoded and self.isSigned)
        if self.length:
            self.limit = 100 ** length - 1 if self.isDecCoded else 0x100 ** length // 2 if self.isSigned else 0x100 ** length - 1
            if not self.isDecCoded:
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
            if self.length and len(value) != self.length:
                raise ValueError("Bad value length %d, expected %d" % (len(value), self.length))
            return hexlify(value)
        if not self.isSigned and value < 0:
            raise ValueError("Negative value %d for unsigned parameter" % value)
        if self.isDecCoded:
            sign = '' if not self.isSigned else self.ENCODE_SIGNS[value >= 0]
            if value:
                if self.length and abs(value) > self.limit:
                    raise ValueError("Value %r is out of range [%d..%d]" % (value, -self.limit, self.limit))
                return sign + self.encodeDecHex(abs(value)).rjust(2 * self.length, '0')
            else: # value is 0
                return self.zero or sign + '0' * 2 * (self.length or 1)
        else: # pure hex
            if self.length:
                if self.isSigned:
                    if not -self.limit <= value < self.limit:
                        raise ValueError("Value %d is out of range [%d..%d]" % (value, -self.limit, self.limit - 1))
                    if value < 0:
                        value += 2 * self.limit
                elif value > self.limit: # unsigned
                    raise ValueError("Value %d is out of range [0..%d]" % (value, self.limit))
                ret = self.ret
            else: # length not specified
                length = (len('%x' % (value if not self.isSigned else 2 * (value if value >= 0 else -value - 1))) + 1) // 2
                if value < 0:
                    value += 0x100 ** length
                ret = self.HEX_FORMAT % (2 * length)
            return ret % value

    @staticmethod
    def decodeDecHex(data):
        assert data
        return reduce(lambda h, c: 100 * h + c, (t * 10 + c for (t, c) in (divmod(ord(d), 16) for d in data)))

    def decode(self, data):
        data = unhexlify(data)
        if self.length:
            if len(data) != self.encodedLength:
                raise ValueError("Got %d bytes of data, expected %d" % (len(data), self.encodedLength))
        elif len(data) < self.encodedLength:
            raise ValueError("Got %d bytes of data, expected at least %d bytes" % (len(data), self.encodedLength))
        if self.isStr:
            return data
        if self.isDecCoded:
            if self.isSigned:
                sign = self.DECODE_SIGNS.get(data[0])
                if not sign:
                    raise ValueError("Bad sign byte in value %s" % hexlify(data).upper())
                return sign * self.decodeDecHex(data[1:])
            else:
                return self.decodeDecHex(data)
        else: # pure hex
            value = reduce(lambda h, c: 0x100 * h + c, (ord(c) for c in data), 0)
            if self.isSigned:
                limit = self.limit if self.length else 0x100 ** len(data) // 2
                if value >= limit:
                    value -= 2 * limit
            return value

    def test(self, value, data):
        assert self.encode(value) == data.upper(), "encode(%s) is %s, not %s" % (value, self.encode(value), data)
        assert self.decode(data) == value, "decode(%s) is %s, not %s" % (data, self.decode(data), value)

class Command(object):
    MAX_TOTAL_LENGTH = 252
    commands = {}

    def __init__(self, tag, params = None, reply = None):
        self.tag = tag
        self.encodedTag = hexlify(chr(tag)).upper()
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
                raise ValueError("Unknown reply %s for command %s" % (hexlify(chr(command.reply)).upper(), hexlify(chr(tag)).upper()))

    def encode(self, *args):
        if len(args) != len(self.params):
            raise ValueError("Bad number of arguments for command %s: %d, expected %s" % (self.encodedTag, len(args), len(self.params)))
        return '#%s,%s\n' % (self.encodedTag, ','.join(param.encode(arg) for (param, arg) in zip(self.params, args)))

    @classmethod
    def encodeCommand(cls, tag, *args):
        command = cls.commands.get(tag)
        if not command:
            raise ValueError("Unknown command %s" % hexlify(chr(tag)).upper())
        return command.encode(*args)

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
        if len(data) < 2 * self.minLength:
            raise ValueError("Data too short, got %d bytes, expected at least %d bytes" % (len(data), self.minLength))
        if len(data) > 2 * self.maxLength:
            raise ValueError("Data too long, got %d bytes, expected at most %d bytes" % (len(data), self.maxLength))
        return (self.tag, tuple(self.decodeParams(data)))

    @classmethod
    def decodeCommand(cls, data):
        data = data.strip()
        if len(data) < 2 and not data.startswith('#'):
            return (None, None)
        data = sub('[, ]+', '', data[1:])
        tag = ord(unhexlify(data[:2]))
        command = cls.commands.get(tag)
        if not command:
            raise ValueError("Unknown command %s" % hexlify(chr(tag)).upper())
        return command.decode(data[2:])

    @classmethod
    def testCommand(cls, tag, data, *args):
        assert cls.encodeCommand(tag, *args) == data.upper(), "encodeCommand(%s, %s) is %s, not %s" % (hexlify(chr(tag)).upper(), ', '.join(repr(arg) for arg in args), cls.encodeCommand(tag, *args), data)
        assert cls.decodeCommand(data) == (tag, args), "decodeCommand(%s, %s) is %s, not %s" % (hexlify(chr(tag)).upper(), data, cls.decodeCommand(data), (tag, args))

def testParam():
    p = Param()
    p.test(0, '00')
    p.test(1, '01')
    p.test(0xff, 'FF')
    p = Param(4)
    p.test(0, '00000000')
    p.test(1, '00000001')
    p.test(0xff00ff00, 'FF00FF00')
    p = Param(0, TYPE_SIGNED_HEX)
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
    p = Param(1, TYPE_SIGNED_HEX)
    p.test(0, '00')
    p.test(1, '01')
    p.test(-1, 'FF')
    p.test(127, '7F')
    p.test(-127, '81')
    p.test(-128, '80')
    p = Param(4, TYPE_SIGNED_HEX)
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
    p.test(0, '0000')
    p.test(1, '0001')
    p.test(-1, 'FF01')
    p.test(99, '0099')
    p.test(100, '000100')
    p.test(127, '000127')
    p.test(-127, 'FF0127')
    p.test(-128, 'FF0128')
    p.test(128, '000128')
    p.test(0xff00ff00, '004278255360')
    p = Param(1, TYPE_SIGNED_DEC)
    p.test(0, '0000')
    p.test(1, '0001')
    p.test(-1, 'FF01')
    p.test(99, '0099')
    p = Param(4, TYPE_SIGNED_DEC)
    p.test(0, '0000000000')
    p.test(1, '0000000001')
    p.test(-1, 'FF00000001')
    p.test(0xff00ff, '0016711935')
    p.test(-99999999, 'FF99999999')
    p = Param(0, TYPE_STR)
    p.test('a', '61')
    p.test('abcd', '61626364')
    p = Param(1, TYPE_STR)
    p.test('a', '61')
    p = Param(2, TYPE_STR)
    p.test('aa', '6161')

def testCommand():
    Command(0, (Param(), Param(4, TYPE_SIGNED_HEX), Param(1, TYPE_SIGNED_DEC), Param(0, TYPE_STR)), 1)
    Command(1, (Param(2), Param(1, TYPE_UNSIGNED_DEC), Param(4, TYPE_SIGNED_DEC), Param(0, TYPE_SIGNED_HEX)), 2)
    Command(2, (Param(1, TYPE_SIGNED_HEX), Param(4, TYPE_UNSIGNED_DEC), Param(1, TYPE_STR), Param(0, TYPE_SIGNED_DEC)), 3)
    Command(3, (Param(0, TYPE_UNSIGNED_HEX),), 4)
    Command(4, (Param(0, TYPE_UNSIGNED_DEC),))
    Command.checkReplies()
    Command.testCommand(0, '#00,FF,7FFFFFFF,FF99,3939393939393939\n', 0xff, 0x7fffffff, -99, '99999999')
    Command.testCommand(1, '#01,1000,99,0099999999,FF00000000000000000001\n', 0x1000, 99, 99999999, -0xffffffffffffffffffff)
    Command.testCommand(2, '#02,80,99999999,61,FF0999999999999999\n', -0x80, 99999999, 'a', -999999999999999)
    Command.testCommand(3, '#03,FFFFFFFFFFFFFFFFFFFF\n', 0xffffffffffffffffffff)
    Command.testCommand(4, '#04,0999999999999999\n', 999999999999999)
    Command.commands.clear()

if __name__ == '__main__':
    testParam()
    testCommand()
