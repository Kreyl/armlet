#include "lcd2630.h"
#include "kl_lib_f2xx.h"
#include <string.h>
#include <stdarg.h>
#include "core_cmInstr.h"

//#include "lcdFont8x8.h"
#include "lcd_font.h"
#include <string.h>

#include "power.h"  // for battery state

// Variables
Lcd_t Lcd;

// Pin driving functions
#define LCD_DELAY()   //DelayLoop(7)
static inline void XRES_Hi() { PinSet  (LCD_GPIO, LCD_XRES); LCD_DELAY(); }
static inline void XRES_Lo() { PinClear(LCD_GPIO, LCD_XRES); LCD_DELAY(); }
static inline void XCS_Hi () { PinSet  (LCD_GPIO, LCD_XCS);  LCD_DELAY(); }
static inline void XCS_Lo () { PinClear(LCD_GPIO, LCD_XCS);  LCD_DELAY(); }
__attribute__ ((always_inline)) static inline void DC_Hi()  { PinSet  (LCD_GPIO, LCD_DC);   LCD_DELAY(); }
__attribute__ ((always_inline)) static inline void DC_Lo()  { PinClear(LCD_GPIO, LCD_DC);   LCD_DELAY(); }
static inline void WR_Hi()   { PinSet  (LCD_GPIO, LCD_WR);   LCD_DELAY(); }
static inline void RD_Hi()   { PinSet  (LCD_GPIO, LCD_RD);   LCD_DELAY(); }
//__attribute__ ((always_inline)) static inline void RD_Lo()  { PinClear(LCD_GPIO, LCD_RD);   LCD_DELAY}

void Lcd_t::Init() {
    // Init pins if not setup
    if(Brightness == 0) {
        BckLt.Init(LCD_BCKLT_GPIO, LCD_BCKLT_PIN, LCD_BCKLT_TMR, LCD_BCKLT_CHNL, LCD_TOP_BRIGHTNESS);
        PinSetupOut(LCD_GPIO, LCD_DC,   omPushPull, pudNone, ps100MHz);
        PinSetupOut(LCD_GPIO, LCD_WR,   omPushPull, pudNone, ps100MHz);
        PinSetupOut(LCD_GPIO, LCD_RD,   omPushPull, pudNone, ps100MHz);
        PinSetupOut(LCD_GPIO, LCD_XRES, omPushPull, pudNone, ps100MHz);
        PinSetupOut(LCD_GPIO, LCD_XCS,  omPushPull, pudNone, ps100MHz);
        // Configure data bus as outputs
        for(uint8_t i=0; i<8; i++) PinSetupOut(LCD_GPIO, i, omPushPull, pudNone, ps100MHz);
        Brightness = LCD_TOP_BRIGHTNESS;
    }
    // ======= Init LCD =======
    XCS_Hi();
    XRES_Lo();  // }
    XRES_Hi();  // } Reset display
    chThdSleepMilliseconds(4);
    DC_Lo();    // Command mode by default
    WR_Hi();    // Default hi
    RD_Hi();    // Default hi
    XCS_Lo();   // Interface is always enabled

    WriteCmd(0x11);         // Sleep out
    WriteCmd(0x13);         // Normal Display Mode ON
#ifdef LCD_12BIT
    WriteCmd(0x3A, 0x03);   // Pixel format: VIPF=0(undef), IFPF=12 bit per pixel
#else
    WriteCmd(0x3A, 0x05);   // Pixel format: VIPF=0(undef), IFPF=16 bit per pixel
#endif
    WriteCmd(0x29);         // Display on
    WriteCmd(0x20);         // Inv off
    WriteCmd(0x13);         // Normal Display Mode ON
    WriteCmd(0x36, 0xA0);   // Display mode: Y inv, X none-inv, Row/Col exchanged

    Cls(clBlack);
    SetBrightness(Brightness);
}

