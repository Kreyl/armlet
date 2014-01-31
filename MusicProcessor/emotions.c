/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EP.py from Emotions.csv and Reasons.csv
 *
 * !!! DO NOT EDIT !!!
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
 /*  0 */ { "fon", 1, ROOT, 0 },
 /*  1 */     { "negativ", 1, 0, 0 },
 /*  2 */         { "ravnodushie", 1, 1, 0 },
 /*  3 */             { "skuka", 1, 2, 0 },
 /*  4 */                 { "apatiya", 1, 3, 0 },
 /*  5 */                     { "depressiya", 1, 4, 0 },
 /*  6 */         { "zlost'", 1, 1, 0 },
 /*  7 */             { "gnev", 1, 6, 0 },
 /*  8 */                 { "yarost'", 1, 7, 0 },
 /*  9 */             { "razdrazhenie", 1, 6, 0 },
 /* 10 */         { "grust'", 1, 1, 0 },
 /* 11 */             { "toska", 1, 10, 0 },
 /* 12 */                 { "obrechennost'", 1, 11, 0 },
 /* 13 */         { "neuverennost'", 1, 1, 0 },
 /* 14 */             { "ozabochennost'", 1, 13, 0 },
 /* 15 */                 { "napryazhennost'", 1, 14, 0 },
 /* 16 */                     { "isterika", 1, 15, 0 },
 /* 17 */             { "somneniya", 1, 13, 0 },
 /* 18 */         { "trevozhnost'", 1, 1, 0 },
 /* 19 */             { "strah", 1, 18, 0 },
 /* 20 */                 { "uzhas", 1, 19, 0 },
 /* 21 */                     { "panika", 1, 20, 0 },
 /* 22 */                         { "fobiya", 1, 21, 0 },
 /* 23 */                         { "paranojya", 1, 21, 0 },
 /* 24 */         { "nepriyazn'", 1, 1, 0 },
 /* 25 */             { "nenavist'", 1, 24, 0 },
 /* 26 */             { "otvraschenie", 1, 24, 0 },
 /* 27 */             { "prezrenie", 1, 24, 0 },
 /* 28 */         { "neudovletvorennost'", 1, 1, 0 },
 /* 29 */     { "razrushenie", 1, 0, 0 },
 /* 30 */     { "draka", 1, 0, 0 },
 /* 31 */     { "ubijstvo", 1, 0, 0 },
 /* 32 */     { "pozitiv", 1, 0, 0 },
 /* 33 */         { "vesel'e", 1, 32, 0 },
 /* 34 */             { "radost'", 1, 33, 0 },
 /* 35 */                 { "vostorg", 1, 34, 0 },
 /* 36 */         { "dovol'stvo", 1, 32, 0 },
 /* 37 */             { "gordost'", 1, 36, 0 },
 /* 38 */             { "udovletvorenie", 1, 36, 0 },
 /* 39 */         { "lyubopyitstvo", 1, 32, 0 },
 /* 40 */             { "interes", 1, 39, 0 },
 /* 41 */             { "uvlechennost'", 1, 39, 0 },
 /* 42 */                 { "azart", 1, 41, 0 },
 /* 43 */                 { "voodushevlenie", 1, 41, 0 },
 /* 44 */                     { "tseleustremlennost'", 1, 43, 0 },
 /* 45 */                 { "vdohnovenie", 1, 41, 0 },
 /* 46 */         { "priyazn'", 1, 32, 0 },
 /* 47 */             { "simpatiya", 1, 46, 0 },
 /* 48 */                 { "privyazannost'", 1, 47, 0 },
 /* 49 */                     { "lyubov'", 1, 48, 0 },
 /* 50 */                 { "vozhdelenie", 1, 47, 0 },
 /* 51 */             { "umilenie", 1, 46, 0 },
 /* 52 */         { "nadezhda", 1, 32, 0 },
 /* 53 */             { "predvkushenie", 1, 52, 0 },
 /* 54 */                 { "vera", 1, 53, 0 },
 /* 55 */                 { "uverennost'", 1, 53, 0 },
 /* 56 */         { "pokoj", 1, 32, 0 },
 /* 57 */             { "umirotvorenie", 1, 56, 0 },
 /* 58 */     { "sozidanie", 1, 0, 0 },
 /* 59 */     { "seks", 1, 0, 0 },
 /* 60 */     { "zhelanie", 1, 0, 0 },
 /* 61 */         { "vlechenie", 1, 60, 0 },
 /* 62 */             { "strast'", 1, 61, 0 },
 /* 63 */                 { "oderzhimost'", 1, 62, 0 },
 /* 64 */                     { "maniya", 1, 63, 0 },
 /* 65 */         { "zainteresovannost'", 1, 60, 0 },
 /* 66 */     { "zavisimost'", 1, 0, 0 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /* 0 */ { "uliza1", 1, 0 }, 	/* fon */
 /* 1 */ { "uliza2", 1, 0 }, 	/* fon */
 /* 2 */ { "uliza3", 1, 0 }, 	/* fon */
 /* 3 */ { "church", 5, 57 }, 	/* umirotvorenie */
 /* 4 */ { "kabak", 2, 34 }, 	/* radost' */
};

const int reasons_number = countof(reasons);

// End of emotions.c
