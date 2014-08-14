/*
 * emotions.c
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from Emotions.csv, Locations.csv, Characters.csv and Intentions.csv
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-08-14 20:06:34
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
 /*  9 */     { "negativ", 1, 0, -1, -1 },
 /* 10 */         { "ravnodushie", 1, 9, -1, -1 },
 /* 11 */             { "skuka", 1, 10, -1, -1 },
 /* 12 */                 { "apatiya", 1, 11, -1, -1 },
 /* 13 */                     { "depressiya", 1, 12, -1, -1 },
 /* 14 */         { "zlost'", 1, 9, -1, -1 },
 /* 15 */             { "gnev", 1, 14, -1, -1 },
 /* 16 */                 { "yarost'", 1, 15, -1, -1 },
 /* 17 */             { "razdrazhenie", 1, 14, -1, -1 },
 /* 18 */         { "grust'", 1, 9, -1, -1 },
 /* 19 */             { "toska", 1, 18, -1, -1 },
 /* 20 */                 { "obrechennost'", 1, 19, -1, -1 },
 /* 21 */                 { "otchayanie", 1, 19, -1, -1 },
 /* 22 */         { "neuverennost'", 1, 9, -1, -1 },
 /* 23 */             { "ozabochennost'", 1, 22, -1, -1 },
 /* 24 */                 { "napryazhennost'", 1, 23, -1, -1 },
 /* 25 */                     { "isterika", 1, 24, -1, -1 },
 /* 26 */             { "somnenie", 1, 22, -1, -1 },
 /* 27 */         { "trevozhnost'", 1, 9, -1, -1 },
 /* 28 */             { "strah", 1, 27, -1, -1 },
 /* 29 */                 { "uzhas", 1, 28, -1, -1 },
 /* 30 */                     { "panika", 1, 29, -1, -1 },
 /* 31 */                         { "fobiya", 1, 30, -1, -1 },
 /* 32 */                         { "paranojya", 1, 30, -1, -1 },
 /* 33 */         { "nepriyazn'", 1, 9, -1, -1 },
 /* 34 */             { "nenavist'", 1, 33, -1, -1 },
 /* 35 */             { "otvraschenie", 1, 33, -1, -1 },
 /* 36 */             { "prezrenie", 1, 33, -1, -1 },
 /* 37 */         { "neudovletvorennost'", 1, 9, -1, -1 },
 /* 38 */         { "sozhalenie", 1, 9, -1, -1 },
 /* 39 */             { "styid", 1, 38, -1, -1 },
 /* 40 */     { "razrushenie", 1, 0, -1, -1 },
 /* 41 */     { "draka", 1, 0, -1, -1 },
 /* 42 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 43 */     { "pozitiv", 1, 0, -1, -1 },
 /* 44 */         { "vesel'e", 1, 43, -1, -1 },
 /* 45 */             { "radost'", 1, 44, -1, -1 },
 /* 46 */                 { "vostorg", 1, 45, -1, -1 },
 /* 47 */         { "dovol'stvo", 1, 43, -1, -1 },
 /* 48 */             { "gordost'", 1, 47, -1, -1 },
 /* 49 */             { "udovletvorenie", 1, 47, -1, -1 },
 /* 50 */         { "lyubopyitstvo", 1, 43, -1, -1 },
 /* 51 */             { "interes", 1, 50, -1, -1 },
 /* 52 */             { "uvlechennost'", 1, 50, -1, -1 },
 /* 53 */                 { "azart", 1, 52, -1, -1 },
 /* 54 */                 { "voodushevlenie", 1, 52, -1, -1 },
 /* 55 */                     { "tseleustremlennost'", 1, 54, -1, -1 },
 /* 56 */                     { "otvaga", 1, 54, -1, -1 },
 /* 57 */                     { "reshimost'", 1, 54, -1, -1 },
 /* 58 */                 { "vdohnovenie", 1, 52, -1, -1 },
 /* 59 */         { "priyazn'", 1, 43, -1, -1 },
 /* 60 */             { "simpatiya", 1, 59, -1, -1 },
 /* 61 */                 { "privyazannost'", 1, 60, -1, -1 },
 /* 62 */                     { "lyubov'", 1, 61, -1, -1 },
 /* 63 */                 { "vozhdelenie", 1, 60, -1, -1 },
 /* 64 */             { "umilenie", 1, 59, -1, -1 },
 /* 65 */         { "nadezhda", 1, 43, -1, -1 },
 /* 66 */             { "predvkushenie", 1, 65, -1, -1 },
 /* 67 */                 { "vera", 1, 66, -1, -1 },
 /* 68 */                 { "uverennost'", 1, 66, -1, -1 },
 /* 69 */         { "pokoj", 1, 43, -1, -1 },
 /* 70 */             { "umirotvorenie", 1, 69, -1, -1 },
 /* 71 */             { "nostal'giya", 1, 69, -1, -1 },
 /* 72 */         { "soperezhivanie", 1, 43, -1, -1 },
 /* 73 */             { "sostradanie", 1, 72, -1, -1 },
 /* 74 */     { "sozidanie", 1, 0, -1, -1 },
 /* 75 */     { "seks", 1, 0, -1, -1 },
 /* 76 */     { "zhelanie", 1, 0, -1, -1 },
 /* 77 */         { "vlechenie", 1, 76, -1, -1 },
 /* 78 */             { "strast'", 1, 77, -1, -1 },
 /* 79 */                 { "oderzhimost'", 1, 78, -1, -1 },
 /* 80 */                     { "maniya", 1, 79, -1, -1 },
 /* 81 */         { "zainteresovannost'", 1, 76, -1, -1 },
 /* 82 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 83 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 84 */         { "trans", 1, 83, -1, -1 },
 /* 85 */         { "meditatsiya", 1, 83, -1, -1 },
 /* 86 */         { "ekzal'tatsiya", 1, 83, -1, -1 },
 /* 87 */         { "prosvetlenie", 1, 83, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "r000", 99, 1 },	/* neverno */
	// Locations
 /*   1 */ { "hub", 99, 2 },	/* masterka */
 /*   2 */ { "uliza", 1, 0 },	/* fon */
 /*   3 */ { "grave", 3, 28 },	/* strah */
 /*   4 */ { "church", 2, 43 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 9 },	/* negativ */
 /*   6 */ { "dead", 99, 6 },	/* mertvyak */
	// end of locations
 /*   7 */ { "r007", 99, 1 },	/* neverno */
 /*   8 */ { "r008", 99, 1 },	/* neverno */
 /*   9 */ { "r009", 99, 1 },	/* neverno */
 /*  10 */ { "r010", 99, 1 },	/* neverno */
 /*  11 */ { "r011", 99, 1 },	/* neverno */
 /*  12 */ { "r012", 99, 1 },	/* neverno */
 /*  13 */ { "r013", 99, 1 },	/* neverno */
 /*  14 */ { "r014", 99, 1 },	/* neverno */
 /*  15 */ { "r015", 99, 1 },	/* neverno */
 /*  16 */ { "r016", 99, 1 },	/* neverno */
 /*  17 */ { "r017", 99, 1 },	/* neverno */
 /*  18 */ { "r018", 99, 1 },	/* neverno */
 /*  19 */ { "r019", 99, 1 },	/* neverno */
 /*  20 */ { "r020", 99, 1 },	/* neverno */
 /*  21 */ { "r021", 99, 1 },	/* neverno */
 /*  22 */ { "r022", 99, 1 },	/* neverno */
 /*  23 */ { "r023", 99, 1 },	/* neverno */
 /*  24 */ { "r024", 99, 1 },	/* neverno */
 /*  25 */ { "r025", 99, 1 },	/* neverno */
 /*  26 */ { "r026", 99, 1 },	/* neverno */
 /*  27 */ { "r027", 99, 1 },	/* neverno */
 /*  28 */ { "r028", 99, 1 },	/* neverno */
 /*  29 */ { "r029", 99, 1 },	/* neverno */
 /*  30 */ { "r030", 99, 1 },	/* neverno */
 /*  31 */ { "r031", 99, 1 },	/* neverno */
 /*  32 */ { "r032", 99, 1 },	/* neverno */
 /*  33 */ { "r033", 99, 1 },	/* neverno */
 /*  34 */ { "r034", 99, 1 },	/* neverno */
 /*  35 */ { "r035", 99, 1 },	/* neverno */
 /*  36 */ { "r036", 99, 1 },	/* neverno */
 /*  37 */ { "r037", 99, 1 },	/* neverno */
 /*  38 */ { "r038", 99, 1 },	/* neverno */
 /*  39 */ { "r039", 99, 1 },	/* neverno */
 /*  40 */ { "r040", 99, 1 },	/* neverno */
 /*  41 */ { "r041", 99, 1 },	/* neverno */
 /*  42 */ { "r042", 99, 1 },	/* neverno */
 /*  43 */ { "r043", 99, 1 },	/* neverno */
 /*  44 */ { "r044", 99, 1 },	/* neverno */
 /*  45 */ { "r045", 99, 1 },	/* neverno */
 /*  46 */ { "r046", 99, 1 },	/* neverno */
 /*  47 */ { "r047", 99, 1 },	/* neverno */
 /*  48 */ { "r048", 99, 1 },	/* neverno */
 /*  49 */ { "r049", 99, 1 },	/* neverno */
 /*  50 */ { "r050", 99, 1 },	/* neverno */
 /*  51 */ { "r051", 99, 1 },	/* neverno */
 /*  52 */ { "r052", 99, 1 },	/* neverno */
 /*  53 */ { "r053", 99, 1 },	/* neverno */
 /*  54 */ { "r054", 99, 1 },	/* neverno */
 /*  55 */ { "r055", 99, 1 },	/* neverno */
 /*  56 */ { "r056", 99, 1 },	/* neverno */
 /*  57 */ { "r057", 99, 1 },	/* neverno */
 /*  58 */ { "r058", 99, 1 },	/* neverno */
 /*  59 */ { "r059", 99, 1 },	/* neverno */
 /*  60 */ { "r060", 99, 1 },	/* neverno */
 /*  61 */ { "r061", 99, 1 },	/* neverno */
 /*  62 */ { "r062", 99, 1 },	/* neverno */
 /*  63 */ { "r063", 99, 1 },	/* neverno */
 /*  64 */ { "r064", 99, 1 },	/* neverno */
 /*  65 */ { "r065", 99, 1 },	/* neverno */
 /*  66 */ { "r066", 99, 1 },	/* neverno */
 /*  67 */ { "r067", 99, 1 },	/* neverno */
 /*  68 */ { "r068", 99, 1 },	/* neverno */
 /*  69 */ { "r069", 99, 1 },	/* neverno */
 /*  70 */ { "r070", 99, 1 },	/* neverno */
 /*  71 */ { "r071", 99, 1 },	/* neverno */
 /*  72 */ { "r072", 99, 1 },	/* neverno */
 /*  73 */ { "r073", 99, 1 },	/* neverno */
 /*  74 */ { "r074", 99, 1 },	/* neverno */
 /*  75 */ { "r075", 99, 1 },	/* neverno */
 /*  76 */ { "r076", 99, 1 },	/* neverno */
 /*  77 */ { "r077", 99, 1 },	/* neverno */
 /*  78 */ { "r078", 99, 1 },	/* neverno */
 /*  79 */ { "r079", 99, 1 },	/* neverno */
 /*  80 */ { "r080", 99, 1 },	/* neverno */
 /*  81 */ { "r081", 99, 1 },	/* neverno */
 /*  82 */ { "r082", 99, 1 },	/* neverno */
 /*  83 */ { "r083", 99, 1 },	/* neverno */
 /*  84 */ { "r084", 99, 1 },	/* neverno */
 /*  85 */ { "r085", 99, 1 },	/* neverno */
 /*  86 */ { "r086", 99, 1 },	/* neverno */
 /*  87 */ { "r087", 99, 1 },	/* neverno */
 /*  88 */ { "r088", 99, 1 },	/* neverno */
 /*  89 */ { "r089", 99, 1 },	/* neverno */
	// Mist sources
 /*  90 */ { "m90", 0, 0 },	/* fon */
 /*  91 */ { "m91", 0, 0 },	/* fon */
 /*  92 */ { "m92", 0, 0 },	/* fon */
 /*  93 */ { "m93", 0, 0 },	/* fon */
 /*  94 */ { "m94", 0, 0 },	/* fon */
 /*  95 */ { "m95", 0, 0 },	/* fon */
 /*  96 */ { "m96", 0, 0 },	/* fon */
 /*  97 */ { "m97", 0, 0 },	/* fon */
 /*  98 */ { "m98", 0, 0 },	/* fon */
 /*  99 */ { "m99", 0, 0 },	/* fon */
	// end of mist sources
 /* 100 */ { "r100", 99, 1 },	/* neverno */
	// Characters
 /* 101 */ { "PAdams", 0, 0 },
 /* 102 */ { "FBuster", 0, 0 },
 /* 103 */ { "ZCampbell", 0, 0 },
 /* 104 */ { "NCassidy", 0, 0 },
 /* 105 */ { "JChamber", 0, 0 },
 /* 106 */ { "SClayton", 0, 0 },
 /* 107 */ { "JCollins", 0, 0 },
 /* 108 */ { "CComes", 0, 0 },
 /* 109 */ { "RConnor", 0, 0 },
 /* 110 */ { "GCross", 0, 0 },
 /* 111 */ { "FCunningham", 0, 0 },
 /* 112 */ { "JDawson", 0, 0 },
 /* 113 */ { "GDay", 0, 0 },
 /* 114 */ { "CDeLorean", 0, 0 },
 /* 115 */ { "ADecker", 0, 0 },
 /* 116 */ { "MDurden", 0, 0 },
 /* 117 */ { "NEpstein", 0, 0 },
 /* 118 */ { "CFranklin", 0, 0 },
 /* 119 */ { "AGraywood", 0, 0 },
 /* 120 */ { "KGreenhall", 0, 0 },
 /* 121 */ { "Guide", 0, 0 },
 /* 122 */ { "MHailey", 0, 0 },
 /* 123 */ { "ZHarriot", 0, 0 },
 /* 124 */ { "BHill", 0, 0 },
 /* 125 */ { "AHuxley", 0, 0 },
 /* 126 */ { "AJoe", 0, 0 },
 /* 127 */ { "LLane", 0, 0 },
 /* 128 */ { "CLefebvre", 0, 0 },
 /* 129 */ { "VLehndorff", 0, 0 },
 /* 130 */ { "TMalone", 0, 0 },
 /* 131 */ { "UMcGee", 0, 0 },
 /* 132 */ { "FMilligan", 0, 0 },
 /* 133 */ { "AMorientes", 0, 0 },
 /* 134 */ { "JOSullivan", 0, 0 },
 /* 135 */ { "MParcs", 0, 0 },
 /* 136 */ { "SRandom", 0, 0 },
 /* 137 */ { "RRay", 0, 0 },
 /* 138 */ { "MReed", 0, 0 },
 /* 139 */ { "MRose", 0, 0 },
 /* 140 */ { "JSherman", 0, 0 },
 /* 141 */ { "CStanfield", 0, 0 },
 /* 142 */ { "DStanley", 0, 0 },
 /* 143 */ { "TSunday", 0, 0 },
 /* 144 */ { "WThomas", 0, 0 },
 /* 145 */ { "EThorp", 0, 0 },
 /* 146 */ { "HWilliams", 0, 0 },
 /* 147 */ { "EWilliams", 0, 0 },
 /* 148 */ { "LYoung", 0, 0 },
 /* 149 */ { "SKendall", 0, 0 },
 /* 150 */ { "ABrown", 0, 0 },
 /* 151 */ { "AJane", 0, 0 },
 /* 152 */ { "DSeldon", 0, 0 },
	// end of characters
 /* 153 */ { "r153", 99, 1 },	/* neverno */
 /* 154 */ { "r154", 99, 1 },	/* neverno */
 /* 155 */ { "r155", 99, 1 },	/* neverno */
 /* 156 */ { "r156", 99, 1 },	/* neverno */
 /* 157 */ { "r157", 99, 1 },	/* neverno */
 /* 158 */ { "r158", 99, 1 },	/* neverno */
 /* 159 */ { "r159", 99, 1 },	/* neverno */
 /* 160 */ { "r160", 99, 1 },	/* neverno */
 /* 161 */ { "r161", 99, 1 },	/* neverno */
 /* 162 */ { "r162", 99, 1 },	/* neverno */
 /* 163 */ { "r163", 99, 1 },	/* neverno */
 /* 164 */ { "r164", 99, 1 },	/* neverno */
 /* 165 */ { "r165", 99, 1 },	/* neverno */
 /* 166 */ { "r166", 99, 1 },	/* neverno */
 /* 167 */ { "r167", 99, 1 },	/* neverno */
 /* 168 */ { "r168", 99, 1 },	/* neverno */
 /* 169 */ { "r169", 99, 1 },	/* neverno */
 /* 170 */ { "r170", 99, 1 },	/* neverno */
 /* 171 */ { "r171", 99, 1 },	/* neverno */
 /* 172 */ { "r172", 99, 1 },	/* neverno */
 /* 173 */ { "r173", 99, 1 },	/* neverno */
 /* 174 */ { "r174", 99, 1 },	/* neverno */
 /* 175 */ { "r175", 99, 1 },	/* neverno */
 /* 176 */ { "r176", 99, 1 },	/* neverno */
 /* 177 */ { "r177", 99, 1 },	/* neverno */
 /* 178 */ { "r178", 99, 1 },	/* neverno */
 /* 179 */ { "r179", 99, 1 },	/* neverno */
	// Intentions
 /* 180 */ { "murder", 1, 42 },	/* ubijstvo */
 /* 181 */ { "creation", 5, 74 },	/* sozidanie */
 /* 182 */ { "destruction", 5, 40 },	/* razrushenie */
 /* 183 */ { "sex", 9, 75 },	/* seks */
 /* 184 */ { "fight", 50, 41 },	/* draka */
 /* 185 */ { "heartbeat", 50, 7 },	/* serdtsebienie */
 /* 186 */ { "weed", 20, 43 },	/* pozitiv */
 /* 187 */ { "heroin", 20, 9 },	/* negativ */
 /* 188 */ { "lsd", 20, 8 },	/* trip */
 /* 189 */ { "krayk", 20, 0 },	/* fon */
 /* 190 */ { "death", 50, 5 },	/* smert' */
 /* 191 */ { "mist", 50, 4 },	/* tuman */
 /* 192 */ { "fear", 40, 28 },	/* strah */
 /* 193 */ { "mSource", 9, 1 },	/* neverno */
 /* 194 */ { "mProject", 9, 1 },	/* neverno */
	// end of intentions
 /* 195 */ { "r195", 99, 1 },	/* neverno */
 /* 196 */ { "r196", 99, 1 },	/* neverno */
 /* 197 */ { "r197", 99, 1 },	/* neverno */
 /* 198 */ { "r198", 99, 1 },	/* neverno */
 /* 199 */ { "r199", 99, 1 },	/* neverno */
	// Emotion fixes
 /* 200 */ { "xFon", 5, 0 },	/* fon */
 /* 201 */ { "xNeverno", 5, 1 },	/* neverno */
 /* 202 */ { "xMasterka", 5, 2 },	/* masterka */
 /* 203 */ { "xTishina", 5, 3 },	/* tishina */
 /* 204 */ { "xTuman", 5, 4 },	/* tuman */
 /* 205 */ { "xSmert'", 5, 5 },	/* smert' */
 /* 206 */ { "xMertvyak", 5, 6 },	/* mertvyak */
 /* 207 */ { "xSerdtsebienie", 5, 7 },	/* serdtsebienie */
 /* 208 */ { "xTrip", 5, 8 },	/* trip */
 /* 209 */ { "xNegativ", 5, 9 },	/* negativ */
 /* 210 */ { "xRavnodushie", 5, 10 },	/* ravnodushie */
 /* 211 */ { "xSkuka", 5, 11 },	/* skuka */
 /* 212 */ { "xApatiya", 5, 12 },	/* apatiya */
 /* 213 */ { "xDepressiya", 5, 13 },	/* depressiya */
 /* 214 */ { "xZlost'", 5, 14 },	/* zlost' */
 /* 215 */ { "xGnev", 5, 15 },	/* gnev */
 /* 216 */ { "xYarost'", 5, 16 },	/* yarost' */
 /* 217 */ { "xRazdrazhenie", 5, 17 },	/* razdrazhenie */
 /* 218 */ { "xGrust'", 5, 18 },	/* grust' */
 /* 219 */ { "xToska", 5, 19 },	/* toska */
 /* 220 */ { "xObrechennost'", 5, 20 },	/* obrechennost' */
 /* 221 */ { "xOtchayanie", 5, 21 },	/* otchayanie */
 /* 222 */ { "xNeuverennost'", 5, 22 },	/* neuverennost' */
 /* 223 */ { "xOzabochennost'", 5, 23 },	/* ozabochennost' */
 /* 224 */ { "xNapryazhennost'", 5, 24 },	/* napryazhennost' */
 /* 225 */ { "xIsterika", 5, 25 },	/* isterika */
 /* 226 */ { "xSomnenie", 5, 26 },	/* somnenie */
 /* 227 */ { "xTrevozhnost'", 5, 27 },	/* trevozhnost' */
 /* 228 */ { "xStrah", 5, 28 },	/* strah */
 /* 229 */ { "xUzhas", 5, 29 },	/* uzhas */
 /* 230 */ { "xPanika", 5, 30 },	/* panika */
 /* 231 */ { "xFobiya", 5, 31 },	/* fobiya */
 /* 232 */ { "xParanojya", 5, 32 },	/* paranojya */
 /* 233 */ { "xNepriyazn'", 5, 33 },	/* nepriyazn' */
 /* 234 */ { "xNenavist'", 5, 34 },	/* nenavist' */
 /* 235 */ { "xOtvraschenie", 5, 35 },	/* otvraschenie */
 /* 236 */ { "xPrezrenie", 5, 36 },	/* prezrenie */
 /* 237 */ { "xNeudovletvorennost'", 5, 37 },	/* neudovletvorennost' */
 /* 238 */ { "xSozhalenie", 5, 38 },	/* sozhalenie */
 /* 239 */ { "xStyid", 5, 39 },	/* styid */
 /* 240 */ { "xRazrushenie", 5, 40 },	/* razrushenie */
 /* 241 */ { "xDraka", 5, 41 },	/* draka */
 /* 242 */ { "xUbijstvo", 5, 42 },	/* ubijstvo */
 /* 243 */ { "xPozitiv", 5, 43 },	/* pozitiv */
 /* 244 */ { "xVesel'e", 5, 44 },	/* vesel'e */
 /* 245 */ { "xRadost'", 5, 45 },	/* radost' */
 /* 246 */ { "xVostorg", 5, 46 },	/* vostorg */
 /* 247 */ { "xDovol'stvo", 5, 47 },	/* dovol'stvo */
 /* 248 */ { "xGordost'", 5, 48 },	/* gordost' */
 /* 249 */ { "xUdovletvorenie", 5, 49 },	/* udovletvorenie */
 /* 250 */ { "xLyubopyitstvo", 5, 50 },	/* lyubopyitstvo */
 /* 251 */ { "xInteres", 5, 51 },	/* interes */
 /* 252 */ { "xUvlechennost'", 5, 52 },	/* uvlechennost' */
 /* 253 */ { "xAzart", 5, 53 },	/* azart */
 /* 254 */ { "xVoodushevlenie", 5, 54 },	/* voodushevlenie */
 /* 255 */ { "xTseleustremlennost'", 5, 55 },	/* tseleustremlennost' */
 /* 256 */ { "xOtvaga", 5, 56 },	/* otvaga */
 /* 257 */ { "xReshimost'", 5, 57 },	/* reshimost' */
 /* 258 */ { "xVdohnovenie", 5, 58 },	/* vdohnovenie */
 /* 259 */ { "xPriyazn'", 5, 59 },	/* priyazn' */
 /* 260 */ { "xSimpatiya", 5, 60 },	/* simpatiya */
 /* 261 */ { "xPrivyazannost'", 5, 61 },	/* privyazannost' */
 /* 262 */ { "xLyubov'", 5, 62 },	/* lyubov' */
 /* 263 */ { "xVozhdelenie", 5, 63 },	/* vozhdelenie */
 /* 264 */ { "xUmilenie", 5, 64 },	/* umilenie */
 /* 265 */ { "xNadezhda", 5, 65 },	/* nadezhda */
 /* 266 */ { "xPredvkushenie", 5, 66 },	/* predvkushenie */
 /* 267 */ { "xVera", 5, 67 },	/* vera */
 /* 268 */ { "xUverennost'", 5, 68 },	/* uverennost' */
 /* 269 */ { "xPokoj", 5, 69 },	/* pokoj */
 /* 270 */ { "xUmirotvorenie", 5, 70 },	/* umirotvorenie */
 /* 271 */ { "xNostal'giya", 5, 71 },	/* nostal'giya */
 /* 272 */ { "xSoperezhivanie", 5, 72 },	/* soperezhivanie */
 /* 273 */ { "xSostradanie", 5, 73 },	/* sostradanie */
 /* 274 */ { "xSozidanie", 5, 74 },	/* sozidanie */
 /* 275 */ { "xSeks", 5, 75 },	/* seks */
 /* 276 */ { "xZhelanie", 5, 76 },	/* zhelanie */
 /* 277 */ { "xVlechenie", 5, 77 },	/* vlechenie */
 /* 278 */ { "xStrast'", 5, 78 },	/* strast' */
 /* 279 */ { "xOderzhimost'", 5, 79 },	/* oderzhimost' */
 /* 280 */ { "xManiya", 5, 80 },	/* maniya */
 /* 281 */ { "xZainteresovannost'", 5, 81 },	/* zainteresovannost' */
 /* 282 */ { "xZavisimost'", 5, 82 },	/* zavisimost' */
 /* 283 */ { "xDuhovnoe", 5, 83 },	/* duhovnoe */
 /* 284 */ { "xTrans", 5, 84 },	/* trans */
 /* 285 */ { "xMeditatsiya", 5, 85 },	/* meditatsiya */
 /* 286 */ { "xEkzal'tatsiya", 5, 86 },	/* ekzal'tatsiya */
 /* 287 */ { "xProsvetlenie", 5, 87 },	/* prosvetlenie */
	// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
