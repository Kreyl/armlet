/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py from Emotions.csv, Locations.csv and Characters.csv
 *
 * !!! DO NOT EDIT !!!
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
 /*  0 */ { "fon", 1, ROOT, -1, -1 },
 /*  1 */     { "wrong", 1, 0, -1, -1 },
 /*  2 */     { "master", 1, 0, -1, -1 },
 /*  3 */     { "silence", 1, 0, -1, -1 },
 /*  4 */     { "tuman", 1, 0, -1, -1 },
 /*  5 */     { "negativ", 1, 0, -1, -1 },
 /*  6 */         { "ravnodushie", 1, 5, -1, -1 },
 /*  7 */             { "skuka", 1, 6, -1, -1 },
 /*  8 */                 { "apatiya", 1, 7, -1, -1 },
 /*  9 */                     { "depressiya", 1, 8, -1, -1 },
 /* 10 */         { "zlost'", 1, 5, -1, -1 },
 /* 11 */             { "gnev", 1, 10, -1, -1 },
 /* 12 */                 { "yarost'", 1, 11, -1, -1 },
 /* 13 */             { "razdrazhenie", 1, 10, -1, -1 },
 /* 14 */         { "grust'", 1, 5, -1, -1 },
 /* 15 */             { "toska", 1, 14, -1, -1 },
 /* 16 */                 { "obrechennost'", 1, 15, -1, -1 },
 /* 17 */         { "neuverennost'", 1, 5, -1, -1 },
 /* 18 */             { "ozabochennost'", 1, 17, -1, -1 },
 /* 19 */                 { "napryazhennost'", 1, 18, -1, -1 },
 /* 20 */                     { "isterika", 1, 19, -1, -1 },
 /* 21 */             { "somnenie", 1, 17, -1, -1 },
 /* 22 */         { "trevozhnost'", 1, 5, -1, -1 },
 /* 23 */             { "strah", 1, 22, -1, -1 },
 /* 24 */                 { "uzhas", 1, 23, -1, -1 },
 /* 25 */                     { "panika", 1, 24, -1, -1 },
 /* 26 */                         { "fobiya", 1, 25, -1, -1 },
 /* 27 */                         { "paranojya", 1, 25, -1, -1 },
 /* 28 */         { "nepriyazn'", 1, 5, -1, -1 },
 /* 29 */             { "nenavist'", 1, 28, -1, -1 },
 /* 30 */             { "otvraschenie", 1, 28, -1, -1 },
 /* 31 */             { "prezrenie", 1, 28, -1, -1 },
 /* 32 */         { "neudovletvorennost'", 1, 5, -1, -1 },
 /* 33 */     { "razrushenie", 1, 0, -1, -1 },
 /* 34 */     { "draka", 1, 0, -1, -1 },
 /* 35 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 36 */     { "pozitiv", 1, 0, -1, -1 },
 /* 37 */         { "vesel'e", 1, 36, -1, -1 },
 /* 38 */             { "radost'", 1, 37, -1, -1 },
 /* 39 */                 { "vostorg", 1, 38, -1, -1 },
 /* 40 */         { "dovol'stvo", 1, 36, -1, -1 },
 /* 41 */             { "gordost'", 1, 40, -1, -1 },
 /* 42 */             { "udovletvorenie", 1, 40, -1, -1 },
 /* 43 */         { "lyubopyitstvo", 1, 36, -1, -1 },
 /* 44 */             { "interes", 1, 43, -1, -1 },
 /* 45 */             { "uvlechennost'", 1, 43, -1, -1 },
 /* 46 */                 { "azart", 1, 45, -1, -1 },
 /* 47 */                 { "voodushevlenie", 1, 45, -1, -1 },
 /* 48 */                     { "tseleustremlennost'", 1, 47, -1, -1 },
 /* 49 */                 { "vdohnovenie", 1, 45, -1, -1 },
 /* 50 */         { "priyazn'", 1, 36, -1, -1 },
 /* 51 */             { "simpatiya", 1, 50, -1, -1 },
 /* 52 */                 { "privyazannost'", 1, 51, -1, -1 },
 /* 53 */                     { "lyubov'", 1, 52, -1, -1 },
 /* 54 */                 { "vozhdelenie", 1, 51, -1, -1 },
 /* 55 */             { "umilenie", 1, 50, -1, -1 },
 /* 56 */         { "nadezhda", 1, 36, -1, -1 },
 /* 57 */             { "predvkushenie", 1, 56, -1, -1 },
 /* 58 */                 { "vera", 1, 57, -1, -1 },
 /* 59 */                 { "uverennost'", 1, 57, -1, -1 },
 /* 60 */         { "pokoj", 1, 36, -1, -1 },
 /* 61 */             { "umirotvorenie", 1, 60, -1, -1 },
 /* 62 */     { "sozidanie", 1, 0, -1, -1 },
 /* 63 */     { "seks", 1, 0, -1, -1 },
 /* 64 */     { "zhelanie", 1, 0, -1, -1 },
 /* 65 */         { "vlechenie", 1, 64, -1, -1 },
 /* 66 */             { "strast'", 1, 65, -1, -1 },
 /* 67 */                 { "oderzhimost'", 1, 66, -1, -1 },
 /* 68 */                     { "maniya", 1, 67, -1, -1 },
 /* 69 */         { "zainteresovannost'", 1, 64, -1, -1 },
 /* 70 */     { "zavisimost'", 1, 0, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
	// Locations
 /*   0 */ { "r000", 99, 1 },	/* wrong */
 /*   1 */ { "hub", 99, 2 },	/* master */
 /*   2 */ { "uliza", 1, 0 },	/* fon */
 /*   3 */ { "tuman", 9, 23 },	/* strah */
 /*   4 */ { "church", 2, 36 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 5 },	/* negativ */
 /*   6 */ { "r006", 99, 1 },	/* wrong */
 /*   7 */ { "r007", 99, 1 },	/* wrong */
 /*   8 */ { "r008", 99, 1 },	/* wrong */
 /*   9 */ { "r009", 99, 1 },	/* wrong */
 /*  10 */ { "r010", 99, 1 },	/* wrong */
 /*  11 */ { "r011", 99, 1 },	/* wrong */
 /*  12 */ { "r012", 99, 1 },	/* wrong */
 /*  13 */ { "r013", 99, 1 },	/* wrong */
 /*  14 */ { "r014", 99, 1 },	/* wrong */
 /*  15 */ { "r015", 99, 1 },	/* wrong */
 /*  16 */ { "r016", 99, 1 },	/* wrong */
 /*  17 */ { "r017", 99, 1 },	/* wrong */
 /*  18 */ { "r018", 99, 1 },	/* wrong */
 /*  19 */ { "r019", 99, 1 },	/* wrong */
 /*  20 */ { "r020", 99, 1 },	/* wrong */
 /*  21 */ { "r021", 99, 1 },	/* wrong */
 /*  22 */ { "r022", 99, 1 },	/* wrong */
 /*  23 */ { "r023", 99, 1 },	/* wrong */
 /*  24 */ { "r024", 99, 1 },	/* wrong */
 /*  25 */ { "r025", 99, 1 },	/* wrong */
 /*  26 */ { "r026", 99, 1 },	/* wrong */
 /*  27 */ { "r027", 99, 1 },	/* wrong */
 /*  28 */ { "r028", 99, 1 },	/* wrong */
 /*  29 */ { "r029", 99, 1 },	/* wrong */
 /*  30 */ { "r030", 99, 1 },	/* wrong */
 /*  31 */ { "r031", 99, 1 },	/* wrong */
 /*  32 */ { "r032", 99, 1 },	/* wrong */
 /*  33 */ { "r033", 99, 1 },	/* wrong */
 /*  34 */ { "r034", 99, 1 },	/* wrong */
 /*  35 */ { "r035", 99, 1 },	/* wrong */
 /*  36 */ { "r036", 99, 1 },	/* wrong */
 /*  37 */ { "r037", 99, 1 },	/* wrong */
 /*  38 */ { "r038", 99, 1 },	/* wrong */
 /*  39 */ { "r039", 99, 1 },	/* wrong */
 /*  40 */ { "r040", 99, 1 },	/* wrong */
 /*  41 */ { "r041", 99, 1 },	/* wrong */
 /*  42 */ { "r042", 99, 1 },	/* wrong */
 /*  43 */ { "r043", 99, 1 },	/* wrong */
 /*  44 */ { "r044", 99, 1 },	/* wrong */
 /*  45 */ { "r045", 99, 1 },	/* wrong */
 /*  46 */ { "r046", 99, 1 },	/* wrong */
 /*  47 */ { "r047", 99, 1 },	/* wrong */
 /*  48 */ { "r048", 99, 1 },	/* wrong */
 /*  49 */ { "r049", 99, 1 },	/* wrong */
 /*  50 */ { "r050", 99, 1 },	/* wrong */
 /*  51 */ { "r051", 99, 1 },	/* wrong */
 /*  52 */ { "r052", 99, 1 },	/* wrong */
 /*  53 */ { "r053", 99, 1 },	/* wrong */
 /*  54 */ { "r054", 99, 1 },	/* wrong */
 /*  55 */ { "r055", 99, 1 },	/* wrong */
 /*  56 */ { "r056", 99, 1 },	/* wrong */
 /*  57 */ { "r057", 99, 1 },	/* wrong */
 /*  58 */ { "r058", 99, 1 },	/* wrong */
 /*  59 */ { "r059", 99, 1 },	/* wrong */
 /*  60 */ { "r060", 99, 1 },	/* wrong */
 /*  61 */ { "r061", 99, 1 },	/* wrong */
 /*  62 */ { "r062", 99, 1 },	/* wrong */
 /*  63 */ { "r063", 99, 1 },	/* wrong */
 /*  64 */ { "r064", 99, 1 },	/* wrong */
 /*  65 */ { "r065", 99, 1 },	/* wrong */
 /*  66 */ { "r066", 99, 1 },	/* wrong */
 /*  67 */ { "r067", 99, 1 },	/* wrong */
 /*  68 */ { "r068", 99, 1 },	/* wrong */
 /*  69 */ { "r069", 99, 1 },	/* wrong */
 /*  70 */ { "r070", 99, 1 },	/* wrong */
 /*  71 */ { "r071", 99, 1 },	/* wrong */
 /*  72 */ { "r072", 99, 1 },	/* wrong */
 /*  73 */ { "r073", 99, 1 },	/* wrong */
 /*  74 */ { "r074", 99, 1 },	/* wrong */
 /*  75 */ { "r075", 99, 1 },	/* wrong */
 /*  76 */ { "r076", 99, 1 },	/* wrong */
 /*  77 */ { "r077", 99, 1 },	/* wrong */
 /*  78 */ { "r078", 99, 1 },	/* wrong */
 /*  79 */ { "r079", 99, 1 },	/* wrong */
 /*  80 */ { "r080", 99, 1 },	/* wrong */
 /*  81 */ { "r081", 99, 1 },	/* wrong */
 /*  82 */ { "r082", 99, 1 },	/* wrong */
 /*  83 */ { "r083", 99, 1 },	/* wrong */
 /*  84 */ { "r084", 99, 1 },	/* wrong */
 /*  85 */ { "r085", 99, 1 },	/* wrong */
 /*  86 */ { "r086", 99, 1 },	/* wrong */
 /*  87 */ { "r087", 99, 1 },	/* wrong */
 /*  88 */ { "r088", 99, 1 },	/* wrong */
 /*  89 */ { "r089", 99, 1 },	/* wrong */
 /*  90 */ { "r090", 99, 1 },	/* wrong */
 /*  91 */ { "r091", 99, 1 },	/* wrong */
 /*  92 */ { "r092", 99, 1 },	/* wrong */
 /*  93 */ { "r093", 99, 1 },	/* wrong */
 /*  94 */ { "r094", 99, 1 },	/* wrong */
 /*  95 */ { "r095", 99, 1 },	/* wrong */
 /*  96 */ { "r096", 99, 1 },	/* wrong */
 /*  97 */ { "r097", 99, 1 },	/* wrong */
 /*  98 */ { "r098", 99, 1 },	/* wrong */
 /*  99 */ { "r099", 99, 1 },	/* wrong */
 /* 100 */ { "r100", 99, 1 },	/* wrong */
	// Characters
 /* 101 */ { "Parcs", 0, 0 },
 /* 102 */ { "Dawson", 0, 0 },
 /* 103 */ { "Huxley", 0, 0 },
 /* 104 */ { "Graywood", 0, 0 },
 /* 105 */ { "Hill", 0, 0 },
 /* 106 */ { "Malone", 0, 0 },
};

const int reasons_number = countof(reasons);

// End of emotions.c
