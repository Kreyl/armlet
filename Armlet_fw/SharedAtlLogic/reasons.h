/*
 * reasons.h
 *
 * Part of Red Arrow Mesh Demonstration.
 *
 * Generated automatically by ReasonProcessor.py
 * from Locations.csv, People.csv
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at 2014-09-26 20:09:57
 */
#ifndef REASONS_H
#define REASONS_H

//
// ID limits
//

#define MASTER_ID_START 1
#define MASTER_ID_END 9

#define PLACEHOLDER_ID_START 50
#define PLACEHOLDER_ID_END 99

#define LOCATION_ID_START 10
#define LOCATIONS_ID_END 49

#define FOREST_ID_START 100
#define FOREST_ID_END 199

#define PERSON_ID_START 200
#define PERSON_ID_END 299

#define MAX_MESH_ID 299

#define MAX_ID 299

//
// Reasons
//

typedef struct Reason {
    const char* name;
} Reason_t;

extern Reason_t reasons[];

#define NUMBER_OF_REASONS   300

#define REASON_ZER0           0
	// Master nodes
#define REASON_HAB            1
#define REASON_MOB2           2
#define REASON_MOB3           3
#define REASON_MOB4           4
#define REASON_MOB5           5
#define REASON_MOB6           6
#define REASON_MOB7           7
#define REASON_MOB8           8
#define REASON_MOB9           9
	// end of master nodes
	// Placeholders
#define REASON_PH10          10
#define REASON_PH11          11
#define REASON_PH12          12
#define REASON_PH13          13
#define REASON_PH14          14
#define REASON_PH15          15
#define REASON_PH16          16
#define REASON_PH17          17
#define REASON_PH18          18
#define REASON_PH19          19
#define REASON_PH20          20
#define REASON_PH21          21
#define REASON_PH22          22
#define REASON_PH23          23
#define REASON_PH24          24
#define REASON_PH25          25
#define REASON_PH26          26
#define REASON_PH27          27
#define REASON_PH28          28
#define REASON_PH29          29
#define REASON_PH30          30
#define REASON_PH31          31
#define REASON_PH32          32
#define REASON_PH33          33
#define REASON_PH34          34
#define REASON_PH35          35
#define REASON_PH36          36
#define REASON_PH37          37
#define REASON_PH38          38
#define REASON_PH39          39
#define REASON_PH40          40
#define REASON_PH41          41
#define REASON_PH42          42
#define REASON_PH43          43
#define REASON_PH44          44
#define REASON_PH45          45
#define REASON_PH46          46
#define REASON_PH47          47
#define REASON_PH48          48
#define REASON_PH49          49
	// end of placeholders
	// Locations
#define REASON_HOLL          50
#define REASON_BAR           51
#define REASON_BOLSHAYA      52
#define REASON_MALAYA        53
#define REASON_PEREGOVORKA   54
#define REASON_A2            55
#define REASON_A3            56
#define REASON_A4            57
#define REASON_A5            58
#define REASON_1_HOLL        59
#define REASON_1_LEVYIJ_1    60
#define REASON_1_PRAVYIJ_1   61
#define REASON_1_LEVYIJ_2    62
#define REASON_1_PRAVYIJ_2   63
#define REASON_1_LEVYIJ_3    64
#define REASON_1_PRAVYIJ_3   65
#define REASON_2_LESTNITSA   66
#define REASON_2_HOLL        67
#define REASON_2_LEVYIJ_1    68
#define REASON_2_PRAVYIJ_1   69
#define REASON_2_LEVYIJ_2    70
#define REASON_2_PRAVYIJ_2   71
#define REASON_2_LEVYIJ_3    72
#define REASON_2_PRAVYIJ_3   73
#define REASON_3_LESTNITSA   74
#define REASON_3_HOLL        75
#define REASON_3_LEVYIJ_1    76
#define REASON_3_PRAVYIJ_1   77
#define REASON_3_LEVYIJ_2    78
#define REASON_3_PRAVYIJ_2   79
#define REASON_3_LEVYIJ_3    80
#define REASON_3_PRAVYIJ_3   81
#define REASON_4_LESTNITSA   82
#define REASON_4_HOLL        83
#define REASON_4_LEVYIJ_1    84
#define REASON_4_PRAVYIJ_1   85
#define REASON_4_LEVYIJ_2    86
#define REASON_4_PRAVYIJ_2   87
#define REASON_4_LEVYIJ_3    88
#define REASON_4_PRAVYIJ_3   89
#define REASON_5_LESTNITSA   90
#define REASON_5_HOLL        91
#define REASON_5_LEVYIJ_1    92
#define REASON_5_PRAVYIJ_1   93
#define REASON_5_LEVYIJ_2    94
#define REASON_5_PRAVYIJ_2   95
#define REASON_5_LEVYIJ_3    96
#define REASON_5_PRAVYIJ_3   97
	// end of locations
