/*
 * usb_f2.cpp
 *
 *  Created on: 22 ����. 2013 �.
 *      Author: kreyl
 */

#include "usb_f2.h"

#define TRDT        3   // AHB clock = 24, => TRDT = 4 * (24/48) +1

// Ep0 size bits
#if (EP0_SZ == 8)
#define EP0_SZ_BITS ((uint32_t)0b11)
#elif (EP0_SZ == 64)
#define EP0_SZ_BITS ((uint32_t)0b00)
#endif

#define EP0_VERBOSE

#ifdef EP0_VERBOSE
#define EP0_PRINT(S)                Uart.Printf(S)
#define EP0_PRINT_V1(S, a1)         Uart.Printf(S, a1)
#define EP0_PRINT_V1V2(S, a1, a2)   Uart.Printf(S, a1, a2)
#else
#define EP0_PRINT(S)
#define EP0_PRINT_V1(S, a1)
#define EP0_PRINT_V1V2(S, a1, a2)
#endif

static uint8_t ZeroArr[2] = {0, 0};
Usb_t Usb;

const uint8_t carr[18] = {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9,
        0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1
};


#if 0 // ============================ Thread ===================================
static WORKING_AREA(waUsbRxThd, 512);
__attribute__((noreturn))
static void UsbRxThread(void *arg) {
    chRegSetThreadName("UsbRx");
    while(1) {
        // TODO: TX handling
        // Check if rx buf empty
        chSysLock();
        if(!(OTG_FS->GINTSTS & GINTSTS_RXFLVL)) {
            OTG_FS->GINTMSK |= GINTMSK_RXFLVLM;     // Reenable rx not empty IRQ
            chSchGoSleepS(THD_STATE_SUSPENDED);
        }
        chSysUnlock();

        // Rx handling
        while(OTG_FS->GINTSTS & GINTSTS_RXFLVL) Usb.IRxHandler();
    } // while 1
}
#endif

#if 1 // ========================== Low level ==================================
void Usb_t::Init() {
    // GPIO
    PinSetupAlterFunc(GPIOA, 11, omOpenDrain, pudNone, AF10);
    PinSetupAlterFunc(GPIOA, 12, omOpenDrain, pudNone, AF10);
    PinSetupIn(GPIOA, 9, pudPullDown);
    // OTG FS clock enable and reset
    rccEnableOTG_FS(FALSE);
    rccResetOTG_FS();

    // Rx thread
    //PRxThd = chThdCreateStatic(waUsbRxThd, sizeof(waUsbRxThd), NORMALPRIO, (tfunc_t)UsbRxThread, NULL);

    // Enable IRQ
    nvicEnableVector(STM32_OTG1_NUMBER, CORTEX_PRIORITY_MASK(IRQ_PRIO_LOW));

    // ==== OTG init ====
    // Forced device mode, USB turn-around time = TRDT_VALUE, Full Speed 1.1 PHY, 0 tuning
    OTG_FS->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT) | GUSBCFG_PHYSEL | 0;
    OTG_FS->DCFG = 0x02200000 | DCFG_DSPD_FS11; // Full-speed (other options are not available, though)
    OTG_FS->PCGCCTL = 0;    // Nothing is stopped or gated
    OTG_FS->GCCFG = GCCFG_PWRDWN /*| GCCFG_NOVBUSSENS*/;    // Power on
    chThdSleepMilliseconds(20);

    // Core reset and delay of at least 3 PHY cycles
    OTG_FS->GRSTCTL = GRSTCTL_CSRST;
    while((OTG_FS->GRSTCTL & GRSTCTL_CSRST) != 0);
    __NOP();
    __NOP();
    // Wait AHB idle condition
    while((OTG_FS->GRSTCTL & GRSTCTL_AHBIDL) == 0);
    __NOP();
    __NOP();

    OTG_FS->GAHBCFG &= ~(GAHBCFG_GINTMSK | GAHBCFG_TXFELVL);    // Disable interrupts, interrupts on TXFIFOs half empty
    IEndpointsDisable();
    // Disable all Device Interrupts, only the USB Reset interrupt is required initially
    OTG_FS->DIEPMSK  = 0;
    OTG_FS->DOEPMSK  = 0;
    OTG_FS->DAINT    = 0xFFFFFFFF;
    OTG_FS->DAINTMSK = 0;
    OTG_FS->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM /*| GINTMSK_USBSUSPM | GINTMSK_ESUSPM  |*/;
    OTG_FS->GINTSTS  = 0xFFFFFFFF;      // Clear all pending IRQs, if any
    OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK; // Global interrupts enable
}

