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
 * Generated at 2014-09-23 00:47:14
 */
#ifndef REASONS_H
#define REASONS_H

//
// ID limits
//

#define LOCATION_ID_START 1
#define LOCATIONS_ID_END 47

#define PLACEHOLDER_ID_START 48
#define PLACEHOLDER_ID_END 99

#define FOREST_ID_START 150
#define FOREST_ID_END 199

#define PERSON_ID_START 201
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

#define NUMBER_OF_REASONS  300

#define REASON_ZER0          0
	// Locations
#define REASON_HUB           1
#define REASON_HERMIT        2
#define REASON_SQUARE        3
#define REASON_FACTORY       4
#define REASON_CITYHALL      5
#define REASON_STATION       6
#define REASON_BAR           7
#define REASON_COFFEE        8
	// end of locations
#define REASON_R009          9
#define REASON_R010         10
#define REASON_R011         11
#define REASON_R012         12
#define REASON_R013         13
#define REASON_R014         14
#define REASON_R015         15
#define REASON_R016         16
#define REASON_R017         17
#define REASON_R018         18
#define REASON_R019         19
#define REASON_R020         20
#define REASON_R021         21
#define REASON_R022         22
#define REASON_R023         23
#define REASON_R024         24
#define REASON_R025         25
#define REASON_R026         26
#define REASON_R027         27
#define REASON_R028         28
#define REASON_R029         29
#define REASON_R030         30
#define REASON_R031         31
#define REASON_R032         32
#define REASON_R033         33
#define REASON_R034         34
#define REASON_R035         35
#define REASON_R036         36
#define REASON_R037         37
#define REASON_R038         38
#define REASON_R039         39
#define REASON_R040         40
#define REASON_R041         41
#define REASON_R042         42
#define REASON_R043         43
#define REASON_R044         44
#define REASON_R045         45
#define REASON_R046         46
#define REASON_R047         47
	// Placeholders
#define REASON_PH48         48
#define REASON_PH49         49
#define REASON_PH50         50
#define REASON_PH51         51
#define REASON_PH52         52
#define REASON_PH53         53
#define REASON_PH54         54
#define REASON_PH55         55
#define REASON_PH56         56
#define REASON_PH57         57
#define REASON_PH58         58
#define REASON_PH59         59
#define REASON_PH60         60
#define REASON_PH61         61
#define REASON_PH62         62
#define REASON_PH63         63
#define REASON_PH64         64
#define REASON_PH65         65
#define REASON_PH66         66
#define REASON_PH67         67
#define REASON_PH68         68
#define REASON_PH69         69
#define REASON_PH70         70
#define REASON_PH71         71
#define REASON_PH72         72
#define REASON_PH73         73
#define REASON_PH74         74
#define REASON_PH75         75
#define REASON_PH76         76
#define REASON_PH77         77
#define REASON_PH78         78
#define REASON_PH79         79
#define REASON_PH80         80
#define REASON_PH81         81
#define REASON_PH82         82
#define REASON_PH83         83
#define REASON_PH84         84
#define REASON_PH85         85
#define REASON_PH86         86
#define REASON_PH87         87
#define REASON_PH88         88
#define REASON_PH89         89
#define REASON_PH90         90
#define REASON_PH91         91
#define REASON_PH92         92
#define REASON_PH93         93
#define REASON_PH94         94
#define REASON_PH95         95
#define REASON_PH96         96
#define REASON_PH97         97
#define REASON_PH98         98
#define REASON_PH99         99
	// end of placeholders
