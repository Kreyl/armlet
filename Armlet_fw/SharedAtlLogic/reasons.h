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
 * Generated at 2014-10-14 20:23:28
 */
#ifndef REASONS_H
#define REASONS_H

//
// ID limits
//

#define MASTER_ID_START 1
#define MASTER_ID_END 9

#define PLACEHOLDER_ID_START 10
#define PLACEHOLDER_ID_END 49

#define LOCATION_ID_START 50
#define LOCATIONS_ID_END 99

#define PERSON_ID_START 100
#define PERSON_ID_END 199

#define MAX_MESH_ID 199

#define MAX_ID 199

//
// Reasons
//

typedef struct Reason {
    const char* name;
} Reason_t;

extern Reason_t reasons[];

#define NUMBER_OF_REASONS   200

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
	// People
#define REASON_JOLAF        100
#define REASON_KREJL        101
#define REASON_MBANKS       102
#define REASON_BBERNHARD    103
#define REASON_ABROWN       104
#define REASON_FBUSTER      105
#define REASON_ZCAMPBELL    106
#define REASON_NCASSIDY     107
#define REASON_SCLAYTON     108
#define REASON_JCOLLINS     109
#define REASON_VCOLLINS     110
#define REASON_CCOMES       111
#define REASON_RCONNOR      112
#define REASON_GCROSS       113
#define REASON_FCUNNINGHAM  114
#define REASON_MDAWKINS     115
#define REASON_JDAWSON      116
#define REASON_GDAY         117
#define REASON_ADECKER      118
#define REASON_CDELOREAN    119
#define REASON_JDOE         120
#define REASON_MDURDEN      121
#define REASON_DELIS        122
#define REASON_NEPSTEIN     123
#define REASON_CFRANKLIN    124
#define REASON_LGEERTZ      125
#define REASON_AGRAYWOOD    126
#define REASON_KGREENHALL   127
#define REASON_AGRISSOM     128
#define REASON_GUIDE        129
#define REASON_MHAILEY      130
#define REASON_ZHARRIOT     131
#define REASON_MHARVENT     132
#define REASON_BHEART       133
#define REASON_BHILL        134
#define REASON_CHONZO       135
#define REASON_AHUXLEY      136
#define REASON_AJANE        137
#define REASON_AJONES       138
#define REASON_EJONES       139
#define REASON_SKENDALL     140
#define REASON_LLANE        141
#define REASON_SLARKEN      142
#define REASON_ALEE         143
#define REASON_CLEFEBVRE    144
#define REASON_VLEHNDORFF   145
#define REASON_TMALONE      146
#define REASON_AMAY         147
#define REASON_UMCGEE       148
#define REASON_FMILLIGAN    149
#define REASON_AMORIENTES   150
#define REASON_MNOVAK       151
#define REASON_SOREILLY     152
#define REASON_JOSULLIVAN   153
#define REASON_MPARCS       154
#define REASON_SRANDOM      155
#define REASON_RRAY         156
#define REASON_MREED        157
#define REASON_MROSE        158
#define REASON_DSELDON      159
#define REASON_PSHANNON     160
#define REASON_JSHERMAN     161
#define REASON_DSILVERSTONE 162
#define REASON_RSOYFER      163
#define REASON_JSPENCER     164
#define REASON_CSTANFIELD   165
#define REASON_DSTANLEY     166
#define REASON_JSULLIVAN    167
#define REASON_SSUMMER      168
#define REASON_TSUNDAY      169
#define REASON_WTHOMAS      170
#define REASON_ETHORP       171
#define REASON_EWILLIAMS    172
#define REASON_HWILLIAMS    173
#define REASON_MWRIGHT      174
#define REASON_LYOUNG       175
#define REASON_NYOUNG       176
#define REASON_ATEST        177
#define REASON_BTEST        178
	// end of people
#define REASON_R179         179
#define REASON_R180         180
#define REASON_R181         181
#define REASON_R182         182
#define REASON_R183         183
#define REASON_R184         184
#define REASON_R185         185
#define REASON_R186         186
#define REASON_R187         187
#define REASON_R188         188
#define REASON_R189         189
#define REASON_R190         190
#define REASON_R191         191
#define REASON_R192         192
#define REASON_R193         193
#define REASON_R194         194
#define REASON_R195         195
#define REASON_R196         196
#define REASON_R197         197
#define REASON_R198         198
#define REASON_R199         199

#endif // ifdef REASONS_H

// End of reasons.h