void Lcd_t::Shutdown(void) {
    XRES_Lo();
    XCS_Lo();
    BckLt.Off();
}

#if 1 // ============================ Local use ================================
__attribute__ ((always_inline)) static inline void ModeWrite() {
    LCD_GPIO->MODER |= LCD_MODE_WRITE;
}
__attribute__ ((always_inline)) static inline void ModeRead() {
    LCD_GPIO->MODER &= LCD_MODE_READ;
}

__attribute__ ((always_inline)) static inline void WriteByte(uint8_t Byte) {
    LCD_GPIO->BSRRH = LCD_MASK_WR;  // Clear bus and set WR Low
    LCD_GPIO->BSRRL = Byte;         // Place data on bus
    LCD_GPIO->BSRRL = (1<<LCD_WR);  // WR high
}
__attribute__ ((always_inline)) static inline uint8_t ReadByte() {
    uint16_t w;
    LCD_GPIO->BSRRH = (1<<LCD_RD);  // RD Low
    LCD_GPIO->BSRRL = (1<<LCD_RD);  // RD high
    w = LCD_GPIO->IDR;              // Read data from bus
    return (uint8_t)w;
}

// ==== WriteCmd ====
void Lcd_t::WriteCmd(uint8_t ACmd) {
    // DC is lo by default => Cmd by default
    WriteByte(ACmd);    // Send Cmd byte
}
void Lcd_t::WriteCmd(uint8_t ACmd, uint8_t AData) {
    // DC is lo by default => Cmd by default
    WriteByte(ACmd);    // Send Cmd byte
    // Send data
    DC_Hi();
    WriteByte(AData);
    DC_Lo();
}
#endif

// ================================= Printf ====================================
__attribute__ ((always_inline)) static inline void SetBounds(uint8_t Left, uint8_t Width, uint8_t Top, uint8_t Height) {
    // Set column bounds
    WriteByte(0x2A);
    DC_Hi();
    WriteByte(0x00);                    // }
    WriteByte(LCD_X_0+Left);            // } Col addr start
    WriteByte(0x00);                    // }
    WriteByte(LCD_X_0+Left+Width-1);    // } Col addr end
    DC_Lo();
    // Set row bounds
    WriteByte(0x2B);
    DC_Hi();
    WriteByte(0x00);                    // }
    WriteByte(LCD_Y_0+Top);             // } Row addr start = 0
    WriteByte(0x00);                    // }
    WriteByte(LCD_Y_0+Top+Height-1);    // } Row addr end
    DC_Lo();
}

void Lcd_t::PutChar(char c) {
    char *PFont = (char*)Font_6x8_Data;  // Font to use
    // Read font params
    uint8_t nCols = PFont[0];
    uint8_t nRows = PFont[1];
    uint16_t nBytes = PFont[2];
    SetBounds(Fnt.X, nCols, Fnt.Y, nRows);
    // Get pointer to the first byte of the desired character
    const char *PChar = Font_6x8_Data + (nBytes * c);
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    // Iterate rows of the char
    uint8_t row, col;
    for(row = 0; row < nRows; row++) {
        if((Fnt.Y+row) >= LCD_H) break;
        uint8_t PixelRow = *PChar++;
        // Loop on each pixel in the row (left to right)
        for(col=0; col < nCols; col++) {
            if((Fnt.X+col) >= LCD_W) break;
            Color_t *PClr = (PixelRow & 0x80)? &Fnt.FClr : &Fnt.BClr;
            PixelRow <<= 1;
            WriteByte(PClr->RGBTo565_HiByte());
            WriteByte(PClr->RGBTo565_LoByte());
        } // col
    } // row
    DC_Lo();
    Fnt.X += nCols;
}

static inline void FLcdPutChar(char c) { Lcd.PutChar(c); }

void Lcd_t::Printf(uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...) {
    Fnt.X = x;
    Fnt.Y = y;
    Fnt.FClr = ForeClr;
    Fnt.BClr = BckClr;
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLcdPutChar, 20, S, args);
    va_end(args);
}

