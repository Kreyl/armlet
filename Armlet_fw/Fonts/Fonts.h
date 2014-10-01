#ifndef FONTS_H_
#define FONTS_H_

#include "Images.h"

typedef struct {
    int Code;
    const tImage *PImage;
} tChar;

typedef struct {
    int Length;
    const tChar *Chars;
} tFont;

extern const tFont B52;
extern const tFont CourierNew;
extern const tFont DSMoster;
extern const tFont Impact;

#endif /* FONTS_H_ */
