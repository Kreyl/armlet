/*
 * emotions.cpp
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from Settings.csv, Emotions.csv, Locations.csv, Characters.csv and Intentions.csv
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-09-03 02:21:35
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
 /*  9 */     { "les", 1, 0, -1, -1 },
 /* 10 */     { "negativ", 1, 0, -1, -1 },
 /* 11 */         { "ravnodushie", 1, 10, -1, -1 },
 /* 12 */             { "skuka", 1, 11, -1, -1 },
 /* 13 */                 { "apatiya", 1, 12, -1, -1 },
 /* 14 */                     { "depressiya", 1, 13, -1, -1 },
 /* 15 */         { "zlost'", 1, 10, -1, -1 },
 /* 16 */             { "gnev", 1, 15, -1, -1 },
 /* 17 */                 { "yarost'", 1, 16, -1, -1 },
 /* 18 */             { "razdrazhenie", 1, 15, -1, -1 },
 /* 19 */         { "grust'", 1, 10, -1, -1 },
 /* 20 */             { "toska", 1, 19, -1, -1 },
 /* 21 */                 { "obrechennost'", 1, 20, -1, -1 },
 /* 22 */                 { "otchayanie", 1, 20, -1, -1 },
 /* 23 */         { "neuverennost'", 1, 10, -1, -1 },
 /* 24 */             { "ozabochennost'", 1, 23, -1, -1 },
 /* 25 */                 { "napryazhennost'", 1, 24, -1, -1 },
 /* 26 */                     { "isterika", 1, 25, -1, -1 },
 /* 27 */             { "somnenie", 1, 23, -1, -1 },
 /* 28 */         { "trevozhnost'", 1, 10, -1, -1 },
 /* 29 */             { "strah", 1, 28, -1, -1 },
 /* 30 */                 { "uzhas", 1, 29, -1, -1 },
 /* 31 */                     { "panika", 1, 30, -1, -1 },
 /* 32 */                         { "fobiya", 1, 31, -1, -1 },
 /* 33 */                         { "paranojya", 1, 31, -1, -1 },
 /* 34 */         { "nepriyazn'", 1, 10, -1, -1 },
 /* 35 */             { "nenavist'", 1, 34, -1, -1 },
 /* 36 */             { "otvraschenie", 1, 34, -1, -1 },
 /* 37 */             { "prezrenie", 1, 34, -1, -1 },
 /* 38 */         { "neudovletvorennost'", 1, 10, -1, -1 },
 /* 39 */         { "sozhalenie", 1, 10, -1, -1 },
 /* 40 */             { "styid", 1, 39, -1, -1 },
 /* 41 */     { "razrushenie", 1, 0, -1, -1 },
 /* 42 */     { "draka", 1, 0, -1, -1 },
 /* 43 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 44 */     { "pozitiv", 1, 0, -1, -1 },
 /* 45 */         { "vesel'e", 1, 44, -1, -1 },
 /* 46 */             { "radost'", 1, 45, -1, -1 },
 /* 47 */                 { "vostorg", 1, 46, -1, -1 },
 /* 48 */         { "dovol'stvo", 1, 44, -1, -1 },
 /* 49 */             { "gordost'", 1, 48, -1, -1 },
 /* 50 */             { "udovletvorenie", 1, 48, -1, -1 },
 /* 51 */         { "lyubopyitstvo", 1, 44, -1, -1 },
 /* 52 */             { "interes", 1, 51, -1, -1 },
 /* 53 */             { "uvlechennost'", 1, 51, -1, -1 },
 /* 54 */                 { "azart", 1, 53, -1, -1 },
 /* 55 */                 { "voodushevlenie", 1, 53, -1, -1 },
 /* 56 */                     { "tseleustremlennost'", 1, 55, -1, -1 },
 /* 57 */                     { "otvaga", 1, 55, -1, -1 },
 /* 58 */                     { "reshimost'", 1, 55, -1, -1 },
 /* 59 */                 { "vdohnovenie", 1, 53, -1, -1 },
 /* 60 */         { "priyazn'", 1, 44, -1, -1 },
 /* 61 */             { "simpatiya", 1, 60, -1, -1 },
 /* 62 */                 { "privyazannost'", 1, 61, -1, -1 },
 /* 63 */                     { "lyubov'", 1, 62, -1, -1 },
 /* 64 */                 { "vozhdelenie", 1, 61, -1, -1 },
 /* 65 */             { "umilenie", 1, 60, -1, -1 },
 /* 66 */         { "nadezhda", 1, 44, -1, -1 },
 /* 67 */             { "predvkushenie", 1, 66, -1, -1 },
 /* 68 */                 { "vera", 1, 67, -1, -1 },
 /* 69 */                 { "uverennost'", 1, 67, -1, -1 },
 /* 70 */         { "pokoj", 1, 44, -1, -1 },
 /* 71 */             { "umirotvorenie", 1, 70, -1, -1 },
 /* 72 */             { "nostal'giya", 1, 70, -1, -1 },
 /* 73 */         { "soperezhivanie", 1, 44, -1, -1 },
 /* 74 */             { "sostradanie", 1, 73, -1, -1 },
 /* 75 */     { "sozidanie", 1, 0, -1, -1 },
 /* 76 */     { "seks", 1, 0, -1, -1 },
 /* 77 */     { "zhelanie", 1, 0, -1, -1 },
 /* 78 */         { "vlechenie", 1, 77, -1, -1 },
 /* 79 */             { "strast'", 1, 78, -1, -1 },
 /* 80 */                 { "oderzhimost'", 1, 79, -1, -1 },
 /* 81 */                     { "maniya", 1, 80, -1, -1 },
 /* 82 */         { "zainteresovannost'", 1, 77, -1, -1 },
 /* 83 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 84 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 85 */         { "trans", 1, 84, -1, -1 },
 /* 86 */         { "meditatsiya", 1, 84, -1, -1 },
 /* 87 */         { "ekzal'tatsiya", 1, 84, -1, -1 },
 /* 88 */         { "prosvetlenie", 1, 84, -1, -1 },
};

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "R000", 0, 0, 1 },         /* neverno */
	// Locations
 /*   1 */ { "hub", 240, 0, 2 },        /* masterka */
 /*   2 */ { "dead", 240, 0, 6 },       /* mertvyak */
 /*   3 */ { "hermit", 40, 0, 71 },     /* umirotvorenie */
 /*   4 */ { "square", 40, 0, 59 },     /* vdohnovenie */
 /*   5 */ { "factory", 40, 0, 12 },    /* skuka */
 /*   6 */ { "cityhall", 40, 0, 49 },   /* gordost' */
 /*   7 */ { "station", 40, 0, 67 },    /* predvkushenie */
 /*   8 */ { "bar", 40, 0, 45 },        /* vesel'e */
 /*   9 */ { "coffee", 40, 0, 52 },     /* interes */
 /*  10 */ { "detskaya", 40, 0, 72 },   /* nostal'giya */
 /*  11 */ { "tent", 40, 0, 14 },       /* depressiya */
 /*  12 */ { "hospital", 40, 0, 21 },   /* obrechennost' */
 /*  13 */ { "university", 40, 0, 64 }, /* vozhdelenie */
 /*  14 */ { "redroom", 40, 0, 85 },    /* trans */
 /*  15 */ { "shop", 40, 0, 38 },       /* neudovletvorennost' */
 /*  16 */ { "blackhouse", 40, 0, 32 }, /* fobiya */
 /*  17 */ { "home", 40, 0, 65 },       /* umilenie */
 /*  18 */ { "trench", 40, 0, 88 },     /* prosvetlenie */
 /*  19 */ { "cinema", 40, 0, 52 },     /* interes */
	// end of locations
 /*  20 */ { "fon_tech", 0, 0, 0 },         /* neverno */
	// Forest
 /*  21 */ { "FA21", 40, 0, 9 },        /* les */
 /*  22 */ { "FA22", 40, 0, 9 },        /* les */
 /*  23 */ { "FA23", 40, 0, 9 },        /* les */
 /*  24 */ { "FA24", 40, 0, 9 },        /* les */
 /*  25 */ { "FA25", 40, 0, 9 },        /* les */
 /*  26 */ { "FA26", 40, 0, 9 },        /* les */
 /*  27 */ { "FA27", 40, 0, 9 },        /* les */
 /*  28 */ { "FA28", 40, 0, 9 },        /* les */
 /*  29 */ { "FA29", 40, 0, 9 },        /* les */
 /*  30 */ { "FA30", 40, 0, 9 },        /* les */
 /*  31 */ { "FA31", 40, 0, 9 },        /* les */
 /*  32 */ { "FA32", 40, 0, 9 },        /* les */
 /*  33 */ { "FA33", 40, 0, 9 },        /* les */
 /*  34 */ { "FA34", 40, 0, 9 },        /* les */
 /*  35 */ { "FA35", 40, 0, 9 },        /* les */
 /*  36 */ { "FA36", 40, 0, 9 },        /* les */
 /*  37 */ { "FA37", 40, 0, 9 },        /* les */
 /*  38 */ { "FA38", 40, 0, 9 },        /* les */
 /*  39 */ { "FA39", 40, 0, 9 },        /* les */
 /*  40 */ { "FA40", 40, 0, 9 },        /* les */
 /*  41 */ { "FA41", 40, 0, 9 },        /* les */
 /*  42 */ { "FA42", 40, 0, 9 },        /* les */
 /*  43 */ { "FA43", 40, 0, 9 },        /* les */
 /*  44 */ { "FA44", 40, 0, 9 },        /* les */
 /*  45 */ { "FA45", 40, 0, 9 },        /* les */
 /*  46 */ { "FA46", 40, 0, 9 },        /* les */
 /*  47 */ { "FA47", 40, 0, 9 },        /* les */
 /*  48 */ { "FA48", 40, 0, 9 },        /* les */
 /*  49 */ { "FA49", 40, 0, 9 },        /* les */
 /*  50 */ { "FA50", 40, 0, 9 },        /* les */
 /*  51 */ { "FA51", 40, 0, 9 },        /* les */
 /*  52 */ { "FA52", 40, 0, 9 },        /* les */
 /*  53 */ { "FA53", 40, 0, 9 },        /* les */
 /*  54 */ { "FA54", 40, 0, 9 },        /* les */
 /*  55 */ { "FA55", 40, 0, 9 },        /* les */
 /*  56 */ { "FB56", 40, 0, 9 },        /* les */
 /*  57 */ { "FC57", 40, 0, 9 },        /* les */
 /*  58 */ { "FB58", 40, 0, 9 },        /* les */
 /*  59 */ { "FC59", 40, 0, 9 },        /* les */
 /*  60 */ { "FB60", 40, 0, 9 },        /* les */
 /*  61 */ { "FC61", 40, 0, 9 },        /* les */
 /*  62 */ { "FB62", 40, 0, 9 },        /* les */
 /*  63 */ { "FC63", 40, 0, 9 },        /* les */
 /*  64 */ { "FB64", 40, 0, 9 },        /* les */
 /*  65 */ { "FC65", 40, 0, 9 },        /* les */
 /*  66 */ { "FB66", 40, 0, 9 },        /* les */
 /*  67 */ { "FC67", 40, 0, 9 },        /* les */
 /*  68 */ { "FB68", 40, 0, 9 },        /* les */
 /*  69 */ { "FC69", 40, 0, 9 },        /* les */
 /*  70 */ { "FB70", 40, 0, 9 },        /* les */
 /*  71 */ { "FC71", 40, 0, 9 },        /* les */
 /*  72 */ { "FB72", 40, 0, 9 },        /* les */
 /*  73 */ { "FC73", 40, 0, 9 },        /* les */
 /*  74 */ { "FB74", 40, 0, 9 },        /* les */
 /*  75 */ { "FC75", 40, 0, 9 },        /* les */
 /*  76 */ { "FB76", 40, 0, 9 },        /* les */
 /*  77 */ { "FC77", 40, 0, 9 },        /* les */
 /*  78 */ { "FB78", 40, 0, 9 },        /* les */
 /*  79 */ { "FC79", 40, 0, 9 },        /* les */
 /*  80 */ { "FB80", 40, 0, 9 },        /* les */
 /*  81 */ { "FC81", 40, 0, 9 },        /* les */
 /*  82 */ { "FB82", 40, 0, 9 },        /* les */
 /*  83 */ { "FC83", 40, 0, 9 },        /* les */
 /*  84 */ { "FB84", 40, 0, 9 },        /* les */
 /*  85 */ { "FC85", 40, 0, 9 },        /* les */
 /*  86 */ { "FB86", 40, 0, 9 },        /* les */
 /*  87 */ { "FC87", 40, 0, 9 },        /* les */
 /*  88 */ { "FB88", 40, 0, 9 },        /* les */
 /*  89 */ { "FC89", 40, 0, 9 },        /* les */
	// end of forest

	// Mist sources
 /*  90 */ { "M90", 0, 0, 0 },          /* fon */
 /*  91 */ { "M91", 0, 0, 0 },          /* fon */
 /*  92 */ { "M92", 0, 0, 0 },          /* fon */
 /*  93 */ { "M93", 0, 0, 0 },          /* fon */
 /*  94 */ { "M94", 0, 0, 0 },          /* fon */
 /*  95 */ { "M95", 0, 0, 0 },          /* fon */
 /*  96 */ { "M96", 0, 0, 0 },          /* fon */
 /*  97 */ { "M97", 0, 0, 0 },          /* fon */
 /*  98 */ { "M98", 0, 0, 0 },          /* fon */
 /*  99 */ { "M99", 0, 0, 0 },          /* fon */
 /* 100 */ { "M100", 0, 0, 0 },         /* fon */
	// end of mist sources

	// Characters
 /* 101 */ { "PAdams", 0, 0, 0 },
 /* 102 */ { "DAllgood", 0, 0, 0 },
 /* 103 */ { "MBanks", 0, 0, 0 },
 /* 104 */ { "BBernhard", 0, 0, 0 },
 /* 105 */ { "ABrown", 0, 0, 0 },
 /* 106 */ { "FBuster", 0, 0, 0 },
 /* 107 */ { "ZCampbell", 0, 0, 0 },
 /* 108 */ { "NCassidy", 0, 0, 0 },
 /* 109 */ { "SClayton", 0, 0, 0 },
 /* 110 */ { "JCollins", 0, 0, 0 },
 /* 111 */ { "VCollins", 0, 0, 0 },
 /* 112 */ { "CComes", 0, 0, 0 },
 /* 113 */ { "RConnor", 0, 0, 0 },
 /* 114 */ { "GCross", 0, 0, 0 },
 /* 115 */ { "FCunningham", 0, 0, 0 },
 /* 116 */ { "MDawkins", 0, 0, 0 },
 /* 117 */ { "JDawson", 0, 0, 0 },
 /* 118 */ { "GDay", 0, 0, 0 },
 /* 119 */ { "ADecker", 0, 0, 0 },
 /* 120 */ { "CDeLorean", 0, 0, 0 },
 /* 121 */ { "JDoe", 0, 0, 0 },
 /* 122 */ { "MDurden", 0, 0, 0 },
 /* 123 */ { "DElis", 80, 0, 64 },      /* vozhdelenie */
 /* 124 */ { "NEpstein", 0, 0, 0 },
 /* 125 */ { "CFranklin", 0, 0, 0 },
 /* 126 */ { "LGeertz", 0, 0, 0 },
 /* 127 */ { "AGraywood", 0, 0, 0 },
 /* 128 */ { "KGreenhall", 0, 0, 0 },
 /* 129 */ { "AGrissom", 0, 0, 0 },
 /* 130 */ { "Guide", 0, 0, 0 },
 /* 131 */ { "MHailey", 0, 0, 0 },
 /* 132 */ { "ZHarriot", 0, 0, 0 },
 /* 133 */ { "MHarvent", 0, 0, 0 },
 /* 134 */ { "BHeart", 0, 0, 0 },
 /* 135 */ { "BHill", 0, 0, 0 },
 /* 136 */ { "CHonzo", 0, 0, 0 },
 /* 137 */ { "AHuxley", 0, 0, 0 },
 /* 138 */ { "AJane", 0, 0, 0 },
 /* 139 */ { "AJones", 0, 0, 0 },
 /* 140 */ { "EJones", 0, 0, 0 },
 /* 141 */ { "SKendall", 0, 0, 0 },
 /* 142 */ { "LLane", 0, 0, 0 },
 /* 143 */ { "SLarken", 0, 0, 0 },
 /* 144 */ { "ALee", 0, 0, 0 },
 /* 145 */ { "CLefebvre", 0, 0, 0 },
 /* 146 */ { "VLehndorff", 0, 0, 0 },
 /* 147 */ { "TMalone", 0, 0, 0 },
 /* 148 */ { "AMay", 0, 0, 0 },
 /* 149 */ { "UMcGee", 0, 0, 0 },
 /* 150 */ { "FMilligan", 0, 0, 0 },
 /* 151 */ { "AMorientes", 0, 0, 0 },
 /* 152 */ { "MNovak", 0, 0, 0 },
 /* 153 */ { "SOReilly", 0, 0, 0 },
 /* 154 */ { "JOSullivan", 0, 0, 0 },
 /* 155 */ { "MParcs", 0, 0, 0 },
 /* 156 */ { "SRandom", 0, 0, 0 },
 /* 157 */ { "RRay", 0, 0, 0 },
 /* 158 */ { "MReed", 0, 0, 0 },
 /* 159 */ { "MRose", 0, 0, 0 },
 /* 160 */ { "DSeldon", 0, 0, 0 },
 /* 161 */ { "PShannon", 0, 0, 0 },
 /* 162 */ { "JSherman", 0, 0, 0 },
 /* 163 */ { "DSilverstone", 0, 0, 0 },
 /* 164 */ { "RSoyfer", 0, 0, 0 },
 /* 165 */ { "JSpencer", 0, 0, 0 },
 /* 166 */ { "CStanfield", 0, 0, 0 },
 /* 167 */ { "DStanley", 0, 0, 0 },
 /* 168 */ { "JSullivan", 0, 0, 0 },
 /* 169 */ { "SSummer", 0, 0, 0 },
 /* 170 */ { "TSunday", 0, 0, 0 },
 /* 171 */ { "WThomas", 0, 0, 0 },
 /* 172 */ { "EThorp", 0, 0, 0 },
 /* 173 */ { "EWilliams", 0, 0, 0 },
 /* 174 */ { "HWilliams", 0, 0, 0 },
 /* 175 */ { "MWright", 0, 0, 0 },
 /* 176 */ { "LYoung", 0, 0, 0 },
 /* 177 */ { "NYoung", 0, 0, 0 },
 /* 178 */ { "ATest", 0, 0, 0 },
 /* 179 */ { "BTest", 0, 0, 0 },
	// end of characters

	// Intentions
 /* 180 */ { "murder", 120, 0, 43 },    /* ubijstvo */
 /* 181 */ { "creation", 120, 0, 75 },  /* sozidanie */
 /* 182 */ { "destruction", 120, 0, 41 }, /* razrushenie */
 /* 183 */ { "sex", 130, 0, 76 },       /* seks */
 /* 184 */ { "fight", 0, 0, 42 },       /* draka */
 /* 185 */ { "heartbeat", 0, 0, 7 },    /* serdtsebienie */
 /* 186 */ { "addiction", 155, 0, 83 }, /* zavisimost' */
 /* 187 */ { "weed", 160, 0, 75 },      /* sozidanie */
 /* 188 */ { "lsd", 160, 0, 8 },        /* trip */
 /* 189 */ { "pejotl", 160, 0, 8 },     /* trip */
 /* 190 */ { "heroin", 160, 0, 41 },    /* razrushenie */
 /* 191 */ { "krayk", 0, 0, 0 },        /* fon */
 /* 192 */ { "death", 280, 0, 5 },      /* smert' */
 /* 193 */ { "mist", 200, 0, 4 },       /* tuman */
 /* 194 */ { "fear", 199, 0, 29 },      /* strah */
 /* 195 */ { "mSource", 0, 0, 1 },      /* neverno */
 /* 196 */ { "mProject", 0, 0, 1 },     /* neverno */
	// end of intentions
 /* 197 */ { "R197", 0, 0, 1 },         /* neverno */
 /* 198 */ { "R198", 0, 0, 1 },         /* neverno */
 /* 199 */ { "R199", 0, 0, 1 },         /* neverno */
	// Emotion fixes
 /* 200 */ { "xFon", 5, 0, 0 },         /* fon */
 /* 201 */ { "xNeverno", 5, 0, 1 },     /* neverno */
 /* 202 */ { "xMasterka", 5, 0, 2 },    /* masterka */
 /* 203 */ { "xTishina", 5, 0, 3 },     /* tishina */
 /* 204 */ { "xTuman", 5, 0, 4 },       /* tuman */
 /* 205 */ { "xSmert'", 5, 0, 5 },      /* smert' */
 /* 206 */ { "xMertvyak", 5, 0, 6 },    /* mertvyak */
 /* 207 */ { "xSerdtsebienie", 5, 0, 7 }, /* serdtsebienie */
 /* 208 */ { "xTrip", 5, 0, 8 },        /* trip */
 /* 209 */ { "xLes", 5, 0, 9 },         /* les */
 /* 210 */ { "xNegativ", 5, 0, 10 },    /* negativ */
 /* 211 */ { "xRavnodushie", 5, 0, 11 }, /* ravnodushie */
 /* 212 */ { "xSkuka", 5, 0, 12 },      /* skuka */
 /* 213 */ { "xApatiya", 5, 0, 13 },    /* apatiya */
 /* 214 */ { "xDepressiya", 5, 0, 14 }, /* depressiya */
 /* 215 */ { "xZlost'", 5, 0, 15 },     /* zlost' */
 /* 216 */ { "xGnev", 5, 0, 16 },       /* gnev */
 /* 217 */ { "xYarost'", 5, 0, 17 },    /* yarost' */
 /* 218 */ { "xRazdrazhenie", 5, 0, 18 }, /* razdrazhenie */
 /* 219 */ { "xGrust'", 5, 0, 19 },     /* grust' */
 /* 220 */ { "xToska", 5, 0, 20 },      /* toska */
 /* 221 */ { "xObrechennost'", 5, 0, 21 }, /* obrechennost' */
 /* 222 */ { "xOtchayanie", 5, 0, 22 }, /* otchayanie */
 /* 223 */ { "xNeuverennost'", 5, 0, 23 }, /* neuverennost' */
 /* 224 */ { "xOzabochennost'", 5, 0, 24 }, /* ozabochennost' */
 /* 225 */ { "xNapryazhennost'", 5, 0, 25 }, /* napryazhennost' */
 /* 226 */ { "xIsterika", 5, 0, 26 },   /* isterika */
 /* 227 */ { "xSomnenie", 5, 0, 27 },   /* somnenie */
 /* 228 */ { "xTrevozhnost'", 5, 0, 28 }, /* trevozhnost' */
 /* 229 */ { "xStrah", 5, 0, 29 },      /* strah */
 /* 230 */ { "xUzhas", 5, 0, 30 },      /* uzhas */
 /* 231 */ { "xPanika", 5, 0, 31 },     /* panika */
 /* 232 */ { "xFobiya", 5, 0, 32 },     /* fobiya */
 /* 233 */ { "xParanojya", 5, 0, 33 },  /* paranojya */
 /* 234 */ { "xNepriyazn'", 5, 0, 34 }, /* nepriyazn' */
 /* 235 */ { "xNenavist'", 5, 0, 35 },  /* nenavist' */
 /* 236 */ { "xOtvraschenie", 5, 0, 36 }, /* otvraschenie */
 /* 237 */ { "xPrezrenie", 5, 0, 37 },  /* prezrenie */
 /* 238 */ { "xNeudovletvorennost'", 5, 0, 38 }, /* neudovletvorennost' */
 /* 239 */ { "xSozhalenie", 5, 0, 39 }, /* sozhalenie */
 /* 240 */ { "xStyid", 5, 0, 40 },      /* styid */
 /* 241 */ { "xRazrushenie", 5, 0, 41 }, /* razrushenie */
 /* 242 */ { "xDraka", 5, 0, 42 },      /* draka */
 /* 243 */ { "xUbijstvo", 5, 0, 43 },   /* ubijstvo */
 /* 244 */ { "xPozitiv", 5, 0, 44 },    /* pozitiv */
 /* 245 */ { "xVesel'e", 5, 0, 45 },    /* vesel'e */
 /* 246 */ { "xRadost'", 5, 0, 46 },    /* radost' */
 /* 247 */ { "xVostorg", 5, 0, 47 },    /* vostorg */
 /* 248 */ { "xDovol'stvo", 5, 0, 48 }, /* dovol'stvo */
 /* 249 */ { "xGordost'", 5, 0, 49 },   /* gordost' */
 /* 250 */ { "xUdovletvorenie", 5, 0, 50 }, /* udovletvorenie */
 /* 251 */ { "xLyubopyitstvo", 5, 0, 51 }, /* lyubopyitstvo */
 /* 252 */ { "xInteres", 5, 0, 52 },    /* interes */
 /* 253 */ { "xUvlechennost'", 5, 0, 53 }, /* uvlechennost' */
 /* 254 */ { "xAzart", 5, 0, 54 },      /* azart */
 /* 255 */ { "xVoodushevlenie", 5, 0, 55 }, /* voodushevlenie */
 /* 256 */ { "xTseleustremlennost'", 5, 0, 56 }, /* tseleustremlennost' */
 /* 257 */ { "xOtvaga", 5, 0, 57 },     /* otvaga */
 /* 258 */ { "xReshimost'", 5, 0, 58 }, /* reshimost' */
 /* 259 */ { "xVdohnovenie", 5, 0, 59 }, /* vdohnovenie */
 /* 260 */ { "xPriyazn'", 5, 0, 60 },   /* priyazn' */
 /* 261 */ { "xSimpatiya", 5, 0, 61 },  /* simpatiya */
 /* 262 */ { "xPrivyazannost'", 5, 0, 62 }, /* privyazannost' */
 /* 263 */ { "xLyubov'", 5, 0, 63 },    /* lyubov' */
 /* 264 */ { "xVozhdelenie", 5, 0, 64 }, /* vozhdelenie */
 /* 265 */ { "xUmilenie", 5, 0, 65 },   /* umilenie */
 /* 266 */ { "xNadezhda", 5, 0, 66 },   /* nadezhda */
 /* 267 */ { "xPredvkushenie", 5, 0, 67 }, /* predvkushenie */
 /* 268 */ { "xVera", 5, 0, 68 },       /* vera */
 /* 269 */ { "xUverennost'", 5, 0, 69 }, /* uverennost' */
 /* 270 */ { "xPokoj", 5, 0, 70 },      /* pokoj */
 /* 271 */ { "xUmirotvorenie", 5, 0, 71 }, /* umirotvorenie */
 /* 272 */ { "xNostal'giya", 5, 0, 72 }, /* nostal'giya */
 /* 273 */ { "xSoperezhivanie", 5, 0, 73 }, /* soperezhivanie */
 /* 274 */ { "xSostradanie", 5, 0, 74 }, /* sostradanie */
 /* 275 */ { "xSozidanie", 5, 0, 75 },  /* sozidanie */
 /* 276 */ { "xSeks", 5, 0, 76 },       /* seks */
 /* 277 */ { "xZhelanie", 5, 0, 77 },   /* zhelanie */
 /* 278 */ { "xVlechenie", 5, 0, 78 },  /* vlechenie */
 /* 279 */ { "xStrast'", 5, 0, 79 },    /* strast' */
 /* 280 */ { "xOderzhimost'", 5, 0, 80 }, /* oderzhimost' */
 /* 281 */ { "xManiya", 5, 0, 81 },     /* maniya */
 /* 282 */ { "xZainteresovannost'", 5, 0, 82 }, /* zainteresovannost' */
 /* 283 */ { "xZavisimost'", 5, 0, 83 }, /* zavisimost' */
 /* 284 */ { "xDuhovnoe", 5, 0, 84 },   /* duhovnoe */
 /* 285 */ { "xTrans", 5, 0, 85 },      /* trans */
 /* 286 */ { "xMeditatsiya", 5, 0, 86 }, /* meditatsiya */
 /* 287 */ { "xEkzal'tatsiya", 5, 0, 87 }, /* ekzal'tatsiya */
 /* 288 */ { "xProsvetlenie", 5, 0, 88 }, /* prosvetlenie */
	// end of emotion fixes
};

// End of emotions.c
