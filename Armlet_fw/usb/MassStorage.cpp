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
    SenseData.AddSenseLen = 0x0A;

    // Thread
    chThdCreateStatic(waUsbOutThd, sizeof(waUsbOutThd), NORMALPRIO, (tfunc_t)UsbOutThd, NULL);
}

#if 1 // ====================== OUT task =======================================
void MassStorage_t::UsbOutTask() {
    // Receive header
    if(chIQReadTimeout(&IOutQueue, (uint8_t*)&CmdBlock, MS_CMD_SZ, TIME_INFINITE) != MS_CMD_SZ) return;
    SCSICmdHandler();
}
#endif

#if 1 // =========================== SCSI ======================================
void MassStorage_t::SCSICmdHandler() {
    Uart.Printf("Sgn=%X; Tag=%X; Len=%u; Flags=%X; LUN=%u; SLen=%u; SCmd=%A\r",
            CmdBlock.Signature, CmdBlock.Tag, CmdBlock.DataTransferLen, CmdBlock.Flags, CmdBlock.LUN, CmdBlock.SCSICmdLen,
            CmdBlock.SCSICmdData, CmdBlock.SCSICmdLen, ' ');
    bool CmdOk = false;
    switch(CmdBlock.SCSICmdData[0]) {
        case SCSI_CMD_INQUIRY:            CmdOk = CmdInquiry(); break;
        case SCSI_CMD_REQUEST_SENSE:      CmdOk = CmdRequestSense(); break;
        case SCSI_CMD_READ_CAPACITY_10:   CmdOk = CmdReadCapacity10(); break;
        case SCSI_CMD_SEND_DIAGNOSTIC:    CmdOk = CmdSendDiagnostic(); break;
        case SCSI_READ_FORMAT_CAPACITIES: CmdOk = CmdReadFormatCapacities(); break;
        case SCSI_CMD_WRITE_10:           CmdOk = CmdWrite10(); break;
        case SCSI_CMD_READ_10:            CmdOk = CmdRead10(); break;
        case SCSI_CMD_MODE_SENSE_6:       CmdOk = CmdModeSense6(); break;
        // These commands should just succeed, no handling required
        case SCSI_CMD_TEST_UNIT_READY:
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        case SCSI_CMD_VERIFY_10:
            CmdOk = true;
            CmdBlock.DataTransferLen = 0;
            break;
        default:
            Uart.Printf("Cmd %X not supported\r", CmdBlock.SCSICmdData[0]);
            // Update the SENSE key to reflect the invalid command
            SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
            SenseData.AdditionalSenseCode = SCSI_ASENSE_INVALID_COMMAND;
            SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
            // Update status
            //CmdStatus.DataTransferResidue = 0;    // 0 or requested length?
            break;
    } // switch
    // Update Sense if command was successfully processed
    if(CmdOk) {
        SenseData.SenseKey = SCSI_SENSE_KEY_GOOD;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_NO_ADDITIONAL_INFORMATION;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
    }

    // Send status
    CmdStatus.Status = CmdOk? MS_SCSI_COMMAND_Pass : MS_SCSI_COMMAND_Fail;
    CmdStatus.Signature = MS_CSW_SIGNATURE;
    CmdStatus.Tag = CmdBlock.Tag;
    // DataTransferLen decreased at cmd handlers
    CmdStatus.DataTransferResidue = CmdBlock.DataTransferLen;
    // Stall if cmd failed and there is data to send
    bool ShouldSendStatus = true;
    if(!CmdOk) {
        Usb.PEpBulkIn->StallIn();
        ShouldSendStatus = (Usb.PEpBulkIn->WaitUntilReady() == OK);
    }
    if(ShouldSendStatus) Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&CmdStatus, sizeof(MS_CommandStatusWrapper_t));
}

