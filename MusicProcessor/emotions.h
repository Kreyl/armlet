/*
 * emotions.h
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Must be compatible with emotions.c that is generated automatically by EP.py
 *
 */
#ifndef EMOTIONS_H
#define EMOTIONS_H

#define ROOT -1

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

typedef struct Emotion {
    const char* name;
    const int weight;
    const int parent;
    int numTracks;
} Emotion_t;

extern Emotion_t emotions[];
extern const int emotions_number;

typedef struct Reason {
    const char* name;
    int weight;
    int eID;
} Reason_t;

extern Reason_t reasons[];
extern const int reasons_number;

#endif

// End of emotions.h
