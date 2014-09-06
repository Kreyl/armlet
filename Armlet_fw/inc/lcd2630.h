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
#include "cmd_uart.h"
#include <string.h>
#include "ff.h"
#include "Fonts.h"
#include "color.h"

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

// Color palette: 5R-6G-5B, 2 bytes per pixel
#define COLOR_RGB_TO_565(R,G,B)     ((uint16_t)((((uint32_t)(R) >> 3) << 11) | (((uint32_t)(G) >> 2) << 5) | ((uint32_t)(B) >> 3)))

// Atlantis' colors
#define clAtlFront  ((Color_t){0x3C, 0x46, 0x3C})
#define clAtlBack   ((Color_t){0xFF, 0xEE, 0xAF})
#define clAtltopstr ((Color_t){0x21, 0x26, 0x21})

// LCD params
#define LCD_X_0             1   // }
#define LCD_Y_0             2   // } Zero pixels are shifted
#define LCD_H               128 // }
#define LCD_W               160 // } Pixels count
#define LCD_TOP_BRIGHTNESS  100 // i.e. 100%

#define LCD_FILE_BUF_SZ     8192    // Must be 512 or larger

struct FontParams_t {
    const tChar *PFirstChar;
    Color_t FClr, BClr;
    uint16_t X, Y;
};

// Battery capacity rectangle
#define BAT_RECT_LEFT       147
#define BAT_RECT_TOP        4
#define BAT_RECT_WIDTH      9
#define BAT_RECT_HEIGHT     4
#define BAT_RECT_FORE_CLR   COLOR_RGB_TO_565(0x61, 0x3E, 0x10)
#define BAT_RECT_BACK_CLR   COLOR_RGB_TO_565(0xFF, 0xEE, 0xAF)
#define BAT_RECT_CHARGE_CLR COLOR_RGB_TO_565(0x00, 0xFF, 0x00)

class Lcd_t {
private:
    FontParams_t Fnt;
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
    void PutFontChar(char c);
    void Printf(uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...);
    void Printf(const tFont &Font, uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...);
    void Cls(Color_t Color);
    void GetBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);
    void PutBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf);
//    void DrawImage(const uint8_t x, const uint8_t y, const uint8_t *Img);
//    void DrawSymbol(const uint8_t x, const uint8_t y, const uint8_t ACode);
    void DrawBmpFile(uint8_t x0, uint8_t y0, const char *Filename);
    void DrawBatteryState();
};

extern Lcd_t Lcd;

#endif

