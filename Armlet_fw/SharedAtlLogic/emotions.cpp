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
 * Generated at 2014-09-04 17:54:03
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
 /*   0 */ { "R000", 0, 0, 1 },         /* neverno */
	// Locations
 /*   1 */ { "hub", 240, 0, 2 },        /* masterka */
 /*   2 */ { "dead", 240, 0, 6 },       /* mertvyak */
 /*   3 */ { "bg", 40, 0, 0 },          /* fon */
 /*   4 */ { "hermit", 40, 0, 73 },     /* umirotvorenie */
 /*   5 */ { "square", 40, 0, 61 },     /* vdohnovenie */
 /*   6 */ { "factory", 40, 0, 14 },    /* skuka */
 /*   7 */ { "cityhall", 40, 0, 51 },   /* gordost' */
 /*   8 */ { "station", 40, 0, 69 },    /* predvkushenie */
 /*   9 */ { "bar", 40, 0, 47 },        /* vesel'e */
 /*  10 */ { "coffee", 40, 0, 54 },     /* interes */
 /*  11 */ { "detskaya", 40, 0, 74 },   /* nostal'giya */
 /*  12 */ { "tent", 40, 0, 16 },       /* depressiya */
 /*  13 */ { "hospital", 40, 0, 23 },   /* obrechennost' */
 /*  14 */ { "university", 40, 0, 66 }, /* vozhdelenie */
 /*  15 */ { "redroom", 40, 0, 87 },    /* trans */
 /*  16 */ { "shop", 40, 0, 40 },       /* neudovletvorennost' */
 /*  17 */ { "blackhouse", 40, 0, 34 }, /* fobiya */
 /*  18 */ { "home", 40, 0, 67 },       /* umilenie */
 /*  19 */ { "trench", 40, 0, 90 },     /* prosvetlenie */
 /*  20 */ { "cinema", 40, 0, 54 },     /* interes */
	// end of locations

	// Forest
 /*  21 */ { "FA21", 40, 0, 11 },       /* les */
 /*  22 */ { "FA22", 40, 0, 11 },       /* les */
 /*  23 */ { "FA23", 40, 0, 11 },       /* les */
 /*  24 */ { "FA24", 40, 0, 11 },       /* les */
 /*  25 */ { "FA25", 40, 0, 11 },       /* les */
 /*  26 */ { "FA26", 40, 0, 11 },       /* les */
 /*  27 */ { "FA27", 40, 0, 11 },       /* les */
 /*  28 */ { "FA28", 40, 0, 11 },       /* les */
 /*  29 */ { "FA29", 40, 0, 11 },       /* les */
 /*  30 */ { "FA30", 40, 0, 11 },       /* les */
 /*  31 */ { "FA31", 40, 0, 11 },       /* les */
 /*  32 */ { "FA32", 40, 0, 11 },       /* les */
 /*  33 */ { "FA33", 40, 0, 11 },       /* les */
 /*  34 */ { "FA34", 40, 0, 11 },       /* les */
 /*  35 */ { "FA35", 40, 0, 11 },       /* les */
 /*  36 */ { "FA36", 40, 0, 11 },       /* les */
 /*  37 */ { "FA37", 40, 0, 11 },       /* les */
 /*  38 */ { "FA38", 40, 0, 11 },       /* les */
 /*  39 */ { "FA39", 40, 0, 11 },       /* les */
 /*  40 */ { "FA40", 40, 0, 11 },       /* les */
 /*  41 */ { "FA41", 40, 0, 11 },       /* les */
 /*  42 */ { "FA42", 40, 0, 11 },       /* les */
 /*  43 */ { "FA43", 40, 0, 11 },       /* les */
 /*  44 */ { "FA44", 40, 0, 11 },       /* les */
 /*  45 */ { "FA45", 40, 0, 11 },       /* les */
 /*  46 */ { "FA46", 40, 0, 11 },       /* les */
 /*  47 */ { "FA47", 40, 0, 11 },       /* les */
 /*  48 */ { "FA48", 40, 0, 11 },       /* les */
 /*  49 */ { "FA49", 40, 0, 11 },       /* les */
 /*  50 */ { "FA50", 40, 0, 11 },       /* les */
 /*  51 */ { "FA51", 40, 0, 11 },       /* les */
 /*  52 */ { "FA52", 40, 0, 11 },       /* les */
 /*  53 */ { "FA53", 40, 0, 11 },       /* les */
 /*  54 */ { "FA54", 40, 0, 11 },       /* les */
 /*  55 */ { "FA55", 40, 0, 11 },       /* les */
 /*  56 */ { "FB56", 40, 0, 11 },       /* les */
 /*  57 */ { "FC57", 40, 0, 11 },       /* les */
 /*  58 */ { "FB58", 40, 0, 11 },       /* les */
 /*  59 */ { "FC59", 40, 0, 11 },       /* les */
 /*  60 */ { "FB60", 40, 0, 11 },       /* les */
 /*  61 */ { "FC61", 40, 0, 11 },       /* les */
 /*  62 */ { "FB62", 40, 0, 11 },       /* les */
 /*  63 */ { "FC63", 40, 0, 11 },       /* les */
 /*  64 */ { "FB64", 40, 0, 11 },       /* les */
 /*  65 */ { "FC65", 40, 0, 11 },       /* les */
 /*  66 */ { "FB66", 40, 0, 11 },       /* les */
 /*  67 */ { "FC67", 40, 0, 11 },       /* les */
 /*  68 */ { "FB68", 40, 0, 11 },       /* les */
 /*  69 */ { "FC69", 40, 0, 11 },       /* les */
 /*  70 */ { "FB70", 40, 0, 11 },       /* les */
 /*  71 */ { "FC71", 40, 0, 11 },       /* les */
 /*  72 */ { "FB72", 40, 0, 11 },       /* les */
 /*  73 */ { "FC73", 40, 0, 11 },       /* les */
 /*  74 */ { "FB74", 40, 0, 11 },       /* les */
 /*  75 */ { "FC75", 40, 0, 11 },       /* les */
 /*  76 */ { "FB76", 40, 0, 11 },       /* les */
 /*  77 */ { "FC77", 40, 0, 11 },       /* les */
 /*  78 */ { "FB78", 40, 0, 11 },       /* les */
 /*  79 */ { "FC79", 40, 0, 11 },       /* les */
 /*  80 */ { "FB80", 40, 0, 11 },       /* les */
 /*  81 */ { "FC81", 40, 0, 11 },       /* les */
 /*  82 */ { "FB82", 40, 0, 11 },       /* les */
 /*  83 */ { "FC83", 40, 0, 11 },       /* les */
 /*  84 */ { "FB84", 40, 0, 11 },       /* les */
 /*  85 */ { "FC85", 40, 0, 11 },       /* les */
 /*  86 */ { "FB86", 40, 0, 11 },       /* les */
 /*  87 */ { "FC87", 40, 0, 11 },       /* les */
 /*  88 */ { "FB88", 40, 0, 11 },       /* les */
 /*  89 */ { "FC89", 40, 0, 11 },       /* les */
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
 /* 123 */ { "DElis", 80, 0, 66 },      /* vozhdelenie */
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
 /* 180 */ { "murder", 120, 0, 45 },    /* ubijstvo */
 /* 181 */ { "creation", 120, 0, 77 },  /* sozidanie */
 /* 182 */ { "destruction", 120, 0, 43 }, /* razrushenie */
 /* 183 */ { "sex", 130, 0, 78 },       /* seks */
 /* 184 */ { "fight", 0, 0, 44 },       /* draka */
 /* 185 */ { "heartbeat", 0, 0, 7 },    /* serdtsebienie */
 /* 186 */ { "addiction", 155, 0, 85 }, /* zavisimost' */
 /* 187 */ { "weed", 160, 0, 77 },      /* sozidanie */
 /* 188 */ { "lsd", 160, 0, 8 },        /* trip */
 /* 189 */ { "peyote", 160, 0, 9 },     /* kaktus */
 /* 190 */ { "heroin", 160, 0, 43 },    /* razrushenie */
 /* 191 */ { "krayk", 0, 0, 0 },        /* fon */
 /* 192 */ { "death", 280, 0, 5 },      /* smert' */
 /* 193 */ { "mist", 200, 0, 4 },       /* tuman */
 /* 194 */ { "fear", 199, 0, 31 },      /* strah */
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
 /* 209 */ { "xKaktus", 5, 0, 9 },      /* kaktus */
 /* 210 */ { "xNarkoman", 5, 0, 10 },   /* narkoman */
 /* 211 */ { "xLes", 5, 0, 11 },        /* les */
 /* 212 */ { "xNegativ", 5, 0, 12 },    /* negativ */
 /* 213 */ { "xRavnodushie", 5, 0, 13 }, /* ravnodushie */
 /* 214 */ { "xSkuka", 5, 0, 14 },      /* skuka */
 /* 215 */ { "xApatiya", 5, 0, 15 },    /* apatiya */
 /* 216 */ { "xDepressiya", 5, 0, 16 }, /* depressiya */
 /* 217 */ { "xZlost'", 5, 0, 17 },     /* zlost' */
 /* 218 */ { "xGnev", 5, 0, 18 },       /* gnev */
 /* 219 */ { "xYarost'", 5, 0, 19 },    /* yarost' */
 /* 220 */ { "xRazdrazhenie", 5, 0, 20 }, /* razdrazhenie */
 /* 221 */ { "xGrust'", 5, 0, 21 },     /* grust' */
 /* 222 */ { "xToska", 5, 0, 22 },      /* toska */
 /* 223 */ { "xObrechennost'", 5, 0, 23 }, /* obrechennost' */
 /* 224 */ { "xOtchayanie", 5, 0, 24 }, /* otchayanie */
 /* 225 */ { "xNeuverennost'", 5, 0, 25 }, /* neuverennost' */
 /* 226 */ { "xOzabochennost'", 5, 0, 26 }, /* ozabochennost' */
 /* 227 */ { "xNapryazhennost'", 5, 0, 27 }, /* napryazhennost' */
 /* 228 */ { "xIsterika", 5, 0, 28 },   /* isterika */
 /* 229 */ { "xSomnenie", 5, 0, 29 },   /* somnenie */
 /* 230 */ { "xTrevozhnost'", 5, 0, 30 }, /* trevozhnost' */
 /* 231 */ { "xStrah", 5, 0, 31 },      /* strah */
 /* 232 */ { "xUzhas", 5, 0, 32 },      /* uzhas */
 /* 233 */ { "xPanika", 5, 0, 33 },     /* panika */
 /* 234 */ { "xFobiya", 5, 0, 34 },     /* fobiya */
 /* 235 */ { "xParanojya", 5, 0, 35 },  /* paranojya */
 /* 236 */ { "xNepriyazn'", 5, 0, 36 }, /* nepriyazn' */
 /* 237 */ { "xNenavist'", 5, 0, 37 },  /* nenavist' */
 /* 238 */ { "xOtvraschenie", 5, 0, 38 }, /* otvraschenie */
 /* 239 */ { "xPrezrenie", 5, 0, 39 },  /* prezrenie */
 /* 240 */ { "xNeudovletvorennost'", 5, 0, 40 }, /* neudovletvorennost' */
 /* 241 */ { "xSozhalenie", 5, 0, 41 }, /* sozhalenie */
 /* 242 */ { "xStyid", 5, 0, 42 },      /* styid */
 /* 243 */ { "xRazrushenie", 5, 0, 43 }, /* razrushenie */
 /* 244 */ { "xDraka", 5, 0, 44 },      /* draka */
 /* 245 */ { "xUbijstvo", 5, 0, 45 },   /* ubijstvo */
 /* 246 */ { "xPozitiv", 5, 0, 46 },    /* pozitiv */
 /* 247 */ { "xVesel'e", 5, 0, 47 },    /* vesel'e */
 /* 248 */ { "xRadost'", 5, 0, 48 },    /* radost' */
 /* 249 */ { "xVostorg", 5, 0, 49 },    /* vostorg */
 /* 250 */ { "xDovol'stvo", 5, 0, 50 }, /* dovol'stvo */
 /* 251 */ { "xGordost'", 5, 0, 51 },   /* gordost' */
 /* 252 */ { "xUdovletvorenie", 5, 0, 52 }, /* udovletvorenie */
 /* 253 */ { "xLyubopyitstvo", 5, 0, 53 }, /* lyubopyitstvo */
 /* 254 */ { "xInteres", 5, 0, 54 },    /* interes */
 /* 255 */ { "xUvlechennost'", 5, 0, 55 }, /* uvlechennost' */
 /* 256 */ { "xAzart", 5, 0, 56 },      /* azart */
 /* 257 */ { "xVoodushevlenie", 5, 0, 57 }, /* voodushevlenie */
 /* 258 */ { "xTseleustremlennost'", 5, 0, 58 }, /* tseleustremlennost' */
 /* 259 */ { "xOtvaga", 5, 0, 59 },     /* otvaga */
 /* 260 */ { "xReshimost'", 5, 0, 60 }, /* reshimost' */
 /* 261 */ { "xVdohnovenie", 5, 0, 61 }, /* vdohnovenie */
 /* 262 */ { "xPriyazn'", 5, 0, 62 },   /* priyazn' */
 /* 263 */ { "xSimpatiya", 5, 0, 63 },  /* simpatiya */
 /* 264 */ { "xPrivyazannost'", 5, 0, 64 }, /* privyazannost' */
 /* 265 */ { "xLyubov'", 5, 0, 65 },    /* lyubov' */
 /* 266 */ { "xVozhdelenie", 5, 0, 66 }, /* vozhdelenie */
 /* 267 */ { "xUmilenie", 5, 0, 67 },   /* umilenie */
 /* 268 */ { "xNadezhda", 5, 0, 68 },   /* nadezhda */
 /* 269 */ { "xPredvkushenie", 5, 0, 69 }, /* predvkushenie */
 /* 270 */ { "xVera", 5, 0, 70 },       /* vera */
 /* 271 */ { "xUverennost'", 5, 0, 71 }, /* uverennost' */
 /* 272 */ { "xPokoj", 5, 0, 72 },      /* pokoj */
 /* 273 */ { "xUmirotvorenie", 5, 0, 73 }, /* umirotvorenie */
 /* 274 */ { "xNostal'giya", 5, 0, 74 }, /* nostal'giya */
 /* 275 */ { "xSoperezhivanie", 5, 0, 75 }, /* soperezhivanie */
 /* 276 */ { "xSostradanie", 5, 0, 76 }, /* sostradanie */
 /* 277 */ { "xSozidanie", 5, 0, 77 },  /* sozidanie */
 /* 278 */ { "xSeks", 5, 0, 78 },       /* seks */
 /* 279 */ { "xZhelanie", 5, 0, 79 },   /* zhelanie */
 /* 280 */ { "xVlechenie", 5, 0, 80 },  /* vlechenie */
 /* 281 */ { "xStrast'", 5, 0, 81 },    /* strast' */
 /* 282 */ { "xOderzhimost'", 5, 0, 82 }, /* oderzhimost' */
 /* 283 */ { "xManiya", 5, 0, 83 },     /* maniya */
 /* 284 */ { "xZainteresovannost'", 5, 0, 84 }, /* zainteresovannost' */
 /* 285 */ { "xZavisimost'", 5, 0, 85 }, /* zavisimost' */
 /* 286 */ { "xDuhovnoe", 5, 0, 86 },   /* duhovnoe */
 /* 287 */ { "xTrans", 5, 0, 87 },      /* trans */
 /* 288 */ { "xMeditatsiya", 5, 0, 88 }, /* meditatsiya */
 /* 289 */ { "xEkzal'tatsiya", 5, 0, 89 }, /* ekzal'tatsiya */
 /* 290 */ { "xProsvetlenie", 5, 0, 90 }, /* prosvetlenie */
	// end of emotion fixes
};

// End of emotions.c
