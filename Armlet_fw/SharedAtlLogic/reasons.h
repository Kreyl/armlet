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
 * Generated at 2014-10-22 22:28:56
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
#define LOCATIONS_ID_END 100

#define PERSON_ID_START 101
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

#define NUMBER_OF_REASONS    200

#define REASON_ZER0            0
	// Master nodes
#define REASON_HAB             1
#define REASON_MOB2            2
#define REASON_MOB3            3
#define REASON_MOB4            4
#define REASON_MOB5            5
#define REASON_MOB6            6
#define REASON_MOB7            7
#define REASON_MOB8            8
#define REASON_MOB9            9
	// end of master nodes
	// Placeholders
#define REASON_PH10           10
#define REASON_PH11           11
#define REASON_PH12           12
#define REASON_PH13           13
#define REASON_PH14           14
#define REASON_PH15           15
#define REASON_PH16           16
#define REASON_PH17           17
#define REASON_PH18           18
#define REASON_PH19           19
#define REASON_PH20           20
#define REASON_PH21           21
#define REASON_PH22           22
#define REASON_PH23           23
#define REASON_PH24           24
#define REASON_PH25           25
#define REASON_PH26           26
#define REASON_PH27           27
#define REASON_PH28           28
#define REASON_PH29           29
#define REASON_PH30           30
#define REASON_PH31           31
#define REASON_PH32           32
#define REASON_PH33           33
#define REASON_PH34           34
#define REASON_PH35           35
#define REASON_PH36           36
#define REASON_PH37           37
#define REASON_PH38           38
#define REASON_PH39           39
#define REASON_PH40           40
#define REASON_PH41           41
#define REASON_PH42           42
#define REASON_PH43           43
#define REASON_PH44           44
#define REASON_PH45           45
#define REASON_PH46           46
#define REASON_PH47           47
#define REASON_PH48           48
#define REASON_PH49           49
	// end of placeholders
	// Locations
#define REASON_GLAVNYIJ       50
#define REASON_BAR            51
#define REASON_BOLSHAYA       52
#define REASON_MALAYA         53
#define REASON_PEREGOVORKA    54
#define REASON_1_HOLL         55
#define REASON_KURILKA        56
#define REASON_2_LESTNITSA    57
#define REASON_2_HOLL         58
#define REASON_2_LEVYIJ_1     59
#define REASON_2_PRAVYIJ_1    60
#define REASON_2_LEVYIJ_2     61
#define REASON_2_PRAVYIJ_2    62
#define REASON_2_LEVYIJ_3     63
#define REASON_3_LESTNITSA    64
#define REASON_3_HOLL         65
#define REASON_3_LEVYIJ_1     66
#define REASON_3_PRAVYIJ_1    67
#define REASON_3_LEVYIJ_2     68
#define REASON_3_PRAVYIJ_2    69
#define REASON_3_LEVYIJ_3     70
	// end of locations
#define REASON_R071           71
#define REASON_R072           72
#define REASON_R073           73
#define REASON_R074           74
#define REASON_R075           75
#define REASON_R076           76
#define REASON_R077           77
#define REASON_R078           78
#define REASON_R079           79
#define REASON_R080           80
#define REASON_R081           81
#define REASON_R082           82
#define REASON_R083           83
#define REASON_R084           84
#define REASON_R085           85
#define REASON_R086           86
#define REASON_R087           87
#define REASON_R088           88
#define REASON_R089           89
#define REASON_R090           90
#define REASON_R091           91
#define REASON_R092           92
#define REASON_R093           93
#define REASON_R094           94
#define REASON_R095           95
#define REASON_R096           96
#define REASON_R097           97
#define REASON_R098           98
#define REASON_R099           99
#define REASON_R100          100
	// People
#define REASON_SHIKO         101
#define REASON_SKAZKA        102
#define REASON_NUSI          103
#define REASON_VEDMA         104
#define REASON_KUZMIN        105
#define REASON_ALKVI         106
#define REASON_MAKDUF        107
#define REASON_AJKA          108
#define REASON_TSINIK        109
#define REASON_ASTAYA        110
#define REASON_KLER          111
#define REASON_VAR           112
#define REASON_SLONTUS       113
#define REASON_SEATA         114
#define REASON_YOVIN         115
#define REASON_VOROBEJKA     116
#define REASON_ORLIK         117
#define REASON_FRIKSI        118
#define REASON_GOLOVANOV     119
#define REASON_KSING         120
#define REASON_RI            121
#define REASON_VETER         122
#define REASON_DEMETRIUS     123
#define REASON_TONYICH       124
#define REASON_HEL           125
#define REASON_KOLENKA       126
#define REASON_DONNA_ANNA    127
#define REASON_MASHKA_L      128
#define REASON_JOLAF         129
#define REASON_RINGIL        130
#define REASON_MORKOVKA      131
#define REASON_VARPO         132
#define REASON_VER           133
#define REASON_NASTYA_K      134
#define REASON_STASKA        135
#define REASON_RUTA          136
#define REASON_RUMATA        137
#define REASON_LISA          138
#define REASON_FRIDA         139
#define REASON_ANZHEJ        140
#define REASON_KRIVDA        141
#define REASON_KROLL         142
#define REASON_KREJL         143
#define REASON_MAKSI         144
#define REASON_KORSAR        145
#define REASON_REJEGAR       146
#define REASON_MISHA         147
#define REASON_DAIR          148
#define REASON_MIRABEL       149
#define REASON_FIDEL         150
#define REASON_LUSTBERG      151
#define REASON_HIUS          152
#define REASON_TEONI         153
#define REASON_HLYON         154
#define REASON_MEDIK         155
#define REASON_LVYONOK       156
#define REASON_ALEKSANDR     157
#define REASON_OELUN         158
#define REASON_SALIM         159
#define REASON_YAROSLAVNA    160
#define REASON_KLEPA         161
#define REASON_DIONIS        162
#define REASON_RYIZHIJ_LIS   163
#define REASON_KIRA          164
#define REASON_GREJ          165
#define REASON_URFIN         166
#define REASON_ASYA          167
#define REASON_HELKA         168
#define REASON_IDEN          169
#define REASON_SARYO         170
#define REASON_DSLAVA        171
#define REASON_RINGLIN       172
#define REASON_FRAM          173
#define REASON_LENORAN       174
#define REASON_SAPFIRKA      175
#define REASON_KRAJK         176
#define REASON_BEZYIMYANNAYA 177
#define REASON_ENNO          178
#define REASON_FEANUR        179
#define REASON_SKADI         180
#define REASON_KSANA         181
#define REASON_KENDER        182
#define REASON_FLO           183
#define REASON_TROITSKAYA    184
#define REASON_MILENA        185
#define REASON_FANTOM        186
#define REASON_AREN          187
#define REASON_ZHENECHKA     188
#define REASON_LEO           189
#define REASON_ATANA         190
#define REASON_LILIAN        191
#define REASON_SHILOV        192
#define REASON_SHIHANA       193
#define REASON_KAMILL        194
	// end of people
#define REASON_P195          195
#define REASON_P196          196
#define REASON_P197          197
#define REASON_P198          198
#define REASON_P199          199

#endif // ifdef REASONS_H

// End of reasons.h
