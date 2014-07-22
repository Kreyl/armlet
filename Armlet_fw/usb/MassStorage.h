/*
 * MassStorage.h
 *
 *  Created on: Oct 7, 2013
 *      Author: g.kruglov
 */

#ifndef MASSSTORAGE_H_
#define MASSSTORAGE_H_

#include "usb_f2.h"
#include "hal.h"
#include "scsi.h"
#include "cmd_uart.h"

// Config
#define READ_ONLY       FALSE

#if 1 // ================= Mass Storage constants and types ====================
// Enum for the Mass Storage class specific control requests that can be issued by the USB bus host
enum MS_ClassRequests_t {
    // Mass Storage class-specific request to retrieve the total number of Logical Units (drives) in the SCSI device.
    MS_REQ_GetMaxLUN = 0xFE,
    // Mass Storage class-specific request to reset the Mass Storage interface, ready for the next command.
    MS_REQ_MassStorageReset = 0xFF,
};

// Enum for the possible command status wrapper return status codes.
enum MS_CommandStatusCodes_t {
    MS_SCSI_COMMAND_Pass       = 0, // Command completed with no error
    MS_SCSI_COMMAND_Fail       = 1, // Command failed to complete - host may check the exact error via a SCSI REQUEST SENSE command
    MS_SCSI_COMMAND_PhaseError = 2, // Command failed due to being invalid in the current phase.
};

// Mass Storage Class Command Block Wrapper
struct MS_CommandBlockWrapper_t {
    uint32_t Signature;         // Command block signature, must be MS_CBW_SIGNATURE to indicate a valid Command Block
    uint32_t Tag;               // Unique command ID value, to associate a command block wrapper with its command status wrapper
    uint32_t DataTransferLen;   // Length of the optional data portion of the issued command, in bytes
    uint8_t  Flags;             // Command block flags, indicating command data direction
    uint8_t  LUN;               // Logical Unit number this command is issued to
    uint8_t  SCSICmdLen;        // Length of the issued SCSI command within the SCSI command data array
    uint8_t  SCSICmdData[16];   // Issued SCSI command in the Command Block
} __attribute__ ((__packed__));
#define MS_CMD_SZ   sizeof(MS_CommandBlockWrapper_t)

// Mass Storage Class Command Status Wrapper
struct MS_CommandStatusWrapper_t {
    uint32_t Signature;          // Status block signature, must be \ref MS_CSW_SIGNATURE to indicate a valid Command Status
    uint32_t Tag;                // Unique command ID value, to associate a command block wrapper with its command status wrapper
    uint32_t DataTransferResidue;// Number of bytes of data not processed in the SCSI command
    uint8_t  Status;             // Status code of the issued command - a value from the MS_CommandStatusCodes_t enum
} __attribute__ ((__packed__));

#endif

#define MS_TIMEOUT_MS   2700
#define MS_DATABUF_SZ   4096

class MassStorage_t {
    MS_CommandBlockWrapper_t CmdBlock;
    MS_CommandStatusWrapper_t CmdStatus;
    SCSI_RequestSenseResponse_t SenseData;
    SCSI_ReadCapacity10Response_t ReadCapacity10Response;
    SCSI_ReadFormatCapacitiesResponse_t ReadFormatCapacitiesResponse;
    void SCSICmdHandler();
    // Scsi commands
    bool CmdInquiry();
    bool CmdRequestSense();
    bool CmdReadCapacity10();
    bool CmdSendDiagnostic();
    bool CmdReadFormatCapacities();
    bool CmdRead10();
    bool CmdWrite10();
    bool CmdModeSense6();
    // Buffers need to be aligned to 4-byte boundaries to allow advanced DMA to use aligned access.
    struct {
        union {
            uint32_t DummyBuf1[(MS_DATABUF_SZ/4)];
            uint8_t Buf1[MS_DATABUF_SZ];
        };
        union {
            uint32_t DummyBuf2[(MS_DATABUF_SZ/4)];
            uint8_t Buf2[MS_DATABUF_SZ];
        };
    };
    bool ReadWriteCommon(uint32_t *PAddr, uint16_t *PLen);
    Thread *PThread;
public:
    void Init();
    void Reset();
    // Inner Use
    void UsbOutTask();
};

extern MassStorage_t MassStorage;

#endif /* MASSSTORAGE_H_ */