void Usb_t::Shutdown() {
    OTG_FS->PCGCCTL |= PCGCCTL_STPPCLK | PCGCCTL_GATEHCLK; // Stop PHY clock, gate HCLK
}

void Usb_t::IDeviceReset() {
    Uart.Printf("Rst\r");
    State = usConnected;
    TxFifoFlush(); // TX FIFO flush
    // Force all EPs in NAK mode and clear irqs
    for (uint8_t i = 0; i <= EP_CNT; i++) {
        OTG_FS->ie[i].DIEPCTL = DIEPCTL_SNAK;
        OTG_FS->oe[i].DOEPCTL = DOEPCTL_SNAK;
        OTG_FS->ie[i].DIEPINT = 0xFF;
        OTG_FS->oe[i].DOEPINT = 0xFF;
    }
    // Disable and clear all EPs irqs
    OTG_FS->DAINT = 0xFFFFFFFF;
    OTG_FS->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);

    IRamInit();     // Setup RAM
    ISetAddress(0); // Reset device address to zero

    // Enable EP irqs
    OTG_FS->GINTMSK  |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
    OTG_FS->DIEPMSK   = DIEPMSK_TOCM    | DIEPMSK_XFRCM;
    OTG_FS->DOEPMSK   = DOEPMSK_STUPM   | DOEPMSK_XFRCM;

    // Init EP0
    OTG_FS->oe[0].DOEPTSIZ = DOEPTSIZ_STUPCNT(3) | DOEPTSIZ_PKTCNT(1) | DOEPTSIZ_XFRSIZ(3 * 8);
    OTG_FS->oe[0].DOEPCTL = DOEPCTL_USBAEP | DOEPCTL_EPTYP_CTRL | EP0_SZ_BITS;
    OTG_FS->ie[0].DIEPTSIZ = 0;
    OTG_FS->ie[0].DIEPCTL = DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL | DIEPCTL_TXFNUM(0) | EP0_SZ_BITS;
}

void Usb_t::IRamInit() {
    // ==== RX FIFO ====
    uint32_t tmp = OTG_FS->GRXFSIZ;
    tmp &= 0xFFFF0000;  // Clear lower bits, save upper ones
    tmp |= USB_RX_SZ_WORDS;
    OTG_FS->GRXFSIZ = tmp;
    // ==== TX FIFO ====
    uint32_t TxAddr = USB_RX_SZ_WORDS;  // FIXME: Really words, not bytes?
    uint32_t RamSz;
    // Ep0 TX
    RamSz = (((2*EP0_SZ) / 4) < 16)? 16 : ((2*EP0_SZ) / 4); // Min size is 16 words
    OTG_FS->DIEPTXF0 = (uint32_t)((RamSz << 16) | TxAddr);
    TxAddr += RamSz;
    // Other endpoints
    for(uint8_t i=1; i<EP_CNT; i++) {
        if(EpCfg[i].InMaxsize != 0) {   // Configure RAM for only IN eps
            RamSz = (((2*EpCfg[i].InMaxsize) / 4) < 16)? 16 : ((2*EpCfg[i].InMaxsize) / 4); // Min size is 16 words
            OTG_FS->DIEPTXF[i-1] = (uint32_t)((RamSz << 16) | TxAddr);
            TxAddr += RamSz;
        } // if !0
    } // for
    // ==== Flush RAM ====
    TxFifoFlush();
    RxFifoFlush();
}

void Usb_t::RxFifoFlush() {
    OTG_FS->GRSTCTL = GRSTCTL_RXFFLSH;
    while((OTG_FS->GRSTCTL & GRSTCTL_RXFFLSH) != 0);
    // Wait for 3 PHY Clocks
    __NOP();
    __NOP();
}
void Usb_t::TxFifoFlush() {
    OTG_FS->GRSTCTL = (0b10000 << 6) | GRSTCTL_TXFFLSH;  // 10000: Flush all the transmit FIFOs
    while((OTG_FS->GRSTCTL & GRSTCTL_TXFFLSH) != 0);
    // Wait for 3 PHY Clocks
    __NOP();
    __NOP();
}

