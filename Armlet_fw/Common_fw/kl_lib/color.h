/*
 * color.h
 *
 *  Created on: 18 июля 2014 г.
 *      Author: g.kruglov
 */

#ifndef COLOR_H_
#define COLOR_H_

// Mixing two colors
#define ClrMix(C, B, L)     ((C * L + B * (255 - L)) / 255)

struct Color_t {
    uint8_t R, G, B;
    bool operator == (Color_t &AColor) { return ((R == AColor.R) and (G == AColor.G) and (B == AColor.B)); }
    bool operator != (Color_t &AColor) { return ((R != AColor.R) or  (G != AColor.G) or  (B != AColor.B)); }
    Color_t& operator=(const Color_t &Right) { R = Right.R; G = Right.G; B = Right.B; return *this; }
    uint8_t RGBTo565_HiByte() {
        uint32_t rslt = R & 0b11111000;
        rslt |= G >> 5;
        return (uint8_t)rslt;
    }
    uint8_t RGBTo565_LoByte() {
        uint32_t rslt = (G << 3) & 0b11100000;
        rslt |= B >> 3;
        return (uint8_t)rslt;
    }
    void MixOf(Color_t &Fore, Color_t &Back, uint32_t Brt) {
        R = ClrMix(Fore.R, Back.R, Brt);
        G = ClrMix(Fore.G, Back.G, Brt);
        B = ClrMix(Fore.B, Back.B, Brt);
    }
};

#define clBlack     ((Color_t){0,   0,   0})
#define clRed       ((Color_t){255, 0,   0})
#define clGreen     ((Color_t){0,   255, 0})
#define clBlue      ((Color_t){0,   0,   255})
#define clYellow    ((Color_t){255, 255, 0})
#define clMagenta   ((Color_t){255, 0, 255})
#define clCyan      ((Color_t){0, 255, 255})
#define clWhite     ((Color_t){255, 255, 255})



#endif /* COLOR_H_ */
