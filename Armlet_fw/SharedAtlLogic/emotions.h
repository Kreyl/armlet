/*
 * emotions.h
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from Master.csv, Locations.csv, Lodges.csv, Characters.csv, Intentions.csv
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-09-07 03:21:55
 */
#ifndef EMOTIONS_H
#define EMOTIONS_H

#define ROOT -1

//
// ID limits
//

#define MASTER_ID_START 1
#define MASTER_ID_END 6

#define MOB_ID_START 7
#define MOB_ID_END 10

#define PLACEHOLDER_ID_START 11
#define PLACEHOLDER_ID_END 40

#define LOCATION_ID_START 41
#define LOCATIONS_ID_END 47

#define FORESTA_ID_START 48
#define FORESTA_ID_END 89

#define LODGE_ID_START 90
#define LODGE_ID_END 99

#define EMOTION_FIX_ID_START 100
#define EMOTION_FIX_ID_END 190

#define MIST_ID_START 191
#define MIST_ID_END 200

#define CHARACTER_ID_START 201
#define CHARACTER_ID_END 279

#define FORESTBC_ID_START 280
#define FORESTBC_ID_END 299

#define MAX_MEsH_ID 299

#define INTENTION_ID_START 300
#define INTENTION_ID_END 320

#define MAX_ID 320

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

#define NUMBER_OF_REASONS         318

#define REASON_ZER0                 0
	// Master locations
#define REASON_HUB                  1
#define REASON_MASTER               2
#define REASON_DEAD                 3
#define REASON_NOFEAR               4
#define REASON_REBOOT               5
#define REASON_RESET                6
	// end of master locations
	// MOBs
#define REASON_MOB07                7
#define REASON_MOB08                8
#define REASON_MOB09                9
#define REASON_MOB10               10
	// end of MOBs
	// Placeholders
#define REASON_PH11                11
#define REASON_PH12                12
#define REASON_PH13                13
#define REASON_PH14                14
#define REASON_PH15                15
#define REASON_PH16                16
#define REASON_PH17                17
#define REASON_PH18                18
#define REASON_PH19                19
#define REASON_PH20                20
#define REASON_PH21                21
#define REASON_PH22                22
#define REASON_PH23                23
#define REASON_PH24                24
#define REASON_PH25                25
#define REASON_PH26                26
#define REASON_PH27                27
#define REASON_PH28                28
#define REASON_PH29                29
#define REASON_PH30                30
#define REASON_PH31                31
#define REASON_PH32                32
#define REASON_PH33                33
#define REASON_PH34                34
#define REASON_PH35                35
#define REASON_PH36                36
#define REASON_PH37                37
#define REASON_PH38                38
#define REASON_PH39                39
#define REASON_PH40                40
	// end of placeholders
	// City locations
#define REASON_HERMIT              41
#define REASON_SQUARE              42
#define REASON_FACTORY             43
#define REASON_CITYHALL            44
#define REASON_STATION             45
#define REASON_BAR                 46
#define REASON_COFFEE              47
	// end of city locations
	// Forest A lights
#define REASON_FA48                48
#define REASON_FA49                49
#define REASON_FA50                50
#define REASON_FA51                51
#define REASON_FA52                52
#define REASON_FA53                53
#define REASON_FA54                54
#define REASON_FA55                55
#define REASON_FA56                56
#define REASON_FA57                57
#define REASON_FA58                58
#define REASON_FA59                59
#define REASON_FA60                60
#define REASON_FA61                61
#define REASON_FA62                62
#define REASON_FA63                63
#define REASON_FA64                64
#define REASON_FA65                65
#define REASON_FA66                66
#define REASON_FA67                67
#define REASON_FA68                68
#define REASON_FA69                69
#define REASON_FA70                70
#define REASON_FA71                71
#define REASON_FA72                72
#define REASON_FA73                73
#define REASON_FA74                74
#define REASON_FA75                75
#define REASON_FA76                76
#define REASON_FA77                77
#define REASON_FA78                78
#define REASON_FA79                79
#define REASON_FA80                80
#define REASON_FA81                81
#define REASON_FA82                82
#define REASON_FA83                83
#define REASON_FA84                84
#define REASON_FA85                85
#define REASON_FA86                86
#define REASON_FA87                87
#define REASON_FA88                88
#define REASON_FA89                89
	// end of forest A lights
	// Lodge locations