#define REASON_R098          98
#define REASON_R099          99
	// Forest lights
#define REASON_FL100        100
#define REASON_FL101        101
#define REASON_FL102        102
#define REASON_FL103        103
#define REASON_FL104        104
#define REASON_FL105        105
#define REASON_FL106        106
#define REASON_FL107        107
#define REASON_FL108        108
#define REASON_FL109        109
#define REASON_FL110        110
#define REASON_FL111        111
#define REASON_FL112        112
#define REASON_FL113        113
#define REASON_FL114        114
#define REASON_FL115        115
#define REASON_FL116        116
#define REASON_FL117        117
#define REASON_FL118        118
#define REASON_FL119        119
#define REASON_FL120        120
#define REASON_FL121        121
#define REASON_FL122        122
#define REASON_FL123        123
#define REASON_FL124        124
#define REASON_FL125        125
#define REASON_FL126        126
#define REASON_FL127        127
#define REASON_FL128        128
#define REASON_FL129        129
#define REASON_FL130        130
#define REASON_FL131        131
#define REASON_FL132        132
#define REASON_FL133        133
#define REASON_FL134        134
#define REASON_FL135        135
#define REASON_FL136        136
#define REASON_FL137        137
#define REASON_FL138        138
#define REASON_FL139        139
#define REASON_FL140        140
#define REASON_FL141        141
#define REASON_FL142        142
#define REASON_FL143        143
#define REASON_FL144        144
#define REASON_FL145        145
#define REASON_FL146        146
#define REASON_FL147        147
#define REASON_FL148        148
#define REASON_FL149        149
#define REASON_FL150        150
#define REASON_FL151        151
#define REASON_FL152        152
#define REASON_FL153        153
#define REASON_FL154        154
#define REASON_FL155        155
#define REASON_FL156        156
#define REASON_FL157        157
#define REASON_FL158        158
#define REASON_FL159        159
#define REASON_FL160        160
#define REASON_FL161        161
#define REASON_FL162        162
#define REASON_FL163        163
#define REASON_FL164        164
#define REASON_FL165        165
#define REASON_FL166        166
#define REASON_FL167        167
#define REASON_FL168        168
#define REASON_FL169        169
#define REASON_FL170        170
#define REASON_FL171        171
#define REASON_FL172        172
#define REASON_FL173        173
#define REASON_FL174        174
#define REASON_FL175        175
#define REASON_FL176        176
#define REASON_FL177        177
#define REASON_FL178        178
#define REASON_FL179        179
#define REASON_FL180        180
#define REASON_FL181        181
#define REASON_FL182        182
#define REASON_FL183        183
#define REASON_FL184        184
#define REASON_FL185        185
#define REASON_FL186        186
#define REASON_FL187        187
#define REASON_FL188        188
#define REASON_FL189        189
#define REASON_FL190        190
#define REASON_FL191        191
#define REASON_FL192        192
#define REASON_FL193        193
#define REASON_FL194        194
#define REASON_FL195        195
#define REASON_FL196        196
#define REASON_FL197        197
#define REASON_FL198        198
#define REASON_FL199        199
	// end of forest lights
	// People
