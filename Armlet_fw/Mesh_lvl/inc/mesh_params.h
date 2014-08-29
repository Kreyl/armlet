/*
1616 * mesh_params.h
 *
 *  Created on: 15 апр. 2014 г.
 *      Author: r.leonov
 */

#ifndef MESH_PARAMS_H_
#define MESH_PARAMS_H_

/*********************** MESH ******************************
 *  |_|_|_|_..._|_|_|_|_|_..._|_|_|_|_|_..._|_|   SLOTS
 *  |_____________|_______...___|_____________|   CYCLES
 *  |_____________________..._________________|   SUPER_CYCLE
 */

#ifdef STM32F2XX
#define ARMLET
#endif

#ifdef ARMLET
#include "kl_lib_f2xx.h"
#else
#include "kl_lib_L15x.h"
#endif

//#define SELF_MESH_ID        1
#define STATIONARY_ID           6
#define STATIONARY_MIN_LEVEL    -120


// ==== MESH PARAMS ====
#define MAX_ABONENTS            300   /* max ID, started from 1 */
#define MESH_CHANNEL            1     /* mesh RF channel */
#define MESH_PKT_TIME           1     /* ms */
#define MESH_GUARD_TIME         1     /* ms */

/* SLOT_TIME is equivalent to doubled time of transmit rf packet */
#define SLOT_TIME               (uint32_t)(MESH_PKT_TIME + MESH_GUARD_TIME)    /* 3 ms normal time for transmit packet, take it as 10 ms */

#define MESH_COUNT_OF_CYCLES    5     /* count of cycles in supercycle */

#define CYCLE_TIME              (uint32_t)((SLOT_TIME * MAX_ABONENTS))
#define S_CYCLE_TIME            (uint32_t)(CYCLE_TIME * COUNT_OF_CYCLES)

/* Time specific */
#define TIME_SZ                 5   //  "hh:mm\0"
#define TIME_SZ_LONG            8   // "hh:mm:ss\0"
#define MESH_MS_IN_DAY          (uint32_t)86400000

#ifdef ARMLET
#define GET_RND_VALUE(Top)  (Random(chTimeNow()) % Top)
#else
#define GET_RND_VALUE(Top)    (rand() % (Top))
#endif


#define TIME_AGE_THRESHOLD  20 /* Cycles */
#define START_CYCLE         1


struct meshradio_t {
    VirtualTimer RxVT;
    bool InRx;
    uint32_t RxEndTime;
    uint32_t CurrentTime;
};

struct state_t {
    uint16_t    Location;
    uint16_t    Reason;
    uint8_t     Emotion;
}__attribute__ ((__packed__));

struct sender_mesh_t {
    uint16_t    SelfID;
    uint32_t    CycleN;
    uint16_t    TimeOwnerID;
    uint8_t     TimeAge;
}__attribute__ ((__packed__));

struct alien_mesh_t {
    uint8_t     Hops;
    uint32_t    Timestamp;
    int32_t     TimeDiff;
}__attribute__ ((__packed__));

struct SenderInfo_t {
    sender_mesh_t   Mesh;
    state_t         State;
}__attribute__ ((__packed__));

struct AlienInfo_t {
    alien_mesh_t    Mesh;
    state_t         State;
}__attribute__ ((__packed__));

struct MeshPkt_t {
    SenderInfo_t    SenderInfo;
    uint16_t        AlienID;
    AlienInfo_t     AlienInfo;
}__attribute__ ((__packed__));

#define MESH_PKT_SZ sizeof(MeshPkt_t)

#endif /* MESH_PARAMS_H_ */
