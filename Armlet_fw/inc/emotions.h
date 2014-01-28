#ifndef EMOTIONS_H
#define EMOTIONS_H

#define EMOTIONTREE_ARRAY_SIZE 5
#define NONTREE_ARRAY_SIZE 0
#define MUSIC_ARRAY_SIZE (EMOTIONTREE_ARRAY_SIZE + NONTREE_ARRAY_SIZE)
#define ROOT_ELEMENT_INDX -1
typedef struct Emotion {
	int weight;
    char* name;
    int parent_indx;
} Emotion;

extern struct Emotion emotionTree[EMOTIONTREE_ARRAY_SIZE];
#endif
