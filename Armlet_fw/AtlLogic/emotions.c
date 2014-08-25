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
 * Generated at 2014-08-25 18:57:00
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

// RID ranges
const int first_location_id = 1;
const int last_location_id = 89;

const int first_mist_id = 90;
const int last_mist_id = 99;

const int first_character_id = 101;
const int last_character_id = 189;

const int first_intention_id = 190;
const int last_intention_id = 209;

const int first_emotion_fix_id = 210;
const int last_emotion_fix_id = 299;

// RIDs are indexes in this array.
Reason_t reasons[] = {
 /*   0 */ { "r000", 99, 0, 1 },	/* neverno */
	// Locations
 /*   1 */ { "hub", 99, 0, 2 },	/* masterka */
 /*   2 */ { "uliza", 1, 0, 0 },	/* fon */
 /*   3 */ { "grave", 3, 0, 28 },	/* strah */
 /*   4 */ { "church", 2, 0, 43 },	/* pozitiv */
 /*   5 */ { "kabak", 2, 0, 9 },	/* negativ */
 /*   6 */ { "dead", 99, 0, 6 },	/* mertvyak */
	// end of locations
 /*   7 */ { "r007", 99, 0, 1 },	/* neverno */
 /*   8 */ { "r008", 99, 0, 1 },	/* neverno */
 /*   9 */ { "r009", 99, 0, 1 },	/* neverno */
 /*  10 */ { "r010", 99, 0, 1 },	/* neverno */
 /*  11 */ { "r011", 99, 0, 1 },	/* neverno */
 /*  12 */ { "r012", 99, 0, 1 },	/* neverno */
 /*  13 */ { "r013", 99, 0, 1 },	/* neverno */
 /*  14 */ { "r014", 99, 0, 1 },	/* neverno */
 /*  15 */ { "r015", 99, 0, 1 },	/* neverno */
 /*  16 */ { "r016", 99, 0, 1 },	/* neverno */
 /*  17 */ { "r017", 99, 0, 1 },	/* neverno */
 /*  18 */ { "r018", 99, 0, 1 },	/* neverno */
 /*  19 */ { "r019", 99, 0, 1 },	/* neverno */
 /*  20 */ { "r020", 99, 0, 1 },	/* neverno */
 /*  21 */ { "r021", 99, 0, 1 },	/* neverno */
 /*  22 */ { "r022", 99, 0, 1 },	/* neverno */
 /*  23 */ { "r023", 99, 0, 1 },	/* neverno */
 /*  24 */ { "r024", 99, 0, 1 },	/* neverno */
 /*  25 */ { "r025", 99, 0, 1 },	/* neverno */
 /*  26 */ { "r026", 99, 0, 1 },	/* neverno */
 /*  27 */ { "r027", 99, 0, 1 },	/* neverno */
 /*  28 */ { "r028", 99, 0, 1 },	/* neverno */
 /*  29 */ { "r029", 99, 0, 1 },	/* neverno */
 /*  30 */ { "r030", 99, 0, 1 },	/* neverno */
 /*  31 */ { "r031", 99, 0, 1 },	/* neverno */
 /*  32 */ { "r032", 99, 0, 1 },	/* neverno */
 /*  33 */ { "r033", 99, 0, 1 },	/* neverno */
 /*  34 */ { "r034", 99, 0, 1 },	/* neverno */
 /*  35 */ { "r035", 99, 0, 1 },	/* neverno */
 /*  36 */ { "r036", 99, 0, 1 },	/* neverno */
 /*  37 */ { "r037", 99, 0, 1 },	/* neverno */
 /*  38 */ { "r038", 99, 0, 1 },	/* neverno */
 /*  39 */ { "r039", 99, 0, 1 },	/* neverno */
 /*  40 */ { "r040", 99, 0, 1 },	/* neverno */
 /*  41 */ { "r041", 99, 0, 1 },	/* neverno */
 /*  42 */ { "r042", 99, 0, 1 },	/* neverno */
 /*  43 */ { "r043", 99, 0, 1 },	/* neverno */
 /*  44 */ { "r044", 99, 0, 1 },	/* neverno */
 /*  45 */ { "r045", 99, 0, 1 },	/* neverno */
 /*  46 */ { "r046", 99, 0, 1 },	/* neverno */
 /*  47 */ { "r047", 99, 0, 1 },	/* neverno */
 /*  48 */ { "r048", 99, 0, 1 },	/* neverno */
 /*  49 */ { "r049", 99, 0, 1 },	/* neverno */
 /*  50 */ { "r050", 99, 0, 1 },	/* neverno */
 /*  51 */ { "r051", 99, 0, 1 },	/* neverno */
 /*  52 */ { "r052", 99, 0, 1 },	/* neverno */
 /*  53 */ { "r053", 99, 0, 1 },	/* neverno */
 /*  54 */ { "r054", 99, 0, 1 },	/* neverno */
 /*  55 */ { "r055", 99, 0, 1 },	/* neverno */
 /*  56 */ { "r056", 99, 0, 1 },	/* neverno */
 /*  57 */ { "r057", 99, 0, 1 },	/* neverno */
 /*  58 */ { "r058", 99, 0, 1 },	/* neverno */
 /*  59 */ { "r059", 99, 0, 1 },	/* neverno */
 /*  60 */ { "r060", 99, 0, 1 },	/* neverno */
 /*  61 */ { "r061", 99, 0, 1 },	/* neverno */
 /*  62 */ { "r062", 99, 0, 1 },	/* neverno */
 /*  63 */ { "r063", 99, 0, 1 },	/* neverno */
 /*  64 */ { "r064", 99, 0, 1 },	/* neverno */
 /*  65 */ { "r065", 99, 0, 1 },	/* neverno */
 /*  66 */ { "r066", 99, 0, 1 },	/* neverno */
 /*  67 */ { "r067", 99, 0, 1 },	/* neverno */
 /*  68 */ { "r068", 99, 0, 1 },	/* neverno */
 /*  69 */ { "r069", 99, 0, 1 },	/* neverno */
 /*  70 */ { "r070", 99, 0, 1 },	/* neverno */
 /*  71 */ { "r071", 99, 0, 1 },	/* neverno */
 /*  72 */ { "r072", 99, 0, 1 },	/* neverno */
 /*  73 */ { "r073", 99, 0, 1 },	/* neverno */
 /*  74 */ { "r074", 99, 0, 1 },	/* neverno */
 /*  75 */ { "r075", 99, 0, 1 },	/* neverno */
 /*  76 */ { "r076", 99, 0, 1 },	/* neverno */
 /*  77 */ { "r077", 99, 0, 1 },	/* neverno */
 /*  78 */ { "r078", 99, 0, 1 },	/* neverno */
 /*  79 */ { "r079", 99, 0, 1 },	/* neverno */
 /*  80 */ { "r080", 99, 0, 1 },	/* neverno */
 /*  81 */ { "r081", 99, 0, 1 },	/* neverno */
 /*  82 */ { "r082", 99, 0, 1 },	/* neverno */
 /*  83 */ { "r083", 99, 0, 1 },	/* neverno */
 /*  84 */ { "r084", 99, 0, 1 },	/* neverno */
 /*  85 */ { "r085", 99, 0, 1 },	/* neverno */
 /*  86 */ { "r086", 99, 0, 1 },	/* neverno */
 /*  87 */ { "r087", 99, 0, 1 },	/* neverno */
 /*  88 */ { "r088", 99, 0, 1 },	/* neverno */
 /*  89 */ { "r089", 99, 0, 1 },	/* neverno */
	// Mist sources
 /*  90 */ { "m90", 0, 0, 0 },	/* fon */
 /*  91 */ { "m91", 0, 0, 0 },	/* fon */
 /*  92 */ { "m92", 0, 0, 0 },	/* fon */
 /*  93 */ { "m93", 0, 0, 0 },	/* fon */
 /*  94 */ { "m94", 0, 0, 0 },	/* fon */
 /*  95 */ { "m95", 0, 0, 0 },	/* fon */
 /*  96 */ { "m96", 0, 0, 0 },	/* fon */
 /*  97 */ { "m97", 0, 0, 0 },	/* fon */
 /*  98 */ { "m98", 0, 0, 0 },	/* fon */
 /*  99 */ { "m99", 0, 0, 0 },	/* fon */
	// end of mist sources
 /* 100 */ { "r100", 99, 0, 1 },	/* neverno */
	// Characters
 /* 101 */ { "PAdams", 0, 0, 0 },
 /* 102 */ { "FBuster", 0, 0, 0 },
 /* 103 */ { "ZCampbellX", 0, 0, 0 },
 /* 104 */ { "NCassidy", 0, 0, 0 },
 /* 105 */ { "JChamberX", 0, 0, 0 },
 /* 106 */ { "SClayton", 0, 0, 0 },
 /* 107 */ { "JCollins", 0, 0, 0 },
 /* 108 */ { "CComes", 0, 0, 0 },
 /* 109 */ { "RConnor", 0, 0, 0 },
 /* 110 */ { "GCross", 0, 0, 0 },
 /* 111 */ { "FCunninghamX", 0, 0, 0 },
 /* 112 */ { "JDawson", 0, 0, 0 },
 /* 113 */ { "GDay", 0, 0, 0 },
 /* 114 */ { "ADecker", 0, 0, 0 },
 /* 115 */ { "CDeLorean", 0, 0, 0 },
 /* 116 */ { "MDurdenX", 0, 0, 0 },
 /* 117 */ { "NEpstein", 0, 0, 0 },
 /* 118 */ { "CFranklin", 0, 0, 0 },
 /* 119 */ { "AGraywood", 0, 0, 0 },
 /* 120 */ { "KGreenhall", 0, 0, 0 },
 /* 121 */ { "Guide", 0, 0, 0 },
 /* 122 */ { "MHailey", 0, 0, 0 },
 /* 123 */ { "ZHarriot", 0, 0, 0 },
 /* 124 */ { "BHill", 0, 0, 0 },
 /* 125 */ { "AHuxley", 0, 0, 0 },
 /* 126 */ { "AJoeX", 0, 0, 0 },
 /* 127 */ { "LLane", 0, 0, 0 },
 /* 128 */ { "CLefebvre", 0, 0, 0 },
 /* 129 */ { "VLehndorff", 0, 0, 0 },
 /* 130 */ { "TMalone", 0, 0, 0 },
 /* 131 */ { "UMcGee", 0, 0, 0 },
 /* 132 */ { "FMilligan", 0, 0, 0 },
 /* 133 */ { "AMorientes", 0, 0, 0 },
 /* 134 */ { "JOSullivan", 0, 0, 0 },
 /* 135 */ { "MParcs", 0, 0, 0 },
 /* 136 */ { "SRandom", 0, 0, 0 },
 /* 137 */ { "RRay", 0, 0, 0 },
 /* 138 */ { "MReed", 0, 0, 0 },
 /* 139 */ { "MRose", 0, 0, 0 },
 /* 140 */ { "JSherman", 0, 0, 0 },
 /* 141 */ { "CStanfield", 0, 0, 0 },
 /* 142 */ { "DStanley", 0, 0, 0 },
 /* 143 */ { "TSunday", 0, 0, 0 },
 /* 144 */ { "WThomas", 0, 0, 0 },
 /* 145 */ { "EThorpX", 0, 0, 0 },
 /* 146 */ { "EWilliams", 0, 0, 0 },
 /* 147 */ { "HWilliams", 0, 0, 0 },
 /* 148 */ { "LYoung", 0, 0, 0 },
 /* 149 */ { "DAllgood", 0, 0, 0 },
 /* 150 */ { "MBanks", 0, 0, 0 },
 /* 151 */ { "BBernhard", 0, 0, 0 },
 /* 152 */ { "ABrown", 0, 0, 0 },
 /* 153 */ { "ZCampbell", 0, 0, 0 },
 /* 154 */ { "VCollins", 0, 0, 0 },
 /* 155 */ { "FCunningham", 0, 0, 0 },
 /* 156 */ { "MDawkins", 0, 0, 0 },
 /* 157 */ { "JDoe", 0, 0, 0 },
 /* 158 */ { "MDurden", 0, 0, 0 },
 /* 159 */ { "DElis", 0, 0, 0 },
 /* 160 */ { "OFry", 0, 0, 0 },
 /* 161 */ { "LGeertz", 0, 0, 0 },
 /* 162 */ { "AGrissom", 0, 0, 0 },
 /* 163 */ { "MHarvent", 0, 0, 0 },
 /* 164 */ { "BHeart", 0, 0, 0 },
 /* 165 */ { "CHonzo", 0, 0, 0 },
 /* 166 */ { "AJane", 0, 0, 0 },
 /* 167 */ { "AJones", 0, 0, 0 },
 /* 168 */ { "SKendall", 0, 0, 0 },
 /* 169 */ { "SLarken", 0, 0, 0 },
 /* 170 */ { "ALee", 0, 0, 0 },
 /* 171 */ { "AMay", 0, 0, 0 },
 /* 172 */ { "MNovak", 0, 0, 0 },
 /* 173 */ { "SOReilly", 0, 0, 0 },
 /* 174 */ { "DSeldon", 0, 0, 0 },
 /* 175 */ { "PShannon", 0, 0, 0 },
 /* 176 */ { "DSilverstone", 0, 0, 0 },
 /* 177 */ { "RSoyfer", 0, 0, 0 },
 /* 178 */ { "JSpencer", 0, 0, 0 },
 /* 179 */ { "JSullivan", 0, 0, 0 },
 /* 180 */ { "SSummer", 0, 0, 0 },
 /* 181 */ { "EThorp", 0, 0, 0 },
 /* 182 */ { "MWright", 0, 0, 0 },
 /* 183 */ { "NYoung", 0, 0, 0 },
	// end of characters
 /* 184 */ { "r184", 99, 0, 1 },	/* neverno */
 /* 185 */ { "r185", 99, 0, 1 },	/* neverno */
 /* 186 */ { "r186", 99, 0, 1 },	/* neverno */
 /* 187 */ { "r187", 99, 0, 1 },	/* neverno */
 /* 188 */ { "r188", 99, 0, 1 },	/* neverno */
 /* 189 */ { "r189", 99, 0, 1 },	/* neverno */
	// Intentions
 /* 190 */ { "murder", 1, 0, 42 },	/* ubijstvo */
 /* 191 */ { "creation", 5, 0, 74 },	/* sozidanie */
 /* 192 */ { "destruction", 5, 0, 40 },	/* razrushenie */
 /* 193 */ { "sex", 9, 0, 75 },	/* seks */
 /* 194 */ { "fight", 50, 0, 41 },	/* draka */
 /* 195 */ { "heartbeat", 50, 0, 7 },	/* serdtsebienie */
 /* 196 */ { "weed", 20, 0, 43 },	/* pozitiv */
 /* 197 */ { "heroin", 20, 0, 9 },	/* negativ */
 /* 198 */ { "lsd", 20, 0, 8 },	/* trip */
 /* 199 */ { "krayk", 20, 0, 0 },	/* fon */
 /* 200 */ { "death", 50, 0, 5 },	/* smert' */
 /* 201 */ { "mist", 50, 0, 4 },	/* tuman */
 /* 202 */ { "fear", 40, 0, 28 },	/* strah */
 /* 203 */ { "mSource", 9, 0, 1 },	/* neverno */
 /* 204 */ { "mProject", 9, 0, 1 },	/* neverno */
	// end of intentions
 /* 205 */ { "r205", 99, 0, 1 },	/* neverno */
 /* 206 */ { "r206", 99, 0, 1 },	/* neverno */
 /* 207 */ { "r207", 99, 0, 1 },	/* neverno */
 /* 208 */ { "r208", 99, 0, 1 },	/* neverno */
 /* 209 */ { "r209", 99, 0, 1 },	/* neverno */
	// Emotion fixes
 /* 210 */ { "xFon", 5, 0, 0 },	/* fon */
 /* 211 */ { "xNeverno", 5, 0, 1 },	/* neverno */
 /* 212 */ { "xMasterka", 5, 0, 2 },	/* masterka */
 /* 213 */ { "xTishina", 5, 0, 3 },	/* tishina */
 /* 214 */ { "xTuman", 5, 0, 4 },	/* tuman */
 /* 215 */ { "xSmert'", 5, 0, 5 },	/* smert' */
 /* 216 */ { "xMertvyak", 5, 0, 6 },	/* mertvyak */
 /* 217 */ { "xSerdtsebienie", 5, 0, 7 },	/* serdtsebienie */
 /* 218 */ { "xTrip", 5, 0, 8 },	/* trip */
 /* 219 */ { "xNegativ", 5, 0, 9 },	/* negativ */
 /* 220 */ { "xRavnodushie", 5, 0, 10 },	/* ravnodushie */
 /* 221 */ { "xSkuka", 5, 0, 11 },	/* skuka */
 /* 222 */ { "xApatiya", 5, 0, 12 },	/* apatiya */
 /* 223 */ { "xDepressiya", 5, 0, 13 },	/* depressiya */
 /* 224 */ { "xZlost'", 5, 0, 14 },	/* zlost' */
 /* 225 */ { "xGnev", 5, 0, 15 },	/* gnev */
 /* 226 */ { "xYarost'", 5, 0, 16 },	/* yarost' */
 /* 227 */ { "xRazdrazhenie", 5, 0, 17 },	/* razdrazhenie */
 /* 228 */ { "xGrust'", 5, 0, 18 },	/* grust' */
 /* 229 */ { "xToska", 5, 0, 19 },	/* toska */
 /* 230 */ { "xObrechennost'", 5, 0, 20 },	/* obrechennost' */
 /* 231 */ { "xOtchayanie", 5, 0, 21 },	/* otchayanie */
 /* 232 */ { "xNeuverennost'", 5, 0, 22 },	/* neuverennost' */
 /* 233 */ { "xOzabochennost'", 5, 0, 23 },	/* ozabochennost' */
 /* 234 */ { "xNapryazhennost'", 5, 0, 24 },	/* napryazhennost' */
 /* 235 */ { "xIsterika", 5, 0, 25 },	/* isterika */
 /* 236 */ { "xSomnenie", 5, 0, 26 },	/* somnenie */
 /* 237 */ { "xTrevozhnost'", 5, 0, 27 },	/* trevozhnost' */
 /* 238 */ { "xStrah", 5, 0, 28 },	/* strah */
 /* 239 */ { "xUzhas", 5, 0, 29 },	/* uzhas */
 /* 240 */ { "xPanika", 5, 0, 30 },	/* panika */
 /* 241 */ { "xFobiya", 5, 0, 31 },	/* fobiya */
 /* 242 */ { "xParanojya", 5, 0, 32 },	/* paranojya */
 /* 243 */ { "xNepriyazn'", 5, 0, 33 },	/* nepriyazn' */
 /* 244 */ { "xNenavist'", 5, 0, 34 },	/* nenavist' */
 /* 245 */ { "xOtvraschenie", 5, 0, 35 },	/* otvraschenie */
 /* 246 */ { "xPrezrenie", 5, 0, 36 },	/* prezrenie */
 /* 247 */ { "xNeudovletvorennost'", 5, 0, 37 },	/* neudovletvorennost' */
 /* 248 */ { "xSozhalenie", 5, 0, 38 },	/* sozhalenie */
 /* 249 */ { "xStyid", 5, 0, 39 },	/* styid */
 /* 250 */ { "xRazrushenie", 5, 0, 40 },	/* razrushenie */
 /* 251 */ { "xDraka", 5, 0, 41 },	/* draka */
 /* 252 */ { "xUbijstvo", 5, 0, 42 },	/* ubijstvo */
 /* 253 */ { "xPozitiv", 5, 0, 43 },	/* pozitiv */
 /* 254 */ { "xVesel'e", 5, 0, 44 },	/* vesel'e */
 /* 255 */ { "xRadost'", 5, 0, 45 },	/* radost' */
 /* 256 */ { "xVostorg", 5, 0, 46 },	/* vostorg */
 /* 257 */ { "xDovol'stvo", 5, 0, 47 },	/* dovol'stvo */
 /* 258 */ { "xGordost'", 5, 0, 48 },	/* gordost' */
 /* 259 */ { "xUdovletvorenie", 5, 0, 49 },	/* udovletvorenie */
 /* 260 */ { "xLyubopyitstvo", 5, 0, 50 },	/* lyubopyitstvo */
 /* 261 */ { "xInteres", 5, 0, 51 },	/* interes */
 /* 262 */ { "xUvlechennost'", 5, 0, 52 },	/* uvlechennost' */
 /* 263 */ { "xAzart", 5, 0, 53 },	/* azart */
 /* 264 */ { "xVoodushevlenie", 5, 0, 54 },	/* voodushevlenie */
 /* 265 */ { "xTseleustremlennost'", 5, 0, 55 },	/* tseleustremlennost' */
 /* 266 */ { "xOtvaga", 5, 0, 56 },	/* otvaga */
 /* 267 */ { "xReshimost'", 5, 0, 57 },	/* reshimost' */
 /* 268 */ { "xVdohnovenie", 5, 0, 58 },	/* vdohnovenie */
 /* 269 */ { "xPriyazn'", 5, 0, 59 },	/* priyazn' */
 /* 270 */ { "xSimpatiya", 5, 0, 60 },	/* simpatiya */
 /* 271 */ { "xPrivyazannost'", 5, 0, 61 },	/* privyazannost' */
 /* 272 */ { "xLyubov'", 5, 0, 62 },	/* lyubov' */
 /* 273 */ { "xVozhdelenie", 5, 0, 63 },	/* vozhdelenie */
 /* 274 */ { "xUmilenie", 5, 0, 64 },	/* umilenie */
 /* 275 */ { "xNadezhda", 5, 0, 65 },	/* nadezhda */
 /* 276 */ { "xPredvkushenie", 5, 0, 66 },	/* predvkushenie */
 /* 277 */ { "xVera", 5, 0, 67 },	/* vera */
 /* 278 */ { "xUverennost'", 5, 0, 68 },	/* uverennost' */
 /* 279 */ { "xPokoj", 5, 0, 69 },	/* pokoj */
 /* 280 */ { "xUmirotvorenie", 5, 0, 70 },	/* umirotvorenie */
 /* 281 */ { "xNostal'giya", 5, 0, 71 },	/* nostal'giya */
 /* 282 */ { "xSoperezhivanie", 5, 0, 72 },	/* soperezhivanie */
 /* 283 */ { "xSostradanie", 5, 0, 73 },	/* sostradanie */
 /* 284 */ { "xSozidanie", 5, 0, 74 },	/* sozidanie */
 /* 285 */ { "xSeks", 5, 0, 75 },	/* seks */
 /* 286 */ { "xZhelanie", 5, 0, 76 },	/* zhelanie */
 /* 287 */ { "xVlechenie", 5, 0, 77 },	/* vlechenie */
 /* 288 */ { "xStrast'", 5, 0, 78 },	/* strast' */
 /* 289 */ { "xOderzhimost'", 5, 0, 79 },	/* oderzhimost' */
 /* 290 */ { "xManiya", 5, 0, 80 },	/* maniya */
 /* 291 */ { "xZainteresovannost'", 5, 0, 81 },	/* zainteresovannost' */
 /* 292 */ { "xZavisimost'", 5, 0, 82 },	/* zavisimost' */
 /* 293 */ { "xDuhovnoe", 5, 0, 83 },	/* duhovnoe */
 /* 294 */ { "xTrans", 5, 0, 84 },	/* trans */
 /* 295 */ { "xMeditatsiya", 5, 0, 85 },	/* meditatsiya */
 /* 296 */ { "xEkzal'tatsiya", 5, 0, 86 },	/* ekzal'tatsiya */
 /* 297 */ { "xProsvetlenie", 5, 0, 87 },	/* prosvetlenie */
	// end of emotion fixes
};

const int reasons_number = countof(reasons);

// End of emotions.c
