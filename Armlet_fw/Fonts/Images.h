#ifndef IMAGES_H_
#define IMAGES_H_

#include <stdint.h>

typedef struct {
    const uint8_t *PData;
    uint8_t Width;
    uint8_t Height;
} tImage;

typedef struct {
    const uint8_t **PData;
    uint8_t NumImages;
    uint8_t Width;
    uint8_t Height;
} tImageSequence;

extern const tImage Gisto0;
extern const tImage Gisto1;
extern const tImage Gisto2;
extern const tImage Gisto3;
extern const tImage Gisto4;
extern const tImage Gisto5;
extern const tImage Gisto6;
extern const tImage Gisto7;

extern const tImageSequence Gisto = {{
        Gisto0.PData,
        Gisto1.PData,
        Gisto2.PData,
        Gisto3.PData,
        Gisto4.PData,
        Gisto5.PData,
        Gisto6.PData,
        Gisto7.PData,
    }, 8, Gisto0.Width, Gisto0.Height };

#endif /* IMAGES_H_ */