void Usb_t::IEndpointsDisable() {
    for(uint8_t i=0; i<EP_CNT; i++) {
        // IN: Disable only if enabled
        if((OTG_FS->ie[i].DIEPCTL & DIEPCTL_EPENA) != 0) {
            OTG_FS->ie[i].DIEPCTL = DIEPCTL_EPDIS | DIEPCTL_SNAK;
            while(!(OTG_FS->ie[i].DIEPINT & DIEPINT_EPDISD)); // Wait for endpoint disable
        }
        else OTG_FS->ie[i].DIEPCTL = 0;
        OTG_FS->ie[i].DIEPTSIZ = 0;
        OTG_FS->ie[i].DIEPINT = 0xFF;
        // OUT: Disable only if enabled
        if((OTG_FS->oe[i].DOEPCTL & DOEPCTL_EPENA) != 0) {
            OTG_FS->oe[i].DOEPCTL = DOEPCTL_EPDIS | DOEPCTL_SNAK;
            while(!(OTG_FS->oe[i].DOEPINT & DOEPINT_OTEPDIS)); // Wait for endpoint disable
        }
        else OTG_FS->oe[i].DOEPCTL = 0;
        OTG_FS->oe[i].DOEPTSIZ = 0;
        OTG_FS->oe[i].DOEPINT = 0xFF;
    } // for
    OTG_FS->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);
}

// Only receives data, does not process it
void Usb_t::IRxHandler() {
    uint32_t sts;//, cnt, EpID;
    Ep_t *PEp;
    sts = OTG_FS->GRXSTSP;
//    Uart.Printf("Rx sts=%X\r", sts);
    Uart.Printf("Rx%u Pkt sts: %X; Len: %u\r", sts&0xF, (sts>>17)&0xF, (sts>>4)&0x7FF);
    switch (sts & GRXSTSP_PKTSTS_MASK) {
        case GRXSTSP_SETUP_DATA:
//            cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
//            EpID = sts & GRXSTSP_EPNUM_MASK;
            PEp = &Ep[0];
            PEp->ReadToBuf(Ep0OutBuf, 8);
            break;
        case GRXSTSP_OUT_DATA:
//          cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
//          EpID = sts & GRXSTSP_EPNUM_MASK;
          // TODO: read to queue
          break;
        case GRXSTSP_SETUP_COMP:    // Setup transaction completed
        case GRXSTSP_OUT_GLOBAL_NAK:
        case GRXSTSP_OUT_COMP:
        default: break;
    } // switch
}
#endif

#if 1 // =========================== High level ================================
void Usb_t::SetupPktHandler() {
    EP0_PRINT("Setup\r");
    Uart.Printf("%A\r", Ep0OutBuf, 8, ' ');
    // Try to handle request
    uint8_t *FPtr;
    uint32_t FLength;
    if((SetupReq.bmRequestType & 0x60) != 0) Ep[0].State = esError;   // Non-standard request
    else Ep[0].State = DefaultReqHandler(&FPtr, &FLength);
    // Prepare to next transaction
    switch(Ep[0].State) {
        case esDataIn:
            Ep[0].PtrIn = (uint8_t*)carr; // FPtr;
            Ep[0].LengthIn = 18;// FLength;
            Ep[0].PrepareInTransaction();
            Ep[0].StartInTransaction();
            break;
        case esStatusIn:
            Ep[0].TransmitZeroPkt();
            break;
        default:
            Ep[0].StallIn();
            Ep[0].StallOut();
            break;
    } // switch
}

// ==== Ep0 callbacks ====
static void SetAddress() {
    uint32_t Addr = Usb.IGetSetupAddr();
    Uart.Printf("SetAddr %u\r", Addr);
    Usb.ISetAddress(Addr);
}
static void SetStateConfigured() {
    Uart.Printf("UsbConfigured\r");
    Usb.State = usConfigured;
    // Allow OUT communications
//    Usb.IStartReception(EP_BULK_OUT_INDX);
}


