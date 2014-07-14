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
 * Generated at 2014-07-12 13:29:30
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
 /* 17 */                 { "otchayanie", 1, 15, -1, -1 },
 /* 18 */         { "neuverennost'", 1, 5, -1, -1 },
 /* 19 */             { "ozabochennost'", 1, 18, -1, -1 },
 /* 20 */                 { "napryazhennost'", 1, 19, -1, -1 },
 /* 21 */                     { "isterika", 1, 20, -1, -1 },
 /* 22 */             { "somnenie", 1, 18, -1, -1 },
 /* 23 */         { "trevozhnost'", 1, 5, -1, -1 },
 /* 24 */             { "strah", 1, 23, -1, -1 },
 /* 25 */                 { "uzhas", 1, 24, -1, -1 },
 /* 26 */                     { "panika", 1, 25, -1, -1 },
 /* 27 */                         { "fobiya", 1, 26, -1, -1 },
 /* 28 */                         { "paranojya", 1, 26, -1, -1 },
 /* 29 */         { "nepriyazn'", 1, 5, -1, -1 },
 /* 30 */             { "nenavist'", 1, 29, -1, -1 },
 /* 31 */             { "otvraschenie", 1, 29, -1, -1 },
 /* 32 */             { "prezrenie", 1, 29, -1, -1 },
 /* 33 */         { "neudovletvorennost'", 1, 5, -1, -1 },
 /* 34 */         { "sozhalenie", 1, 5, -1, -1 },
 /* 35 */             { "styid", 1, 34, -1, -1 },
 /* 36 */     { "razrushenie", 1, 0, -1, -1 },
 /* 37 */     { "draka", 1, 0, -1, -1 },
 /* 38 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 39 */     { "pozitiv", 1, 0, -1, -1 },
 /* 40 */         { "vesel'e", 1, 39, -1, -1 },
 /* 41 */             { "radost'", 1, 40, -1, -1 },
 /* 42 */                 { "vostorg", 1, 41, -1, -1 },
 /* 43 */         { "dovol'stvo", 1, 39, -1, -1 },
 /* 44 */             { "gordost'", 1, 43, -1, -1 },
 /* 45 */             { "udovletvorenie", 1, 43, -1, -1 },
 /* 46 */         { "lyubopyitstvo", 1, 39, -1, -1 },
 /* 47 */             { "interes", 1, 46, -1, -1 },
 /* 48 */             { "uvlechennost'", 1, 46, -1, -1 },
 /* 49 */                 { "azart", 1, 48, -1, -1 },
 /* 50 */                 { "voodushevlenie", 1, 48, -1, -1 },
 /* 51 */                     { "tseleustremlennost'", 1, 50, -1, -1 },
 /* 52 */                     { "otvaga", 1, 50, -1, -1 },
 /* 53 */                     { "reshimost'", 1, 50, -1, -1 },
 /* 54 */                 { "vdohnovenie", 1, 48, -1, -1 },
 /* 55 */         { "priyazn'", 1, 39, -1, -1 },
 /* 56 */             { "simpatiya", 1, 55, -1, -1 },
 /* 57 */                 { "privyazannost'", 1, 56, -1, -1 },
 /* 58 */                     { "lyubov'", 1, 57, -1, -1 },
 /* 59 */                 { "vozhdelenie", 1, 56, -1, -1 },
 /* 60 */             { "umilenie", 1, 55, -1, -1 },
 /* 61 */         { "nadezhda", 1, 39, -1, -1 },
 /* 62 */             { "predvkushenie", 1, 61, -1, -1 },
 /* 63 */                 { "vera", 1, 62, -1, -1 },
 /* 64 */                 { "uverennost'", 1, 62, -1, -1 },
 /* 65 */         { "pokoj", 1, 39, -1, -1 },
 /* 66 */             { "umirotvorenie", 1, 65, -1, -1 },
 /* 67 */             { "nostal'giya", 1, 65, -1, -1 },
 /* 68 */         { "soperezhivanie", 1, 39, -1, -1 },
 /* 69 */             { "sostradanie", 1, 68, -1, -1 },
 /* 70 */     { "sozidanie", 1, 0, -1, -1 },
 /* 71 */     { "seks", 1, 0, -1, -1 },
 /* 72 */     { "zhelanie", 1, 0, -1, -1 },
 /* 73 */         { "vlechenie", 1, 72, -1, -1 },
 /* 74 */             { "strast'", 1, 73, -1, -1 },
 /* 75 */                 { "oderzhimost'", 1, 74, -1, -1 },
 /* 76 */                     { "maniya", 1, 75, -1, -1 },
 /* 77 */         { "zainteresovannost'", 1, 72, -1, -1 },
 /* 78 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 79 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 80 */         { "trans", 1, 79, -1, -1 },
 /* 81 */         { "meditatsiya", 1, 79, -1, -1 },
 /* 82 */         { "ekzal'tatsiya", 1, 79, -1, -1 },
 /* 83 */         { "prosvetlenie", 1, 79, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "r000", 99, 1 },	/* wrong */
	// Locations
 /*   1 */ { "hub", 99, 2 },	/* master */
 /*   2 */ { "uliza", 1, 0 },	/* fon */
 /*   3 */ { "grave", 9, 24 },	/* strah */
 /*   4 */ { "church", 2, 39 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 5 },	/* negativ */
	// end of locations
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
 /* 101 */ { "MParcs", 0, 0 },
 /* 102 */ { "JDawson", 0, 0 },
 /* 103 */ { "AHuxley", 0, 0 },
 /* 104 */ { "AGraywood", 0, 0 },
 /* 105 */ { "BHill", 0, 0 },
 /* 106 */ { "TMalone", 0, 0 },
 /* 107 */ { "SClayton", 0, 0 },
 /* 108 */ { "MDurden", 0, 0 },
 /* 109 */ { "NEpstein", 0, 0 },
 /* 110 */ { "TSunday", 0, 0 },
 /* 111 */ { "VLehndorff", 0, 0 },
 /* 112 */ { "HWilliams", 0, 0 },
 /* 113 */ { "GCross", 0, 0 },
 /* 114 */ { "JChamber", 0, 0 },
 /* 115 */ { "EWilliams", 0, 0 },
 /* 116 */ { "WThomas", 0, 0 },
 /* 117 */ { "JOSullivan", 0, 0 },
 /* 118 */ { "UMcGee", 0, 0 },
 /* 119 */ { "DStanley", 0, 0 },
 /* 120 */ { "JCollins", 0, 0 },
 /* 121 */ { "AJoe", 0, 0 },
 /* 122 */ { "RGilmore", 0, 0 },
 /* 123 */ { "CLefebvre", 0, 0 },
 /* 124 */ { "LYoung", 0, 0 },
 /* 125 */ { "CDeLorean", 0, 0 },
 /* 126 */ { "CStanfield", 0, 0 },
 /* 127 */ { "FCunningham", 0, 0 },
 /* 128 */ { "ADecker", 0, 0 },
 /* 129 */ { "MHailey", 0, 0 },
 /* 130 */ { "PAdams", 0, 0 },
 /* 131 */ { "JDay", 0, 0 },
 /* 132 */ { "LLane", 0, 0 },
 /* 133 */ { "FBuster", 0, 0 },
 /* 134 */ { "ZHarriot", 0, 0 },
 /* 135 */ { "CComes", 0, 0 },
 /* 136 */ { "ZCampbell", 0, 0 },
 /* 137 */ { "MReed", 0, 0 },
 /* 138 */ { "KGreenhall", 0, 0 },
	// end of characters
 /* 139 */ { "r139", 99, 1 },	/* wrong */
 /* 140 */ { "r140", 99, 1 },	/* wrong */
 /* 141 */ { "r141", 99, 1 },	/* wrong */
 /* 142 */ { "r142", 99, 1 },	/* wrong */
 /* 143 */ { "r143", 99, 1 },	/* wrong */
 /* 144 */ { "r144", 99, 1 },	/* wrong */
 /* 145 */ { "r145", 99, 1 },	/* wrong */
 /* 146 */ { "r146", 99, 1 },	/* wrong */
 /* 147 */ { "r147", 99, 1 },	/* wrong */
 /* 148 */ { "r148", 99, 1 },	/* wrong */
 /* 149 */ { "r149", 99, 1 },	/* wrong */
 /* 150 */ { "r150", 99, 1 },	/* wrong */
 /* 151 */ { "r151", 99, 1 },	/* wrong */
 /* 152 */ { "r152", 99, 1 },	/* wrong */
 /* 153 */ { "r153", 99, 1 },	/* wrong */
 /* 154 */ { "r154", 99, 1 },	/* wrong */
 /* 155 */ { "r155", 99, 1 },	/* wrong */
 /* 156 */ { "r156", 99, 1 },	/* wrong */
 /* 157 */ { "r157", 99, 1 },	/* wrong */
 /* 158 */ { "r158", 99, 1 },	/* wrong */
 /* 159 */ { "r159", 99, 1 },	/* wrong */
 /* 160 */ { "r160", 99, 1 },	/* wrong */
 /* 161 */ { "r161", 99, 1 },	/* wrong */
 /* 162 */ { "r162", 99, 1 },	/* wrong */
 /* 163 */ { "r163", 99, 1 },	/* wrong */
 /* 164 */ { "r164", 99, 1 },	/* wrong */
 /* 165 */ { "r165", 99, 1 },	/* wrong */
 /* 166 */ { "r166", 99, 1 },	/* wrong */
 /* 167 */ { "r167", 99, 1 },	/* wrong */
 /* 168 */ { "r168", 99, 1 },	/* wrong */
 /* 169 */ { "r169", 99, 1 },	/* wrong */
 /* 170 */ { "r170", 99, 1 },	/* wrong */
 /* 171 */ { "r171", 99, 1 },	/* wrong */
 /* 172 */ { "r172", 99, 1 },	/* wrong */
 /* 173 */ { "r173", 99, 1 },	/* wrong */
 /* 174 */ { "r174", 99, 1 },	/* wrong */
 /* 175 */ { "r175", 99, 1 },	/* wrong */
 /* 176 */ { "r176", 99, 1 },	/* wrong */
 /* 177 */ { "r177", 99, 1 },	/* wrong */
 /* 178 */ { "r178", 99, 1 },	/* wrong */
 /* 179 */ { "r179", 99, 1 },	/* wrong */
 /* 180 */ { "r180", 99, 1 },	/* wrong */
 /* 181 */ { "r181", 99, 1 },	/* wrong */
 /* 182 */ { "r182", 99, 1 },	/* wrong */
 /* 183 */ { "r183", 99, 1 },	/* wrong */
 /* 184 */ { "r184", 99, 1 },	/* wrong */
 /* 185 */ { "r185", 99, 1 },	/* wrong */
 /* 186 */ { "r186", 99, 1 },	/* wrong */
 /* 187 */ { "r187", 99, 1 },	/* wrong */
 /* 188 */ { "r188", 99, 1 },	/* wrong */
 /* 189 */ { "r189", 99, 1 },	/* wrong */
	// Intentions
 /* 190 */ { "murder", 1, 38 },	/* ubijstvo */
 /* 191 */ { "creation", 5, 70 },	/* sozidanie */
 /* 192 */ { "destruction", 5, 36 },	/* razrushenie */
 /* 193 */ { "sex", 9, 71 },	/* seks */
 /* 194 */ { "fight", 50, 37 },	/* draka */
 /* 195 */ { "drug", 20, 78 },	/* zavisimost' */
	// end of intentions
 /* 196 */ { "r196", 99, 1 },	/* wrong */
 /* 197 */ { "r197", 99, 1 },	/* wrong */
 /* 198 */ { "r198", 99, 1 },	/* wrong */
 /* 199 */ { "r199", 99, 1 },	/* wrong */
	// Emotion fixes
 /* 200 */ { "xFon", 5, 0 },	/* fon */
 /* 201 */ { "xWrong", 5, 1 },	/* wrong */
 /* 202 */ { "xMaster", 5, 2 },	/* master */
 /* 203 */ { "xSilence", 5, 3 },	/* silence */
 /* 204 */ { "xTuman", 5, 4 },	/* tuman */
 /* 205 */ { "xNegativ", 5, 5 },	/* negativ */
 /* 206 */ { "xRavnodushie", 5, 6 },	/* ravnodushie */
 /* 207 */ { "xSkuka", 5, 7 },	/* skuka */
 /* 208 */ { "xApatiya", 5, 8 },	/* apatiya */
 /* 209 */ { "xDepressiya", 5, 9 },	/* depressiya */
 /* 210 */ { "xZlost'", 5, 10 },	/* zlost' */
 /* 211 */ { "xGnev", 5, 11 },	/* gnev */
 /* 212 */ { "xYarost'", 5, 12 },	/* yarost' */
 /* 213 */ { "xRazdrazhenie", 5, 13 },	/* razdrazhenie */
 /* 214 */ { "xGrust'", 5, 14 },	/* grust' */
 /* 215 */ { "xToska", 5, 15 },	/* toska */
 /* 216 */ { "xObrechennost'", 5, 16 },	/* obrechennost' */
 /* 217 */ { "xOtchayanie", 5, 17 },	/* otchayanie */
 /* 218 */ { "xNeuverennost'", 5, 18 },	/* neuverennost' */
 /* 219 */ { "xOzabochennost'", 5, 19 },	/* ozabochennost' */
 /* 220 */ { "xNapryazhennost'", 5, 20 },	/* napryazhennost' */
 /* 221 */ { "xIsterika", 5, 21 },	/* isterika */
 /* 222 */ { "xSomnenie", 5, 22 },	/* somnenie */
 /* 223 */ { "xTrevozhnost'", 5, 23 },	/* trevozhnost' */
 /* 224 */ { "xStrah", 5, 24 },	/* strah */
 /* 225 */ { "xUzhas", 5, 25 },	/* uzhas */
 /* 226 */ { "xPanika", 5, 26 },	/* panika */
 /* 227 */ { "xFobiya", 5, 27 },	/* fobiya */
 /* 228 */ { "xParanojya", 5, 28 },	/* paranojya */
 /* 229 */ { "xNepriyazn'", 5, 29 },	/* nepriyazn' */
 /* 230 */ { "xNenavist'", 5, 30 },	/* nenavist' */
 /* 231 */ { "xOtvraschenie", 5, 31 },	/* otvraschenie */
 /* 232 */ { "xPrezrenie", 5, 32 },	/* prezrenie */
 /* 233 */ { "xNeudovletvorennost'", 5, 33 },	/* neudovletvorennost' */
 /* 234 */ { "xSozhalenie", 5, 34 },	/* sozhalenie */
 /* 235 */ { "xStyid", 5, 35 },	/* styid */
 /* 236 */ { "xRazrushenie", 5, 36 },	/* razrushenie */
 /* 237 */ { "xDraka", 5, 37 },	/* draka */
 /* 238 */ { "xUbijstvo", 5, 38 },	/* ubijstvo */
 /* 239 */ { "xPozitiv", 5, 39 },	/* pozitiv */
 /* 240 */ { "xVesel'e", 5, 40 },	/* vesel'e */
 /* 241 */ { "xRadost'", 5, 41 },	/* radost' */
 /* 242 */ { "xVostorg", 5, 42 },	/* vostorg */
 /* 243 */ { "xDovol'stvo", 5, 43 },	/* dovol'stvo */
 /* 244 */ { "xGordost'", 5, 44 },	/* gordost' */
 /* 245 */ { "xUdovletvorenie", 5, 45 },	/* udovletvorenie */
 /* 246 */ { "xLyubopyitstvo", 5, 46 },	/* lyubopyitstvo */
 /* 247 */ { "xInteres", 5, 47 },	/* interes */
 /* 248 */ { "xUvlechennost'", 5, 48 },	/* uvlechennost' */
 /* 249 */ { "xAzart", 5, 49 },	/* azart */
 /* 250 */ { "xVoodushevlenie", 5, 50 },	/* voodushevlenie */
 /* 251 */ { "xTseleustremlennost'", 5, 51 },	/* tseleustremlennost' */
 /* 252 */ { "xOtvaga", 5, 52 },	/* otvaga */
 /* 253 */ { "xReshimost'", 5, 53 },	/* reshimost' */
 /* 254 */ { "xVdohnovenie", 5, 54 },	/* vdohnovenie */
 /* 255 */ { "xPriyazn'", 5, 55 },	/* priyazn' */
 /* 256 */ { "xSimpatiya", 5, 56 },	/* simpatiya */
 /* 257 */ { "xPrivyazannost'", 5, 57 },	/* privyazannost' */
 /* 258 */ { "xLyubov'", 5, 58 },	/* lyubov' */
 /* 259 */ { "xVozhdelenie", 5, 59 },	/* vozhdelenie */
 /* 260 */ { "xUmilenie", 5, 60 },	/* umilenie */
 /* 261 */ { "xNadezhda", 5, 61 },	/* nadezhda */
 /* 262 */ { "xPredvkushenie", 5, 62 },	/* predvkushenie */
 /* 263 */ { "xVera", 5, 63 },	/* vera */
 /* 264 */ { "xUverennost'", 5, 64 },	/* uverennost' */
 /* 265 */ { "xPokoj", 5, 65 },	/* pokoj */
 /* 266 */ { "xUmirotvorenie", 5, 66 },	/* umirotvorenie */
 /* 267 */ { "xNostal'giya", 5, 67 },	/* nostal'giya */
 /* 268 */ { "xSoperezhivanie", 5, 68 },	/* soperezhivanie */
 /* 269 */ { "xSostradanie", 5, 69 },	/* sostradanie */
 /* 270 */ { "xSozidanie", 5, 70 },	/* sozidanie */
 /* 271 */ { "xSeks", 5, 71 },	/* seks */
 /* 272 */ { "xZhelanie", 5, 72 },	/* zhelanie */
 /* 273 */ { "xVlechenie", 5, 73 },	/* vlechenie */
 /* 274 */ { "xStrast'", 5, 74 },	/* strast' */
 /* 275 */ { "xOderzhimost'", 5, 75 },	/* oderzhimost' */
 /* 276 */ { "xManiya", 5, 76 },	/* maniya */
 /* 277 */ { "xZainteresovannost'", 5, 77 },	/* zainteresovannost' */
 /* 278 */ { "xZavisimost'", 5, 78 },	/* zavisimost' */
 /* 279 */ { "xDuhovnoe", 5, 79 },	/* duhovnoe */
 /* 280 */ { "xTrans", 5, 80 },	/* trans */
 /* 281 */ { "xMeditatsiya", 5, 81 },	/* meditatsiya */
 /* 282 */ { "xEkzal'tatsiya", 5, 82 },	/* ekzal'tatsiya */
 /* 283 */ { "xProsvetlenie", 5, 83 },	/* prosvetlenie */
	// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
