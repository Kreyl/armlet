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
UART_PILL_DATA = 'pillData32'
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

UART_MESH_GET_SETTINGS = 'getMeshInfo'
UART_MESH_SETTINGS = 'meshInfo'

UART_MESH_SET_CYCLE = 'setMeshCycle'
UART_MESH_CYCLE_SET = 'meshCycle'

UART_MESH_NODE_INFO = 'node'

UART_FF = 'ff'
UART_FF_GET = 'ffGet'
UART_FF_SET = 'ffSet'

UART_FF_RGB = 'rgb'
UART_FF_WAIT = 'wait'
UART_FF_GOTO = 'goto'
UART_FF_LIGHT = 'light'

# Command definitions

pingCommand = Command(UART_PING, '', UART_ACK)
ackResponse = Command(UART_ACK, 'd')

pillGetStateCommand = Command(UART_PILL_GET_STATE, 'd', UART_PILL_RET_STATE)
pillRetStateResponse = Command(UART_PILL_RET_STATE, 'dd')

pillWrite32Command = Command(UART_PILL_WRITE, 'dd*', UART_ACK)
pillRead32Command = Command(UART_PILL_READ, 'dd', UART_PILL_DATA)
pillData32Response = Command(UART_PILL_DATA, 'd*')
pillRepeatWrite32Command = Command(UART_PILL_REPEAT_WRITE, 'dd*', UART_ACK)

setIDCommand = Command(UART_SET_ID, 'd', UART_ACK)
getIDCommand = Command(UART_GET_ID, '', UART_RET_ID)
retIDResponse = Command(UART_RET_ID, 'd')

setTypeCommand = Command(UART_SET_TYPE, 'd', UART_ACK)
getTypeCommand = Command(UART_GET_TYPE, '', UART_RET_TYPE)
retTypeResponse = Command(UART_RET_TYPE, 'd')

setDoseTopCommand = Command(UART_SET_DOSE_TOP, 'd')

meshGetSettingsCommand = Command(UART_MESH_GET_SETTINGS, '', UART_MESH_SETTINGS)
meshGetSettingsResponse = Command(UART_MESH_SETTINGS, 'dd')

meshSetCycleCommand = Command(UART_MESH_SET_CYCLE, 'd', UART_MESH_CYCLE_SET)
meshSetCycleResponse = Command(UART_MESH_CYCLE_SET, 'd')

meshNodeInfoResponse = Command(UART_MESH_NODE_INFO, 'ddddddd')

ffGetCommand = Command(UART_FF_GET, '', UART_FF)
ffSetCommand = Command(UART_FF_SET, 's*', UART_ACK)
ffResponse = Command(UART_FF, 's*')

Command.linkReplies()