#if 1 // ============================= Graphics ================================
void Lcd_t::Cls(Color_t Color) {
    SetBounds(0, LCD_W, 0, LCD_H);
    // Prepare variables
    uint32_t Cnt = LCD_W * LCD_H;
    uint8_t b1 = Color.RGBTo565_HiByte();
    uint8_t b2 = Color.RGBTo565_LoByte();
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    for(uint32_t i=0; i<Cnt; i++) {
        WriteByte(b1);
        WriteByte(b2);
    }
    DC_Lo();
}

void Lcd_t::GetBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf) {
    SetBounds(x0, x0+Width, y0, y0+Height);
    // Prepare variables
    uint32_t Cnt = Width * Height;
    uint16_t R, G, B;
    // Read RAM
    WriteByte(0x2E);    // RAMRD
    DC_Hi();
    ModeRead();
    ReadByte();         // Dummy read
    for(uint32_t i=0; i<Cnt; i++) {
        R = ReadByte(); // }
        G = ReadByte(); // }
        B = ReadByte(); // } Inside LCD, data is always in 18bit format.
        // Produce 4R-4G-4B from 6R-6G-6B
        *PBuf++ = ((R & 0xF0) << 4) | (G & 0xF0) | ((B & 0xF0) >> 4);
    }
    ModeWrite();
    DC_Lo();
}

void Lcd_t::PutBitmap(uint8_t x0, uint8_t y0, uint8_t Width, uint8_t Height, uint16_t *PBuf) {
    //Uart.Printf("%u %u %u %u %u\r", x0, y0, Width, Height, *PBuf);
    SetBounds(x0, x0+Width, y0, y0+Height);
    // Prepare variables
#ifdef LCD_18BIT

#elif defined LCD_16BIT
    uint16_t Clr;
    uint32_t Cnt = (uint32_t)Width * (uint32_t)Height;    // One pixel at one time
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    for(uint32_t i=0; i<Cnt; i++) {
        Clr = *PBuf++;
        uint16_t R = (Clr >> 8) & 0x000F;
        uint16_t G = (Clr >> 4) & 0x000F;
        uint16_t B = (Clr     ) & 0x000F;
        R = (R << 4) | (G >> 1);
        G = (G << 7) | (B << 1);
        WriteByte(R & 0x0F);   // RRRR0-GGG
        WriteByte(G & 0x0F);   // G00-BBBB0
    }
    DC_Lo();
#else
    uint16_t Clr1, Clr2;
    uint32_t Cnt = (uint32_t)Width * Height / 2;      // Two pixels at one time
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    for(uint32_t i=0; i<Cnt; i++) {
        Clr1 = (*PBuf++) & 0x0FFF;
        Clr2 = (*PBuf++) & 0x0FFF;
        WriteByte(Clr1 >> 4);    // RRRR-GGGG
        WriteByte(((Clr1 & 0x00F) << 4) | (Clr2 >> 8));   // BBBB-RRRR
        WriteByte(Clr2 & 0x0FF); // GGGG-BBBB
    }
    DC_Lo();
#endif
}
#endif

#if 1 // ======================== Font printf ==================================
void Lcd_t::PutFontChar(char c) {
    const tChar *PChar = Fnt.PFirstChar + (c - Fnt.PFirstChar->Code);
    // Read char params
    uint8_t nCols = PChar->PImage->Width;
    uint8_t nRows = PChar->PImage->Height;
    SetBounds(Fnt.X, nCols, Fnt.Y, nRows);
    // Get pointer to the first byte of the desired character
    const uint8_t *p = PChar->PImage->PData;
    Color_t MixClr;
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    // Iterate rows of the char
    uint8_t row, col;
    for(row = 0; row < nRows; row++) {
        if((Fnt.Y+row) >= LCD_H) break;
        for(col=0; col < nCols; col++) {
            if((Fnt.X+col) >= LCD_W) break;
            uint32_t L = *p++;
            MixClr.MixOf(Fnt.FClr, Fnt.BClr, L);
            WriteByte(MixClr.RGBTo565_HiByte());
            WriteByte(MixClr.RGBTo565_LoByte());
//            Uart.Printf("\rRGB: %02X %02X %02X; 565: %02X %02X", MixClr.R, MixClr.G, MixClr.B, MixClr.RGBTo565_HiByte(), MixClr.RGBTo565_LoByte());
        } // col
    } // row
    DC_Lo();
    Fnt.X += nCols;
}

