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
 * Generated at 2014-07-15 04:27:07
 */
#include "emotions.h"

// EIDs are indexes in this array.
Emotion_t emotions[] = {
 /*  0 */ { "fon", 1, ROOT, -1, -1 },
 /*  1 */     { "wrong", 1, 0, -1, -1 },
 /*  2 */     { "master", 1, 0, -1, -1 },
 /*  3 */     { "silence", 1, 0, -1, -1 },
 /*  4 */     { "tuman", 1, 0, -1, -1 },
 /*  5 */     { "smert'", 1, 0, -1, -1 },
 /*  6 */     { "mertvyak", 1, 0, -1, -1 },
 /*  7 */     { "negativ", 1, 0, -1, -1 },
 /*  8 */         { "ravnodushie", 1, 7, -1, -1 },
 /*  9 */             { "skuka", 1, 8, -1, -1 },
 /* 10 */                 { "apatiya", 1, 9, -1, -1 },
 /* 11 */                     { "depressiya", 1, 10, -1, -1 },
 /* 12 */         { "zlost'", 1, 7, -1, -1 },
 /* 13 */             { "gnev", 1, 12, -1, -1 },
 /* 14 */                 { "yarost'", 1, 13, -1, -1 },
 /* 15 */             { "razdrazhenie", 1, 12, -1, -1 },
 /* 16 */         { "grust'", 1, 7, -1, -1 },
 /* 17 */             { "toska", 1, 16, -1, -1 },
 /* 18 */                 { "obrechennost'", 1, 17, -1, -1 },
 /* 19 */                 { "otchayanie", 1, 17, -1, -1 },
 /* 20 */         { "neuverennost'", 1, 7, -1, -1 },
 /* 21 */             { "ozabochennost'", 1, 20, -1, -1 },
 /* 22 */                 { "napryazhennost'", 1, 21, -1, -1 },
 /* 23 */                     { "isterika", 1, 22, -1, -1 },
 /* 24 */             { "somnenie", 1, 20, -1, -1 },
 /* 25 */         { "trevozhnost'", 1, 7, -1, -1 },
 /* 26 */             { "strah", 1, 25, -1, -1 },
 /* 27 */                 { "uzhas", 1, 26, -1, -1 },
 /* 28 */                     { "panika", 1, 27, -1, -1 },
 /* 29 */                         { "fobiya", 1, 28, -1, -1 },
 /* 30 */                         { "paranojya", 1, 28, -1, -1 },
 /* 31 */         { "nepriyazn'", 1, 7, -1, -1 },
 /* 32 */             { "nenavist'", 1, 31, -1, -1 },
 /* 33 */             { "otvraschenie", 1, 31, -1, -1 },
 /* 34 */             { "prezrenie", 1, 31, -1, -1 },
 /* 35 */         { "neudovletvorennost'", 1, 7, -1, -1 },
 /* 36 */         { "sozhalenie", 1, 7, -1, -1 },
 /* 37 */             { "styid", 1, 36, -1, -1 },
 /* 38 */     { "razrushenie", 1, 0, -1, -1 },
 /* 39 */     { "draka", 1, 0, -1, -1 },
 /* 40 */     { "ubijstvo", 1, 0, -1, -1 },
 /* 41 */     { "pozitiv", 1, 0, -1, -1 },
 /* 42 */         { "vesel'e", 1, 41, -1, -1 },
 /* 43 */             { "radost'", 1, 42, -1, -1 },
 /* 44 */                 { "vostorg", 1, 43, -1, -1 },
 /* 45 */         { "dovol'stvo", 1, 41, -1, -1 },
 /* 46 */             { "gordost'", 1, 45, -1, -1 },
 /* 47 */             { "udovletvorenie", 1, 45, -1, -1 },
 /* 48 */         { "lyubopyitstvo", 1, 41, -1, -1 },
 /* 49 */             { "interes", 1, 48, -1, -1 },
 /* 50 */             { "uvlechennost'", 1, 48, -1, -1 },
 /* 51 */                 { "azart", 1, 50, -1, -1 },
 /* 52 */                 { "voodushevlenie", 1, 50, -1, -1 },
 /* 53 */                     { "tseleustremlennost'", 1, 52, -1, -1 },
 /* 54 */                     { "otvaga", 1, 52, -1, -1 },
 /* 55 */                     { "reshimost'", 1, 52, -1, -1 },
 /* 56 */                 { "vdohnovenie", 1, 50, -1, -1 },
 /* 57 */         { "priyazn'", 1, 41, -1, -1 },
 /* 58 */             { "simpatiya", 1, 57, -1, -1 },
 /* 59 */                 { "privyazannost'", 1, 58, -1, -1 },
 /* 60 */                     { "lyubov'", 1, 59, -1, -1 },
 /* 61 */                 { "vozhdelenie", 1, 58, -1, -1 },
 /* 62 */             { "umilenie", 1, 57, -1, -1 },
 /* 63 */         { "nadezhda", 1, 41, -1, -1 },
 /* 64 */             { "predvkushenie", 1, 63, -1, -1 },
 /* 65 */                 { "vera", 1, 64, -1, -1 },
 /* 66 */                 { "uverennost'", 1, 64, -1, -1 },
 /* 67 */         { "pokoj", 1, 41, -1, -1 },
 /* 68 */             { "umirotvorenie", 1, 67, -1, -1 },
 /* 69 */             { "nostal'giya", 1, 67, -1, -1 },
 /* 70 */         { "soperezhivanie", 1, 41, -1, -1 },
 /* 71 */             { "sostradanie", 1, 70, -1, -1 },
 /* 72 */     { "sozidanie", 1, 0, -1, -1 },
 /* 73 */     { "seks", 1, 0, -1, -1 },
 /* 74 */     { "zhelanie", 1, 0, -1, -1 },
 /* 75 */         { "vlechenie", 1, 74, -1, -1 },
 /* 76 */             { "strast'", 1, 75, -1, -1 },
 /* 77 */                 { "oderzhimost'", 1, 76, -1, -1 },
 /* 78 */                     { "maniya", 1, 77, -1, -1 },
 /* 79 */         { "zainteresovannost'", 1, 74, -1, -1 },
 /* 80 */     { "zavisimost'", 1, 0, -1, -1 },
 /* 81 */     { "duhovnoe", 1, 0, -1, -1 },
 /* 82 */         { "trans", 1, 81, -1, -1 },
 /* 83 */         { "meditatsiya", 1, 81, -1, -1 },
 /* 84 */         { "ekzal'tatsiya", 1, 81, -1, -1 },
 /* 85 */         { "prosvetlenie", 1, 81, -1, -1 },
};

