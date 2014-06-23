/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py from Emotions.csv and Reasons.csv
 *
 * !!! DO NOT EDIT !!!
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
 /*  0 */ { "fon", 1, ROOT, -1, -1 },
 /*  1 */     { "negativ", 1, 0, -1, -1 },
 /*  2 */         { "ravnodushie", 1, 1, -1, -1 },
 /*  3 */             { "skuka", 1, 2, -1, -1 },
 /*  4 */                 { "apatiya", 1, 3, -1, -1 },
 /*  5 */                     { "depressiya", 1, 4, -1, -1 },
 /*  6 */         { "zlost'", 1, 1, -1, -1 },
 /*  7 */             { "gnev", 1, 6, -1, -1 },
 /*  8 */                 { "yarost'", 1, 7, -1, -1 },
 /*  9 */             { "razdrazhenie", 1, 6, -1, -1 },
 /* 10 */         { "grust'", 1, 1, -1, -1 },
 /* 11 */             { "toska", 1, 10, -1, -1 },
 /* 12 */                 { "obrechennost'", 1, 11, -1, -1 },
 /* 13 */         { "neuverennost'", 1, 1, -1, -1 },
 /* 14 */             { "ozabochennost'", 1, 13, -1, -1 },
 /* 15 */                 { "napryazhennost'", 1, 14, -1, -1 },
 /* 16 */                     { "isterika", 1, 15, -1, -1 },
 /* 17 */             { "somneniya", 1, 13, -1, -1 },
 /* 18 */         { "trevozhnost'", 1, 1, -1, -1 },
 /* 19 */             { "strah", 1, 18, -1, -1 },
 /* 20 */                 { "uzhas", 1, 19, -1, -1 },
 /* 21 */                     { "panika", 1, 20, -1, -1 },
 /* 22 */                         { "fobiya", 1, 21, -1, -1 },
 /* 23 */                         { "paranojya", 1, 21, -1, -1 },
 /* 24 */         { "nepriyazn'", 1, 1, -1, -1 },
 /* 25 */             { "nenavist'", 1, 24, -1, -1 },
 /* 26 */             { "otvraschenie", 1, 24, -1, -1 },
 /* 27 */             { "prezrenie", 1, 24, -1, -1 },
 /* 28 */         { "neudovletvorennost'", 1, 1, -1, -1 },
 /* 29 */     { "razrushenie", 1, 0, -1, -1 },
 /* 30 */     { "draka", 1, 0, -1, -1 },
 /* 31 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 32 */     { "pozitiv", 1, 0, -1, -1 },
 /* 33 */         { "vesel'e", 1, 32, -1, -1 },
 /* 34 */             { "radost'", 1, 33, -1, -1 },
 /* 35 */                 { "vostorg", 1, 34, -1, -1 },
 /* 36 */         { "dovol'stvo", 1, 32, -1, -1 },
 /* 37 */             { "gordost'", 1, 36, -1, -1 },
 /* 38 */             { "udovletvorenie", 1, 36, -1, -1 },
 /* 39 */         { "lyubopyitstvo", 1, 32, -1, -1 },
 /* 40 */             { "interes", 1, 39, -1, -1 },
 /* 41 */             { "uvlechennost'", 1, 39, -1, -1 },
 /* 42 */                 { "azart", 1, 41, -1, -1 },
 /* 43 */                 { "voodushevlenie", 1, 41, -1, -1 },
 /* 44 */                     { "tseleustremlennost'", 1, 43, -1, -1 },
 /* 45 */                 { "vdohnovenie", 1, 41, -1, -1 },
 /* 46 */         { "priyazn'", 1, 32, -1, -1 },
 /* 47 */             { "simpatiya", 1, 46, -1, -1 },
 /* 48 */                 { "privyazannost'", 1, 47, -1, -1 },
 /* 49 */                     { "lyubov'", 1, 48, -1, -1 },
 /* 50 */                 { "vozhdelenie", 1, 47, -1, -1 },
 /* 51 */             { "umilenie", 1, 46, -1, -1 },
 /* 52 */         { "nadezhda", 1, 32, -1, -1 },
 /* 53 */             { "predvkushenie", 1, 52, -1, -1 },
 /* 54 */                 { "vera", 1, 53, -1, -1 },
 /* 55 */                 { "uverennost'", 1, 53, -1, -1 },
 /* 56 */         { "pokoj", 1, 32, -1, -1 },
 /* 57 */             { "umirotvorenie", 1, 56, -1, -1 },
 /* 58 */     { "sozidanie", 1, 0, -1, -1 },
 /* 59 */     { "seks", 1, 0, -1, -1 },
 /* 60 */     { "zhelanie", 1, 0, -1, -1 },
 /* 61 */         { "vlechenie", 1, 60, -1, -1 },
 /* 62 */             { "strast'", 1, 61, -1, -1 },
 /* 63 */                 { "oderzhimost'", 1, 62, -1, -1 },
 /* 64 */                     { "maniya", 1, 63, -1, -1 },
 /* 65 */         { "zainteresovannost'", 1, 60, -1, -1 },
 /* 66 */     { "zavisimost'", 1, 0, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array. reason_id
Reason_t reasons[] = {
 /* 0 */ { "uliza1", 1, 0 }, 	/* fon */
 /* 1 */ { "uliza2", 1, 0 }, 	/* fon */
 //hiding reasons
 /* 2 */ { "master", 77, 0 }, 	/* masterka, fon playing */ //TODO add emofor this
 /* 3 */ { "tuman", 77, 19 },  /* tuman, allplayers melody*/ //TODO add emofor this
 /* 4 */ { "silence", 77, 0 }, 	/* nothing playing */ //TODO add emofor this
 //user emit reasons
 /* 5 */ { "murder", 77, 1 }, 	/* negativ */
 /* 6 */ { "creation", 77, 32 },   /* fon */
 /* 8 */ { "destruction", 77, 1 },   /* fon */
 /* 9 */ { "sex", 77, 32 },   /* strah */
 /* 10 */ { "fight", 77, 1 },   /* fon */
 /* 11 */ { "narco", 77, 1 },   /* strah */
 //user emit reasons end
 /* 3 */ { "church", 2, 32 },   /* pozitiv */
 /* 4 */ { "kabak", 2, 1 },     /* negativ */
};


//{0,20,120,300,120},//murder
//{1,40,120,300,120},//creation
//{2,40,120,300,120},//destruction
//{3,2000,1,300,120},//sex
//{4,2000,1,60,120},//fight
//{5,2000,1200,2000,400},//narco

const int reasons_number = countof(reasons);

// End of emotions.c
