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

// Config
#define NUMBER_OF_LUNS  1   // Number of logical disks

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
    uint32_t Signature;          // Command block signature, must be MS_CBW_SIGNATURE to indicate a valid Command Block
    uint32_t Tag;                // Unique command ID value, to associate a command block wrapper with its command status wrapper
    uint32_t DataTransferLength; // Length of the optional data portion of the issued command, in bytes
    uint8_t  Flags;              // Command block flags, indicating command data direction
    uint8_t  LUN;                // Logical Unit number this command is issued to
    uint8_t  SCSICommandLength;  // Length of the issued SCSI command within the SCSI command data array
    uint8_t  SCSICommandData[16];// Issued SCSI command in the Command Block
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

enum ReadWrite_t {rwRead, rwWrite};
#define MS_OUTBUF_SZ     128
class MassStorage_t {
    uint8_t QueueBuf[MS_OUTBUF_SZ];
    InputQueue IOutQueue;   // Host to Device, USB convention
    MS_CommandBlockWrapper_t CmdBlock;
    MS_CommandStatusWrapper_t CmdStatus;
    SCSI_Request_Sense_Response_t SenceData;
    bool DecodeSCSICommand();
    // Scsi commands
    bool CmdInquiry();
    bool CmdRequestSense();
    bool CmdReadCapacity10();
    bool CmdSendDiagnostic();
    bool CmdReadWrite10(ReadWrite_t ReadWrite);
    bool CmdModeSense6();
public:
    void Init();
    void UsbOutTask();
};

extern MassStorage_t MassStorage;

#endif /* MASSSTORAGE_H_ */
