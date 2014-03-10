/*
 * cc1101.h
 *
 *  Created on: Feb 12, 2013
 *      Author: g.kruglov
 */

#ifndef CC1101_H_
#define CC1101_H_

/*
 * Low-level operations are here.
 * Pkt level at top side, and SPI at bottom.
 */

#include <inttypes.h>
#include "kl_lib_f2xx.h"
#include "cc1101defins.h"
#include "cc1101_rf_settings.h"
#include "cc_gpio.h"

enum CCState_t {ccIdle, ccSleeping, ccReceiving, ccTransmitting};

class cc1101_t {
private:
    uint8_t IState; // Inner CC state, returned as first byte
    uint8_t IPktSz;
    Thread *PWaitingThread;
    Spi_t ISpi;
    // Pins
    IrqPin_t IGdo0;
    void CsHi() { PinSet(CC_GPIO, CC_CS); }
    void CsLo() { PinClear(CC_GPIO, CC_CS); }
    bool GDO0IsHi() { return PinIsSet(CC_GPIO, CC_GDO0); }
    bool GDO2IsHi() { return PinIsSet(CC_GPIO, CC_GDO2); }
    void BusyWait() { while(PinIsSet(CC_GPIO, CC_MISO)); }
    // General
    void IHandleAsync();
    void RfConfig();
    int8_t RSSI_dBm(uint8_t ARawRSSI);
    // Registers and buffers
    void WriteRegister (const uint8_t Addr, const uint8_t AData);
    uint8_t ReadRegister (const uint8_t Addr);
    void WriteStrobe(uint8_t AStrobe);
    void WriteTX(uint8_t* Ptr, uint8_t Length);
    // Strobes
    void CReset()      { WriteStrobe(CC_SRES); }
    void EnterTX()     { WriteStrobe(CC_STX);  }
    void FlushTxFIFO() { WriteStrobe(CC_SFTX); }
    void GetState()    { WriteStrobe(CC_SNOP); }
    void EnterRX()     { WriteStrobe(CC_SRX);  }
    void FlushRxFIFO() { WriteStrobe(CC_SFRX); }
public:
    CCState_t State;
    void Init();
    void SetChannel(uint8_t AChannel);
    void SetTxPower(uint8_t APwr) { WriteRegister(CC_PATABLE, APwr); }
    void SetPktSize(uint8_t ASize) { IPktSz = ASize; WriteRegister(CC_PKTLEN, ASize); }
    // State change
    void TransmitSync(void *Ptr);
    uint8_t ReceiveSync(uint32_t Timeout_ms, void *Ptr, int8_t *PRssi);
    void TransmitAsync(void *Ptr);
    void ReceiveAsync();
    void EnterIdle()  { WriteStrobe(CC_SIDLE); State = ccIdle; }
    void Sleep() { WriteStrobe(CC_SPWD); State = ccSleeping; }
    void Recalibrate() {
        while(IState != CC_STB_IDLE) EnterIdle();
        WriteStrobe(CC_SCAL);
        BusyWait();
    }
    uint8_t ReadFIFO(void *Ptr, int8_t *PRssi);
    // Inner use
    void IGdo0IrqHandler();
};

extern cc1101_t CC;

#endif /* CC1101_H_ */