EpState_t Usb_t::DefaultReqHandler(uint8_t **PPtr, uint32_t *PLen) {
    uint8_t Recipient = SetupReq.bmRequestType & USB_RTYPE_RECIPIENT_MASK;
    if(Recipient == USB_RTYPE_RECIPIENT_DEVICE) {
        //Uart.Printf("Dev\r\n");
        switch(SetupReq.bRequest) {
            case USB_REQ_GET_STATUS:    // Just return the current status word
                EP0_PRINT("GetStatus\r");
                *PPtr = ZeroArr;    // Remote wakeup = 0, selfpowered = 0
                *PLen = 2;
                return esDataIn;
                break;
            case USB_REQ_SET_ADDRESS:
                EP0_PRINT("SetAddr\r");
                *PLen = 0;
                Ep[0].cbEndTransaction = SetAddress;
                return esStatusIn;
                break;
            case USB_REQ_GET_DESCRIPTOR:
                EP0_PRINT_V1V2("GetDesc t=%u i=%u\r", SetupReq.Type, SetupReq.Indx);
                GetDescriptor(SetupReq.Type, SetupReq.Indx, PPtr, PLen);
                // Trim descriptor if needed, as host can request part of descriptor.
                TRIM_VALUE(*PLen, SetupReq.wLength);
//                Uart.Printf("DescLen=%u\r", PBuf->Len);
                if(*PLen != 0) return esDataIn;
                break;
            case USB_REQ_GET_CONFIGURATION:
                EP0_PRINT("GetCnf\r");
//                *Ptr = &Configuration;
//                *PLen = 1;
//                return OK;
                break;
            case USB_REQ_SET_CONFIGURATION:
                EP0_PRINT_V1("SetCnf %u\r", SetupReq.wValue);
                *PLen = 0;
                Ep[0].cbEndTransaction = SetStateConfigured;
                return esStatusIn;
                break;
            default: break;
        } // switch
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_INTERFACE) {
        if(SetupReq.bRequest == USB_REQ_GET_STATUS) {
            EP0_PRINT("InterfGetSta\r");
//            *Ptr = (uint8_t*)ZeroStatus;
//            *PLen = 2;
//            return OK;
        }
    }
    else if(Recipient == USB_RTYPE_RECIPIENT_ENDPOINT) {
        EP0_PRINT("Ep\r");
        switch(SetupReq.bRequest) {
            case USB_REQ_SYNCH_FRAME:
//                *Ptr = (uint8_t*)ZeroStatus;
//                *PLen = 2;
//                return OK;
                break;
            case USB_REQ_GET_STATUS:
                break;
        }
    }
    return esError;
}

#endif

#if 1 // =============================== IRQ ===================================
extern "C" {
CH_IRQ_HANDLER(STM32_OTG1_HANDLER) {
    CH_IRQ_PROLOGUE();
    Usb.IIrqHandler();
    CH_IRQ_EPILOGUE();
}
} // extern C

void Usb_t::IIrqHandler() {
    uint32_t irqs;
    // Get irq flag
    irqs = OTG_FS->GINTSTS & OTG_FS->GINTMSK;
    OTG_FS->GINTSTS = irqs;
    Uart.Printf("Irq: %X\r", irqs);

    // Reset
    if(irqs & GINTSTS_USBRST) IDeviceReset();
    // Enumeration done
    if(irqs & GINTSTS_ENUMDNE) { (void)OTG_FS->DSTS; }    // FIXME: really needed?
    // RX FIFO not empty
    if(irqs & GINTSTS_RXFLVL) {
        // The interrupt must be disabled while the thread has control or it would be triggered again
//        chSysLockFromIsr();
//        OTG_FS->GINTMSK &= ~GINTMSK_RXFLVLM;
//        if(PRxThd != NULL) {
//            if(PRxThd->p_state == THD_STATE_SUSPENDED) chThdResumeI(PRxThd);
//        }
//        chSysUnlockFromIsr();
        IRxHandler();
    }
    // OUT & IN event handling
    if(irqs & GINTSTS_IEPINT) {
        uint32_t src = OTG_FS->DAINT;
        if(src & (1 << 0)) IEpInHandler(0);
        if(src & (1 << 1)) IEpInHandler(1);
        if(src & (1 << 2)) IEpInHandler(2);
        if(src & (1 << 3)) IEpInHandler(3);
    }
    if(irqs & GINTSTS_OEPINT) {
        uint32_t src = OTG_FS->DAINT;
        if(src & (1 << 16)) IEpOutHandler(0);
        if(src & (1 << 17)) IEpOutHandler(1);
        if(src & (1 << 18)) IEpOutHandler(2);
        if(src & (1 << 19)) IEpOutHandler(3);
    }
}

void Usb_t::IEpOutHandler(uint8_t EpID) {
    uint32_t epint = OTG_FS->oe[EpID].DOEPINT;  // Read IRQs
    OTG_FS->oe[EpID].DOEPINT = 0xFF;      // Reset IRQs
    Uart.Printf("=== out %u: %X\r\n", EpID, epint);
//    Uart.Printf("DOEPTSIZ: %X\r\n", OTG_FS->oe[0].DOEPTSIZ);
    // Setup packets handling
    if ((epint & DOEPINT_STUP) and (OTG_FS->DOEPMSK & DOEPMSK_STUPM))
        SetupPktHandler();
    // Receive transfer complete
    if ((epint & DOEPINT_XFRC) and (OTG_FS->DOEPMSK & DOEPMSK_XFRCM)) {
        Uart.Printf(" XFR cmp\r\n");
//        Ep[EpN].IsReceiving = false;
        //if(EpN == 0) Ep0OutCallback();
//        if(Ep[EpN].cbOut != NULL) Ep[EpN].cbOut();
    }


}

