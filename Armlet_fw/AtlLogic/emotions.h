/*
 * emotions.h
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Must be compatible with emotions.c
 * that is generated automatically by EmotionProcessor.py
 */
#ifndef EMOTIONS_H
#define EMOTIONS_H

#define ROOT -1

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

extern const int first_location_id;
extern const int last_location_id;

extern const int first_mist_id;
extern const int last_mist_id;

extern const int first_character_id;
extern const int last_character_id;

extern const int first_intention_id;
extern const int last_intention_id;

extern const int first_emotion_fix_id;
extern const int last_emotion_fix_id;

typedef struct Emotion {
    const char* name;
    const int weight;
    const int parent;
    int numTracks;
    int lastPlayedTrack;
} Emotion_t;

extern Emotion_t emotions[];
extern const int emotions_number;
//age>=0 - give me another music timer
//age<=0 - meeting important. give another here doesnt give less weight
#define AGE_SEC_REDUCE 180
#define AGE_WEIGHT_SCALE_REDUCE 1
#define AGE_MAX_WEIGHT_REDUCE 5
typedef struct Reason {
    const char* name;
    int weight;
    int age;
    int eID;
} Reason_t;

extern Reason_t reasons[];
extern const int reasons_number;

#endif

// End of emotions.h