static inline void FLcdPutFontChar(char c) { Lcd.PutFontChar(c); }

void Lcd_t::Printf(const tFont &Font, uint8_t x, uint8_t y, Color_t ForeClr, Color_t BckClr, const char *S, ...) {
    Fnt.X = x;
    Fnt.Y = y;
    Fnt.FClr = ForeClr;
    Fnt.BClr = BckClr;
    Fnt.PFirstChar = Font.Chars;
    va_list args;
    va_start(args, S);
    kl_vsprintf(FLcdPutFontChar, 20, S, args);
    va_end(args);
}
#endif

void Lcd_t::DrawBatteryState() {
//    Uart.Printf("\rCapacity=%u", Power.CapacityPercent);
    uint8_t clr1bHi, clr1bLo, clr2bHi, clr2bLo;
    // Colors' components
    if(Power.IsCharging()) {    // Draw with other color
        clr1bHi = (BAT_RECT_CHARGE_CLR >> 8) & 0x00FF;
        clr1bLo = BAT_RECT_CHARGE_CLR & 0x00FF;
    }
    else {
        clr1bHi = (BAT_RECT_FORE_CLR >> 8) & 0x00FF;
        clr1bLo = BAT_RECT_FORE_CLR & 0x00FF;
    }
    clr2bHi = (BAT_RECT_BACK_CLR >> 8) & 0x00FF;
    clr2bLo = BAT_RECT_BACK_CLR & 0x00FF;
    // Edge of color
    int Edge = BAT_RECT_WIDTH - (Power.CapacityPercent / (BAT_RECT_WIDTH + 1));
    // Draw the rectangle
    SetBounds(BAT_RECT_LEFT, BAT_RECT_WIDTH, BAT_RECT_TOP, BAT_RECT_HEIGHT);
    WriteByte(0x2C); // Memory write
    DC_Hi();
    for(int y=0; y < BAT_RECT_HEIGHT; y++) {
        for(int x=1; x <= BAT_RECT_WIDTH; x++) {
            if(x > Edge) {
                WriteByte(clr1bHi);
                WriteByte(clr1bLo);
            }
            else {
                WriteByte(clr2bHi);
                WriteByte(clr2bLo);
            }
        } // x
    } // y
    DC_Lo();
}

void Lcd_t::DrawBatteryBounds() {
    uint8_t clrbHi, clrbLo, backClrHi, backClrLo;
    SetBounds(BAT_BORD_LEFT, BAT_BORD_WIDTH, BAT_BORD_TOP, BAT_BORD_HEIGHT);
    clrbHi = (BAT_RECT_FORE_CLR >> 8) & 0x00FF;
    clrbLo = BAT_RECT_FORE_CLR & 0x00FF;
    backClrHi = (BAT_RECT_BACK_CLR >> 8) & 0x00FF;
    backClrLo = BAT_RECT_BACK_CLR & 0x00FF;
    WriteByte(0x2C); // Memory write
    DC_Hi();
    for(int y=0; y < BAT_BORD_HEIGHT; y++) {
        for(int x=1; x <= BAT_BORD_WIDTH; x++) {
            if( ((x>2 && x<14) && (y>0 && y<7)) || ((x == 1) && (y == 0 || y == 7))) {
                WriteByte(backClrHi);
                WriteByte(backClrLo);
            }
            else {
                WriteByte(clrbHi);
                WriteByte(clrbLo);
            }
        } // x
    } // y
    DC_Lo();
}