#define REASON_JOLAF        200
#define REASON_KREJL        201
#define REASON_MBANKS       202
#define REASON_BBERNHARD    203
#define REASON_ABROWN       204
#define REASON_FBUSTER      205
#define REASON_ZCAMPBELL    206
#define REASON_NCASSIDY     207
#define REASON_SCLAYTON     208
#define REASON_JCOLLINS     209
#define REASON_VCOLLINS     210
#define REASON_CCOMES       211
#define REASON_RCONNOR      212
#define REASON_GCROSS       213
#define REASON_FCUNNINGHAM  214
#define REASON_MDAWKINS     215
#define REASON_JDAWSON      216
#define REASON_GDAY         217
#define REASON_ADECKER      218
#define REASON_CDELOREAN    219
#define REASON_JDOE         220
#define REASON_MDURDEN      221
#define REASON_DELIS        222
#define REASON_NEPSTEIN     223
#define REASON_CFRANKLIN    224
#define REASON_LGEERTZ      225
#define REASON_AGRAYWOOD    226
#define REASON_KGREENHALL   227
#define REASON_AGRISSOM     228
#define REASON_GUIDE        229
#define REASON_MHAILEY      230
#define REASON_ZHARRIOT     231
#define REASON_MHARVENT     232
#define REASON_BHEART       233
#define REASON_BHILL        234
#define REASON_CHONZO       235
#define REASON_AHUXLEY      236
#define REASON_AJANE        237
#define REASON_AJONES       238
#define REASON_EJONES       239
#define REASON_SKENDALL     240
#define REASON_LLANE        241
#define REASON_SLARKEN      242
#define REASON_ALEE         243
#define REASON_CLEFEBVRE    244
#define REASON_VLEHNDORFF   245
#define REASON_TMALONE      246
#define REASON_AMAY         247
#define REASON_UMCGEE       248
#define REASON_FMILLIGAN    249
#define REASON_AMORIENTES   250
#define REASON_MNOVAK       251
#define REASON_SOREILLY     252
#define REASON_JOSULLIVAN   253
#define REASON_MPARCS       254
#define REASON_SRANDOM      255
#define REASON_RRAY         256
#define REASON_MREED        257
#define REASON_MROSE        258
#define REASON_DSELDON      259
#define REASON_PSHANNON     260
#define REASON_JSHERMAN     261
#define REASON_DSILVERSTONE 262
#define REASON_RSOYFER      263
#define REASON_JSPENCER     264
#define REASON_CSTANFIELD   265
#define REASON_DSTANLEY     266
#define REASON_JSULLIVAN    267
#define REASON_SSUMMER      268
#define REASON_TSUNDAY      269
#define REASON_WTHOMAS      270
#define REASON_ETHORP       271
#define REASON_EWILLIAMS    272
#define REASON_HWILLIAMS    273
#define REASON_MWRIGHT      274
#define REASON_LYOUNG       275
#define REASON_NYOUNG       276
#define REASON_ATEST        277
#define REASON_BTEST        278
	// end of people
#define REASON_R279         279
#define REASON_R280         280
#define REASON_R281         281
#define REASON_R282         282
#define REASON_R283         283
#define REASON_R284         284
#define REASON_R285         285
#define REASON_R286         286
#define REASON_R287         287
#define REASON_R288         288
#define REASON_R289         289
#define REASON_R290         290
#define REASON_R291         291
#define REASON_R292         292
#define REASON_R293         293
#define REASON_R294         294
#define REASON_R295         295
#define REASON_R296         296
#define REASON_R297         297
#define REASON_R298         298
#define REASON_R299         299

#endif // ifdef REASONS_H

// End of reasons.h