#define REASON_DETSKAYA            90
#define REASON_TENT                91
#define REASON_HOSPITAL            92
#define REASON_UNIVERSITY          93
#define REASON_REDROOM             94
#define REASON_SHOP                95
#define REASON_BLACKHOUSE          96
#define REASON_HOME                97
#define REASON_TRENCH              98
#define REASON_CINEMA              99
	// end of lodge locations
	// Emotion fixes
#define REASON_XFON                100
#define REASON_XNEVERNO            101
#define REASON_XMASTERKA           102
#define REASON_XTISHINA            103
#define REASON_XTUMAN              104
#define REASON_XSMERT              105
#define REASON_XMERTVYAK           106
#define REASON_XSERDTSEBIENIE      107
#define REASON_XTRIP               108
#define REASON_XKAKTUS             109
#define REASON_XNARKOMAN           110
#define REASON_XLES                111
#define REASON_XNEGATIV            112
#define REASON_XRAVNODUSHIE        113
#define REASON_XSKUKA              114
#define REASON_XAPATIYA            115
#define REASON_XDEPRESSIYA         116
#define REASON_XZLOST              117
#define REASON_XGNEV               118
#define REASON_XYAROST             119
#define REASON_XRAZDRAZHENIE       120
#define REASON_XGRUST              121
#define REASON_XTOSKA              122
#define REASON_XOBRECHENNOST       123
#define REASON_XOTCHAYANIE         124
#define REASON_XNEUVERENNOST       125
#define REASON_XOZABOCHENNOST      126
#define REASON_XNAPRYAZHENNOST     127
#define REASON_XISTERIKA           128
#define REASON_XSOMNENIE           129
#define REASON_XTREVOZHNOST        130
#define REASON_XSTRAH              131
#define REASON_XUZHAS              132
#define REASON_XPANIKA             133
#define REASON_XFOBIYA             134
#define REASON_XPARANOJYA          135
#define REASON_XNEPRIYAZN          136
#define REASON_XNENAVIST           137
#define REASON_XOTVRASCHENIE       138
#define REASON_XPREZRENIE          139
#define REASON_XNEUDOVLETVORENNOST 140
#define REASON_XSOZHALENIE         141
#define REASON_XSTYID              142
#define REASON_XRAZRUSHENIE        143
#define REASON_XDRAKA              144
#define REASON_XUBIJSTVO           145
#define REASON_XPOZITIV            146
#define REASON_XVESELE             147
#define REASON_XRADOST             148
#define REASON_XVOSTORG            149
#define REASON_XDOVOLSTVO          150
#define REASON_XGORDOST            151
#define REASON_XUDOVLETVORENIE     152
#define REASON_XLYUBOPYITSTVO      153
#define REASON_XINTERES            154
#define REASON_XUVLECHENNOST       155
#define REASON_XAZART              156
#define REASON_XVOODUSHEVLENIE     157
#define REASON_XTSELEUSTREMLENNOST 158
#define REASON_XOTVAGA             159
#define REASON_XRESHIMOST          160
#define REASON_XVDOHNOVENIE        161
#define REASON_XPRIYAZN            162
#define REASON_XSIMPATIYA          163
#define REASON_XPRIVYAZANNOST      164
#define REASON_XLYUBOV             165
#define REASON_XVOZHDELENIE        166
#define REASON_XUMILENIE           167
#define REASON_XNADEZHDA           168
#define REASON_XPREDVKUSHENIE      169
#define REASON_XVERA               170
#define REASON_XUVERENNOST         171
#define REASON_XPOKOJ              172
#define REASON_XUMIROTVORENIE      173
#define REASON_XNOSTALGIYA         174
#define REASON_XSOPEREZHIVANIE     175
#define REASON_XSOSTRADANIE        176
#define REASON_XSOZIDANIE          177
#define REASON_XSEKS               178
#define REASON_XZHELANIE           179
#define REASON_XVLECHENIE          180
#define REASON_XSTRAST             181
#define REASON_XODERZHIMOST        182
#define REASON_XMANIYA             183
#define REASON_XZAINTERESOVANNOST  184
#define REASON_XZAVISIMOST         185
#define REASON_XDUHOVNOE           186
#define REASON_XTRANS              187
#define REASON_XMEDITATSIYA        188
#define REASON_XEKZALTATSIYA       189
#define REASON_XPROSVETLENIE       190
	// end of emotion fixes
	// Mist sources
#define REASON_M191                191
#define REASON_M192                192
#define REASON_M193                193
#define REASON_M194                194
#define REASON_M195                195
#define REASON_M196                196
#define REASON_M197                197
#define REASON_M198                198
#define REASON_M199                199
#define REASON_M200                200
	// end of mist sources
	// Characters
