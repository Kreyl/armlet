/*
 * scsi.h
 *
 *  Created on: Oct 9, 2013
 *      Author: g.kruglov
 */

#ifndef SCSI_H_
#define SCSI_H_

#include "inttypes.h"
#include "stdbool.h"

/** Mass Storage Class SCSI Inquiry Structure.
 *  Type define for a SCSI Inquiry structure. Structures of this type are filled out by the
 *  device via the MS_Host_GetInquiryData() function, retrieving the attached device's
 *  information.
 *  For details of the structure contents, refer to the SCSI specifications.
 */
typedef struct {
    unsigned DeviceType          : 5;
    unsigned PeripheralQualifier : 3;

    unsigned Reserved            : 7;
    unsigned Removable           : 1;

    uint8_t  Version;

    unsigned ResponseDataFormat  : 4;
    unsigned Reserved2           : 1;
    unsigned NormACA             : 1;
    unsigned TrmTsk              : 1;
    unsigned AERC                : 1;

    uint8_t  AdditionalLength;
    uint8_t  Reserved3[2];

    unsigned SoftReset           : 1;
    unsigned CmdQue              : 1;
    unsigned Reserved4           : 1;
    unsigned Linked              : 1;
    unsigned Sync                : 1;
    unsigned WideBus16Bit        : 1;
    unsigned WideBus32Bit        : 1;
    unsigned RelAddr             : 1;

    uint8_t  VendorID[8];
    uint8_t  ProductID[16];
    uint8_t  RevisionID[4];
} __attribute__ ((__packed__)) SCSI_Inquiry_Response_t;

extern const SCSI_Inquiry_Response_t InquiryData;

#endif /* SCSI_H_ */