void Lcd_t::DrawSignal(uint8_t x0, uint8_t y0, Color_t ForeClr, Color_t BckClr, uint8_t Pwr) {
    const tImage *NeedGisto;
    if       (Pwr > 90)                 NeedGisto = &Gisto7;
    else if ((Pwr > 80) && (90 >= Pwr))  NeedGisto = &Gisto6;
    else if ((Pwr > 70) && (80 >= Pwr))  NeedGisto = &Gisto5;
    else if ((Pwr > 60) && (70 >= Pwr))  NeedGisto = &Gisto4;
    else if ((Pwr > 50) && (60 >= Pwr))  NeedGisto = &Gisto3;
    else if ((Pwr > 40) && (50 >= Pwr))  NeedGisto = &Gisto2;
    else if ((Pwr > 20) && (40 >= Pwr))  NeedGisto = &Gisto1;
    else                                 NeedGisto = &Gisto0;
    uint8_t nCols = NeedGisto->Width;
    uint8_t nRows = NeedGisto->Height;
    SetBounds(x0, nCols, y0, nRows);
    // Get pointer to the first byte of the desired character
    const uint8_t *p = NeedGisto->PData;
    Color_t MixClr;
    // Write RAM
    WriteByte(0x2C);    // Memory write
    DC_Hi();
    // Iterate rows of the char
    uint8_t row, col;
    for(row = 0; row < nRows; row++) {
        if((y0+row) >= LCD_H) break;
        for(col=0; col < nCols; col++) {
            if((x0+col) >= LCD_W) break;
            uint32_t L = *p++;
            MixClr.MixOf(ForeClr, BckClr, L);
            WriteByte(MixClr.RGBTo565_HiByte());
            WriteByte(MixClr.RGBTo565_LoByte());
        } // col
    } // row
    DC_Lo();
}

//void Lcd_t::DrawSignalPower(uint8_t y, Color_t Color, uint8_t Pwr) {
//    uint8_t clrbHi, clrbLo, backClrHi, backClrLo;
//    SetBounds(PWR_BORD_LEFT, PWR_BORD_WIDTH, y + PWR_BORD_TOP, PWR_BORD_HEIGHT);
//    clrbHi = (PWR_ORG_COLOR >> 8) & 0x00FF;
//    clrbLo = PWR_ORG_COLOR & 0x00FF;
//    backClrHi = (PWR_BCK_COLOR >> 8) & 0x00FF;
//    backClrLo = PWR_BCK_COLOR & 0x00FF;
//    WriteByte(0x2C); // Memory write
//    DC_Hi();
//    for(int y=0; y < PWR_BORD_HEIGHT; y++) {
//        for(int x=1; x <= PWR_BORD_WIDTH; x++) {
//            WriteByte(clrbHi);
//            WriteByte(clrbLo);
//        } // x
//    } // y
//    DC_Lo();
//}

#if 1 // ============================= BMP =====================================
struct BmpHeader_t {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t Reserved[2];
    uint32_t bfOffBits;
} __packed;
#define BMP_HEADER_SZ   sizeof(BmpHeader_t)

struct BmpInfo_t {
    int32_t Size;
    int32_t Width;
    int32_t Height;
    uint16_t Planes;
    uint16_t BitCnt;
    uint32_t Compression;
    uint32_t SzImage;
    uint32_t XPelsPerMeter, YPelsPerMeter;
    uint32_t ClrUsed, ClrImportant;
    // v4 begins. Only Adobe version here
    uint32_t RedMsk, GreenMsk, BlueMsk, AlphaMsk;
} __packed;
#define BMP_INFO_SZ   sizeof(BmpInfo_t)

