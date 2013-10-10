/*
 * scsi.c
 *
 *  Created on: Oct 9, 2013
 *      Author: g.kruglov
 */

#include "scsi.h"

const SCSI_InquiryResponse_t InquiryData = {
        DeviceType: 0,     // Block Media device
        PeripheralQualifier: 0,

        Removable: true,

        Version: 0,

        ResponseDataFormat: 2,
        NormACA: false,
        TrmTsk: false,
        AERC: false,

        AdditionalLength: 0x1F,

        SoftReset: false,
        CmdQue: false,
        Linked: false,
        Sync: false,
        WideBus16Bit: false,
        WideBus32Bit: false,
        RelAddr: false,

        VendorID: "Ostranna",
        ProductID: "MassStorage",
        RevisionID: {'1', '.', '0', '0'}
};
