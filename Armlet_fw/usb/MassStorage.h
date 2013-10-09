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

/*  Mass Storage Class SCSI Sense Structure
 *  Type define for a SCSI Sense structure. Structures of this type are filled out by the
 *  device via the MS_Host_RequestSense() function, indicating the current sense data of the
 *  device (giving explicit error codes for the last issued command). For details of the
 *  structure contents, refer to the SCSI specifications.
 */
struct SCSI_Request_Sense_Response_t {
    uint8_t  ResponseCode;
    uint8_t  SegmentNumber;
    unsigned SenseKey            : 4;
    unsigned Reserved            : 1;
    unsigned ILI                 : 1;
    unsigned EOM                 : 1;
    unsigned FileMark            : 1;
    uint8_t  Information[4];
    uint8_t  AdditionalLength;
    uint8_t  CmdSpecificInformation[4];
    uint8_t  AdditionalSenseCode;
    uint8_t  AdditionalSenseQualifier;
    uint8_t  FieldReplaceableUnitCode;
    uint8_t  SenseKeySpecific[3];
} __attribute__ ((__packed__));

#endif

#if 1 //==== SCSI Commands ====
#define SCSI_CMD_INQUIRY                               0x12
#define SCSI_CMD_REQUEST_SENSE                         0x03
#define SCSI_CMD_TEST_UNIT_READY                       0x00
#define SCSI_CMD_READ_CAPACITY_10                      0x25
#define SCSI_CMD_SEND_DIAGNOSTIC                       0x1D
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL          0x1E
#define SCSI_CMD_WRITE_10                              0x2A
#define SCSI_CMD_READ_10                               0x28
#define SCSI_CMD_WRITE_6                               0x0A
#define SCSI_CMD_READ_6                                0x08
#define SCSI_CMD_VERIFY_10                             0x2F
#define SCSI_CMD_MODE_SENSE_6                          0x1A
#define SCSI_CMD_MODE_SENSE_10                         0x5A
#endif

#if 1 // ==== SCSI Sense Key Values ====
/** SCSI Sense Code to indicate no error has occurred. */
#define SCSI_SENSE_KEY_GOOD                            0x00
/** SCSI Sense Code to indicate that the device has recovered from an error. */
#define SCSI_SENSE_KEY_RECOVERED_ERROR                 0x01
/** SCSI Sense Code to indicate that the device is not ready for a new command. */
#define SCSI_SENSE_KEY_NOT_READY                       0x02
/** SCSI Sense Code to indicate an error whilst accessing the medium. */
#define SCSI_SENSE_KEY_MEDIUM_ERROR                    0x03
/** SCSI Sense Code to indicate a hardware error has occurred. */
#define SCSI_SENSE_KEY_HARDWARE_ERROR                  0x04
/** SCSI Sense Code to indicate that an illegal request has been issued. */
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                 0x05
/** SCSI Sense Code to indicate that the unit requires attention from the host to indicate
 *  a reset event, medium removal or other condition. */
#define SCSI_SENSE_KEY_UNIT_ATTENTION                  0x06
/** SCSI Sense Code to indicate that a write attempt on a protected block has been made. */
#define SCSI_SENSE_KEY_DATA_PROTECT                    0x07
/** SCSI Sense Code to indicate an error while trying to write to a write-once medium. */
#define SCSI_SENSE_KEY_BLANK_CHECK                     0x08
/** SCSI Sense Code to indicate a vendor specific error has occurred. */
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                 0x09
/** SCSI Sense Code to indicate that an EXTENDED COPY command has aborted due to an error. */
#define SCSI_SENSE_KEY_COPY_ABORTED                    0x0A
/** SCSI Sense Code to indicate that the device has aborted the issued command. */
#define SCSI_SENSE_KEY_ABORTED_COMMAND                 0x0B
/** SCSI Sense Code to indicate an attempt to write past the end of a partition has been made. */
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                 0x0D
/** SCSI Sense Code to indicate that the source data did not match the data read from the medium. */
#define SCSI_SENSE_KEY_MISCOMPARE                      0x0E
#endif

#if 1 // ==== SCSI Additional Sense Codes ====
/** SCSI Additional Sense Code to indicate no additional sense information is available. */
#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION          0x00
/** SCSI Additional Sense Code to indicate that the logical unit (LUN) addressed is not ready. */
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY             0x04
/** SCSI Additional Sense Code to indicate an invalid field was encountered while processing the issued command. */
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB               0x24
/** SCSI Additional Sense Code to indicate that a medium that was previously indicated as not ready has now become ready for use. */
#define SCSI_ASENSE_NOT_READY_TO_READY_CHANGE          0x28
/** SCSI Additional Sense Code to indicate that an attempt to write to a protected area was made. */
#define SCSI_ASENSE_WRITE_PROTECTED                    0x27
/** SCSI Additional Sense Code to indicate an error whilst formatting the device medium. */
#define SCSI_ASENSE_FORMAT_ERROR                       0x31
/** SCSI Additional Sense Code to indicate an invalid command was issued. */
#define SCSI_ASENSE_INVALID_COMMAND                    0x20
/** SCSI Additional Sense Code to indicate a write to a block out outside of the medium's range was issued. */
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21
/** SCSI Additional Sense Code to indicate that no removable medium is inserted into the device. */
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A
#endif

#if 1 // ==== SCSI Additional Sense Key Code Qualifiers ====
/** SCSI Additional Sense Qualifier Code to indicate no additional sense qualifier information is available. */
#define SCSI_ASENSEQ_NO_QUALIFIER                      0x00
/** SCSI Additional Sense Qualifier Code to indicate that a medium format command failed to complete. */
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01
// SCSI Additional Sense Qualifier Code to indicate that an initializing command must be issued before the issued command can be executed.
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02
/** SCSI Additional Sense Qualifier Code to indicate that an operation is currently in progress. */
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07
#endif

enum ReadWrite_t {rwRead, rwWrite};
#define MS_OUTBUF_SZ     128
class MassStorage_t {
    uint8_t QueueBuf[MS_OUTBUF_SZ];
    InputQueue IOutQueue;   // Host to Device, USB convention
    MS_CommandBlockWrapper_t CmdBlock;
    MS_CommandStatusWrapper_t CmdStatus;
    SCSI_Request_Sense_Response_t SenceData;
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
