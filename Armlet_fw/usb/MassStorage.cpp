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
    Usb.AssignEpOutQueue(EP_BULK_OUT_INDX, &IOutQueue);
    // Thread
    chThdCreateStatic(waUsbOutThd, sizeof(waUsbOutThd), NORMALPRIO, (tfunc_t)UsbOutThd, NULL);
}

void MassStorage_t::UsbOutTask() {
    // Receive header
    if(chIQReadTimeout(&IOutQueue, (uint8_t*)&Cmd, MS_CMD_SZ, TIME_INFINITE) != MS_CMD_SZ) return;
    Uart.Printf("Signature: %X\r", Cmd.Signature);
}

#endif /* MASSSTORAGE_CPP_ */
