/*
 * kl_usb_f2.h
 *
 *  Created on: 22 сент. 2013 г.
 *      Author: kreyl
 */

#ifndef KL_USB_F2_H_
#define KL_USB_F2_H_

#include "ch.h"
#include "hal.h"
#include "kl_lib_f2xx.h"
#include "descriptors.h"
#include "stm32_otg.h"

#if 1 // ========================== Endpoint ===================================
enum EpState_t {esIdle, esSetup, esInData, esOutData, esInStatus, esOutStatus, esError};
enum EpPktState_t {psNoPkt, psDataPkt, psZeroPkt};

class Ep_t {
private:
    uint8_t Indx;
    InputQueue *POutQueue;
    EpState_t State;
    EpPktState_t PktState;
    Thread *PThread;
    bool Buzy;
    uint8_t *PtrIn, *PtrOut;
    uint32_t LengthIn, LengthOut;
    bool TransmitFinalZeroPkt;
    void ClearInNAK() { OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_CNAK; }
    // IRQ
    bool FifoEmtyIRQEnabled()    { return (OTG_FS->DIEPEMPMSK & (1 << Indx)); }
    void EnableInFifoEmptyIRQ()  { OTG_FS->DIEPEMPMSK |=  (1 << Indx); }
    void DisableInFifoEmptyIRQ() { OTG_FS->DIEPEMPMSK &= ~(1 << Indx); }
    bool InFifoEmptyIRQEnabled() { return (OTG_FS->DIEPEMPMSK & DIEPEMPMSK_INEPTXFEM(Indx)); }
    // ==== IN ====
    void BufToFifo();
    void PrepareInTransaction() {
        uint32_t pcnt = (LengthIn + EpCfg[Indx].InMaxsize - 1) / EpCfg[Indx].InMaxsize;
        OTG_FS->ie[Indx].DIEPTSIZ = DIEPTSIZ_PKTCNT(pcnt) | LengthIn;
    }
    void StartInTransaction()  {
        OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;   // Enable Ep and clear NAK
        EnableInFifoEmptyIRQ();
    }
    void TransmitZeroPkt();
    // ==== OUT ====
    void PrepareOutTransaction(uint32_t PktCnt, uint32_t Len) {
        OTG_FS->oe[Indx].DOEPTSIZ = DOEPTSIZ_STUPCNT(3) | DOEPTSIZ_PKTCNT(PktCnt) | Len;
    }
    void PrepareOutTransaction(uint32_t Len) {
        uint32_t PktCnt = (Len + EpCfg[Indx].OutMaxsize - 1) / EpCfg[Indx].OutMaxsize;
        OTG_FS->oe[Indx].DOEPTSIZ = DOEPTSIZ_STUPCNT(3) | DOEPTSIZ_PKTCNT(PktCnt) | Len;
    }
    void StartOutTransaction() { OTG_FS->oe[Indx].DOEPCTL |= DOEPCTL_CNAK; }
    void FifoToBuf(uint8_t *PDstBuf, uint32_t Len);
    void FifoToQueue(uint32_t Len);
//    uint16_t GetRxDataLength();
    void ResumeWaitingThd(uint8_t ReadyMsg);
public:
    // OUT
    uint32_t GetRcvResidueLen() { return LengthOut; }
    void StartReceiveToBuf(uint8_t *PDst, uint32_t Len);
    void AssignOutQueue(InputQueue *PQueue) { POutQueue = PQueue; }
    // IN
    void StartTransmitBuf(uint8_t *PSrc, uint32_t ALen);
    // Common
    uint8_t WaitUntilReady();
    // Stall operations
    void SetStallIn()    { OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_STALL; }
    void SetStallOut()   { OTG_FS->oe[Indx].DOEPCTL |= DOEPCTL_STALL; }
    void ClearStallIn()  { OTG_FS->ie[Indx].DIEPCTL &= ~DIEPCTL_STALL; }
    void ClearStallOut() { OTG_FS->oe[Indx].DOEPCTL &= ~DOEPCTL_STALL; }
    bool IsStalledIn()   { return (OTG_FS->ie[Indx].DIEPCTL & DIEPCTL_STALL); }
    bool IsStalledOut()  { return (OTG_FS->oe[Indx].DOEPCTL & DOEPCTL_STALL); }
    // NAK operations
    void SetNakOut() { OTG_FS->oe[Indx].DOEPCTL |= DOEPCTL_SNAK; }
    // Inner use
    friend class Usb_t;
};
#endif