void Usb_t::IEpInHandler(uint8_t EpID) {
    uint32_t epint = OTG_FS->ie[EpID].DIEPINT;
    OTG_FS->ie[EpID].DIEPINT = 0xFF;   // Reset IRQs
    Uart.Printf("=== in %X i%X\r\n", EpID, epint);
    // Transmit transfer complete
    if((epint & DIEPINT_XFRC) and (OTG_FS->DIEPMSK & DIEPMSK_XFRCM)) {
        Uart.Printf("XFRC\r\n");
//        Ep[0].ClearOutNAK();
//        Ep[0].EnableOut();
//        Ep[EpN].IsTransmitting = false;
        //if(EpID == 0) Ep0InCallback();
//        if(Ep[EpN].cbIn != NULL) Ep[EpN].cbIn();
    }
    // TX FIFO empty
    if((epint & DIEPINT_TXFE) and (OTG_FS->DIEPEMPMSK & DIEPEMPMSK_INEPTXFEM(EpID))) {
        Uart.Printf("TXFE\r\n");
        Ep[EpID].FillInBuf();

    } // if txfe
}

#endif

#if 1 // ============================ Endpoints ================================
void Ep_t::Init(const EpCfg_t *PCfg) {
    Disable();
    Indx = PCfg->Indx;
    // Process waiting thread if any
    ResumeWaitingThd(RDY_RESET);
    // Set EP type, NAK both directions
    SetType(PCfg->Type);
    NakOutSet();
    NakInSet();
    // Size and address initialization

    Enable();
}

void Ep_t::ResumeWaitingThd(msg_t ReadyMsg) {
    if(PThread != NULL) {
        chSysLockFromIsr();
        if(PThread->p_state == THD_STATE_SUSPENDED) {
            PThread->p_u.rdymsg = ReadyMsg;
            chSchReadyI(PThread);
        }
        chSysUnlockFromIsr();
        PThread = NULL;
    }
}

void Ep_t::ReadToBuf(uint8_t *PDstBuf, uint16_t Len) {
    // Get pointer to Fifo
    volatile uint32_t *PFifo = OTG_FS->FIFO[Indx]; // FIXME: Indx, not 0?
    Len = (Len + 3) / 4;    // Convert bytes count to words count
    while(Len--) {
        uint32_t w = *PFifo;
        *((uint32_t*)PDstBuf) = w;
        PDstBuf += 4;
    }
}

// Fill USB memory with BufIn's data
void Ep_t::FillInBuf() {
    Uart.Printf("In%u: %A\r", Indx, PtrIn, LengthIn, ' ');
    // Prepare variables
    volatile uint32_t *pDst = OTG_FS->FIFO[Indx];
    //uint16_t WrittenBytesCnt = 0;
    // Fill from buffer
    uint32_t n = (LengthIn + 3) / 4;
    while (n > 0) {
        *pDst = *((uint32_t*)PtrIn);
        PtrIn+=4;
        n--;
    }

    //    while((LengthIn != 0) and (WrittenBytesCnt < EpCfg[Indx].InMaxsize)) {
//        // Add quarta of bytes
//        *pDst = *(uint32_t*)PSrc;
//        PSrc += 4;
//        WrittenBytesCnt += 4;
//        // Check if data buffer is not empty
//        if(LengthIn >= 4) LengthIn -= 4;
//        else LengthIn = 0;
//    }


//    while((LengthIn != 0) and (n < EpCfg[Indx].InMaxsize)) {
//        // Add quarta of bytes
//        *pDst = *(uint32_t*)PtrIn;
//        PtrIn += 4;
//        n += 4;
//        // Check if data buffer is not empty
//        if(LengthIn >= 4) LengthIn -= 4;
//        else LengthIn = 0;
//    }
//    TransmitFinalZeroPkt = (n == EpCfg[Indx].InMaxsize) and (LengthIn == 0);
    // Disable TxEmpty IRQ if everything is sent
    //if(LengthIn == 0)
    DisableInFifoEmptyIRQ();

}

void Ep_t::TransmitZeroPkt() {
    OTG_FS->ie[Indx].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
    StartInTransaction();
}

#endif