#define REASON_R100         100
#define REASON_R101         101
#define REASON_R102         102
#define REASON_R103         103
#define REASON_R104         104
#define REASON_R105         105
#define REASON_R106         106
#define REASON_R107         107
#define REASON_R108         108
#define REASON_R109         109
#define REASON_R110         110
#define REASON_R111         111
#define REASON_R112         112
#define REASON_R113         113
#define REASON_R114         114
#define REASON_R115         115
#define REASON_R116         116
#define REASON_R117         117
#define REASON_R118         118
#define REASON_R119         119
#define REASON_R120         120
#define REASON_R121         121
#define REASON_R122         122
#define REASON_R123         123
#define REASON_R124         124
#define REASON_R125         125
#define REASON_R126         126
#define REASON_R127         127
#define REASON_R128         128
#define REASON_R129         129
#define REASON_R130         130
#define REASON_R131         131
#define REASON_R132         132
#define REASON_R133         133
#define REASON_R134         134
#define REASON_R135         135
#define REASON_R136         136
#define REASON_R137         137
#define REASON_R138         138
#define REASON_R139         139
#define REASON_R140         140
#define REASON_R141         141
#define REASON_R142         142
#define REASON_R143         143
#define REASON_R144         144
#define REASON_R145         145
#define REASON_R146         146
#define REASON_R147         147
#define REASON_R148         148
#define REASON_R149         149
	// Forest lights
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
#define REASON_R200         200
	// People
#define REASON_JOLAF        201
#define REASON_KREJL        202
#define REASON_MBANKS       203
#define REASON_BBERNHARD    204
#define REASON_ABROWN       205
#define REASON_FBUSTER      206
#define REASON_ZCAMPBELL    207
#define REASON_NCASSIDY     208
#define REASON_SCLAYTON     209
#define REASON_JCOLLINS     210
#define REASON_VCOLLINS     211
#define REASON_CCOMES       212
#define REASON_RCONNOR      213
#define REASON_GCROSS       214
#define REASON_FCUNNINGHAM  215
#define REASON_MDAWKINS     216
#define REASON_JDAWSON      217
#define REASON_GDAY         218
#define REASON_ADECKER      219
#define REASON_CDELOREAN    220
#define REASON_JDOE         221
#define REASON_MDURDEN      222
#define REASON_DELIS        223
#define REASON_NEPSTEIN     224
#define REASON_CFRANKLIN    225
#define REASON_LGEERTZ      226
#define REASON_AGRAYWOOD    227
#define REASON_KGREENHALL   228
#define REASON_AGRISSOM     229
#define REASON_GUIDE        230
#define REASON_MHAILEY      231
#define REASON_ZHARRIOT     232
#define REASON_MHARVENT     233
#define REASON_BHEART       234
#define REASON_BHILL        235
#define REASON_CHONZO       236
#define REASON_AHUXLEY      237
#define REASON_AJANE        238
#define REASON_AJONES       239
#define REASON_EJONES       240
#define REASON_SKENDALL     241
#define REASON_LLANE        242
#define REASON_SLARKEN      243
#define REASON_ALEE         244
#define REASON_CLEFEBVRE    245
#define REASON_VLEHNDORFF   246
#define REASON_TMALONE      247
#define REASON_AMAY         248
#define REASON_UMCGEE       249
#define REASON_FMILLIGAN    250
#define REASON_AMORIENTES   251
#define REASON_MNOVAK       252
#define REASON_SOREILLY     253
#define REASON_JOSULLIVAN   254
#define REASON_MPARCS       255
#define REASON_SRANDOM      256
#define REASON_RRAY         257
#define REASON_MREED        258
#define REASON_MROSE        259
#define REASON_DSELDON      260
#define REASON_PSHANNON     261
#define REASON_JSHERMAN     262
#define REASON_DSILVERSTONE 263
#define REASON_RSOYFER      264
#define REASON_JSPENCER     265
#define REASON_CSTANFIELD   266
#define REASON_DSTANLEY     267
#define REASON_JSULLIVAN    268
#define REASON_SSUMMER      269
#define REASON_TSUNDAY      270
#define REASON_WTHOMAS      271
#define REASON_ETHORP       272
#define REASON_EWILLIAMS    273
#define REASON_HWILLIAMS    274
#define REASON_MWRIGHT      275
#define REASON_LYOUNG       276
#define REASON_NYOUNG       277
#define REASON_ATEST        278
#define REASON_BTEST        279
	// end of people
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