struct UsbSetupReq_t {
    uint8_t bmRequestType;
    uint8_t bRequest;
    union {
        uint16_t wValue;
        struct {
            uint8_t Indx;
            uint8_t Type;
        } __attribute__ ((__packed__));
    } __attribute__ ((__packed__));
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__ ((__packed__));

#if 1 // ============================ Usb_t ====================================
#define USB_RX_SZ_WORDS     256 // => Sz_in_bytes = 256*4 = 1024; 256 is maximum

class Usb_t {
private:
    Ep_t Ep[EP_CNT];
    uint8_t Ep0InBuf[EP0_SZ];
    union {
        uint8_t Ep0OutBuf[EP0_SZ];
        UsbSetupReq_t SetupReq;
    };
    uint8_t Ep1OutBuf[EP0_SZ];
    uint8_t Configuration;
    // Initialization
    void IDeviceReset();
    void IEndpointsDisable();
    void IRamInit();
    void IEpOutHandler(uint8_t EpID);
    void IEpInHandler(uint8_t EpID);
    void RxFifoFlush();
    void TxFifoFlush();
    void SetupPktHandler();
    void IRxHandler();
    uint16_t IGetSetupAddr() { return SetupReq.wValue; }
    void ISetAddress(uint8_t AAddr) { OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(AAddr); }
    EpState_t DefaultReqHandler(uint8_t **PPtr, uint32_t *PLen);
    void PrepareInTransaction(uint8_t *Ptr, uint32_t ALen);
    void IEndpointsInit();
public:
    bool IsReady;
    Thread *PThread;
    void Init();
    void Connect()    { OTG_FS->GCCFG |=  GCCFG_VBUSBSEN | GCCFG_NOVBUSSENS; }
    void Disconnect() { OTG_FS->GCCFG &= ~GCCFG_VBUSBSEN; }
    void Shutdown();
    void DeInit() {
        Disconnect();
        Shutdown();
        IDeviceReset();
        IEndpointsDisable();
    }
    Ep_t *PEpBulkOut, *PEpBulkIn;
    // Data operations
    EpState_t NonStandardControlRequestHandler(uint8_t **PPtr, uint32_t *PLen);
    // Inner use
    void IIrqHandler();
//    inline void IStartReception(uint8_t EpID) { Ep[EpID].StartOutTransaction(); }
    friend class Ep_t;
};

extern Usb_t Usb;
#endif

#if 1 // =========================== Constants =================================
#define USB_REQ_GET_STATUS                  0
#define USB_REQ_CLEAR_FEATURE               1
#define USB_REQ_SET_FEATURE                 3
#define USB_REQ_SET_ADDRESS                 5
#define USB_REQ_GET_DESCRIPTOR              6
#define USB_REQ_SET_DESCRIPTOR              7
#define USB_REQ_GET_CONFIGURATION           8
#define USB_REQ_SET_CONFIGURATION           9
#define USB_REQ_GET_INTERFACE               10
#define USB_REQ_SET_INTERFACE               11
#define USB_REQ_SYNCH_FRAME                 12

#define USB_FEATURE_ENDPOINT_HALT           0
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP    1
#define USB_FEATURE_TEST_MODE               2

// Setup request type (bmRequestType)
#define USB_REQTYPE_DEV2HOST                (1<<7)
#define USB_REQTYPE_HOST2DEV                (0<<7)
#define USB_REQTYPE_STANDARD                (0<<5)
#define USB_REQTYPE_CLASS                   (1<<5)
#define USB_REQTYPE_VENDOR                  (2<<5)
#define USB_REQTYPE_RECIPIENT_MASK          0x1F
#define USB_REQTYPE_RECIPIENT_DEVICE        0x00
#define USB_REQTYPE_RECIPIENT_INTERFACE     0x01
#define USB_REQTYPE_RECIPIENT_ENDPOINT      0x02
#define USB_REQTYPE_RECIPIENT_OTHER         0x03
#endif

#endif /* KL_USB_F2_H_ */
