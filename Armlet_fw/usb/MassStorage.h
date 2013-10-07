/*
 * MassStorage.h
 *
 *  Created on: Oct 7, 2013
 *      Author: g.kruglov
 */

#ifndef MASSSTORAGE_H_
#define MASSSTORAGE_H_

#include "usb_f2.h"

// Config
#define NUMBER_OF_LUNS  1   // Number of logical disks

#if 1 // ======================= Mass Storage constants ========================
// Enum for the Mass Storage class specific control requests that can be issued by the USB bus host
enum MS_ClassRequests_t {
    // Mass Storage class-specific request to retrieve the total number of Logical Units (drives) in the SCSI device.
    MS_REQ_GetMaxLUN = 0xFE,
    // Mass Storage class-specific request to reset the Mass Storage interface, ready for the next command.
    MS_REQ_MassStorageReset = 0xFF,
};
#endif

#define MASS_STORAGE_BUF_SZ     128
class MassStorage_t {
    uint8_t Buf[MASS_STORAGE_BUF_SZ];

public:
    void Init();
};

extern MassStorage_t MassStorage;

#endif /* MASSSTORAGE_H_ */
