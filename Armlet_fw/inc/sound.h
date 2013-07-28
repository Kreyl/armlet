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
#define VS_DREQ         5
#define VS_XCLK         13
#define VS_SO           14
#define VS_SI           15
// SPI
#define VS_SPI          SPI2
#define VS_AF           AF5
#define VS_SPI_RCC_EN() rccEnableSPI2(FALSE)

// Constants
#define VS_TIMEOUT              8000000
#define VS_TRAILING_0_COUNT     11

// Command codes
#define VS_READ_OPCODE  0b00000011
#define VS_WRITE_OPCODE 0b00000010

// ==== Error codes ====
#define VS_OK           0
#define VS_BUSY_ER      1
#define VS_TIMEOUT_ER   2

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

enum sndState_t {sndStopped, sndPlaying, sndMustStop};

class Sound_t {
private:
    uint8_t CmdRead(uint8_t AAddr, uint16_t *AData);
    uint8_t CmdWrite(uint8_t AAddr, uint16_t AData);
public:
    sndState_t State;
    void Init();
    void Play(const char* AFilename);
    void Stop() { State = sndMustStop; }
    void SetVolume(uint8_t Attenuation) { CmdWrite(VS_REG_VOL, ((Attenuation*256)+Attenuation)); }
};

extern Sound_t Sound;

#endif	/* MEDIA_H */

