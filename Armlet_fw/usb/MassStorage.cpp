/*
 * MassStorage.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: g.kruglov
 */

#ifndef MASSSTORAGE_CPP_
#define MASSSTORAGE_CPP_

#include "MassStorage.h"

MassStorage_t MassStorage;
static uint8_t SByte;

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


#endif /* MASSSTORAGE_CPP_ */
