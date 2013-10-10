/*
 * MassStorage.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: g.kruglov
 */

#ifndef MASSSTORAGE_CPP_
#define MASSSTORAGE_CPP_

#include "MassStorage.h"
#include "sdc_lld.h"

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
    SenseData.ResponseCode = 0x70;
    SenseData.AdditionalLength = 0x0A;
    // Thread
    chThdCreateStatic(waUsbOutThd, sizeof(waUsbOutThd), NORMALPRIO, (tfunc_t)UsbOutThd, NULL);
}

#if 1 // ====================== OUT task =======================================
void MassStorage_t::UsbOutTask() {
    bool ShouldSendStatus;
    ShouldSendStatus = true;
    // Receive header
    if(chIQReadTimeout(&IOutQueue, (uint8_t*)&CmdBlock, MS_CMD_SZ, TIME_INFINITE) != MS_CMD_SZ) return;
    bool CmdRsltOk = DecodeSCSICommand();
    CmdStatus.Status = CmdRsltOk? MS_SCSI_COMMAND_Pass : MS_SCSI_COMMAND_Fail;
    CmdStatus.Signature = MS_CSW_SIGNATURE;
    CmdStatus.Tag = CmdBlock.Tag;
    CmdStatus.DataTransferResidue = CmdBlock.DataTransferLength;
    // Stall if cmd failed and there is data to send
    if(!CmdRsltOk and (CmdStatus.DataTransferResidue != 0)) {
        Usb.PEpBulkIn->StallIn();
        ShouldSendStatus = (Usb.PEpBulkIn->WaitUntilReady() == OK);
    }
    if(ShouldSendStatus) Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&CmdStatus, sizeof(MS_CommandStatusWrapper_t));
}
#endif

#if 1 // =========================== SCSI ======================================
bool MassStorage_t::DecodeSCSICommand() {
    Uart.Printf("Sgn=%X; Tag=%X; Len=%u; Flags=%X; LUN=%u; SLen=%u; SCmd=%A\r",
            CmdBlock.Signature, CmdBlock.Tag, CmdBlock.DataTransferLength, CmdBlock.Flags, CmdBlock.LUN, CmdBlock.SCSICommandLength,
            CmdBlock.SCSICommandData, CmdBlock.SCSICommandLength, ' ');
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
            Uart.Printf("Cmd %X not supported\r", CmdBlock.SCSICommandData[0]);
            // Update the SENSE key to reflect the invalid command
            SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
            SenseData.AdditionalSenseCode = SCSI_ASENSE_INVALID_COMMAND;
            SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
            break;
    } // switch

    // Check if command was successfully processed
    if(CommandSuccess) {
        SenseData.SenseKey = SCSI_SENSE_KEY_GOOD;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_NO_ADDITIONAL_INFORMATION;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
    }
    return CommandSuccess;
}

bool MassStorage_t::CmdInquiry() {
    Uart.Printf("CmdInquiry\r");
    uint16_t RequestedLength = BuildUint16(CmdBlock.SCSICommandData[4], CmdBlock.SCSICommandData[3]);
    uint16_t BytesToTransfer = MIN(RequestedLength, sizeof(InquiryData));
    // Only the standard INQUIRY data is supported, check if any optional INQUIRY bits set
    if((CmdBlock.SCSICommandData[1] & ((1 << 0) | (1 << 1))) or CmdBlock.SCSICommandData[2]) {
        // Optional but unsupported bits set - update the SENSE key and fail the request
        SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_INVALID_FIELD_IN_CDB;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }
    // Transmit InquiryData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&InquiryData, BytesToTransfer);
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLength -= BytesToTransfer;
    return true;
}
bool MassStorage_t::CmdRequestSense() {
    Uart.Printf("CmdRequestSense\r");
    uint16_t RequestedLength = CmdBlock.SCSICommandData[4];
    uint16_t BytesToTransfer = MIN(RequestedLength, sizeof(SenseData));
    // Transmit SenceData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&SenseData, BytesToTransfer);
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLength -= BytesToTransfer;
    return true;
}
bool MassStorage_t::CmdReadCapacity10() {
    Uart.Printf("CmdReadCapacity10\r");
    ReadCapacity10Response.LastBlockAddr = __REV(SDCD1.capacity - 1);
    ReadCapacity10Response.BlockSize = __REV(MMCSD_BLOCK_SIZE);
    // Transmit SenceData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&ReadCapacity10Response, sizeof(ReadCapacity10Response));
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLength -= sizeof(ReadCapacity10Response);
    return true;
}
bool MassStorage_t::CmdSendDiagnostic() {
    Uart.Printf("CmdSendDiagnostic\r");
    return true;
}
bool MassStorage_t::CmdReadWrite10(ReadWrite_t ReadWrite) {
    Uart.Printf("CmdReadWrite10\r");
    // Check if read-only
    if((ReadWrite == rwWrite) and READ_ONLY) {
        SenseData.SenseKey = SCSI_SENSE_KEY_DATA_PROTECT;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_WRITE_PROTECTED;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }
    uint32_t BlockAddress = BuildUint32(CmdBlock.SCSICommandData[5], CmdBlock.SCSICommandData[4], CmdBlock.SCSICommandData[3], CmdBlock.SCSICommandData[2]);
    uint16_t TotalBlocks  = BuildUint16(CmdBlock.SCSICommandData[8], CmdBlock.SCSICommandData[7]);
    Uart.Printf("Addr=%u; Len=%u\r", BlockAddress, TotalBlocks);
    // Check block addr
    if(BlockAddress >= SDCD1.capacity) {
        SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }




    return true;
}
bool MassStorage_t::CmdModeSense6() {
    Uart.Printf("CmdModeSense6\r");
    return true;
}

#endif

#endif /* MASSSTORAGE_CPP_ */
