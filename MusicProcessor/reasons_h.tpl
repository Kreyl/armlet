/*
 * reasons.h
 *
 * Part of Red Arrow Mesh Demonstration.
 *
 * Generated automatically by ReasonProcessor.py
 * from %s
 *
 * !!! DO NOT EDIT !!!
 *
 * Generated at %%s
 */
#ifndef REASONS_H
#define REASONS_H

//
// ID limits
//

#define LOCATION_ID_START %d
#define LOCATIONS_ID_END %d

#define PLACEHOLDER_ID_START %d
#define PLACEHOLDER_ID_END %d

#define FOREST_ID_START %d
#define FOREST_ID_END %d

#define PERSON_ID_START %d
#define PERSON_ID_END %d

#define MAX_MESH_ID %d

#define MAX_ID %d

//
// Reasons
//

typedef struct Reason {
    const char* name;
} Reason_t;

extern Reason_t reasons[];

#define NUMBER_OF_REASONS%%s%%d

%%s	// Locations
%%s	// end of locations
%%s	// Placeholders
%%s	// end of placeholders
%%s	// Forest lights
%%s	// end of forest lights
%%s	// People
%%s	// end of people
%%s
#endif // ifdef REASONS_H

// End of reasons.h
