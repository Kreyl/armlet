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
 * Generated at 2014-07-18 22:00:43
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
 /*  8 */     { "negativ", 1, 0, -1, -1 },
 /*  9 */         { "ravnodushie", 1, 8, -1, -1 },
 /* 10 */             { "skuka", 1, 9, -1, -1 },
 /* 11 */                 { "apatiya", 1, 10, -1, -1 },
 /* 12 */                     { "depressiya", 1, 11, -1, -1 },
 /* 13 */         { "zlost'", 1, 8, -1, -1 },
 /* 14 */             { "gnev", 1, 13, -1, -1 },
 /* 15 */                 { "yarost'", 1, 14, -1, -1 },
 /* 16 */             { "razdrazhenie", 1, 13, -1, -1 },
 /* 17 */         { "grust'", 1, 8, -1, -1 },
 /* 18 */             { "toska", 1, 17, -1, -1 },
 /* 19 */                 { "obrechennost'", 1, 18, -1, -1 },
 /* 20 */                 { "otchayanie", 1, 18, -1, -1 },
 /* 21 */         { "neuverennost'", 1, 8, -1, -1 },
 /* 22 */             { "ozabochennost'", 1, 21, -1, -1 },
 /* 23 */                 { "napryazhennost'", 1, 22, -1, -1 },
 /* 24 */                     { "isterika", 1, 23, -1, -1 },
 /* 25 */             { "somnenie", 1, 21, -1, -1 },
 /* 26 */         { "trevozhnost'", 1, 8, -1, -1 },
 /* 27 */             { "strah", 1, 26, -1, -1 },
 /* 28 */                 { "uzhas", 1, 27, -1, -1 },
 /* 29 */                     { "panika", 1, 28, -1, -1 },
 /* 30 */                         { "fobiya", 1, 29, -1, -1 },
 /* 31 */                         { "paranojya", 1, 29, -1, -1 },
 /* 32 */         { "nepriyazn'", 1, 8, -1, -1 },
 /* 33 */             { "nenavist'", 1, 32, -1, -1 },
 /* 34 */             { "otvraschenie", 1, 32, -1, -1 },
 /* 35 */             { "prezrenie", 1, 32, -1, -1 },
 /* 36 */         { "neudovletvorennost'", 1, 8, -1, -1 },
 /* 37 */         { "sozhalenie", 1, 8, -1, -1 },
 /* 38 */             { "styid", 1, 37, -1, -1 },
 /* 39 */     { "razrushenie", 1, 0, -1, -1 },
 /* 40 */     { "draka", 1, 0, -1, -1 },
 /* 41 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 42 */     { "pozitiv", 1, 0, -1, -1 },
 /* 43 */         { "vesel'e", 1, 42, -1, -1 },
 /* 44 */             { "radost'", 1, 43, -1, -1 },
 /* 45 */                 { "vostorg", 1, 44, -1, -1 },
 /* 46 */         { "dovol'stvo", 1, 42, -1, -1 },
 /* 47 */             { "gordost'", 1, 46, -1, -1 },
 /* 48 */             { "udovletvorenie", 1, 46, -1, -1 },
 /* 49 */         { "lyubopyitstvo", 1, 42, -1, -1 },
 /* 50 */             { "interes", 1, 49, -1, -1 },
 /* 51 */             { "uvlechennost'", 1, 49, -1, -1 },
 /* 52 */                 { "azart", 1, 51, -1, -1 },
 /* 53 */                 { "voodushevlenie", 1, 51, -1, -1 },
 /* 54 */                     { "tseleustremlennost'", 1, 53, -1, -1 },
 /* 55 */                     { "otvaga", 1, 53, -1, -1 },
 /* 56 */                     { "reshimost'", 1, 53, -1, -1 },
 /* 57 */                 { "vdohnovenie", 1, 51, -1, -1 },
 /* 58 */         { "priyazn'", 1, 42, -1, -1 },
 /* 59 */             { "simpatiya", 1, 58, -1, -1 },
 /* 60 */                 { "privyazannost'", 1, 59, -1, -1 },
 /* 61 */                     { "lyubov'", 1, 60, -1, -1 },
 /* 62 */                 { "vozhdelenie", 1, 59, -1, -1 },
 /* 63 */             { "umilenie", 1, 58, -1, -1 },
 /* 64 */         { "nadezhda", 1, 42, -1, -1 },
 /* 65 */             { "predvkushenie", 1, 64, -1, -1 },
 /* 66 */                 { "vera", 1, 65, -1, -1 },
 /* 67 */                 { "uverennost'", 1, 65, -1, -1 },
 /* 68 */         { "pokoj", 1, 42, -1, -1 },
 /* 69 */             { "umirotvorenie", 1, 68, -1, -1 },
 /* 70 */             { "nostal'giya", 1, 68, -1, -1 },
 /* 71 */         { "soperezhivanie", 1, 42, -1, -1 },
 /* 72 */             { "sostradanie", 1, 71, -1, -1 },
 /* 73 */     { "sozidanie", 1, 0, -1, -1 },
 /* 74 */     { "seks", 1, 0, -1, -1 },
 /* 75 */     { "zhelanie", 1, 0, -1, -1 },
 /* 76 */         { "vlechenie", 1, 75, -1, -1 },
 /* 77 */             { "strast'", 1, 76, -1, -1 },
 /* 78 */                 { "oderzhimost'", 1, 77, -1, -1 },
 /* 79 */                     { "maniya", 1, 78, -1, -1 },
 /* 80 */         { "zainteresovannost'", 1, 75, -1, -1 },
 /* 81 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 82 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 83 */         { "trans", 1, 82, -1, -1 },
 /* 84 */         { "meditatsiya", 1, 82, -1, -1 },
 /* 85 */         { "ekzal'tatsiya", 1, 82, -1, -1 },
 /* 86 */         { "prosvetlenie", 1, 82, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "r000", 99, 1 },	/* neverno */
	// Locations
 /*   1 */ { "hub", 99, 2 },	/* masterka */
 /*   2 */ { "uliza", 1, 0 },	/* fon */
 /*   3 */ { "grave", 3, 27 },	/* strah */
 /*   4 */ { "church", 2, 42 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 8 },	/* negativ */
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
 /* 104 */ { "JChamber", 0, 0 },
 /* 105 */ { "SClayton", 0, 0 },
 /* 106 */ { "JCollins", 0, 0 },
 /* 107 */ { "CComes", 0, 0 },
 /* 108 */ { "RConnor", 0, 0 },
 /* 109 */ { "GCross", 0, 0 },
 /* 110 */ { "FCunningham", 0, 0 },
 /* 111 */ { "JDawson", 0, 0 },
 /* 112 */ { "GDay", 0, 0 },
 /* 113 */ { "CDeLorean", 0, 0 },
 /* 114 */ { "ADecker", 0, 0 },
 /* 115 */ { "MDurden", 0, 0 },
 /* 116 */ { "NEpstein", 0, 0 },
 /* 117 */ { "CFranklin", 0, 0 },
 /* 118 */ { "RGilmore", 0, 0 },
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
 /* 137 */ { "MReed", 0, 0 },
 /* 138 */ { "MRose", 0, 0 },
 /* 139 */ { "JSherman", 0, 0 },
 /* 140 */ { "CStanfield", 0, 0 },
 /* 141 */ { "DStanley", 0, 0 },
 /* 142 */ { "TSunday", 0, 0 },
 /* 143 */ { "WThomas", 0, 0 },
 /* 144 */ { "EThorp", 0, 0 },
 /* 145 */ { "HWilliams", 0, 0 },
 /* 146 */ { "EWilliams", 0, 0 },
 /* 147 */ { "LYoung", 0, 0 },
	// end of characters
 /* 148 */ { "r148", 99, 1 },	/* neverno */
 /* 149 */ { "r149", 99, 1 },	/* neverno */
 /* 150 */ { "r150", 99, 1 },	/* neverno */
 /* 151 */ { "r151", 99, 1 },	/* neverno */
 /* 152 */ { "r152", 99, 1 },	/* neverno */
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
 /* 180 */ { "murder", 1, 41 },	/* ubijstvo */
 /* 181 */ { "creation", 5, 73 },	/* sozidanie */
 /* 182 */ { "destruction", 5, 39 },	/* razrushenie */
 /* 183 */ { "sex", 9, 74 },	/* seks */
 /* 184 */ { "fight", 50, 40 },	/* draka */
 /* 185 */ { "heartbeat", 50, 7 },	/* serdtsebienie */
 /* 186 */ { "drug", 20, 81 },	/* zavisimost' */
 /* 187 */ { "death", 50, 5 },	/* smert' */
 /* 188 */ { "mist", 50, 4 },	/* tuman */
 /* 189 */ { "fear", 40, 27 },	/* strah */
 /* 190 */ { "mSource", 9, 1 },	/* neverno */
 /* 191 */ { "mProject", 9, 1 },	/* neverno */
	// end of intentions
 /* 192 */ { "r192", 99, 1 },	/* neverno */
 /* 193 */ { "r193", 99, 1 },	/* neverno */
 /* 194 */ { "r194", 99, 1 },	/* neverno */
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
 /* 208 */ { "xNegativ", 5, 8 },	/* negativ */
 /* 209 */ { "xRavnodushie", 5, 9 },	/* ravnodushie */
 /* 210 */ { "xSkuka", 5, 10 },	/* skuka */
 /* 211 */ { "xApatiya", 5, 11 },	/* apatiya */
 /* 212 */ { "xDepressiya", 5, 12 },	/* depressiya */
 /* 213 */ { "xZlost'", 5, 13 },	/* zlost' */
 /* 214 */ { "xGnev", 5, 14 },	/* gnev */
 /* 215 */ { "xYarost'", 5, 15 },	/* yarost' */
 /* 216 */ { "xRazdrazhenie", 5, 16 },	/* razdrazhenie */
 /* 217 */ { "xGrust'", 5, 17 },	/* grust' */
 /* 218 */ { "xToska", 5, 18 },	/* toska */
 /* 219 */ { "xObrechennost'", 5, 19 },	/* obrechennost' */
 /* 220 */ { "xOtchayanie", 5, 20 },	/* otchayanie */
 /* 221 */ { "xNeuverennost'", 5, 21 },	/* neuverennost' */
 /* 222 */ { "xOzabochennost'", 5, 22 },	/* ozabochennost' */
 /* 223 */ { "xNapryazhennost'", 5, 23 },	/* napryazhennost' */
 /* 224 */ { "xIsterika", 5, 24 },	/* isterika */
 /* 225 */ { "xSomnenie", 5, 25 },	/* somnenie */
 /* 226 */ { "xTrevozhnost'", 5, 26 },	/* trevozhnost' */
 /* 227 */ { "xStrah", 5, 27 },	/* strah */
 /* 228 */ { "xUzhas", 5, 28 },	/* uzhas */
 /* 229 */ { "xPanika", 5, 29 },	/* panika */
 /* 230 */ { "xFobiya", 5, 30 },	/* fobiya */
 /* 231 */ { "xParanojya", 5, 31 },	/* paranojya */
 /* 232 */ { "xNepriyazn'", 5, 32 },	/* nepriyazn' */
 /* 233 */ { "xNenavist'", 5, 33 },	/* nenavist' */
 /* 234 */ { "xOtvraschenie", 5, 34 },	/* otvraschenie */
 /* 235 */ { "xPrezrenie", 5, 35 },	/* prezrenie */
 /* 236 */ { "xNeudovletvorennost'", 5, 36 },	/* neudovletvorennost' */
 /* 237 */ { "xSozhalenie", 5, 37 },	/* sozhalenie */
 /* 238 */ { "xStyid", 5, 38 },	/* styid */
 /* 239 */ { "xRazrushenie", 5, 39 },	/* razrushenie */
 /* 240 */ { "xDraka", 5, 40 },	/* draka */
 /* 241 */ { "xUbijstvo", 5, 41 },	/* ubijstvo */
 /* 242 */ { "xPozitiv", 5, 42 },	/* pozitiv */
 /* 243 */ { "xVesel'e", 5, 43 },	/* vesel'e */
 /* 244 */ { "xRadost'", 5, 44 },	/* radost' */
 /* 245 */ { "xVostorg", 5, 45 },	/* vostorg */
 /* 246 */ { "xDovol'stvo", 5, 46 },	/* dovol'stvo */
 /* 247 */ { "xGordost'", 5, 47 },	/* gordost' */
 /* 248 */ { "xUdovletvorenie", 5, 48 },	/* udovletvorenie */
 /* 249 */ { "xLyubopyitstvo", 5, 49 },	/* lyubopyitstvo */
 /* 250 */ { "xInteres", 5, 50 },	/* interes */
 /* 251 */ { "xUvlechennost'", 5, 51 },	/* uvlechennost' */
 /* 252 */ { "xAzart", 5, 52 },	/* azart */
 /* 253 */ { "xVoodushevlenie", 5, 53 },	/* voodushevlenie */
 /* 254 */ { "xTseleustremlennost'", 5, 54 },	/* tseleustremlennost' */
 /* 255 */ { "xOtvaga", 5, 55 },	/* otvaga */
 /* 256 */ { "xReshimost'", 5, 56 },	/* reshimost' */
 /* 257 */ { "xVdohnovenie", 5, 57 },	/* vdohnovenie */
 /* 258 */ { "xPriyazn'", 5, 58 },	/* priyazn' */
 /* 259 */ { "xSimpatiya", 5, 59 },	/* simpatiya */
 /* 260 */ { "xPrivyazannost'", 5, 60 },	/* privyazannost' */
 /* 261 */ { "xLyubov'", 5, 61 },	/* lyubov' */
 /* 262 */ { "xVozhdelenie", 5, 62 },	/* vozhdelenie */
 /* 263 */ { "xUmilenie", 5, 63 },	/* umilenie */
 /* 264 */ { "xNadezhda", 5, 64 },	/* nadezhda */
 /* 265 */ { "xPredvkushenie", 5, 65 },	/* predvkushenie */
 /* 266 */ { "xVera", 5, 66 },	/* vera */
 /* 267 */ { "xUverennost'", 5, 67 },	/* uverennost' */
 /* 268 */ { "xPokoj", 5, 68 },	/* pokoj */
 /* 269 */ { "xUmirotvorenie", 5, 69 },	/* umirotvorenie */
 /* 270 */ { "xNostal'giya", 5, 70 },	/* nostal'giya */
 /* 271 */ { "xSoperezhivanie", 5, 71 },	/* soperezhivanie */
 /* 272 */ { "xSostradanie", 5, 72 },	/* sostradanie */
 /* 273 */ { "xSozidanie", 5, 73 },	/* sozidanie */
 /* 274 */ { "xSeks", 5, 74 },	/* seks */
 /* 275 */ { "xZhelanie", 5, 75 },	/* zhelanie */
 /* 276 */ { "xVlechenie", 5, 76 },	/* vlechenie */
 /* 277 */ { "xStrast'", 5, 77 },	/* strast' */
 /* 278 */ { "xOderzhimost'", 5, 78 },	/* oderzhimost' */
 /* 279 */ { "xManiya", 5, 79 },	/* maniya */
 /* 280 */ { "xZainteresovannost'", 5, 80 },	/* zainteresovannost' */
 /* 281 */ { "xZavisimost'", 5, 81 },	/* zavisimost' */
 /* 282 */ { "xDuhovnoe", 5, 82 },	/* duhovnoe */
 /* 283 */ { "xTrans", 5, 83 },	/* trans */
 /* 284 */ { "xMeditatsiya", 5, 84 },	/* meditatsiya */
 /* 285 */ { "xEkzal'tatsiya", 5, 85 },	/* ekzal'tatsiya */
 /* 286 */ { "xProsvetlenie", 5, 86 },	/* prosvetlenie */
	// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
