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
#include "buf_sizes.h"
#include "stm32_otg.h"

#if 1 // ========================== Endpoint ===================================
enum EpState_t {esIdle, esSetup, esInData, esOutData, esInStatus, esOutStatus, esError};
enum EpPktState_t {psNoPkt, psDataPkt, psZeroPkt};

class Ep_t {
private:
    uint8_t Indx;
public:
    EpState_t State;
    EpPktState_t PktState;
    Thread *PThread;
    uint8_t *PtrIn;
    uint32_t LengthIn;
    bool TransmitFinalZeroPkt;
//    InputQueue *POutQueue;
    void Init(const EpCfg_t *PCfg);
    void StallIn()  { OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_STALL; }
    void StallOut() { OTG_FS->oe[Indx].DOEPCTL |= DOEPCTL_STALL; }
    inline bool FifoEmtyIRQEnabled()    { return (OTG_FS->DIEPEMPMSK & (1 << Indx)); }
    inline void EnableInFifoEmptyIRQ()  { OTG_FS->DIEPEMPMSK |=  (1 << Indx); }
    inline void DisableInFifoEmptyIRQ() { OTG_FS->DIEPEMPMSK &= ~(1 << Indx); }
    inline bool InFifoEmptyIRQEnabled() { return (OTG_FS->DIEPEMPMSK & DIEPEMPMSK_INEPTXFEM(Indx)); }

    void FillInBuf();
    inline void PrepareInTransaction() {
        uint32_t pcnt = (LengthIn + EpCfg[Indx].InMaxsize - 1) / EpCfg[Indx].InMaxsize;
        OTG_FS->ie[Indx].DIEPTSIZ = DIEPTSIZ_PKTCNT(pcnt) | LengthIn;
    }
    inline void StartInTransaction()  {
        OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;   // Enable Ep and clear NAK
        EnableInFifoEmptyIRQ();
    }
    inline void StartOutTransaction() { OTG_FS->oe[Indx].DOEPCTL |= DOEPCTL_CNAK; }
    //void TransmitDataChunk() { FillInBuf(); StartInTransaction(); }
    void TransmitZeroPkt();
    void ReceivePkt();
    void ReadToBuf(uint8_t *PDstBuf, uint16_t Len);
//    void ReadToQueue(uint16_t Len);
//    uint16_t GetRxDataLength();
//    void FlushRx(uint16_t Len);
    void ResumeWaitingThd(msg_t ReadyMsg);
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
#define USB_RX_SZ_WORDS     128//256 // => Sz_in_bytes = 256*4 = 1024; 256 is maximum
//#define USB_TX_SZ


enum UsbState_t {usDisconnected, usConnected, usConfigured};

class Usb_t {
private:
    Ep_t Ep[EP_CNT];
    uint8_t Ep0InBuf[EP0_SZ];
    union {
        uint8_t Ep0OutBuf[EP0_SZ];
        UsbSetupReq_t SetupReq;
    };
    uint8_t Ep1OutBuf[EP0_SZ];
    // Initialization
    void IDeviceReset();
    void IEndpointsDisable();
    void IRamInit();
    void ICtrHandlerIN(uint16_t EpID);
    void ICtrHandlerOUT(uint16_t EpID, uint16_t Epr);
    void IEpOutHandler(uint8_t EpID);
    void IEpInHandler(uint8_t EpID);
    void RxFifoFlush();
    void TxFifoFlush();
    void SetupPktHandler();
    EpState_t DefaultReqHandler(uint8_t **PPtr, uint32_t *PLen);
    void PrepareInTransaction(uint8_t *Ptr, uint32_t ALen);
public:
    UsbState_t State;
    void Init();
    void Connect()    { OTG_FS->GCCFG |=  GCCFG_VBUSBSEN | GCCFG_NOVBUSSENS; }
    void Disconnect() { OTG_FS->GCCFG &= ~GCCFG_VBUSBSEN; }
    void Shutdown();
    // Data operations
//    void StartTransmitBuf(uint8_t EpID, uint8_t *Ptr, uint32_t ALen) { StartTransmitTwoBufs(EpID, Ptr, ALen, NULL, 0); }
//    void StartTransmitTwoBufs(uint8_t EpID, uint8_t *Ptr1, uint32_t ALen1, uint8_t *Ptr2, uint32_t ALen2);
//    uint8_t WaitTransactionEnd(uint8_t EpID);
//    inline void AssignEpOutQueue(uint8_t EpID, InputQueue *PQueue) { Ep[EpID].POutQueue = PQueue; }
    // Inner use
    void IIrqHandler();
    void IRxHandler();
    uint16_t IGetSetupAddr() { return SetupReq.wValue; }
    void ISetAddress(uint8_t AAddr) { OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(AAddr); }
//    inline void IStartReception(uint8_t EpID) { Ep[EpID].StartOutTransaction(); }
//    uint16_t IGetSetupAddr() { return SetupReq.wValue; }
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

#define USB_RTYPE_DIR_DEV2HOST              0x80
#define USB_RTYPE_RECIPIENT_MASK            0x1F
#define USB_RTYPE_RECIPIENT_DEVICE          0x00
#define USB_RTYPE_RECIPIENT_INTERFACE       0x01
#define USB_RTYPE_RECIPIENT_ENDPOINT        0x02
#define USB_RTYPE_RECIPIENT_OTHER           0x03
#endif

#if 1 // ============================ stm32f2 related ==========================
#define EP_TYPE_CONTROL



#endif

#endif /* KL_USB_F2_H_ */
