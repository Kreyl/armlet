/*
 * MassStorage.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: g.kruglov
 */

#ifndef MASSSTORAGE_CPP_
#define MASSSTORAGE_CPP_

#include "MassStorage.h"
#include "scsi.h"

MassStorage_t MassStorage;
static uint8_t SByte;

// Handler of non-standard control pkt
EpState_t Usb_t::NonStandardControlRequestHandler(uint8_t **PPtr, uint32_t *PLen) {
    //Uart.Printf("NonStandard Request\r");
    if(SetupReq.wIndex != 0) return esError;    // If incorrect interface
    switch(SetupReq.bRequest) {
            case MS_REQ_MassStorageReset:
                Uart.Printf("MS_REQ_MassStorageReset\r");
                if(SetupReq.bmRequestType == (USB_REQTYPE_RECIPIENT_DEVICE | USB_REQTYPE_CLASS | USB_REQTYPE_RECIPIENT_INTERFACE)) {
//                    MSInterfaceInfo->State.IsMassStoreReset = true;
                    // TODO: remove Stall condition
                    return esOutStatus; // Acknowledge reception
                }
                break;
            case MS_REQ_GetMaxLUN:
                Uart.Printf("MS_REQ_GetMaxLUN\r");
                if(SetupReq.bmRequestType == (USB_REQTYPE_DEV2HOST | USB_REQTYPE_CLASS | USB_REQTYPE_RECIPIENT_INTERFACE)) {
                    SByte = NUMBER_OF_LUNS;
                    *PPtr = &SByte;
                    *PLen = 1;
                    return esInData;
                }
                break;
        }
    return esError;
}

// Threads
static WORKING_AREA(waUsbOutThd, 128);
static void UsbOutThd(void *arg) {
    chRegSetThreadName("UsbOut");
    while(1) MassStorage.UsbOutTask();
}

void MassStorage_t::Init() {
    chIQInit(&IOutQueue, QueueBuf, MS_OUTBUF_SZ, NULL, NULL);
    Usb.PEpBulkOut->AssignOutQueue(&IOutQueue);
    SenceData.ResponseCode = 0x70;
    SenceData.AdditionalLength = 0x0A;
    // Thread
    chThdCreateStatic(waUsbOutThd, sizeof(waUsbOutThd), NORMALPRIO, (tfunc_t)UsbOutThd, NULL);
}

void MassStorage_t::UsbOutTask() {
    // Receive header
    if(chIQReadTimeout(&IOutQueue, (uint8_t*)&CmdBlock, MS_CMD_SZ, TIME_INFINITE) != MS_CMD_SZ) return;
    Uart.Printf("Sgn=%X; Tag=%X; Len=%u; Flags=%X; LUN=%u; SLen=%u; SCmd=%A\r",
            CmdBlock.Signature, CmdBlock.Tag, CmdBlock.DataTransferLength, CmdBlock.Flags, CmdBlock.LUN, CmdBlock.SCSICommandLength,
            CmdBlock.SCSICommandData, CmdBlock.SCSICommandLength, ' ');
    // ==== Decode SCSI cmd ====
    bool CommandSuccess = false;
    switch(CmdBlock.SCSICommandData[0]) {
        case SCSI_CMD_INQUIRY:          CommandSuccess = CmdInquiry(); break;
        case SCSI_CMD_REQUEST_SENSE:    CommandSuccess = CmdRequestSense(); break;
        case SCSI_CMD_READ_CAPACITY_10: CommandSuccess = CmdReadCapacity10(); break;
        case SCSI_CMD_SEND_DIAGNOSTIC:  CommandSuccess = CmdSendDiagnostic(); break;
        case SCSI_CMD_WRITE_10:         CommandSuccess = CmdReadWrite10(rwWrite); break;
        case SCSI_CMD_READ_10:          CommandSuccess = CmdReadWrite10(rwRead); break;
        case SCSI_CMD_MODE_SENSE_6:     CommandSuccess = CmdModeSense6(); break;
        // These commands should just succeed, no handling required
        case SCSI_CMD_TEST_UNIT_READY:
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        case SCSI_CMD_VERIFY_10:
            CommandSuccess = true;
            CmdBlock.DataTransferLength = 0;
            break;
        default:
            // Update the SENSE key to reflect the invalid command
            SenceData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
            SenceData.AdditionalSenseCode = SCSI_ASENSE_INVALID_COMMAND;
            SenceData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
            break;
    } // switch

    // Check if command was successfully processed
    if(CommandSuccess) {
        SenceData.SenseKey = SCSI_SENSE_KEY_GOOD;
        SenceData.AdditionalSenseCode = SCSI_ASENSE_NO_ADDITIONAL_INFORMATION;
        SenceData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
    }
}

bool MassStorage_t::CmdInquiry() {
    Uart.Printf("CmdInquiry\r");
    uint16_t RequestedLength = BuildUint16(CmdBlock.SCSICommandData[4], CmdBlock.SCSICommandData[3]);
    uint16_t BytesToTransfer = MIN(RequestedLength, sizeof(InquiryData));
    Uart.Printf("L=%X, b=%u\r", RequestedLength, BytesToTransfer);
    // Only the standard INQUIRY data is supported, check if any optional INQUIRY bits set
    if((CmdBlock.SCSICommandData[1] & ((1 << 0) | (1 << 1))) or CmdBlock.SCSICommandData[2]) {
        // Optional but unsupported bits set - update the SENSE key and fail the request
        SenceData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
        SenceData.AdditionalSenseCode = SCSI_ASENSE_INVALID_FIELD_IN_CDB;
        SenceData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }
    // Transmit InquiryData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&InquiryData, BytesToTransfer);
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLength -= BytesToTransfer;
    return true;
}
bool MassStorage_t::CmdRequestSense() {
    Uart.Printf("CmdRequestSense\r");
    return true;
}
bool MassStorage_t::CmdReadCapacity10() {
    Uart.Printf("CmdReadCapacity10\r");
    return true;
}
bool MassStorage_t::CmdSendDiagnostic() {
    Uart.Printf("CmdSendDiagnostic\r");
    return true;
}
bool MassStorage_t::CmdReadWrite10(ReadWrite_t ReadWrite) {
    Uart.Printf("CmdReadWrite10\r");
    return true;
}
bool MassStorage_t::CmdModeSense6() {
    Uart.Printf("CmdModeSense6\r");
    return true;
}

#endif /* MASSSTORAGE_CPP_ */