#define REASON_PADAMS              201
#define REASON_DALLGOOD            202
#define REASON_MBANKS              203
#define REASON_BBERNHARD           204
#define REASON_ABROWN              205
#define REASON_FBUSTER             206
#define REASON_ZCAMPBELL           207
#define REASON_NCASSIDY            208
#define REASON_SCLAYTON            209
#define REASON_JCOLLINS            210
#define REASON_VCOLLINS            211
#define REASON_CCOMES              212
#define REASON_RCONNOR             213
#define REASON_GCROSS              214
#define REASON_FCUNNINGHAM         215
#define REASON_MDAWKINS            216
#define REASON_JDAWSON             217
#define REASON_GDAY                218
#define REASON_ADECKER             219
#define REASON_CDELOREAN           220
#define REASON_JDOE                221
#define REASON_MDURDEN             222
#define REASON_DELIS               223
#define REASON_NEPSTEIN            224
#define REASON_CFRANKLIN           225
#define REASON_LGEERTZ             226
#define REASON_AGRAYWOOD           227
#define REASON_KGREENHALL          228
#define REASON_AGRISSOM            229
#define REASON_GUIDE               230
#define REASON_MHAILEY             231
#define REASON_ZHARRIOT            232
#define REASON_MHARVENT            233
#define REASON_BHEART              234
#define REASON_BHILL               235
#define REASON_CHONZO              236
#define REASON_AHUXLEY             237
#define REASON_AJANE               238
#define REASON_AJONES              239
#define REASON_EJONES              240
#define REASON_SKENDALL            241
#define REASON_LLANE               242
#define REASON_SLARKEN             243
#define REASON_ALEE                244
#define REASON_CLEFEBVRE           245
#define REASON_VLEHNDORFF          246
#define REASON_TMALONE             247
#define REASON_AMAY                248
#define REASON_UMCGEE              249
#define REASON_FMILLIGAN           250
#define REASON_AMORIENTES          251
#define REASON_MNOVAK              252
#define REASON_SOREILLY            253
#define REASON_JOSULLIVAN          254
#define REASON_MPARCS              255
#define REASON_SRANDOM             256
#define REASON_RRAY                257
#define REASON_MREED               258
#define REASON_MROSE               259
#define REASON_DSELDON             260
#define REASON_PSHANNON            261
#define REASON_JSHERMAN            262
#define REASON_DSILVERSTONE        263
#define REASON_RSOYFER             264
#define REASON_JSPENCER            265
#define REASON_CSTANFIELD          266
#define REASON_DSTANLEY            267
#define REASON_JSULLIVAN           268
#define REASON_SSUMMER             269
#define REASON_TSUNDAY             270
#define REASON_WTHOMAS             271
#define REASON_ETHORP              272
#define REASON_EWILLIAMS           273
#define REASON_HWILLIAMS           274
#define REASON_MWRIGHT             275
#define REASON_LYOUNG              276
#define REASON_NYOUNG              277
#define REASON_ATEST               278
#define REASON_BTEST               279
	// end of characters
	// Forest BC lights
#define REASON_FB280               280
#define REASON_FC281               281
#define REASON_FB282               282
#define REASON_FC283               283
#define REASON_FB284               284
#define REASON_FC285               285
#define REASON_FB286               286
#define REASON_FC287               287
#define REASON_FB288               288
#define REASON_FC289               289
#define REASON_FB290               290
#define REASON_FC291               291
#define REASON_FB292               292
#define REASON_FC293               293
#define REASON_FB294               294
#define REASON_FC295               295
#define REASON_FB296               296
#define REASON_FC297               297
#define REASON_FB298               298
#define REASON_FC299               299
	// end of forest BC lights
	// Intentions
#define REASON_MURDER              300
#define REASON_CREATION            301
#define REASON_DESTRUCTION         302
#define REASON_SEX                 303
#define REASON_FIGHT               304
#define REASON_HEARTBEAT           305
#define REASON_ADDICTION           306
#define REASON_WEED                307
#define REASON_LSD                 308
#define REASON_PEYOTE              309
#define REASON_HEROIN              310
#define REASON_KRAYK               311
#define REASON_DEATH               312
#define REASON_MIST                313
#define REASON_FEAR                314
#define REASON_MSOURCE             315
#define REASON_MPROJECT            316
#define REASON_BG                  317
	// end of intentions

#endif // ifdef EMOTIONS_H

// End of emotions.h
