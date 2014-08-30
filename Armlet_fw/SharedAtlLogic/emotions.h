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
 * Generated at 2014-08-30 21:09:19
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

#define NUMBER_OF_EMOTIONS         89

#define EMOTION_FON                 0
#define EMOTION_NEVERNO             1
#define EMOTION_MASTERKA            2
#define EMOTION_TISHINA             3
#define EMOTION_TUMAN               4
#define EMOTION_SMERT               5
#define EMOTION_MERTVYAK            6
#define EMOTION_SERDTSEBIENIE       7
#define EMOTION_TRIP                8
#define EMOTION_LES                 9
#define EMOTION_NEGATIV            10
#define EMOTION_RAVNODUSHIE        11
#define EMOTION_SKUKA              12
#define EMOTION_APATIYA            13
#define EMOTION_DEPRESSIYA         14
#define EMOTION_ZLOST              15
#define EMOTION_GNEV               16
#define EMOTION_YAROST             17
#define EMOTION_RAZDRAZHENIE       18
#define EMOTION_GRUST              19
#define EMOTION_TOSKA              20
#define EMOTION_OBRECHENNOST       21
#define EMOTION_OTCHAYANIE         22
#define EMOTION_NEUVERENNOST       23
#define EMOTION_OZABOCHENNOST      24
#define EMOTION_NAPRYAZHENNOST     25
#define EMOTION_ISTERIKA           26
#define EMOTION_SOMNENIE           27
#define EMOTION_TREVOZHNOST        28
#define EMOTION_STRAH              29
#define EMOTION_UZHAS              30
#define EMOTION_PANIKA             31
#define EMOTION_FOBIYA             32
#define EMOTION_PARANOJYA          33
#define EMOTION_NEPRIYAZN          34
#define EMOTION_NENAVIST           35
#define EMOTION_OTVRASCHENIE       36
#define EMOTION_PREZRENIE          37
#define EMOTION_NEUDOVLETVORENNOST 38
#define EMOTION_SOZHALENIE         39
#define EMOTION_STYID              40
#define EMOTION_RAZRUSHENIE        41
#define EMOTION_DRAKA              42
#define EMOTION_UBIJSTVO           43
#define EMOTION_POZITIV            44
#define EMOTION_VESELE             45
#define EMOTION_RADOST             46
#define EMOTION_VOSTORG            47
#define EMOTION_DOVOLSTVO          48
#define EMOTION_GORDOST            49
#define EMOTION_UDOVLETVORENIE     50
#define EMOTION_LYUBOPYITSTVO      51
#define EMOTION_INTERES            52
#define EMOTION_UVLECHENNOST       53
#define EMOTION_AZART              54
#define EMOTION_VOODUSHEVLENIE     55
#define EMOTION_TSELEUSTREMLENNOST 56
#define EMOTION_OTVAGA             57
#define EMOTION_RESHIMOST          58
#define EMOTION_VDOHNOVENIE        59
#define EMOTION_PRIYAZN            60
#define EMOTION_SIMPATIYA          61
#define EMOTION_PRIVYAZANNOST      62
#define EMOTION_LYUBOV             63
#define EMOTION_VOZHDELENIE        64
#define EMOTION_UMILENIE           65
#define EMOTION_NADEZHDA           66
#define EMOTION_PREDVKUSHENIE      67
#define EMOTION_VERA               68
#define EMOTION_UVERENNOST         69
#define EMOTION_POKOJ              70
#define EMOTION_UMIROTVORENIE      71
#define EMOTION_NOSTALGIYA         72
#define EMOTION_SOPEREZHIVANIE     73
#define EMOTION_SOSTRADANIE        74
#define EMOTION_SOZIDANIE          75
#define EMOTION_SEKS               76
#define EMOTION_ZHELANIE           77
#define EMOTION_VLECHENIE          78
#define EMOTION_STRAST             79
#define EMOTION_ODERZHIMOST        80
#define EMOTION_MANIYA             81
#define EMOTION_ZAINTERESOVANNOST  82
#define EMOTION_ZAVISIMOST         83
#define EMOTION_DUHOVNOE           84
#define EMOTION_TRANS              85
#define EMOTION_MEDITATSIYA        86
#define EMOTION_EKZALTATSIYA       87
#define EMOTION_PROSVETLENIE       88

//
// Reasons
//

typedef struct Reason {
    const char* name;
    int weight;
    int age;
    int eID;
} Reason_t;

extern Reason_t reasons[];

#define NUMBER_OF_REASONS         289

#define REASON_R000                 0
	// Locations