const int emotions_number = countof(emotions);

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "r000", 99, 1 },	/* wrong */
	// Locations
 /*   1 */ { "hub", 99, 2 },	/* master */
 /*   2 */ { "uliza", 1, 0 },	/* fon */
 /*   3 */ { "grave", 3, 26 },	/* strah */
 /*   4 */ { "church", 2, 41 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 7 },	/* negativ */
 /*   6 */ { "mSource", 9, 26 },	/* strah */
 /*   7 */ { "mProject", 9, 26 },	/* strah */
 /*   8 */ { "dead", 99, 6 },	/* mertvyak */
	// end of locations
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
	// Mist sources
 /*  90 */ { "m90", 99, 4 },	/* tuman */
 /*  91 */ { "m91", 99, 4 },	/* tuman */
 /*  92 */ { "m92", 99, 4 },	/* tuman */
 /*  93 */ { "m93", 99, 4 },	/* tuman */
 /*  94 */ { "m94", 99, 4 },	/* tuman */
 /*  95 */ { "m95", 99, 4 },	/* tuman */
 /*  96 */ { "m96", 99, 4 },	/* tuman */
 /*  97 */ { "m97", 99, 4 },	/* tuman */
 /*  98 */ { "m98", 99, 4 },	/* tuman */
 /*  99 */ { "m99", 99, 4 },	/* tuman */
	// end of mist sources
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
 /* 131 */ { "GDay", 0, 0 },
 /* 132 */ { "LLane", 0, 0 },
 /* 133 */ { "FBuster", 0, 0 },
 /* 134 */ { "ZHarriot", 0, 0 },
 /* 135 */ { "CComes", 0, 0 },
 /* 136 */ { "ZCampbell", 0, 0 },
 /* 137 */ { "MReed", 0, 0 },
 /* 138 */ { "KGreenhall", 0, 0 },
 /* 139 */ { "Guide", 0, 0 },
 /* 140 */ { "EThorp", 0, 0 },
 /* 141 */ { "RConnor", 0, 0 },
	// end of characters
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
 /* 190 */ { "murder", 1, 40 },	/* ubijstvo */
 /* 191 */ { "creation", 5, 72 },	/* sozidanie */
 /* 192 */ { "destruction", 5, 38 },	/* razrushenie */
 /* 193 */ { "sex", 9, 73 },	/* seks */
 /* 194 */ { "fight", 50, 39 },	/* draka */
 /* 195 */ { "drug", 20, 80 },	/* zavisimost' */
 /* 196 */ { "death", 50, 5 },	/* smert' */
 /* 197 */ { "mist", 50, 4 },	/* tuman */
 /* 198 */ { "fear", 40, 26 },	/* strah */
	// end of intentions
 /* 199 */ { "r199", 99, 1 },	/* wrong */
	// Emotion fixes
 /* 200 */ { "xFon", 5, 0 },	/* fon */
 /* 201 */ { "xWrong", 5, 1 },	/* wrong */
 /* 202 */ { "xMaster", 5, 2 },	/* master */
 /* 203 */ { "xSilence", 5, 3 },	/* silence */
 /* 204 */ { "xTuman", 5, 4 },	/* tuman */
 /* 205 */ { "xSmert'", 5, 5 },	/* smert' */
 /* 206 */ { "xMertvyak", 5, 6 },	/* mertvyak */
 /* 207 */ { "xNegativ", 5, 7 },	/* negativ */
 /* 208 */ { "xRavnodushie", 5, 8 },	/* ravnodushie */
 /* 209 */ { "xSkuka", 5, 9 },	/* skuka */
 /* 210 */ { "xApatiya", 5, 10 },	/* apatiya */
 /* 211 */ { "xDepressiya", 5, 11 },	/* depressiya */
 /* 212 */ { "xZlost'", 5, 12 },	/* zlost' */
 /* 213 */ { "xGnev", 5, 13 },	/* gnev */
 /* 214 */ { "xYarost'", 5, 14 },	/* yarost' */
 /* 215 */ { "xRazdrazhenie", 5, 15 },	/* razdrazhenie */
 /* 216 */ { "xGrust'", 5, 16 },	/* grust' */
 /* 217 */ { "xToska", 5, 17 },	/* toska */
 /* 218 */ { "xObrechennost'", 5, 18 },	/* obrechennost' */
 /* 219 */ { "xOtchayanie", 5, 19 },	/* otchayanie */
 /* 220 */ { "xNeuverennost'", 5, 20 },	/* neuverennost' */
 /* 221 */ { "xOzabochennost'", 5, 21 },	/* ozabochennost' */
 /* 222 */ { "xNapryazhennost'", 5, 22 },	/* napryazhennost' */
 /* 223 */ { "xIsterika", 5, 23 },	/* isterika */
 /* 224 */ { "xSomnenie", 5, 24 },	/* somnenie */
 /* 225 */ { "xTrevozhnost'", 5, 25 },	/* trevozhnost' */
 /* 226 */ { "xStrah", 5, 26 },	/* strah */
 /* 227 */ { "xUzhas", 5, 27 },	/* uzhas */
 /* 228 */ { "xPanika", 5, 28 },	/* panika */
 /* 229 */ { "xFobiya", 5, 29 },	/* fobiya */
 /* 230 */ { "xParanojya", 5, 30 },	/* paranojya */
 /* 231 */ { "xNepriyazn'", 5, 31 },	/* nepriyazn' */
 /* 232 */ { "xNenavist'", 5, 32 },	/* nenavist' */
 /* 233 */ { "xOtvraschenie", 5, 33 },	/* otvraschenie */
 /* 234 */ { "xPrezrenie", 5, 34 },	/* prezrenie */
 /* 235 */ { "xNeudovletvorennost'", 5, 35 },	/* neudovletvorennost' */
 /* 236 */ { "xSozhalenie", 5, 36 },	/* sozhalenie */
 /* 237 */ { "xStyid", 5, 37 },	/* styid */
 /* 238 */ { "xRazrushenie", 5, 38 },	/* razrushenie */
 /* 239 */ { "xDraka", 5, 39 },	/* draka */
 /* 240 */ { "xUbijstvo", 5, 40 },	/* ubijstvo */
 /* 241 */ { "xPozitiv", 5, 41 },	/* pozitiv */
 /* 242 */ { "xVesel'e", 5, 42 },	/* vesel'e */
 /* 243 */ { "xRadost'", 5, 43 },	/* radost' */
 /* 244 */ { "xVostorg", 5, 44 },	/* vostorg */
 /* 245 */ { "xDovol'stvo", 5, 45 },	/* dovol'stvo */
 /* 246 */ { "xGordost'", 5, 46 },	/* gordost' */
 /* 247 */ { "xUdovletvorenie", 5, 47 },	/* udovletvorenie */
 /* 248 */ { "xLyubopyitstvo", 5, 48 },	/* lyubopyitstvo */
 /* 249 */ { "xInteres", 5, 49 },	/* interes */
 /* 250 */ { "xUvlechennost'", 5, 50 },	/* uvlechennost' */
 /* 251 */ { "xAzart", 5, 51 },	/* azart */
 /* 252 */ { "xVoodushevlenie", 5, 52 },	/* voodushevlenie */
 /* 253 */ { "xTseleustremlennost'", 5, 53 },	/* tseleustremlennost' */
 /* 254 */ { "xOtvaga", 5, 54 },	/* otvaga */
 /* 255 */ { "xReshimost'", 5, 55 },	/* reshimost' */
 /* 256 */ { "xVdohnovenie", 5, 56 },	/* vdohnovenie */
 /* 257 */ { "xPriyazn'", 5, 57 },	/* priyazn' */
 /* 258 */ { "xSimpatiya", 5, 58 },	/* simpatiya */
 /* 259 */ { "xPrivyazannost'", 5, 59 },	/* privyazannost' */
 /* 260 */ { "xLyubov'", 5, 60 },	/* lyubov' */
 /* 261 */ { "xVozhdelenie", 5, 61 },	/* vozhdelenie */
 /* 262 */ { "xUmilenie", 5, 62 },	/* umilenie */
 /* 263 */ { "xNadezhda", 5, 63 },	/* nadezhda */
 /* 264 */ { "xPredvkushenie", 5, 64 },	/* predvkushenie */
 /* 265 */ { "xVera", 5, 65 },	/* vera */
 /* 266 */ { "xUverennost'", 5, 66 },	/* uverennost' */
 /* 267 */ { "xPokoj", 5, 67 },	/* pokoj */
 /* 268 */ { "xUmirotvorenie", 5, 68 },	/* umirotvorenie */
 /* 269 */ { "xNostal'giya", 5, 69 },	/* nostal'giya */
 /* 270 */ { "xSoperezhivanie", 5, 70 },	/* soperezhivanie */
 /* 271 */ { "xSostradanie", 5, 71 },	/* sostradanie */
 /* 272 */ { "xSozidanie", 5, 72 },	/* sozidanie */
 /* 273 */ { "xSeks", 5, 73 },	/* seks */
 /* 274 */ { "xZhelanie", 5, 74 },	/* zhelanie */
 /* 275 */ { "xVlechenie", 5, 75 },	/* vlechenie */
 /* 276 */ { "xStrast'", 5, 76 },	/* strast' */
 /* 277 */ { "xOderzhimost'", 5, 77 },	/* oderzhimost' */
 /* 278 */ { "xManiya", 5, 78 },	/* maniya */
 /* 279 */ { "xZainteresovannost'", 5, 79 },	/* zainteresovannost' */
 /* 280 */ { "xZavisimost'", 5, 80 },	/* zavisimost' */
 /* 281 */ { "xDuhovnoe", 5, 81 },	/* duhovnoe */
 /* 282 */ { "xTrans", 5, 82 },	/* trans */
 /* 283 */ { "xMeditatsiya", 5, 83 },	/* meditatsiya */
 /* 284 */ { "xEkzal'tatsiya", 5, 84 },	/* ekzal'tatsiya */
 /* 285 */ { "xProsvetlenie", 5, 85 },	/* prosvetlenie */
	// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
