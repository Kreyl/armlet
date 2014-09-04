/*
 * emotions.h
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
#ifndef EMOTIONS_H
#define EMOTIONS_H

#define ROOT -1

//
// ID limits
//

#define LOCATION_ID_START 1		// start of stationary ID interval
#define LOCATION_IN_GAME_ID_START 3	// start of in-game IDs
#define LOCATIONS_ID_END 20

#define FOREST_ID_START 21
#define FOREST_ALL_ID_END 55
#define FOREST_ID_END 89		// end of stationary ID interval

#define MIST_ID_START 90
#define MIST_ID_END 100

#define CHARACTER_ID_START 101
#define CHARACTER_ID_END 179

#define INTENTION_ID_START 180
#define INTENTION_ID_END 199

#define EMOTION_FIX_ID_START 200	// start of stationary ID interval
#define EMOTION_FIX_ID_END 299		// end of stationary ID interval

//
// Emotions
//

typedef struct Emotion {
    const char* name;
    const int weight;
    const int parent;
    int numTracks;
    int lastPlayedTrack;
} Emotion_t;

extern Emotion_t emotions[];

#define NUMBER_OF_EMOTIONS         91

#define EMOTION_FON                 0
#define EMOTION_NEVERNO             1
#define EMOTION_MASTERKA            2
#define EMOTION_TISHINA             3
#define EMOTION_TUMAN               4
#define EMOTION_SMERT               5
#define EMOTION_MERTVYAK            6
#define EMOTION_SERDTSEBIENIE       7
#define EMOTION_TRIP                8
#define EMOTION_KAKTUS              9
#define EMOTION_NARKOMAN           10
#define EMOTION_LES                11
#define EMOTION_NEGATIV            12
#define EMOTION_RAVNODUSHIE        13
#define EMOTION_SKUKA              14
#define EMOTION_APATIYA            15
#define EMOTION_DEPRESSIYA         16
#define EMOTION_ZLOST              17
#define EMOTION_GNEV               18
#define EMOTION_YAROST             19
#define EMOTION_RAZDRAZHENIE       20
#define EMOTION_GRUST              21
#define EMOTION_TOSKA              22
#define EMOTION_OBRECHENNOST       23
#define EMOTION_OTCHAYANIE         24
#define EMOTION_NEUVERENNOST       25
#define EMOTION_OZABOCHENNOST      26
#define EMOTION_NAPRYAZHENNOST     27
#define EMOTION_ISTERIKA           28
#define EMOTION_SOMNENIE           29
#define EMOTION_TREVOZHNOST        30
#define EMOTION_STRAH              31
#define EMOTION_UZHAS              32
#define EMOTION_PANIKA             33
#define EMOTION_FOBIYA             34
#define EMOTION_PARANOJYA          35
#define EMOTION_NEPRIYAZN          36
#define EMOTION_NENAVIST           37
#define EMOTION_OTVRASCHENIE       38
#define EMOTION_PREZRENIE          39
#define EMOTION_NEUDOVLETVORENNOST 40
#define EMOTION_SOZHALENIE         41
#define EMOTION_STYID              42
#define EMOTION_RAZRUSHENIE        43
#define EMOTION_DRAKA              44
#define EMOTION_UBIJSTVO           45
#define EMOTION_POZITIV            46
#define EMOTION_VESELE             47
#define EMOTION_RADOST             48
#define EMOTION_VOSTORG            49
#define EMOTION_DOVOLSTVO          50
#define EMOTION_GORDOST            51
#define EMOTION_UDOVLETVORENIE     52
#define EMOTION_LYUBOPYITSTVO      53
#define EMOTION_INTERES            54
#define EMOTION_UVLECHENNOST       55
#define EMOTION_AZART              56
#define EMOTION_VOODUSHEVLENIE     57
#define EMOTION_TSELEUSTREMLENNOST 58
#define EMOTION_OTVAGA             59
#define EMOTION_RESHIMOST          60
#define EMOTION_VDOHNOVENIE        61
#define EMOTION_PRIYAZN            62
#define EMOTION_SIMPATIYA          63
#define EMOTION_PRIVYAZANNOST      64
#define EMOTION_LYUBOV             65
#define EMOTION_VOZHDELENIE        66
#define EMOTION_UMILENIE           67
#define EMOTION_NADEZHDA           68
#define EMOTION_PREDVKUSHENIE      69
#define EMOTION_VERA               70
#define EMOTION_UVERENNOST         71
#define EMOTION_POKOJ              72
#define EMOTION_UMIROTVORENIE      73
#define EMOTION_NOSTALGIYA         74
#define EMOTION_SOPEREZHIVANIE     75
#define EMOTION_SOSTRADANIE        76
#define EMOTION_SOZIDANIE          77
#define EMOTION_SEKS               78
#define EMOTION_ZHELANIE           79
#define EMOTION_VLECHENIE          80
#define EMOTION_STRAST             81
#define EMOTION_ODERZHIMOST        82
#define EMOTION_MANIYA             83
#define EMOTION_ZAINTERESOVANNOST  84
#define EMOTION_ZAVISIMOST         85
#define EMOTION_DUHOVNOE           86
#define EMOTION_TRANS              87
#define EMOTION_MEDITATSIYA        88
#define EMOTION_EKZALTATSIYA       89
#define EMOTION_PROSVETLENIE       90

//
// Reasons
//

typedef struct Reason {
    const char* name;
    int weight;
    long age;
    int eID;
} Reason_t;

extern Reason_t reasons[];

#define NUMBER_OF_REASONS         291

#define REASON_R000                 0
	// Locations
#define REASON_HUB                  1
#define REASON_DEAD                 2
#define REASON_BG                   3
#define REASON_HERMIT               4
#define REASON_SQUARE               5
#define REASON_FACTORY              6
#define REASON_CITYHALL             7
#define REASON_STATION              8
#define REASON_BAR                  9
#define REASON_COFFEE              10
#define REASON_DETSKAYA            11
#define REASON_TENT                12
#define REASON_HOSPITAL            13
#define REASON_UNIVERSITY          14
#define REASON_REDROOM             15
#define REASON_SHOP                16
#define REASON_BLACKHOUSE          17
#define REASON_HOME                18
#define REASON_TRENCH              19
#define REASON_CINEMA              20
	// end of locations

	// Forest
#define REASON_FA21                21
#define REASON_FA22                22
#define REASON_FA23                23
#define REASON_FA24                24
#define REASON_FA25                25
#define REASON_FA26                26
#define REASON_FA27                27
#define REASON_FA28                28
#define REASON_FA29                29
#define REASON_FA30                30
#define REASON_FA31                31
#define REASON_FA32                32
#define REASON_FA33                33
#define REASON_FA34                34
#define REASON_FA35                35
#define REASON_FA36                36
#define REASON_FA37                37
#define REASON_FA38                38
#define REASON_FA39                39
#define REASON_FA40                40
#define REASON_FA41                41
#define REASON_FA42                42
#define REASON_FA43                43
#define REASON_FA44                44
#define REASON_FA45                45
#define REASON_FA46                46
#define REASON_FA47                47
#define REASON_FA48                48
#define REASON_FA49                49
#define REASON_FA50                50
#define REASON_FA51                51
#define REASON_FA52                52
#define REASON_FA53                53
#define REASON_FA54                54
#define REASON_FA55                55
#define REASON_FB56                56
#define REASON_FC57                57
#define REASON_FB58                58
#define REASON_FC59                59
#define REASON_FB60                60
#define REASON_FC61                61
#define REASON_FB62                62
#define REASON_FC63                63
#define REASON_FB64                64
#define REASON_FC65                65
#define REASON_FB66                66
#define REASON_FC67                67
#define REASON_FB68                68
#define REASON_FC69                69
#define REASON_FB70                70
#define REASON_FC71                71
#define REASON_FB72                72
#define REASON_FC73                73
#define REASON_FB74                74
#define REASON_FC75                75
#define REASON_FB76                76
#define REASON_FC77                77
#define REASON_FB78                78
#define REASON_FC79                79
#define REASON_FB80                80
#define REASON_FC81                81
#define REASON_FB82                82
#define REASON_FC83                83
#define REASON_FB84                84
#define REASON_FC85                85
#define REASON_FB86                86
#define REASON_FC87                87
#define REASON_FB88                88
#define REASON_FC89                89
	// end of forest

	// Mist sources
#define REASON_M90                 90
#define REASON_M91                 91
#define REASON_M92                 92
#define REASON_M93                 93
#define REASON_M94                 94
#define REASON_M95                 95
#define REASON_M96                 96
#define REASON_M97                 97
#define REASON_M98                 98
#define REASON_M99                 99
#define REASON_M100                100
	// end of mist sources

	// Characters
#define REASON_PADAMS              101
#define REASON_DALLGOOD            102
#define REASON_MBANKS              103
#define REASON_BBERNHARD           104
#define REASON_ABROWN              105
#define REASON_FBUSTER             106
#define REASON_ZCAMPBELL           107
#define REASON_NCASSIDY            108
#define REASON_SCLAYTON            109
#define REASON_JCOLLINS            110
#define REASON_VCOLLINS            111
#define REASON_CCOMES              112
#define REASON_RCONNOR             113
#define REASON_GCROSS              114
#define REASON_FCUNNINGHAM         115
#define REASON_MDAWKINS            116
#define REASON_JDAWSON             117
#define REASON_GDAY                118
#define REASON_ADECKER             119
#define REASON_CDELOREAN           120
#define REASON_JDOE                121
#define REASON_MDURDEN             122
#define REASON_DELIS               123
#define REASON_NEPSTEIN            124
#define REASON_CFRANKLIN           125
#define REASON_LGEERTZ             126
#define REASON_AGRAYWOOD           127
#define REASON_KGREENHALL          128
#define REASON_AGRISSOM            129
#define REASON_GUIDE               130
#define REASON_MHAILEY             131
#define REASON_ZHARRIOT            132
#define REASON_MHARVENT            133
#define REASON_BHEART              134
#define REASON_BHILL               135
#define REASON_CHONZO              136
#define REASON_AHUXLEY             137
#define REASON_AJANE               138
#define REASON_AJONES              139
#define REASON_EJONES              140
#define REASON_SKENDALL            141
#define REASON_LLANE               142
#define REASON_SLARKEN             143
#define REASON_ALEE                144
#define REASON_CLEFEBVRE           145
#define REASON_VLEHNDORFF          146
#define REASON_TMALONE             147
#define REASON_AMAY                148
#define REASON_UMCGEE              149
#define REASON_FMILLIGAN           150
#define REASON_AMORIENTES          151
#define REASON_MNOVAK              152
#define REASON_SOREILLY            153
#define REASON_JOSULLIVAN          154
#define REASON_MPARCS              155
#define REASON_SRANDOM             156
#define REASON_RRAY                157
#define REASON_MREED               158
#define REASON_MROSE               159
#define REASON_DSELDON             160
#define REASON_PSHANNON            161
#define REASON_JSHERMAN            162
#define REASON_DSILVERSTONE        163
#define REASON_RSOYFER             164
#define REASON_JSPENCER            165
#define REASON_CSTANFIELD          166
#define REASON_DSTANLEY            167
#define REASON_JSULLIVAN           168
#define REASON_SSUMMER             169
#define REASON_TSUNDAY             170
#define REASON_WTHOMAS             171
#define REASON_ETHORP              172
#define REASON_EWILLIAMS           173
#define REASON_HWILLIAMS           174
#define REASON_MWRIGHT             175
#define REASON_LYOUNG              176
#define REASON_NYOUNG              177
#define REASON_ATEST               178
#define REASON_BTEST               179
	// end of characters

	// Intentions
#define REASON_MURDER              180
#define REASON_CREATION            181
#define REASON_DESTRUCTION         182
#define REASON_SEX                 183
#define REASON_FIGHT               184
#define REASON_HEARTBEAT           185
#define REASON_ADDICTION           186
#define REASON_WEED                187
#define REASON_LSD                 188
#define REASON_PEYOTE              189
#define REASON_HEROIN              190
#define REASON_KRAYK               191
#define REASON_DEATH               192
#define REASON_MIST                193
#define REASON_FEAR                194
#define REASON_MSOURCE             195
#define REASON_MPROJECT            196
	// end of intentions
#define REASON_R197                197
#define REASON_R198                198
#define REASON_R199                199
	// Emotion fixes
#define REASON_XFON                200
#define REASON_XNEVERNO            201
#define REASON_XMASTERKA           202
#define REASON_XTISHINA            203
#define REASON_XTUMAN              204
#define REASON_XSMERT              205
#define REASON_XMERTVYAK           206
#define REASON_XSERDTSEBIENIE      207
#define REASON_XTRIP               208
#define REASON_XKAKTUS             209
#define REASON_XNARKOMAN           210
#define REASON_XLES                211
#define REASON_XNEGATIV            212
#define REASON_XRAVNODUSHIE        213
#define REASON_XSKUKA              214
#define REASON_XAPATIYA            215
#define REASON_XDEPRESSIYA         216
#define REASON_XZLOST              217
#define REASON_XGNEV               218
#define REASON_XYAROST             219
#define REASON_XRAZDRAZHENIE       220
#define REASON_XGRUST              221
#define REASON_XTOSKA              222
#define REASON_XOBRECHENNOST       223
#define REASON_XOTCHAYANIE         224
#define REASON_XNEUVERENNOST       225
#define REASON_XOZABOCHENNOST      226
#define REASON_XNAPRYAZHENNOST     227
#define REASON_XISTERIKA           228
#define REASON_XSOMNENIE           229
#define REASON_XTREVOZHNOST        230
#define REASON_XSTRAH              231
#define REASON_XUZHAS              232
#define REASON_XPANIKA             233
#define REASON_XFOBIYA             234
#define REASON_XPARANOJYA          235
#define REASON_XNEPRIYAZN          236
#define REASON_XNENAVIST           237
#define REASON_XOTVRASCHENIE       238
#define REASON_XPREZRENIE          239
#define REASON_XNEUDOVLETVORENNOST 240
#define REASON_XSOZHALENIE         241
#define REASON_XSTYID              242
#define REASON_XRAZRUSHENIE        243
#define REASON_XDRAKA              244
#define REASON_XUBIJSTVO           245
#define REASON_XPOZITIV            246
#define REASON_XVESELE             247
#define REASON_XRADOST             248
#define REASON_XVOSTORG            249
#define REASON_XDOVOLSTVO          250
#define REASON_XGORDOST            251
#define REASON_XUDOVLETVORENIE     252
#define REASON_XLYUBOPYITSTVO      253
#define REASON_XINTERES            254
#define REASON_XUVLECHENNOST       255
#define REASON_XAZART              256
#define REASON_XVOODUSHEVLENIE     257
#define REASON_XTSELEUSTREMLENNOST 258
#define REASON_XOTVAGA             259
#define REASON_XRESHIMOST          260
#define REASON_XVDOHNOVENIE        261
#define REASON_XPRIYAZN            262
#define REASON_XSIMPATIYA          263
#define REASON_XPRIVYAZANNOST      264
#define REASON_XLYUBOV             265
#define REASON_XVOZHDELENIE        266
#define REASON_XUMILENIE           267
#define REASON_XNADEZHDA           268
#define REASON_XPREDVKUSHENIE      269
#define REASON_XVERA               270
#define REASON_XUVERENNOST         271
#define REASON_XPOKOJ              272
#define REASON_XUMIROTVORENIE      273
#define REASON_XNOSTALGIYA         274
#define REASON_XSOPEREZHIVANIE     275
#define REASON_XSOSTRADANIE        276
#define REASON_XSOZIDANIE          277
#define REASON_XSEKS               278
#define REASON_XZHELANIE           279
#define REASON_XVLECHENIE          280
#define REASON_XSTRAST             281
#define REASON_XODERZHIMOST        282
#define REASON_XMANIYA             283
#define REASON_XZAINTERESOVANNOST  284
#define REASON_XZAVISIMOST         285
#define REASON_XDUHOVNOE           286
#define REASON_XTRANS              287
#define REASON_XMEDITATSIYA        288
#define REASON_XEKZALTATSIYA       289
#define REASON_XPROSVETLENIE       290
	// end of emotion fixes

#endif

// End of emotions.h
