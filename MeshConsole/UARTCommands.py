#!/usr/bin/python
#
# Ostranna UART commands
#
# See https://docs.google.com/document/d/1J2z4WCSR-WekH4tEe7bfqvjPJ0Hx7Ww-26ECBoLwl4s
#
from UARTProtocol import Command, Param
from UARTProtocol import TYPE_STR, TYPE_SIGNED_HEX, TYPE_SIGNED_DEC, TYPE_UNSIGNED_DEC

# ACK status codes
UART_STATUS_OK = 0
UART_STATUS_FAIL = 1
UART_STATUS_TIMEOUT = 2
UART_STATUS_BUSY = 3
UART_STATUS_NEW = 4
UART_STATUS_IN_PROGRESS = 5
UART_STATUS_LAST = 6
UART_STATUS_CMD_ERROR = 7
UART_STATUS_WRITE_PROTECT = 8

# Command codes
UART_PING = 0x01
UART_ACK = 0x90

UART_PILL_STATUS = 0x30
UART_RET_PILL_STATUS = 0xC0

UART_WRITE_PILL = 0x31
UART_RET_WRITE_PILL = 0xC1

UART_READ_PILL = 0x32
UART_RET_READ_PILL = 0xC2

UART_SET_ID = 0x10
UART_GET_ID = 0x11
UART_RET_GET_ID = 0xA1

UART_SET_TYPE = 0x16
UART_GET_TYPE = 0x17
UART_RET_GET_TYPE = 0xA7

UART_MESH_GET_SETTINGS = 0x81
UART_MESH_SETTINGS = 0x82

UART_MESH_SET_TIME = 0x71
UART_MESH_TIME_SET = 0x72

UART_MESH_NODE_INFO = 0x91

#Command(tag, (Param(length = 1, type = TYPE_UNSIGNED_HEX),), replyTag or None)

Command(UART_PING, (), UART_ACK)
ackResponse = Command(UART_ACK, (Param(),))

Command(UART_PILL_STATUS, (Param(),), UART_RET_PILL_STATUS)
Command(UART_RET_PILL_STATUS, (Param(), Param()))

Command(UART_WRITE_PILL, (Param(), Param(0, TYPE_STR)), UART_RET_WRITE_PILL)
Command(UART_RET_WRITE_PILL, (Param(), Param()))

Command(UART_READ_PILL, (Param(), Param()), UART_RET_READ_PILL)
Command(UART_RET_READ_PILL, (Param(), Param(), Param(0, TYPE_STR)))

Command(UART_SET_ID, (Param(2),), UART_ACK)
Command(UART_GET_ID, (), UART_RET_GET_ID)
Command(UART_RET_GET_ID, (Param(2),))

Command(UART_SET_TYPE, (Param(),), UART_ACK)
Command(UART_GET_TYPE, (), UART_RET_GET_TYPE)
Command(UART_RET_GET_TYPE, (Param(),))

meshGetSettingsCommand = Command(UART_MESH_GET_SETTINGS, (), UART_MESH_SETTINGS)
meshGetSettingsResponse = Command(UART_MESH_SETTINGS, (Param(2, TYPE_UNSIGNED_DEC), Param(3, TYPE_UNSIGNED_DEC)))

meshSetTimeCommand = Command(UART_MESH_SET_TIME, (Param(0, TYPE_UNSIGNED_DEC),), UART_MESH_TIME_SET)
meshSetTimeResponse = Command(UART_MESH_TIME_SET, (Param(0, TYPE_SIGNED_DEC),))

meshNodeInfoResponse = Command(UART_MESH_NODE_INFO, (Param(2), Param(1, TYPE_UNSIGNED_DEC), Param(4, TYPE_SIGNED_HEX), Param(4, TYPE_SIGNED_HEX), Param(2), Param(2), Param()))

Command.checkReplies()