#define REASON_HUB                  1
#define REASON_DEAD                 2
#define REASON_HERMIT               3
#define REASON_SQUARE               4
#define REASON_FACTORY              5
#define REASON_CITYHALL             6
#define REASON_STATION              7
#define REASON_BAR                  8
#define REASON_COFFEE               9
#define REASON_DETSKAYA            10
#define REASON_TENT                11
#define REASON_HOSPITAL            12
#define REASON_UNIVERSITY          13
#define REASON_REDROOM             14
#define REASON_SHOP                15
#define REASON_BLACKHOUSE          16
#define REASON_HOME                17
#define REASON_TRENCH              18
#define REASON_CINEMA              19
	// end of locations
#define REASON_R020                20
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
#define REASON_HEROIN              189
#define REASON_KRAYK               190
#define REASON_DEATH               191
#define REASON_MIST                192
#define REASON_FEAR                193
#define REASON_MSOURCE             194
#define REASON_MPROJECT            195
	// end of intentions
#define REASON_R196                196
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
#define REASON_XLES                209
#define REASON_XNEGATIV            210
#define REASON_XRAVNODUSHIE        211
#define REASON_XSKUKA              212
#define REASON_XAPATIYA            213
#define REASON_XDEPRESSIYA         214
#define REASON_XZLOST              215
#define REASON_XGNEV               216
#define REASON_XYAROST             217
#define REASON_XRAZDRAZHENIE       218
#define REASON_XGRUST              219
#define REASON_XTOSKA              220
#define REASON_XOBRECHENNOST       221
#define REASON_XOTCHAYANIE         222
#define REASON_XNEUVERENNOST       223
#define REASON_XOZABOCHENNOST      224
#define REASON_XNAPRYAZHENNOST     225
#define REASON_XISTERIKA           226
#define REASON_XSOMNENIE           227
#define REASON_XTREVOZHNOST        228
#define REASON_XSTRAH              229
#define REASON_XUZHAS              230
#define REASON_XPANIKA             231
#define REASON_XFOBIYA             232
#define REASON_XPARANOJYA          233
#define REASON_XNEPRIYAZN          234
#define REASON_XNENAVIST           235
#define REASON_XOTVRASCHENIE       236
#define REASON_XPREZRENIE          237
#define REASON_XNEUDOVLETVORENNOST 238
#define REASON_XSOZHALENIE         239
#define REASON_XSTYID              240
#define REASON_XRAZRUSHENIE        241
#define REASON_XDRAKA              242
#define REASON_XUBIJSTVO           243
#define REASON_XPOZITIV            244
#define REASON_XVESELE             245
#define REASON_XRADOST             246
#define REASON_XVOSTORG            247
#define REASON_XDOVOLSTVO          248
#define REASON_XGORDOST            249
#define REASON_XUDOVLETVORENIE     250
#define REASON_XLYUBOPYITSTVO      251
#define REASON_XINTERES            252
#define REASON_XUVLECHENNOST       253
#define REASON_XAZART              254
#define REASON_XVOODUSHEVLENIE     255
#define REASON_XTSELEUSTREMLENNOST 256
#define REASON_XOTVAGA             257
#define REASON_XRESHIMOST          258
#define REASON_XVDOHNOVENIE        259
#define REASON_XPRIYAZN            260
#define REASON_XSIMPATIYA          261
#define REASON_XPRIVYAZANNOST      262
#define REASON_XLYUBOV             263
#define REASON_XVOZHDELENIE        264
#define REASON_XUMILENIE           265
#define REASON_XNADEZHDA           266
#define REASON_XPREDVKUSHENIE      267
#define REASON_XVERA               268
#define REASON_XUVERENNOST         269
#define REASON_XPOKOJ              270
#define REASON_XUMIROTVORENIE      271
#define REASON_XNOSTALGIYA         272
#define REASON_XSOPEREZHIVANIE     273
#define REASON_XSOSTRADANIE        274
#define REASON_XSOZIDANIE          275
#define REASON_XSEKS               276
#define REASON_XZHELANIE           277
#define REASON_XVLECHENIE          278
#define REASON_XSTRAST             279
#define REASON_XODERZHIMOST        280
#define REASON_XMANIYA             281
#define REASON_XZAINTERESOVANNOST  282
#define REASON_XZAVISIMOST         283
#define REASON_XDUHOVNOE           284
#define REASON_XTRANS              285
#define REASON_XMEDITATSIYA        286
#define REASON_XEKZALTATSIYA       287
#define REASON_XPROSVETLENIE       288
	// end of emotion fixes

#endif

// End of emotions.h
