#!/usr/bin/python
#
# Ostranna UART text protocol commands
#
# See https://docs.google.com/document/d/1J2z4WCSR-WekH4tEe7bfqvjPJ0Hx7Ww-26ECBoLwl4s
#
from UARTTextProtocol import Command

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

# Command tags
UART_PING = 'ping'
UART_ACK = 'ack'

UART_PILL_GET_STATE = 'pillState'
UART_PILL_RET_STATE = 'pill'

UART_PILL_WRITE = 'pillWrite32'
UART_PILL_READ = 'pillRead32'
UART_PILL_DATA = 'pillData'
UART_PILL_REPEAT_WRITE = 'pillRepeatWrite32'

UART_SET_ID = 'setID'
UART_GET_ID = 'getID'
UART_RET_ID = 'ID'

UART_SET_TYPE = 'setType'
UART_GET_TYPE = 'getType'
UART_RET_TYPE = 'type'

UART_SET_DOSE = 'setDose'
UART_GET_DOSE = 'getDose'
UART_RET_DOSE = 'dose'

UART_SET_DOSE_TOP = 'setDoseTop'
UART_GET_DOSE_TOP = 'getDoseTop'
UART_RET_DOSE_TOP = 'doseTop'

UART_MESH_GET_SETTINGS = 'getSettings'
UART_MESH_SETTINGS = 'settings'

UART_MESH_SET_TIME = 'setTime'
UART_MESH_TIME_SET = 'time'

UART_MESH_NODE_INFO = 'node'

#Command(tag, (Param(length = 1, type = TYPE_UNSIGNED_HEX),), replyTag or None)

pingCommand = Command(UART_PING, '', UART_ACK)
ackResponse = Command(UART_ACK, 'd')

Command(UART_PILL_GET_STATE, 'd', UART_PILL_RET_STATE)
Command(UART_PILL_RET_STATE, 'dd')

Command(UART_PILL_WRITE, 'dd*', UART_ACK)
Command(UART_PILL_READ, 'dd', UART_PILL_DATA)
Command(UART_PILL_DATA, 'd*')

Command(UART_SET_ID, 'd', UART_ACK)
Command(UART_GET_ID, '', UART_RET_ID)
Command(UART_RET_ID, 'd')

Command(UART_SET_TYPE, 'd', UART_ACK)
Command(UART_GET_TYPE, '', UART_RET_TYPE)
Command(UART_RET_TYPE, 'd')

Command(UART_SET_DOSE_TOP, 'd')

meshGetSettingsCommand = Command(UART_MESH_GET_SETTINGS, '', UART_MESH_SETTINGS)
meshGetSettingsResponse = Command(UART_MESH_SETTINGS, 'dd')

meshSetTimeCommand = Command(UART_MESH_SET_TIME, 'd', UART_MESH_TIME_SET)
meshSetTimeResponse = Command(UART_MESH_TIME_SET, 'd')

meshNodeInfoResponse = Command(UART_MESH_NODE_INFO, 'ddddddd')

Command.checkReplies()
