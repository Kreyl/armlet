/*
 * usb_f2.cpp
 *
 *  Created on: 22 сент. 2013 г.
 *      Author: kreyl
 */

#include "usb_f2.h"

#define TRDT        3   // AHB clock = 24, => TRDT = 24/48 +1

Usb_t Usb;

void Usb_t::Init() {
    // GPIO
    PinSetupAlterFunc(GPIOA, 11, omOpenDrain, pudNone, AF10);
    PinSetupAlterFunc(GPIOA, 12, omOpenDrain, pudNone, AF10);
    PinSetupIn(GPIOA, 9, pudPullDown);
    // OTG FS clock enable and reset
    rccEnableOTG_FS(FALSE);
    rccResetOTG_FS();
    // Enable IRQ
    nvicEnableVector(STM32_OTG1_NUMBER, CORTEX_PRIORITY_MASK(IRQ_PRIO_LOW));

    IReset();
}

void Usb_t::Shutdown() {
    OTG_FS->PCGCCTL |= PCGCCTL_STPPCLK | PCGCCTL_GATEHCLK; // Stop PHY clock, gate HCLK
}

void Usb_t::IReset() {
    State = usConnected;
    // ==== OTG init ====
    // Forced device mode, USB turn-around time = TRDT_VALUE, Full Speed 1.1 PHY, 0 tuning
    OTG_FS->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT) | GUSBCFG_PHYSEL | 0;
    OTG_FS->DCFG = 0x02200000 | DCFG_NZLSOHSK | DCFG_DSPD_FS11; // Full-speed (other options are not available, though)
    OTG_FS->PCGCCTL = 0;    // Nothing is stopped or gated
    OTG_FS->GCCFG = GCCFG_VBUSASEN | GCCFG_VBUSBSEN | GCCFG_PWRDWN /*| GCCFG_NOVBUSSENS*/;

    // Core reset and delay of at least 3 PHY cycles
    OTG_FS->GRSTCTL = GRSTCTL_CSRST;
    while((OTG_FS->GRSTCTL & GRSTCTL_CSRST) != 0);
    __NOP();
    __NOP();
    // Wait AHB idle condition
    while((OTG_FS->GRSTCTL & GRSTCTL_AHBIDL) == 0);
    __NOP();
    __NOP();

    OTG_FS->GAHBCFG = 0;    // Interrupts on TXFIFOs half empty, mask interrupts
    // Clear all pending Device Interrupts, only the USB Reset interrupt is required initially
    OTG_FS->DIEPMSK  = 0;
    OTG_FS->DOEPMSK  = 0;
    OTG_FS->DAINTMSK = 0;
    OTG_FS->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM /*| GINTMSK_USBSUSPM | GINTMSK_ESUSPM  |*/;
    OTG_FS->GINTSTS  = 0xFFFFFFFF;      // Clear all pending IRQs, if any
    OTG_FS->GAHBCFG |= GAHBCFG_GINTMSK; // Global interrupts enable

    IRamInit();
    // Reset device address to zero
    OTG_FS->DCFG = (OTG_FS->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(0);

    for(uint8_t i=0; i<EP_CNT; i++) Ep[i].Init(&EpCfg[i]);
    Ep[0].State = esSetup;
}

void Usb_t::IRamInit() {
    // ==== RX FIFO ====
    uint32_t tmp = OTG_FS->GRXFSIZ;
    tmp &= 0xFFFF0000;  // Clear lower bits, save upper ones
    tmp |= USB_RX_SZ_WORDS;
    OTG_FS->GRXFSIZ = tmp;
    RxFifoFlush();
    // ==== TX FIFO ====
    uint32_t TxAddr = USB_RX_SZ_WORDS;
    uint32_t RamSz;
    // Ep0 TX
    RamSz = ((EP0_SZ / 4) < 16)? 16 : (EP0_SZ / 4); // Min size is 16 words
    OTG_FS->DIEPTXF0 = (uint32_t)((RamSz << 16) | TxAddr);
    TxAddr += RamSz;
    // Other endpoints
    for(uint8_t i=1; i<EP_CNT; i++) {
        if(EpCfg[i].InMaxsize != 0) {
            RamSz = ((EpCfg[i].InMaxsize / 4) < 16)? 16 : (EpCfg[i].InMaxsize / 4); // Min size is 16 words
            OTG_FS->DIEPTXF[i-1] = (uint32_t)((RamSz << 16) | TxAddr);
            TxAddr += RamSz;
        } // if !0
    } // for
}

void Usb_t::RxFifoFlush() {
    OTG_FS->GRSTCTL = GRSTCTL_RXFFLSH;
    while((OTG_FS->GRSTCTL & GRSTCTL_RXFFLSH) != 0);
    // Wait for 3 PHY Clocks
    __NOP();
    __NOP();
}

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
#endif


