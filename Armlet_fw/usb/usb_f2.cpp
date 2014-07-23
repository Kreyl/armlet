/*
 * usb_f2.cpp
 *
 *  Created on: 22 сент. 2013 г.
 *      Author: kreyl
 */

#include "usb_f2.h"
#include "evt_mask.h"
#include "cmd_uart.h"

#define TRDT        0xF  // AHB clock = 24, => TRDT = 4 * (24/48) +1

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

Usb_t Usb;
// Constants
const uint8_t cZero[2] = {0, 0};
const uint8_t cOne[2]  = {1, 0};

#if 1 // ========================== Low level ==================================
void Usb_t::Init() {
    // GPIO
    PinSetupAlterFunc(GPIOA, 11, omOpenDrain, pudNone, AF10);
    PinSetupAlterFunc(GPIOA, 12, omOpenDrain, pudNone, AF10);
    PinSetupIn(GPIOA, 9, pudPullDown);
    // Init Eps' indxs
    PEpBulkOut = &Ep[1];    // Out endpoint
    PEpBulkIn  = &Ep[2];    // In endpoint
    for(uint8_t i=0; i<EP_CNT; i++) Ep[i].Indx = i;

    // OTG FS clock enable and reset
    rccEnableOTG_FS(FALSE);
    rccResetOTG_FS();

    // Enable IRQ
    nvicEnableVector(STM32_OTG1_NUMBER, CORTEX_PRIORITY_MASK(IRQ_PRIO_LOW));

    // ==== OTG init ====
    // Forced device mode, USB turn-around time = TRDT_VALUE, Full Speed 1.1 PHY, 0 tuning
    OTG_FS->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT) | GUSBCFG_PHYSEL | 0;
    chThdSleepMilliseconds(27);
    OTG_FS->DCFG = 0x02200000 | DCFG_DSPD_FS11; // Full-speed (other options are not available, though)
    OTG_FS->PCGCCTL = 0;    // Nothing is stopped or gated
    OTG_FS->GCCFG = GCCFG_PWRDWN /*| GCCFG_NOVBUSSENS*/;    // Power on
    chThdSleepMilliseconds(20);

    // Core reset and delay of at least 3 PHY cycles
    OTG_FS->GRSTCTL = GRSTCTL_CSRST;
    while((OTG_FS->GRSTCTL & GRSTCTL_CSRST) != 0);
    chThdSleepMilliseconds(1);
    // Wait AHB idle condition
    while((OTG_FS->GRSTCTL & GRSTCTL_AHBIDL) == 0);
    chThdSleepMilliseconds(1);

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
    chSysLock();
    OTG_FS->PCGCCTL |= PCGCCTL_STPPCLK | PCGCCTL_GATEHCLK; // Stop PHY clock, gate HCLK
    rccDisableOTG_FS(FALSE);
    nvicDisableVector(STM32_OTG1_NUMBER);
    IsReady = false;
    chSysUnlock();
}

