/*
 * emotions.h
 *
 * Part of "Ticket to Atlantis" LARP music engine.
 *
 * Generated automatically by EmotionProcessor.py
 * from %s
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at %%s
 */
#ifndef EMOTIONS_H
#define EMOTIONS_H

#define ROOT -1

//
// ID limits
//

#define MASTER_ID_START %d
#define MASTER_ID_END %d

#define MOB_ID_START %d
#define MOB_ID_END %d

#define PLACEHOLDER_ID_START %d
#define PLACEHOLDER_ID_END %d

#define LOCATION_ID_START %d
#define LOCATIONS_ID_END %d

#define FORESTA_ID_START %d
#define FORESTA_ID_END %d

#define EMOTION_FIX_ID_START %d
#define EMOTION_FIX_ID_END %d

#define LODGE_ID_START %d
#define LODGE_ID_END %d

#define CHARACTER_ID_START %d
#define CHARACTER_ID_END %d

#define FORESTBC_ID_START %d
#define FORESTBC_ID_END %d

#define MIST_ID_START %d
#define MIST_ID_END %d

#define INTENTION_ID_START %d
#define INTENTION_ID_END %d

#define MAX_ID %d

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

#define NUMBER_OF_EMOTIONS%%s%%d

%%s

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

#define NUMBER_OF_REASONS%%s%%d

%%s	// Master locations
%%s	// end of master locations
%%s	// MOBs
%%s	// end of MOBs
%%s	// Placeholders
%%s	// end of placeholders
%%s	// City locations
%%s	// end of city locations
%%s	// Forest A lights
%%s	// end of forest A lights
%%s	// Emotion fixes
%%s	// end of emotion fixes
%%s	// Lodge locations
%%s	// end of lodge locations
%%s	// Characters
%%s	// end of characters
%%s	// Forest BC lights
%%s	// end of forest BC lights
%%s	// Mist sources
%%s	// end of mist sources
%%s	// Intentions
%%s	// end of intentions

#endif // ifdef EMOTIONS_H

// End of emotions.h