bool MassStorage_t::CmdInquiry() {
    Uart.Printf("CmdInquiry\r");
    uint16_t RequestedLength = BuildUint16(CmdBlock.SCSICmdData[4], CmdBlock.SCSICmdData[3]);
    uint16_t BytesToTransfer;
    if(CmdBlock.SCSICmdData[1] & 0x01) {
        BytesToTransfer = MIN(RequestedLength, PAGE0_INQUIRY_DATA_SZ);
        Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&Page00InquiryData, BytesToTransfer);
    }
    else {
        // Transmit InquiryData
        BytesToTransfer = MIN(RequestedLength, sizeof(SCSI_InquiryResponse_t));
        Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&InquiryData, BytesToTransfer);
    }
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLen -= BytesToTransfer;
    return true;
}
bool MassStorage_t::CmdRequestSense() {
    Uart.Printf("CmdRequestSense\r");
    uint16_t RequestedLength = CmdBlock.SCSICmdData[4];
    uint16_t BytesToTransfer = MIN(RequestedLength, sizeof(SenseData));
    // Transmit SenceData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&SenseData, BytesToTransfer);
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLen -= BytesToTransfer;
    return true;
}
bool MassStorage_t::CmdReadCapacity10() {
    Uart.Printf("CmdReadCapacity10\r");
    ReadCapacity10Response.LastBlockAddr = __REV((uint32_t)((SDCD1.capacity / MMCSD_BLOCK_SIZE) - 1));
    ReadCapacity10Response.BlockSize = __REV((uint32_t)MMCSD_BLOCK_SIZE);
    // Transmit SenceData
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&ReadCapacity10Response, sizeof(ReadCapacity10Response));
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLen -= sizeof(ReadCapacity10Response);
    return true;
}
bool MassStorage_t::CmdSendDiagnostic() {
    Uart.Printf("CmdSendDiagnostic\r");
    return false;
}
bool MassStorage_t::CmdReadFormatCapacities() {
    Uart.Printf("CmdReadFormatCapacities\r");
    ReadFormatCapacitiesResponse.Length = 0x08;
    ReadFormatCapacitiesResponse.NumberOfBlocks = __REV(SDCD1.capacity / MMCSD_BLOCK_SIZE);
    // 01b Unformatted Media - Maximum formattable capacity for this cartridge
    // 10b Formatted Media - Current media capacity
    // 11b No Cartridge in Drive - Maximum formattable capacity
    ReadFormatCapacitiesResponse.DescCode = 0x02;
    ReadFormatCapacitiesResponse.BlockSize[0] = (uint8_t)(MMCSD_BLOCK_SIZE >> 16);
    ReadFormatCapacitiesResponse.BlockSize[1] = (uint8_t)(MMCSD_BLOCK_SIZE >> 8);
    ReadFormatCapacitiesResponse.BlockSize[2] = (uint8_t)(MMCSD_BLOCK_SIZE);
    // Transmit Data
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&ReadFormatCapacitiesResponse, sizeof(ReadFormatCapacitiesResponse));
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLen -= sizeof(ReadFormatCapacitiesResponse);
    return true;

}
bool MassStorage_t::CmdRead10() {
    Uart.Printf("CmdRead10\r");
    uint32_t BlockAddress = BuildUint32(CmdBlock.SCSICmdData[5], CmdBlock.SCSICmdData[4], CmdBlock.SCSICmdData[3], CmdBlock.SCSICmdData[2]);
    uint16_t TotalBlocks  = BuildUint16(CmdBlock.SCSICmdData[8], CmdBlock.SCSICmdData[7]);
    Uart.Printf("Addr=%u; Len=%u\r", BlockAddress, TotalBlocks);
    // Check block addr
    if((BlockAddress + TotalBlocks) >= (SDCD1.capacity / MMCSD_BLOCK_SIZE)) {
        Uart.Printf("Out Of Range\r");
        SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }
    // Check cases 4 & 5: Hi != Dn
    if(CmdBlock.DataTransferLen != TotalBlocks * MMCSD_BLOCK_SIZE) {
        Uart.Printf("Wrong length\r");
        SenseData.SenseKey = SCSI_SENSE_KEY_ILLEGAL_REQUEST;
        SenseData.AdditionalSenseCode = SCSI_ASENSE_INVALID_COMMAND;
        SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
        return false;
    }
    // Read data to buffers
    //uint32_t LenToRead = MIN(MS_DATABUF_SZ, TotalBlocks*MMCSD_BLOCK_SIZE);
    uint32_t BlocksToRead = MIN(MS_DATABUF_SZ / MMCSD_BLOCK_SIZE, TotalBlocks);
    bool Rslt = sdcRead(&SDCD1, BlockAddress, Buf1, BlocksToRead);
    uint32_t BytesToSend = BlocksToRead * MMCSD_BLOCK_SIZE;
    if(Rslt == CH_SUCCESS) {
        Uart.Printf("Rd ok\r");
        //Uart.Printf("Rd %u blocks: %A\r", BlocksToRead, Buf1, BlocksToRead*512, ' ');
        Usb.PEpBulkIn->StartTransmitBuf(Buf1, BytesToSend);
        Usb.PEpBulkIn->WaitUntilReady();
        // Succeed the command and update the bytes transferred counter
        CmdBlock.DataTransferLen -= BytesToSend;
        return true;
    }
    else {
        Uart.Printf("Rd fail\r");
        // TODO: handle read error
    }
    return false;
}

bool MassStorage_t::CmdWrite10() {
    Uart.Printf("CmdWrite10\r");
#if READ_ONLY
    SenseData.SenseKey = SCSI_SENSE_KEY_DATA_PROTECT;
    SenseData.AdditionalSenseCode = SCSI_ASENSE_WRITE_PROTECTED;
    SenseData.AdditionalSenseQualifier = SCSI_ASENSEQ_NO_QUALIFIER;
    return false;
#else
    return false;
#endif
}

bool MassStorage_t::CmdModeSense6() {
    Uart.Printf("CmdModeSense6\r");
    Usb.PEpBulkIn->StartTransmitBuf((uint8_t*)&Mode_Sense6_data, MODE_SENSE6_DATA_SZ);
    Usb.PEpBulkIn->WaitUntilReady();
    // Succeed the command and update the bytes transferred counter
    CmdBlock.DataTransferLen -= MODE_SENSE6_DATA_SZ;
    return true;
}

#endif

#endif /* MASSSTORAGE_CPP_ */
