/*
 * emotions.cpp
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from Master.csv, Locations.csv, Lodges.csv, Characters.csv, Intentions.csv
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-10-23 21:04:17
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
 /*  0 */ { "fon", 1, ROOT, -1, -1 },
 /*  1 */     { "neverno", 1, 0, -1, -1 },
 /*  2 */     { "masterka", 1, 0, -1, -1 },
 /*  3 */     { "tishina", 1, 0, -1, -1 },
 /*  4 */     { "tuman", 1, 0, -1, -1 },
 /*  5 */     { "smert'", 1, 0, -1, -1 },
 /*  6 */     { "mertvyak", 1, 0, -1, -1 },
 /*  7 */     { "serdtsebienie", 1, 0, -1, -1 },
 /*  8 */     { "trip", 1, 0, -1, -1 },
 /*  9 */     { "kaktus", 1, 0, -1, -1 },
 /* 10 */     { "narkoman", 1, 0, -1, -1 },
 /* 11 */     { "les", 1, 0, -1, -1 },
 /* 12 */     { "negativ", 1, 0, -1, -1 },
 /* 13 */         { "ravnodushie", 1, 12, -1, -1 },
 /* 14 */             { "skuka", 1, 13, -1, -1 },
 /* 15 */                 { "apatiya", 1, 14, -1, -1 },
 /* 16 */                     { "depressiya", 1, 15, -1, -1 },
 /* 17 */         { "zlost'", 1, 12, -1, -1 },
 /* 18 */             { "gnev", 1, 17, -1, -1 },
 /* 19 */                 { "yarost'", 1, 18, -1, -1 },
 /* 20 */             { "razdrazhenie", 1, 17, -1, -1 },
 /* 21 */         { "grust'", 1, 12, -1, -1 },
 /* 22 */             { "toska", 1, 21, -1, -1 },
 /* 23 */                 { "obrechennost'", 1, 22, -1, -1 },
 /* 24 */                 { "otchayanie", 1, 22, -1, -1 },
 /* 25 */         { "neuverennost'", 1, 12, -1, -1 },
 /* 26 */             { "ozabochennost'", 1, 25, -1, -1 },
 /* 27 */                 { "napryazhennost'", 1, 26, -1, -1 },
 /* 28 */                     { "isterika", 1, 27, -1, -1 },
 /* 29 */             { "somnenie", 1, 25, -1, -1 },
 /* 30 */         { "trevozhnost'", 1, 12, -1, -1 },
 /* 31 */             { "strah", 1, 30, -1, -1 },
 /* 32 */                 { "uzhas", 1, 31, -1, -1 },
 /* 33 */                     { "panika", 1, 32, -1, -1 },
 /* 34 */                         { "fobiya", 1, 33, -1, -1 },
 /* 35 */                         { "paranojya", 1, 33, -1, -1 },
 /* 36 */         { "nepriyazn'", 1, 12, -1, -1 },
 /* 37 */             { "nenavist'", 1, 36, -1, -1 },
 /* 38 */             { "otvraschenie", 1, 36, -1, -1 },
 /* 39 */             { "prezrenie", 1, 36, -1, -1 },
 /* 40 */         { "neudovletvorennost'", 1, 12, -1, -1 },
 /* 41 */         { "sozhalenie", 1, 12, -1, -1 },
 /* 42 */             { "styid", 1, 41, -1, -1 },
 /* 43 */     { "razrushenie", 1, 0, -1, -1 },
 /* 44 */     { "draka", 1, 0, -1, -1 },
 /* 45 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 46 */     { "pozitiv", 1, 0, -1, -1 },
 /* 47 */         { "vesel'e", 1, 46, -1, -1 },
 /* 48 */             { "radost'", 1, 47, -1, -1 },
 /* 49 */                 { "vostorg", 1, 48, -1, -1 },
 /* 50 */         { "dovol'stvo", 1, 46, -1, -1 },
 /* 51 */             { "gordost'", 1, 50, -1, -1 },
 /* 52 */             { "udovletvorenie", 1, 50, -1, -1 },
 /* 53 */         { "lyubopyitstvo", 1, 46, -1, -1 },
 /* 54 */             { "interes", 1, 53, -1, -1 },
 /* 55 */             { "uvlechennost'", 1, 53, -1, -1 },
 /* 56 */                 { "azart", 1, 55, -1, -1 },
 /* 57 */                 { "voodushevlenie", 1, 55, -1, -1 },
 /* 58 */                     { "tseleustremlennost'", 1, 57, -1, -1 },
 /* 59 */                     { "otvaga", 1, 57, -1, -1 },
 /* 60 */                     { "reshimost'", 1, 57, -1, -1 },
 /* 61 */                 { "vdohnovenie", 1, 55, -1, -1 },
 /* 62 */         { "priyazn'", 1, 46, -1, -1 },
 /* 63 */             { "simpatiya", 1, 62, -1, -1 },
 /* 64 */                 { "privyazannost'", 1, 63, -1, -1 },
 /* 65 */                     { "lyubov'", 1, 64, -1, -1 },
 /* 66 */                 { "vozhdelenie", 1, 63, -1, -1 },
 /* 67 */             { "umilenie", 1, 62, -1, -1 },
 /* 68 */         { "nadezhda", 1, 46, -1, -1 },
 /* 69 */             { "predvkushenie", 1, 68, -1, -1 },
 /* 70 */                 { "vera", 1, 69, -1, -1 },
 /* 71 */                 { "uverennost'", 1, 69, -1, -1 },
 /* 72 */         { "pokoj", 1, 46, -1, -1 },
 /* 73 */             { "umirotvorenie", 1, 72, -1, -1 },
 /* 74 */             { "nostal'giya", 1, 72, -1, -1 },
 /* 75 */         { "soperezhivanie", 1, 46, -1, -1 },
 /* 76 */             { "sostradanie", 1, 75, -1, -1 },
 /* 77 */     { "sozidanie", 1, 0, -1, -1 },
 /* 78 */     { "seks", 1, 0, -1, -1 },
 /* 79 */     { "zhelanie", 1, 0, -1, -1 },
 /* 80 */         { "vlechenie", 1, 79, -1, -1 },
 /* 81 */             { "strast'", 1, 80, -1, -1 },
 /* 82 */                 { "oderzhimost'", 1, 81, -1, -1 },
 /* 83 */                     { "maniya", 1, 82, -1, -1 },
 /* 84 */         { "zainteresovannost'", 1, 79, -1, -1 },
 /* 85 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 86 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 87 */         { "trans", 1, 86, -1, -1 },
 /* 88 */         { "meditatsiya", 1, 86, -1, -1 },
 /* 89 */         { "ekzal'tatsiya", 1, 86, -1, -1 },
 /* 90 */         { "prosvetlenie", 1, 86, -1, -1 },
};

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "ZER0", 0, 0, 0 },         /* fon */
	// Master locations
 /*   1 */ { "hub", 0, 0, 0 },          /* fon */
 /*   2 */ { "master", 240, 0, 2 },     /* masterka */
 /*   3 */ { "dead", 240, 0, 6 },       /* mertvyak */
 /*   4 */ { "nofear", 0, 0, 0 },       /* fon */
 /*   5 */ { "reboot", 0, 0, 0 },       /* fon */
 /*   6 */ { "reset", 0, 0, 0 },        /* fon */
	// end of master locations
	// MOBs
 /*   7 */ { "Mob07", 0, 0, 0 },        /* fon */
 /*   8 */ { "Mob08", 0, 0, 0 },        /* fon */
 /*   9 */ { "Mob09", 0, 0, 0 },        /* fon */
 /*  10 */ { "Mob10", 0, 0, 0 },        /* fon */
	// end of MOBs
	// Placeholders
 /*  11 */ { "PH11", 0, 0, 0 },         /* fon */
 /*  12 */ { "PH12", 0, 0, 0 },         /* fon */
 /*  13 */ { "PH13", 0, 0, 0 },         /* fon */
 /*  14 */ { "PH14", 0, 0, 0 },         /* fon */
 /*  15 */ { "PH15", 0, 0, 0 },         /* fon */
 /*  16 */ { "PH16", 0, 0, 0 },         /* fon */
 /*  17 */ { "PH17", 0, 0, 0 },         /* fon */
 /*  18 */ { "PH18", 0, 0, 0 },         /* fon */
 /*  19 */ { "PH19", 0, 0, 0 },         /* fon */
 /*  20 */ { "PH20", 0, 0, 0 },         /* fon */
 /*  21 */ { "PH21", 0, 0, 0 },         /* fon */
 /*  22 */ { "PH22", 0, 0, 0 },         /* fon */
 /*  23 */ { "PH23", 0, 0, 0 },         /* fon */
 /*  24 */ { "PH24", 0, 0, 0 },         /* fon */
 /*  25 */ { "PH25", 0, 0, 0 },         /* fon */
 /*  26 */ { "PH26", 0, 0, 0 },         /* fon */
 /*  27 */ { "PH27", 0, 0, 0 },         /* fon */
 /*  28 */ { "PH28", 0, 0, 0 },         /* fon */
 /*  29 */ { "PH29", 0, 0, 0 },         /* fon */
 /*  30 */ { "PH30", 0, 0, 0 },         /* fon */
 /*  31 */ { "PH31", 0, 0, 0 },         /* fon */
 /*  32 */ { "PH32", 0, 0, 0 },         /* fon */
 /*  33 */ { "PH33", 0, 0, 0 },         /* fon */
 /*  34 */ { "PH34", 0, 0, 0 },         /* fon */
 /*  35 */ { "PH35", 0, 0, 0 },         /* fon */
 /*  36 */ { "PH36", 0, 0, 0 },         /* fon */
 /*  37 */ { "PH37", 0, 0, 0 },         /* fon */
 /*  38 */ { "PH38", 0, 0, 0 },         /* fon */
 /*  39 */ { "PH39", 0, 0, 0 },         /* fon */
 /*  40 */ { "PH40", 0, 0, 0 },         /* fon */
	// end of placeholders
	// City locations
 /*  41 */ { "hermit", 40, 0, 73 },     /* umirotvorenie */
 /*  42 */ { "square", 40, 0, 61 },     /* vdohnovenie */
 /*  43 */ { "factory", 40, 0, 14 },    /* skuka */
 /*  44 */ { "cityhall", 40, 0, 51 },   /* gordost' */
 /*  45 */ { "station", 40, 0, 69 },    /* predvkushenie */
 /*  46 */ { "bar", 40, 0, 47 },        /* vesel'e */
 /*  47 */ { "coffee", 40, 0, 54 },     /* interes */
	// end of city locations
	// Forest A lights
 /*  48 */ { "FA48", 40, 0, 11 },       /* les */
 /*  49 */ { "FA49", 40, 0, 11 },       /* les */
 /*  50 */ { "FA50", 40, 0, 11 },       /* les */
 /*  51 */ { "FA51", 40, 0, 11 },       /* les */
 /*  52 */ { "FA52", 40, 0, 11 },       /* les */
 /*  53 */ { "FA53", 40, 0, 11 },       /* les */
 /*  54 */ { "FA54", 40, 0, 11 },       /* les */
 /*  55 */ { "FA55", 40, 0, 11 },       /* les */
 /*  56 */ { "FA56", 40, 0, 11 },       /* les */
 /*  57 */ { "FA57", 40, 0, 11 },       /* les */
 /*  58 */ { "FA58", 40, 0, 11 },       /* les */
 /*  59 */ { "FA59", 40, 0, 11 },       /* les */
 /*  60 */ { "FA60", 40, 0, 11 },       /* les */
 /*  61 */ { "FA61", 40, 0, 11 },       /* les */
 /*  62 */ { "FA62", 40, 0, 11 },       /* les */
 /*  63 */ { "FA63", 40, 0, 11 },       /* les */
 /*  64 */ { "FA64", 40, 0, 11 },       /* les */
 /*  65 */ { "FA65", 40, 0, 11 },       /* les */
 /*  66 */ { "FA66", 40, 0, 11 },       /* les */
 /*  67 */ { "FA67", 40, 0, 11 },       /* les */
 /*  68 */ { "FA68", 40, 0, 11 },       /* les */
 /*  69 */ { "FA69", 40, 0, 11 },       /* les */
 /*  70 */ { "FA70", 40, 0, 11 },       /* les */
 /*  71 */ { "FA71", 40, 0, 11 },       /* les */
 /*  72 */ { "FA72", 40, 0, 11 },       /* les */
 /*  73 */ { "FA73", 40, 0, 11 },       /* les */
 /*  74 */ { "FA74", 40, 0, 11 },       /* les */
 /*  75 */ { "FA75", 40, 0, 11 },       /* les */
 /*  76 */ { "FA76", 40, 0, 11 },       /* les */
 /*  77 */ { "FA77", 40, 0, 11 },       /* les */
 /*  78 */ { "FA78", 40, 0, 11 },       /* les */
 /*  79 */ { "FA79", 40, 0, 11 },       /* les */
 /*  80 */ { "FA80", 40, 0, 11 },       /* les */
 /*  81 */ { "FA81", 40, 0, 11 },       /* les */
 /*  82 */ { "FA82", 40, 0, 11 },       /* les */
 /*  83 */ { "FA83", 40, 0, 11 },       /* les */
 /*  84 */ { "FA84", 40, 0, 11 },       /* les */
 /*  85 */ { "FA85", 40, 0, 11 },       /* les */
 /*  86 */ { "FA86", 40, 0, 11 },       /* les */
 /*  87 */ { "FA87", 40, 0, 11 },       /* les */
 /*  88 */ { "FA88", 40, 0, 11 },       /* les */
 /*  89 */ { "FA89", 40, 0, 11 },       /* les */
	// end of forest A lights
	// Lodge locations
 /*  90 */ { "detskaya", 40, 0, 74 },   /* nostal'giya */
 /*  91 */ { "tent", 40, 0, 16 },       /* depressiya */
 /*  92 */ { "hospital", 40, 0, 23 },   /* obrechennost' */
 /*  93 */ { "university", 40, 0, 66 }, /* vozhdelenie */
 /*  94 */ { "redroom", 40, 0, 87 },    /* trans */
 /*  95 */ { "shop", 40, 0, 40 },       /* neudovletvorennost' */
 /*  96 */ { "blackhouse", 40, 0, 34 }, /* fobiya */
 /*  97 */ { "home", 40, 0, 67 },       /* umilenie */
 /*  98 */ { "trench", 40, 0, 90 },     /* prosvetlenie */
 /*  99 */ { "cinema", 40, 0, 54 },     /* interes */
	// end of lodge locations
	// Emotion fixes
 /* 100 */ { "xFon", 40, 0, 0 },        /* fon */
 /* 101 */ { "xNeverno", 40, 0, 1 },    /* neverno */
 /* 102 */ { "xMasterka", 40, 0, 2 },   /* masterka */
 /* 103 */ { "xTishina", 40, 0, 3 },    /* tishina */
 /* 104 */ { "xTuman", 40, 0, 4 },      /* tuman */
 /* 105 */ { "xSmert'", 40, 0, 5 },     /* smert' */
 /* 106 */ { "xMertvyak", 40, 0, 6 },   /* mertvyak */
 /* 107 */ { "xSerdtsebienie", 40, 0, 7 }, /* serdtsebienie */
 /* 108 */ { "xTrip", 40, 0, 8 },       /* trip */
 /* 109 */ { "xKaktus", 40, 0, 9 },     /* kaktus */
 /* 110 */ { "xNarkoman", 40, 0, 10 },  /* narkoman */
 /* 111 */ { "xLes", 40, 0, 11 },       /* les */
 /* 112 */ { "xNegativ", 40, 0, 12 },   /* negativ */
 /* 113 */ { "xRavnodushie", 40, 0, 13 }, /* ravnodushie */
 /* 114 */ { "xSkuka", 40, 0, 14 },     /* skuka */
 /* 115 */ { "xApatiya", 40, 0, 15 },   /* apatiya */
 /* 116 */ { "xDepressiya", 40, 0, 16 }, /* depressiya */
 /* 117 */ { "xZlost'", 40, 0, 17 },    /* zlost' */
 /* 118 */ { "xGnev", 40, 0, 18 },      /* gnev */
 /* 119 */ { "xYarost'", 40, 0, 19 },   /* yarost' */
 /* 120 */ { "xRazdrazhenie", 40, 0, 20 }, /* razdrazhenie */
 /* 121 */ { "xGrust'", 40, 0, 21 },    /* grust' */
 /* 122 */ { "xToska", 40, 0, 22 },     /* toska */
 /* 123 */ { "xObrechennost'", 40, 0, 23 }, /* obrechennost' */
 /* 124 */ { "xOtchayanie", 40, 0, 24 }, /* otchayanie */
 /* 125 */ { "xNeuverennost'", 40, 0, 25 }, /* neuverennost' */
 /* 126 */ { "xOzabochennost'", 40, 0, 26 }, /* ozabochennost' */
 /* 127 */ { "xNapryazhennost'", 40, 0, 27 }, /* napryazhennost' */
 /* 128 */ { "xIsterika", 40, 0, 28 },  /* isterika */
 /* 129 */ { "xSomnenie", 40, 0, 29 },  /* somnenie */
 /* 130 */ { "xTrevozhnost'", 40, 0, 30 }, /* trevozhnost' */
 /* 131 */ { "xStrah", 40, 0, 31 },     /* strah */
 /* 132 */ { "xUzhas", 40, 0, 32 },     /* uzhas */
 /* 133 */ { "xPanika", 40, 0, 33 },    /* panika */
 /* 134 */ { "xFobiya", 40, 0, 34 },    /* fobiya */
 /* 135 */ { "xParanojya", 40, 0, 35 }, /* paranojya */
 /* 136 */ { "xNepriyazn'", 40, 0, 36 }, /* nepriyazn' */
 /* 137 */ { "xNenavist'", 40, 0, 37 }, /* nenavist' */
 /* 138 */ { "xOtvraschenie", 40, 0, 38 }, /* otvraschenie */
 /* 139 */ { "xPrezrenie", 40, 0, 39 }, /* prezrenie */
 /* 140 */ { "xNeudovletvorennost'", 40, 0, 40 }, /* neudovletvorennost' */
 /* 141 */ { "xSozhalenie", 40, 0, 41 }, /* sozhalenie */
 /* 142 */ { "xStyid", 40, 0, 42 },     /* styid */
 /* 143 */ { "xRazrushenie", 40, 0, 43 }, /* razrushenie */
 /* 144 */ { "xDraka", 40, 0, 44 },     /* draka */
 /* 145 */ { "xUbijstvo", 40, 0, 45 },  /* ubijstvo */
 /* 146 */ { "xPozitiv", 40, 0, 46 },   /* pozitiv */
 /* 147 */ { "xVesel'e", 40, 0, 47 },   /* vesel'e */
 /* 148 */ { "xRadost'", 40, 0, 48 },   /* radost' */
 /* 149 */ { "xVostorg", 40, 0, 49 },   /* vostorg */
 /* 150 */ { "xDovol'stvo", 40, 0, 50 }, /* dovol'stvo */
 /* 151 */ { "xGordost'", 40, 0, 51 },  /* gordost' */
 /* 152 */ { "xUdovletvorenie", 40, 0, 52 }, /* udovletvorenie */
 /* 153 */ { "xLyubopyitstvo", 40, 0, 53 }, /* lyubopyitstvo */
 /* 154 */ { "xInteres", 40, 0, 54 },   /* interes */
 /* 155 */ { "xUvlechennost'", 40, 0, 55 }, /* uvlechennost' */
 /* 156 */ { "xAzart", 40, 0, 56 },     /* azart */
 /* 157 */ { "xVoodushevlenie", 40, 0, 57 }, /* voodushevlenie */
 /* 158 */ { "xTseleustremlennost'", 40, 0, 58 }, /* tseleustremlennost' */
 /* 159 */ { "xOtvaga", 40, 0, 59 },    /* otvaga */
 /* 160 */ { "xReshimost'", 40, 0, 60 }, /* reshimost' */
 /* 161 */ { "xVdohnovenie", 40, 0, 61 }, /* vdohnovenie */
 /* 162 */ { "xPriyazn'", 40, 0, 62 },  /* priyazn' */
 /* 163 */ { "xSimpatiya", 40, 0, 63 }, /* simpatiya */
 /* 164 */ { "xPrivyazannost'", 40, 0, 64 }, /* privyazannost' */
 /* 165 */ { "xLyubov'", 40, 0, 65 },   /* lyubov' */
 /* 166 */ { "xVozhdelenie", 40, 0, 66 }, /* vozhdelenie */
 /* 167 */ { "xUmilenie", 40, 0, 67 },  /* umilenie */
 /* 168 */ { "xNadezhda", 40, 0, 68 },  /* nadezhda */
 /* 169 */ { "xPredvkushenie", 40, 0, 69 }, /* predvkushenie */
 /* 170 */ { "xVera", 40, 0, 70 },      /* vera */
 /* 171 */ { "xUverennost'", 40, 0, 71 }, /* uverennost' */
 /* 172 */ { "xPokoj", 40, 0, 72 },     /* pokoj */
 /* 173 */ { "xUmirotvorenie", 40, 0, 73 }, /* umirotvorenie */
 /* 174 */ { "xNostal'giya", 40, 0, 74 }, /* nostal'giya */
 /* 175 */ { "xSoperezhivanie", 40, 0, 75 }, /* soperezhivanie */
 /* 176 */ { "xSostradanie", 40, 0, 76 }, /* sostradanie */
 /* 177 */ { "xSozidanie", 40, 0, 77 }, /* sozidanie */
 /* 178 */ { "xSeks", 40, 0, 78 },      /* seks */
 /* 179 */ { "xZhelanie", 40, 0, 79 },  /* zhelanie */
 /* 180 */ { "xVlechenie", 40, 0, 80 }, /* vlechenie */
 /* 181 */ { "xStrast'", 40, 0, 81 },   /* strast' */
 /* 182 */ { "xOderzhimost'", 40, 0, 82 }, /* oderzhimost' */
 /* 183 */ { "xManiya", 40, 0, 83 },    /* maniya */
 /* 184 */ { "xZainteresovannost'", 40, 0, 84 }, /* zainteresovannost' */
 /* 185 */ { "xZavisimost'", 40, 0, 85 }, /* zavisimost' */
 /* 186 */ { "xDuhovnoe", 40, 0, 86 },  /* duhovnoe */
 /* 187 */ { "xTrans", 40, 0, 87 },     /* trans */
 /* 188 */ { "xMeditatsiya", 40, 0, 88 }, /* meditatsiya */
 /* 189 */ { "xEkzal'tatsiya", 40, 0, 89 }, /* ekzal'tatsiya */
 /* 190 */ { "xProsvetlenie", 40, 0, 90 }, /* prosvetlenie */
	// end of emotion fixes
	// Mist sources
 /* 191 */ { "M191", 0, 0, 0 },         /* fon */
 /* 192 */ { "M192", 0, 0, 0 },         /* fon */
 /* 193 */ { "M193", 0, 0, 0 },         /* fon */
 /* 194 */ { "M194", 0, 0, 0 },         /* fon */
 /* 195 */ { "M195", 0, 0, 0 },         /* fon */
 /* 196 */ { "M196", 0, 0, 0 },         /* fon */
 /* 197 */ { "M197", 0, 0, 0 },         /* fon */
 /* 198 */ { "M198", 0, 0, 0 },         /* fon */
 /* 199 */ { "M199", 0, 0, 0 },         /* fon */
 /* 200 */ { "M200", 0, 0, 0 },         /* fon */
	// end of mist sources
	// Characters
 /* 201 */ { "PAdams", 0, 0, 0 },
 /* 202 */ { "DAllgood", 0, 0, 0 },
	// end of characters
 /* 203 */ { "R203", 0, 0, 0 },         /* fon */
 /* 204 */ { "R204", 0, 0, 0 },         /* fon */
 /* 205 */ { "R205", 0, 0, 0 },         /* fon */
 /* 206 */ { "R206", 0, 0, 0 },         /* fon */
 /* 207 */ { "R207", 0, 0, 0 },         /* fon */
 /* 208 */ { "R208", 0, 0, 0 },         /* fon */
 /* 209 */ { "R209", 0, 0, 0 },         /* fon */
 /* 210 */ { "R210", 0, 0, 0 },         /* fon */
 /* 211 */ { "R211", 0, 0, 0 },         /* fon */
 /* 212 */ { "R212", 0, 0, 0 },         /* fon */
 /* 213 */ { "R213", 0, 0, 0 },         /* fon */
 /* 214 */ { "R214", 0, 0, 0 },         /* fon */
 /* 215 */ { "R215", 0, 0, 0 },         /* fon */
 /* 216 */ { "R216", 0, 0, 0 },         /* fon */
 /* 217 */ { "R217", 0, 0, 0 },         /* fon */
 /* 218 */ { "R218", 0, 0, 0 },         /* fon */
 /* 219 */ { "R219", 0, 0, 0 },         /* fon */
 /* 220 */ { "R220", 0, 0, 0 },         /* fon */
 /* 221 */ { "R221", 0, 0, 0 },         /* fon */
 /* 222 */ { "R222", 0, 0, 0 },         /* fon */
 /* 223 */ { "R223", 0, 0, 0 },         /* fon */
 /* 224 */ { "R224", 0, 0, 0 },         /* fon */
 /* 225 */ { "R225", 0, 0, 0 },         /* fon */
 /* 226 */ { "R226", 0, 0, 0 },         /* fon */
 /* 227 */ { "R227", 0, 0, 0 },         /* fon */
 /* 228 */ { "R228", 0, 0, 0 },         /* fon */
 /* 229 */ { "R229", 0, 0, 0 },         /* fon */
 /* 230 */ { "R230", 0, 0, 0 },         /* fon */
 /* 231 */ { "R231", 0, 0, 0 },         /* fon */
 /* 232 */ { "R232", 0, 0, 0 },         /* fon */
 /* 233 */ { "R233", 0, 0, 0 },         /* fon */
 /* 234 */ { "R234", 0, 0, 0 },         /* fon */
 /* 235 */ { "R235", 0, 0, 0 },         /* fon */
 /* 236 */ { "R236", 0, 0, 0 },         /* fon */
 /* 237 */ { "R237", 0, 0, 0 },         /* fon */
 /* 238 */ { "R238", 0, 0, 0 },         /* fon */
 /* 239 */ { "R239", 0, 0, 0 },         /* fon */
 /* 240 */ { "R240", 0, 0, 0 },         /* fon */
 /* 241 */ { "R241", 0, 0, 0 },         /* fon */
 /* 242 */ { "R242", 0, 0, 0 },         /* fon */
 /* 243 */ { "R243", 0, 0, 0 },         /* fon */
 /* 244 */ { "R244", 0, 0, 0 },         /* fon */
 /* 245 */ { "R245", 0, 0, 0 },         /* fon */
 /* 246 */ { "R246", 0, 0, 0 },         /* fon */
 /* 247 */ { "R247", 0, 0, 0 },         /* fon */
 /* 248 */ { "R248", 0, 0, 0 },         /* fon */
 /* 249 */ { "R249", 0, 0, 0 },         /* fon */
 /* 250 */ { "R250", 0, 0, 0 },         /* fon */
 /* 251 */ { "R251", 0, 0, 0 },         /* fon */
 /* 252 */ { "R252", 0, 0, 0 },         /* fon */
 /* 253 */ { "R253", 0, 0, 0 },         /* fon */
 /* 254 */ { "R254", 0, 0, 0 },         /* fon */
 /* 255 */ { "R255", 0, 0, 0 },         /* fon */
 /* 256 */ { "R256", 0, 0, 0 },         /* fon */
 /* 257 */ { "R257", 0, 0, 0 },         /* fon */
 /* 258 */ { "R258", 0, 0, 0 },         /* fon */
 /* 259 */ { "R259", 0, 0, 0 },         /* fon */
 /* 260 */ { "R260", 0, 0, 0 },         /* fon */
 /* 261 */ { "R261", 0, 0, 0 },         /* fon */
 /* 262 */ { "R262", 0, 0, 0 },         /* fon */
 /* 263 */ { "R263", 0, 0, 0 },         /* fon */
 /* 264 */ { "R264", 0, 0, 0 },         /* fon */
 /* 265 */ { "R265", 0, 0, 0 },         /* fon */
 /* 266 */ { "R266", 0, 0, 0 },         /* fon */
 /* 267 */ { "R267", 0, 0, 0 },         /* fon */
 /* 268 */ { "R268", 0, 0, 0 },         /* fon */
 /* 269 */ { "R269", 0, 0, 0 },         /* fon */
 /* 270 */ { "R270", 0, 0, 0 },         /* fon */
 /* 271 */ { "R271", 0, 0, 0 },         /* fon */
 /* 272 */ { "R272", 0, 0, 0 },         /* fon */
 /* 273 */ { "R273", 0, 0, 0 },         /* fon */
 /* 274 */ { "R274", 0, 0, 0 },         /* fon */
 /* 275 */ { "R275", 0, 0, 0 },         /* fon */
 /* 276 */ { "R276", 0, 0, 0 },         /* fon */
 /* 277 */ { "R277", 0, 0, 0 },         /* fon */
 /* 278 */ { "R278", 0, 0, 0 },         /* fon */
 /* 279 */ { "R279", 0, 0, 0 },         /* fon */
	// Forest BC lights
 /* 280 */ { "FB280", 40, 0, 11 },      /* les */
 /* 281 */ { "FC281", 40, 0, 11 },      /* les */
 /* 282 */ { "FB282", 40, 0, 11 },      /* les */
 /* 283 */ { "FC283", 40, 0, 11 },      /* les */
 /* 284 */ { "FB284", 40, 0, 11 },      /* les */
 /* 285 */ { "FC285", 40, 0, 11 },      /* les */
 /* 286 */ { "FB286", 40, 0, 11 },      /* les */
 /* 287 */ { "FC287", 40, 0, 11 },      /* les */
 /* 288 */ { "FB288", 40, 0, 11 },      /* les */
 /* 289 */ { "FC289", 40, 0, 11 },      /* les */
 /* 290 */ { "FB290", 40, 0, 11 },      /* les */
 /* 291 */ { "FC291", 40, 0, 11 },      /* les */
 /* 292 */ { "FB292", 40, 0, 11 },      /* les */
 /* 293 */ { "FC293", 40, 0, 11 },      /* les */
 /* 294 */ { "FB294", 40, 0, 11 },      /* les */
 /* 295 */ { "FC295", 40, 0, 11 },      /* les */
 /* 296 */ { "FB296", 40, 0, 11 },      /* les */
 /* 297 */ { "FC297", 40, 0, 11 },      /* les */
 /* 298 */ { "FB298", 40, 0, 11 },      /* les */
 /* 299 */ { "FC299", 40, 0, 11 },      /* les */
	// end of forest BC lights
	// Intentions
 /* 300 */ { "murder", 120, 0, 45 },    /* ubijstvo */
 /* 301 */ { "creation", 120, 0, 77 },  /* sozidanie */
 /* 302 */ { "destruction", 120, 0, 43 }, /* razrushenie */
 /* 303 */ { "sex", 130, 0, 78 },       /* seks */
 /* 304 */ { "fight", 0, 0, 44 },       /* draka */
 /* 305 */ { "heartbeat", 0, 0, 7 },    /* serdtsebienie */
 /* 306 */ { "addiction", 155, 0, 85 }, /* zavisimost' */
 /* 307 */ { "weed", 160, 0, 77 },      /* sozidanie */
 /* 308 */ { "lsd", 160, 0, 8 },        /* trip */
 /* 309 */ { "peyote", 160, 0, 9 },     /* kaktus */
 /* 310 */ { "heroin", 160, 0, 43 },    /* razrushenie */
 /* 311 */ { "krayk", 0, 0, 0 },        /* fon */
 /* 312 */ { "death", 280, 0, 5 },      /* smert' */
 /* 313 */ { "mist", 200, 0, 4 },       /* tuman */
 /* 314 */ { "fear", 199, 0, 31 },      /* strah */
 /* 315 */ { "mSource", 0, 0, 1 },      /* neverno */
 /* 316 */ { "mProject", 0, 0, 1 },     /* neverno */
 /* 317 */ { "bg", 40, 0, 0 },          /* fon */
	// end of intentions
};

// End of emotions.c