void Lcd_t::DrawBmpFile(uint8_t x0, uint8_t y0, const char *Filename) {
    //Uart.Printf("\r%S %S x=%d y=%d", __func__, Filename, x0, y0);
    // Open file
    FRESULT rslt = f_open(&IFile, Filename, FA_READ+FA_OPEN_EXISTING);
//    Uart.Printf("\r%u; fsz=%u", rslt, IFile.fsize);
    if(rslt != FR_OK) {
        if (rslt == FR_NO_FILE) Uart.Printf("\r%S: not found", Filename);
        else Uart.Printf("\rOpenFile error: %u", rslt);
        return;
    }
    // Check if zero file
    if(IFile.fsize == 0) {
        Uart.Printf("\rEmpty file");
        f_close(&IFile);
        return;
    }
    // Read piece of file
    unsigned int RCnt=0;
    if((rslt = f_read(&IFile, IFileBuf, LCD_FILE_BUF_SZ, &RCnt)) != 0) { f_close(&IFile); return; }
    // ==== BMPHEADER ====
    if(RCnt < BMP_HEADER_SZ) { f_close(&IFile); return; }
    BmpHeader_t *PHdr = (BmpHeader_t*)IFileBuf;
//    Uart.Printf("T=%X; Sz=%u; Off=%u\r", PHdr->bfType, PHdr->bfSize, PHdr->bfOffBits);
    if(PHdr->bfOffBits < BMP_HEADER_SZ) { f_close(&IFile); return; } // return if offset is too large

    // ==== BITMAPINFO ====
    if(RCnt < (BMP_INFO_SZ + BMP_HEADER_SZ)) { f_close(&IFile); return; }
    BmpInfo_t *PInfo = (BmpInfo_t*)&IFileBuf[BMP_HEADER_SZ];
    // Get struct size => version
    if((PInfo->Size == 40) or (PInfo->Size == 52) or (PInfo->Size == 56)) {  // V3 or V4 adobe
        if(PInfo->Height < 0) PInfo->Height = -PInfo->Height;
        //Uart.Printf("\rW=%u; H=%u; BitCnt=%u; Cmp=%u; Sz=%u;  MskR=%X; MskG=%X; MskB=%X; MskA=%X", PInfo->Width, PInfo->Height, PInfo->BitCnt, PInfo->Compression, PInfo->SzImage, PInfo->RedMsk, PInfo->GreenMsk, PInfo->BlueMsk, PInfo->AlphaMsk);
        SetBounds(x0, PInfo->Width, y0, PInfo->Height);

        // ==== Write RAM ====
        int32_t Sz = PInfo->SzImage;
        bool WidthIsOdd = PInfo->Width % 2; // Odd-widthed images require special action
        uint32_t NPixel=0, W = PInfo->Width+1, Edge = W;
        WriteByte(0x2C); // Memory write
        DC_Hi();
        // First, write piece currently in memory
        for(uint32_t i=PHdr->bfOffBits; i<RCnt; i+=2) {
            if(WidthIsOdd) if(++NPixel == Edge) {   // Ignore last pixel in a row in case of odd width
                Edge += W;
                continue;
            }
            WriteByte(IFileBuf[i+1]);
            WriteByte(IFileBuf[i]);
        }
        Sz -= (RCnt - PHdr->bfOffBits);
        // Read remainder
        while(Sz > 0) {
            rslt = f_read(&IFile, IFileBuf, LCD_FILE_BUF_SZ, &RCnt);
            if((rslt != 0) or (RCnt == 0)) break; // if error or eof
            for(uint32_t i=0; i<RCnt; i+=2) {
                if(WidthIsOdd) if(++NPixel == Edge) {   // Ignore last pixel in a row in case of odd width
                    Edge += W;
                    continue;
                }
                WriteByte(IFileBuf[i+1]);
                WriteByte(IFileBuf[i]);
            }
            Sz -= RCnt;
        }
        DC_Lo();
    }
    f_close(&IFile);
}
#endif
