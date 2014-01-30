/*
 * File:   media.h
 * Author: g.kruglov
 *
 * Created on June 15, 2011, 4:44 PM
 */

#ifndef MEDIA_H
#define	MEDIA_H

#include "kl_sd.h"
#include <stdint.h>
#include "kl_lib_f2xx.h"

// ==== Defines ====
#define VS_GPIO         GPIOB
// Pins
#define VS_XCS          3
#define VS_XDCS         8
#define VS_RST          4
#define VS_DREQ         0
#define VS_XCLK         13
#define VS_SO           14
#define VS_SI           15
// SPI
#define VS_SPI          SPI2
#define VS_AF           AF5
#define VS_SPI_RCC_EN() rccEnableSPI2(FALSE)
// DMA
#define VS_DMA          STM32_DMA1_STREAM4
#define VS_DMA_CHNL     0
#define VS_DMA_MODE     STM32_DMA_CR_CHSEL(VS_DMA_CHNL) | \
                        DMA_PRIORITY_LOW | \
                        STM32_DMA_CR_MSIZE_BYTE | \
                        STM32_DMA_CR_PSIZE_BYTE | \
                        STM32_DMA_CR_DIR_M2P |    /* Direction is memory to peripheral */ \
                        STM32_DMA_CR_TCIE         /* Enable Transmission Complete IRQ */


// Command codes
#define VS_READ_OPCODE  0b00000011
#define VS_WRITE_OPCODE 0b00000010

// Registers
#define VS_REG_MODE         0x00
#define VS_REG_STATUS       0x01
#define VS_REG_BASS         0x02
#define VS_REG_CLOCKF       0x03
#define VS_REG_DECODE_TIME  0x04
#define VS_REG_AUDATA       0x05
#define VS_REG_WRAM         0x06
#define VS_REG_WRAMADDR     0x07
#define VS_REG_HDAT0        0x08
#define VS_REG_HDAT1        0x09
#define VS_REG_AIADDR       0x0A
#define VS_REG_VOL          0x0B

enum sndState_t {sndStopped, sndPlaying, sndWritingZeroes};

union VsCmd_t {
    struct {
        uint8_t OpCode;
        uint8_t Address;
        uint16_t Data;
    } PACKED;
    msg_t Msg;
};

#define VS_VOLUME_STEP          4
#define VS_INITIAL_ATTENUATION  0x33
#define VS_CMD_BUF_SZ           9
#define VS_DATA_BUF_SZ          512    // bytes. Must be multiply of 512.

struct VsBuf_t {
    uint8_t Data[VS_DATA_BUF_SZ], *PData;
    UINT DataSz;
    FRESULT ReadFromFile(FIL *PFile) {
        PData = Data;   // Set pointer at beginning
        return f_read(PFile, Data, VS_DATA_BUF_SZ, &DataSz);
    }
};

// Event mask to wake from IRQ
#define VS_EVT_READ_NEXT    (eventmask_t)1
#define VS_EVT_STOP         (eventmask_t)2
#define VS_EVT_COMPLETED    (eventmask_t)4

class Sound_t {
private:
    msg_t CmdBuf[VS_CMD_BUF_SZ];
    Mailbox CmdBox;
    VsCmd_t ICmd;
    VsBuf_t Buf1, Buf2, *PBuf;
    uint32_t ZeroesCount;
    Thread *PThread;
    FIL IFile;
    const char* IFilename;
    bool IDmaIdle;
    int16_t IAttenuation;
    // Pin operations
    inline void Rst_Lo()   { PinClear(VS_GPIO, VS_RST); }
    inline void Rst_Hi()   { PinSet(VS_GPIO, VS_RST); }
    inline void XCS_Lo()   { PinClear(VS_GPIO, VS_XCS); }
    inline void XCS_Hi()   { PinSet(VS_GPIO, VS_XCS); }
    inline void XDCS_Lo()  { PinClear(VS_GPIO, VS_XDCS); }
    inline void XDCS_Hi()  { PinSet(VS_GPIO, VS_XDCS); }
    // Cmds
    uint8_t CmdRead(uint8_t AAddr, uint16_t *AData);
    uint8_t CmdWrite(uint8_t AAddr, uint16_t AData);
    void AddCmd(uint8_t AAddr, uint16_t AData);
    inline void StartTransmissionIfNotBusy() {
        if(IDmaIdle and IDreq.IsHi()) {
//            Uart.Printf("T");
            IDreq.EnableIrq(IRQ_PRIO_MEDIUM);
            IDreq.GenerateIrq();    // Do not call SendNexData directly because of its interrupt context
        }
    }
    void PrepareToStop();
    void SendZeroes();
    void IPlayNew();
public:
    sndState_t State;
    void Init();
    void Play(const char* AFilename) {
        IFilename = AFilename;
        chSysLock();
        if(State == sndStopped) chEvtSignalI(PThread, VS_EVT_COMPLETED);
        else chEvtSignalI(PThread, VS_EVT_STOP);
        chSysUnlock();
    }
    void Stop() {
        IFilename = NULL;
        chEvtSignal(PThread, VS_EVT_STOP);
    }
    // 0...254
    void SetVolume(uint8_t AVolume) {
        if(AVolume == 0xFF) AVolume = 0xFE;
        IAttenuation = 0xFE - AVolume; // Transform Volume to attenuation
        AddCmd(VS_REG_VOL, ((IAttenuation * 256) + IAttenuation));
    }
    void VolumeIncrease() {
        IAttenuation -= VS_VOLUME_STEP;
        if(IAttenuation < 0) IAttenuation = 0;
        AddCmd(VS_REG_VOL, ((IAttenuation * 256) + IAttenuation));
    }
    void VolumeDecrease() {
        IAttenuation += VS_VOLUME_STEP;
        if(IAttenuation > 0x8F) IAttenuation = 0x8F;
        AddCmd(VS_REG_VOL, ((IAttenuation * 256) + IAttenuation));
    }
    // Inner use
    IrqPin_t IDreq;
    void IrqDreqHandler();
    void ITask();
    void ISendNextData();
    void IrqDmaHandler();
};

extern Sound_t Sound;

#endif	/* MEDIA_H */