void Usb_t::IDeviceReset() {
//    Uart.Printf("Rst\r");
    IsReady = false;
    TxFifoFlush(); // TX FIFO flush
    // Force all EPs in NAK mode and clear irqs
    for(uint8_t i = 0; i < EP_CNT; i++) {
        OTG_FS->ie[i].DIEPCTL = DIEPCTL_SNAK;
        OTG_FS->oe[i].DOEPCTL = DOEPCTL_SNAK;
        OTG_FS->ie[i].DIEPINT = 0xFF;
        OTG_FS->oe[i].DOEPINT = 0xFF;
        Ep[i].State = esIdle;
        Ep[i].PktState = psNoPkt;
        Ep[i].ResumeWaitingThd(FAILURE);
    }
    // Disable and clear all EPs irqs
    OTG_FS->DAINT = 0xFFFFFFFF;
    OTG_FS->DAINTMSK = DAINTMSK_OEPM(0) | DAINTMSK_IEPM(0);

    IRamInit();     // Setup RAM
    ISetAddress(0); // Reset device address to zero

    // Enable EP irqs
    OTG_FS->GINTMSK |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
    OTG_FS->DIEPMSK  = DIEPMSK_TOCM    | DIEPMSK_XFRCM;
    OTG_FS->DOEPMSK  = DOEPMSK_STUPM   | DOEPMSK_XFRCM;

    // Init EP0
    Ep[0].State = esSetup;
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
    uint32_t TxAddr = USB_RX_SZ_WORDS;
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

void Usb_t::IEndpointsInit() {
    uint32_t ctl=0;
    for(uint8_t i=1; i<EP_CNT; i++) {
        switch(EpCfg[i].Type) {
            case EP_TYPE_ISOCHRONOUS: ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_ISO;  break;
            case EP_TYPE_BULK:        ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_BULK; break;
            case EP_TYPE_INTERRUPT:   ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_INTR; break;
            default: break;
        }
        // OUT endpoint setup
        if(EpCfg[i].OutMaxsize != 0) {  // really out endpoint
            OTG_FS->oe[i].DOEPCTL = ctl | DOEPCTL_SNAK | EpCfg[i].OutMaxsize; // Do not receive
            OTG_FS->DAINTMSK |= DAINTMSK_OEPM(i);       // Enable out IRQ
        }
        else {
            OTG_FS->oe[i].DOEPTSIZ = 0;
            OTG_FS->oe[i].DOEPCTL &= ~DOEPCTL_USBAEP;   // Disable endpoint
            OTG_FS->DAINTMSK &= ~DAINTMSK_OEPM(i);      // Disable out IRQ
        }

        // IN endpoint activation
        OTG_FS->ie[i].DIEPTSIZ = 0;
        if(EpCfg[i].InMaxsize != 0) {
            OTG_FS->ie[i].DIEPCTL = ctl | DIEPCTL_TXFNUM(i) | DIEPCTL_MPSIZ(EpCfg[i].InMaxsize);
            OTG_FS->DAINTMSK |= DAINTMSK_IEPM(i);
        }
        else {
            OTG_FS->ie[i].DIEPCTL &= ~DIEPCTL_USBAEP;
            OTG_FS->DAINTMSK &= ~DAINTMSK_IEPM(i);
        }
    } // for i
}
#endif

#if 1 // =========================== High level ================================
void Usb_t::SetupPktHandler() {
//    EP0_PRINT("Setup\r");
//    Uart.Printf("%A\r", Ep0OutBuf, 8, ' ');
    // Try to handle request
    uint8_t *FPtr;
    uint32_t FLength;
    Ep[0].State = DefaultReqHandler(&FPtr, &FLength);
    // If standard request handler failed, try handle it in application
    if(Ep[0].State == esError) Ep[0].State = NonStandardControlRequestHandler(&FPtr, &FLength);
    // Prepare to next transaction
    switch(Ep[0].State) {
        case esInData:
            Ep[0].PtrIn = FPtr;
            Ep[0].LengthIn = FLength;
            Ep[0].PrepareInTransaction();   // Just prepare; may not fill fifo here
            Ep[0].StartInTransaction();
            break;
        case esOutStatus:
            Ep[0].TransmitZeroPkt();
            break;
        default:
            Ep[0].SetStallIn();
            Ep[0].SetStallOut();
            break;
    } // switch
}

EpState_t Usb_t::DefaultReqHandler(uint8_t **PPtr, uint32_t *PLen) {
    uint8_t Recipient = SetupReq.bmRequestType & USB_REQTYPE_RECIPIENT_MASK;
    uint32_t Addr;
    uint8_t EpID;
    if(Recipient == USB_REQTYPE_RECIPIENT_DEVICE) {
        //Uart.Printf("Dev\r\n");
        switch(SetupReq.bRequest) {
            case USB_REQ_GET_STATUS:        // Just return the current status word
//                EP0_PRINT("GetStatus\r");
                *PPtr = (uint8_t*)cZero;    // Remote wakeup = 0, selfpowered = 0
                *PLen = 2;
                return esInData;
                break;
            case USB_REQ_SET_ADDRESS:
                Addr = IGetSetupAddr();
//                EP0_PRINT_V1("SetAddr %u\r", Addr);
                *PLen = 0;
                ISetAddress(Addr);
                return esOutStatus;
                break;
            case USB_REQ_GET_DESCRIPTOR:
//                EP0_PRINT_V1V2("GetDesc t=%u i=%u\r", SetupReq.Type, SetupReq.Indx);
                GetDescriptor(SetupReq.Type, SetupReq.Indx, PPtr, PLen);
                // Trim descriptor if needed, as host can request part of descriptor.
                TRIM_VALUE(*PLen, SetupReq.wLength);
                if(*PLen != 0) return esInData;
                break;
            case USB_REQ_GET_CONFIGURATION:
//                EP0_PRINT("GetCnf\r");
                *PPtr = &Configuration;
                *PLen = 1;
                return esInData;
                break;
            case USB_REQ_SET_CONFIGURATION:
//                EP0_PRINT_V1("SetCnf %u\r", SetupReq.wValue);
                Configuration = (uint8_t)(SetupReq.wValue & 0xFF);
                *PLen = 0;
//                Uart.Printf("*******UsbConfigured\r");
                IEndpointsInit();
                IsReady = true;
                if(PThread != nullptr) {
                    chSysLockFromIsr();
                    chEvtSignalI(PThread, EVTMSK_USB_READY);
                    chSysUnlockFromIsr();
                }
                return esOutStatus;
                break;
            default: break;
        } // switch
    }
//    else if(Recipient == USB_REQTYPE_RECIPIENT_INTERFACE) {
//        if(SetupReq.bRequest == USB_REQ_SET_INTERFACE) {
//            //EP0_PRINT("InterfaceSet\r");
//            return esOutStatus;
//        }
//    }
    else if(Recipient == USB_REQTYPE_RECIPIENT_ENDPOINT) {
//        EP0_PRINT("Ep\r");
        switch(SetupReq.bRequest) {
            case USB_REQ_SYNCH_FRAME:
                *PPtr = (uint8_t*)cZero;
                *PLen = 2;
                return esInData;
                break;
            case USB_REQ_SET_FEATURE:
                // Only ENDPOINT_HALT is handled as feature
                if(SetupReq.wValue != 0) return esError;
                // Handle only non-control eps
                EpID = SetupReq.wIndex & 0x0F;
                if(EpID != 0) {
                    if(SetupReq.wIndex & 0x80) Ep[EpID].SetStallIn();
                    else Ep[EpID].SetStallOut();
                    Ep[EpID].ResumeWaitingThd(OK);
                }
                return esOutStatus;
            case USB_REQ_CLEAR_FEATURE:
                // Only ENDPOINT_HALT is handled as feature
                if(SetupReq.wValue != 0) return esError;
                // Handle only non-control eps
                EpID = SetupReq.wIndex & 0x0F;
                if(EpID != 0) {
                    if(SetupReq.wIndex & 0x80) Ep[EpID].ClearStallIn();
                    else Ep[EpID].ClearStallOut();
                    Ep[EpID].ResumeWaitingThd(OK);
                }
                return esOutStatus;
                break;
            case USB_REQ_GET_STATUS:
                EpID = SetupReq.wIndex & 0x0F;
                if(SetupReq.wIndex & 0x80) *PPtr = Ep[EpID].IsStalledIn()?  (uint8_t*)cOne : (uint8_t*)cZero;
                else                       *PPtr = Ep[EpID].IsStalledOut()? (uint8_t*)cOne : (uint8_t*)cZero;
                *PLen = 2;
                return esInData;
                break;
        } // switch bRequest
    } // if Ep
    return esError;
}

__attribute__((weak))
EpState_t Usb_t::NonStandardControlRequestHandler(uint8_t **PPtr, uint32_t *PLen) {
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
//    Uart.Printf("Irq: %X\r", irqs);

    // Reset
    if(irqs & GINTSTS_USBRST) IDeviceReset();
    // Enumeration done
    if(irqs & GINTSTS_ENUMDNE) { (void)OTG_FS->DSTS; }
    // RX FIFO not empty
    if(irqs & GINTSTS_RXFLVL) {  IRxHandler(); }
    // OUT & IN event handling
    if(irqs & GINTSTS_IEPINT) {
        uint32_t src = OTG_FS->DAINT;
        if(src & (1 << 0)) IEpInHandler(0);
        if(src & (1 << 1)) IEpInHandler(1);
        if(src & (1 << 2)) IEpInHandler(2);
//        if(src & (1 << 3)) IEpInHandler(3);
    }
    if(irqs & GINTSTS_OEPINT) {
        uint32_t src = OTG_FS->DAINT;
        if(src & (1 << 16)) IEpOutHandler(0);
        if(src & (1 << 17)) IEpOutHandler(1);
        if(src & (1 << 18)) IEpOutHandler(2);
//        if(src & (1 << 19)) IEpOutHandler(3);
    }
}

void Usb_t::IEpOutHandler(uint8_t EpID) {
    uint32_t epint = OTG_FS->oe[EpID].DOEPINT;  // Read IRQs
    OTG_FS->oe[EpID].DOEPINT = 0xFF;      // Reset IRQs
//    Uart.Printf("=== out %u: %X\r\n", EpID, epint);
    // Setup pkt handler
    if((epint & DOEPINT_STUP) and (OTG_FS->DOEPMSK & DOEPMSK_STUPM)) {
        Ep[0].State = esSetup; // In case of setup pkt, reset stage to Setup
        SetupPktHandler();
        Ep[0].PktState = psNoPkt;
    }
    // Receive transfer complete
    if((epint & DOEPINT_XFRC) and (OTG_FS->DOEPMSK & DOEPMSK_XFRCM)) {
//        Uart.Printf("Out XFR cmp\r\n");
        if(EpID == 0) {
            if(Ep[0].PktState == psZeroPkt) {
                Ep[0].PktState = psNoPkt;
                Ep[0].State = esSetup;
                Ep[0].PrepareOutTransaction(1, 3*8);
                Ep[0].StartOutTransaction();
            }
        }
        else {// Restart reception if needed
//            Uart.Printf("OutXFRC #%u\r", EpID);
            // Queue
            if(Ep[EpID].POutQueue != nullptr) {
                if(chIQGetEmptyI(Ep[EpID].POutQueue) != 0) { // Restart reception if Queue is not full
                    Ep[EpID].PrepareOutTransaction(1, EpCfg[EpID].OutMaxsize);
                    Ep[EpID].StartOutTransaction();
                }
            }
            // Buffer
            else if(Ep[EpID].PtrOut != nullptr) {
                if(Ep[EpID].LengthOut != 0) {
                    Ep[EpID].PrepareOutTransaction(Ep[EpID].LengthOut);
                    Ep[EpID].StartOutTransaction();
                }
                else {  // Transmission completed
                    Ep[EpID].PtrOut = nullptr;
                    Ep[EpID].SetNakOut();
                    Ep[EpID].ResumeWaitingThd(OK);
                }
            } // if buffer
        }
    } // if XFRC
}

void Usb_t::IEpInHandler(uint8_t EpID) {
    uint32_t epint = OTG_FS->ie[EpID].DIEPINT;
    OTG_FS->ie[EpID].DIEPINT = 0xFF;   // Reset IRQs
//    Uart.Printf("=== in %X i%X\r\n", EpID, epint);
    Ep_t *ep = &Ep[EpID];
    // Transmit transfer complete
    if((epint & DIEPINT_XFRC) and (OTG_FS->DIEPMSK & DIEPMSK_XFRCM)) {
//        Uart.Printf("In XFRC\r\n");
        if(EpID == 0) {
            switch(ep->State) {
                case esInData:
                    if((ep->LengthIn != 0) or ep->TransmitFinalZeroPkt) {
                        ep->BufToFifo(); // Transfer not completed
                        ep->StartInTransaction();
                    }
                    else {  // Buf len == 0
                        ep->State = esInStatus; // Receive zero pkt
                        ep->PrepareOutTransaction(1, 3*8);
                        ep->StartOutTransaction();
                    }
                    break;

                case esOutStatus:
                    ep->State = esSetup;    // No need to explicitly start reception
                    break;

                default:
                    EP0_PRINT("e\r");
                    ep->State = esError;
                    ep->SetStallIn();
                    ep->SetStallOut();
                    break;
            } // switch
        } // if(EpID == 0)
        else {
            ep->ResumeWaitingThd(OK);
        }
    }
    // TX FIFO empty
    if((epint & DIEPINT_TXFE) and Ep[EpID].InFifoEmptyIRQEnabled()) {
//        Uart.Printf("In TXFE\r");
        Ep[EpID].BufToFifo();
    } // if txfe
}

// Only receives data, does not process it
void Usb_t::IRxHandler() {
    uint32_t sts, Len, EpID;
    sts = OTG_FS->GRXSTSP;
//    Uart.Printf("Rx sts=%X\r", sts);
//    Uart.Printf("Rx%u Pkt sts: %X; Len: %u\r", sts&0xF, (sts>>17)&0xF, (sts>>4)&0x7FF);
    switch (sts & GRXSTSP_PKTSTS_MASK) {
        case GRXSTSP_SETUP_DATA:
            Ep[0].FifoToBuf(Ep0OutBuf, 8);
            Ep[0].PktState = psDataPkt;
            break;
        case GRXSTSP_OUT_DATA:
            Len = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
            EpID = sts & GRXSTSP_EPNUM_MASK;
            if(Len == 0) {
                Ep[EpID].PktState = psZeroPkt;
            }
            else {
                Ep[EpID].PktState = psDataPkt;
//                Uart.Printf("DataRcvd for %u\r", EpID);
                // Read to queue
                if(Ep[EpID].POutQueue != nullptr) {
                    Ep[EpID].FifoToQueue(Len);
                    return;
                }
                // Read to buffer
                else if((Ep[EpID].PtrOut != nullptr) and (Ep[EpID].LengthOut != 0)) {
                    Len = MIN(Len, Ep[EpID].LengthOut);
                    Ep[EpID].FifoToBuf(Ep[EpID].PtrOut, Len);
                    Ep[EpID].LengthOut -= Len;
                    Ep[EpID].PtrOut += Len;
                    return;
                }
                Uart.Printf("\rUSB flush");
                RxFifoFlush();
            }
            break;
        case GRXSTSP_SETUP_COMP:    // Setup transaction completed
        case GRXSTSP_OUT_GLOBAL_NAK:
        case GRXSTSP_OUT_COMP:
        default: break;
    } // switch
}

#endif

#if 1 // ============================ Endpoints ================================
void Ep_t::ResumeWaitingThd(uint8_t ReadyMsg) {
    Buzy = false;
    if(PThread != nullptr) {
        chSysLockFromIsr();
        if(PThread->p_state == THD_STATE_SUSPENDED) {
            PThread->p_u.rdymsg = ReadyMsg;
            chSchReadyI(PThread);
        }
        PThread = nullptr;
        chSysUnlockFromIsr();
    }
}

uint8_t Ep_t::WaitUntilReady() {
    if(!Buzy) return OK;
    chSysLock();
    uint8_t rslt = OK;
    PThread = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    PThread = nullptr;
    if(chThdSelf()->p_u.rdymsg != RDY_OK) rslt = FAILURE;
    chSysUnlock();
    return rslt;
}

// ==== Buffers ====
void Ep_t::FifoToBuf(uint8_t *PDstBuf, uint32_t Len) {
    // Get pointer to Fifo
    volatile uint32_t *PFifo = OTG_FS->FIFO[Indx];
    Len = (Len + 3) / 4;    // Convert bytes count to words count
    while(Len--) {
        uint32_t w = *PFifo;
        *((uint32_t*)PDstBuf) = w;
        PDstBuf += 4;
    }
}

void Ep_t::FifoToQueue(uint32_t Len) {
//    Uart.Printf("F2Q %u\r", Len);
    // Get pointer to Fifo
    volatile uint32_t *PFifo = OTG_FS->FIFO[0]; // It is really 0
    chSysLockFromIsr();
    while(Len != 0) {
        uint32_t w = *PFifo;    // Read up to 4 bytes from FIFO
        // Put to queue byte by byte
        for(uint8_t i=0; (i<4) and (Len>0); i++) {
            chIQPutI(POutQueue, (uint8_t)(w & 0xFF));
            Len--;
            w >>= 8;
        }
    }
    chSysUnlockFromIsr();
}

// Fill USB memory with BufIn's data
void Ep_t::BufToFifo() {
//    Uart.Printf("In%u: %A\r", Indx, PtrIn, LengthIn, ' ');
    // Prepare variables
    volatile uint32_t *pDst = OTG_FS->FIFO[Indx];
    uint16_t WrittenBytesCnt = 0;
    // Fill from buffer
    while((LengthIn != 0) and (WrittenBytesCnt < EpCfg[Indx].InMaxsize)) {
        // Add quarta of bytes
        *pDst = *((uint32_t*)PtrIn);
        PtrIn += 4;
        WrittenBytesCnt += 4;
        // Check if data buffer is not empty
        if(LengthIn >= 4) LengthIn -= 4;
        else LengthIn = 0;
    }
    TransmitFinalZeroPkt = (WrittenBytesCnt == EpCfg[Indx].InMaxsize) and (LengthIn == 0);
    // Disable TxEmpty IRQ if everything is sent
    if(LengthIn == 0) DisableInFifoEmptyIRQ();
}

// ==== Transactions ====
void Ep_t::TransmitZeroPkt() {
//    Uart.Printf("Tx0\r");
    OTG_FS->ie[Indx].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
    OTG_FS->ie[Indx].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;   // Enable Ep and clear NAK
}

void Ep_t::StartTransmitBuf(uint8_t *Ptr, uint32_t ALen) {
 //    Uart.Printf("TxBuf Ep%u; %A\r", Indx, Ptr, ALen, ' ');
    chSysLock();
    PtrIn = Ptr;
    Buzy = true;
    LengthIn = ALen;
    PrepareInTransaction();   // Just prepare; may not fill fifo here
    StartInTransaction();
    chSysUnlock();
}

void Ep_t::StartReceiveToBuf(uint8_t *PDst, uint32_t Len) {
    chSysLock();
    PtrOut = PDst;
    LengthOut = Len;
    Buzy = true;
    PrepareOutTransaction(Len);
    StartOutTransaction();
//    Uart.Printf("#%u; %X; %X\r", Indx, OTG_FS->oe[Indx].DOEPCTL, OTG_FS->oe[Indx].DOEPTSIZ);
    chSysUnlock();
}

#endif
