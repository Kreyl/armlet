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

enum sndState_t {sndStopped, sndPlaying, sndWritingZeroes, sndMustStop};

union VsCmd_t {
    struct {
        uint8_t OpCode;
        uint8_t Address;
        uint16_t Data;
    } PACKED;
    msg_t Msg;
};

#define VS_CMD_BUF_SZ   4
#define VS_DATA_BUF_SZ  512    // bytes. Must be multiply of 512.

struct VsBuf_t {
    uint8_t Data[VS_DATA_BUF_SZ], *PData;
    UINT DataSz;
    void ReadFromFile(FIL *PFile) {
        f_read(PFile, Data, VS_DATA_BUF_SZ, &DataSz);
        PData = Data;   // Set pointer at beginning
    }
};

class Sound_t {
private:
    msg_t CmdBuf[VS_CMD_BUF_SZ];
    Mailbox CmdBox;
    VsCmd_t ICmd;
    VsBuf_t Buf1, Buf2, *PBuf;
    uint8_t CmdRead(uint8_t AAddr, uint16_t *AData);
    uint8_t CmdWrite(uint8_t AAddr, uint16_t AData);
    void AddCmd(uint8_t AAddr, uint16_t AData);
    void StopNow();
    void PrepareToReadNextChunk();
    FIL IFile;
    // Flags
    bool IDmaIdle;
public:
    sndState_t State;
    void Init();
    void Play(const char* AFilename);
    void Stop() { State = sndMustStop; }
    // Inner use
    IrqPin_t IDreq;
    void IrqDreqHandler();
    void IrqDmaTcHandler();
    void ISendDataTask();
    void ISendNextData();
    //void SetVolume(uint8_t Attenuation) { AddCmd(VS_REG_VOL, ((Attenuation*256)+Attenuation)); }
};

extern Sound_t Sound;

#endif	/* MEDIA_H */

