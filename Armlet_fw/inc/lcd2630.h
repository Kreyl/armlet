/*
 * File:   lcd6101.h
 * Author: Kreyl Laurelindo
 *
 * Created on 10.11.2012
 */

#ifndef LCD6101_H
#define LCD6101_H

#include "stm32f2xx.h"
#include "kl_lib_f2xx.h"
#include <string.h>
#include "ff.h"

// ================================ Defines ====================================
#define LCD_GPIO        GPIOE
#define LCD_DC          8
#define LCD_WR          9
#define LCD_RD          10
#define LCD_XCS         11
#define LCD_XRES        12
#define LCD_MASK_WR     (0x00FF | (1<<LCD_WR))  // clear bus and set WR low
#define LCD_MODE_READ   0xFFFF0000
#define LCD_MODE_WRITE  0x00005555

#define LCD_BCKLT_GPIO  GPIOB
#define LCD_BCKLT_PIN   9
#define LCD_BCKLT_TMR   TIM11
#define LCD_BCKLT_CHNL  1

//#define LCD_12BIT
#ifndef LCD_12BIT
#define LCD_16BIT
#endif

#ifdef LCD_12BIT
// Color palette: 4R-4G-4B, 3 bytes per two pixels
enum Color_t {
    clBlack     = 0x000,
    clRed       = 0xF00,
    clGreen     = 0x0F0,
    clBlue      = 0x00F,
    clYellow    = 0xFF0,
    clMagenta   = 0xF0F,
    clCyan      = 0x0FF,
    clWhite     = 0xFFF,
};
#else
// Color palette: 5R-6G-5B, 2 bytes per pixel
enum Color_t {
    clBlack     = 0x0000,
    clRed       = 0xF800,
    clGreen     = 0x07E0,
    clBlue      = 0x001F,
    clYellow    = 0xFFE0,
    clMagenta   = 0xF81F,
    clCyan      = 0x07FF,
    clWhite     = 0xFFFF,
};
#endif

#define LCD_X_0             1   // }
#define LCD_Y_0             2   // } Zero pixels are shifted
#define LCD_H               128 // }
#define LCD_W               160 // } Pixels count
#define LCD_TOP_BRIGHTNESS  100 // i.e. 100%

#define LCD_FILE_BUF_SZ     8192    // Must be 512 or larger

class Lcd_t {
private:
    uint16_t IX, IY;
    Color_t IForeClr, IBckClr;
    PwmPin_t BckLt;
    void WriteCmd(uint8_t ACmd);
    void WriteCmd(uint8_t ACmd, uint8_t AData);
    FIL IFile;
    char IFileBuf[LCD_FILE_BUF_SZ];
public:
    // General use
    uint16_t Brightness;
    void Init();
    void Shutdown();
    void SetBrightness(uint16_t ABrightness)  { BckLt.Set(Brightness = ABrightness); }

    // High-level
    void PutChar(char c);
    void Printf(uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...);
    void Cls(Color_t Color);
    void GetBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);
    void PutBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);
//    void DrawImage(const uint8_t x, const uint8_t y, const uint8_t *Img);
//    void DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode);
    void DrawBmpFile(uint8_t x0, uint8_t y0, const char *Filename);
};

extern Lcd_t Lcd;

#endif

