/*
 * Fonts.h
 *
 *  Created on: 17 июля 2014 г.
 *      Author: g.kruglov
 */

#ifndef FONTS_H_
#define FONTS_H_

// Types
typedef struct {
    const uint8_t *PData;
    uint8_t Width;
    uint8_t Height;
} tImage;

typedef struct {
    int Code;
    const tImage *PImage;
} tChar;

typedef struct {
    int Length;
    const tChar *Chars;
} tFont;

// Fonts
extern const tFont Impact;

#endif /* FONTS_H_ */
