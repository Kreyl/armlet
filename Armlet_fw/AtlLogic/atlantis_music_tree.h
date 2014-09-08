/*
 * atlantis_music_tree.h
 *
 *  Created on: 21 €нв. 2014 г.
 *      Author: Fil
 */

#ifndef ATLANTIS_MUSIC_TREE_H_
#define ATLANTIS_MUSIC_TREE_H_

#include "emotions.h"
#include "integer.h"
#include "kl_sd.h"


#define ROOT_ELEMENT_INDX -1

//extern const MusList_t MusicList;

//структура определ€юща€ всю музыку под один нод, если таковой имеетс€
//float1000 - используетс€ инк как флоат, с точностью 1/1000
//music_array_size = music_array_size
//emotion_number_fix = emotion_number_fix
//intentions = reason
const int non_tree_array_size = 0;
const int music_array_size=NUMBER_OF_EMOTIONS+non_tree_array_size;

typedef struct string_limited512
{
	char s[512];
}string_limited512;
typedef struct filedatalimited
{
	string_limited512 data[4];
	}filedatalimited;
typedef struct emotionMusicFile
{
	char full_filename[MAX_NAME_LEN];
	}emotionMusicFile;

typedef struct emotionMusicNodeFiles {
	int node_indx;//+
	int node_weight;//+
	int num_files_at_node;//+
} emotionMusicNodeFiles;

//в +1 будут хранитьс€ файлы, дл€ которых не нашлось соответстви€.
//в последствии надо будет завести место под служебные файлы музыки
extern struct emotionMusicNodeFiles emotionTreeMusicNodeFiles[music_array_size];

void Print_emotionTreeMusicNodeFiles_ToUART();
int Init_emotionTreeMusicNodeFiles_FromFile(const char * filename);
void Init_emotionTreeMusicNodeFiles_FromFileIterrator();
void Init_emotionTreeMusicNode();
void CreateFileWithString(char* filename, char * filestring,int string_size, BYTE open_mode);
void CreateFileWithStringArray(char* filename, int string_size, BYTE open_mode );
int PrintFileToUART(char* filename);
int GetRandomEmoToPlay();
int GetRealEmoForEmoToPlay(int emo_id);//checker for emo
int GetFileNumerForEmoToPlay(int emo_id);
char * GetFileNameToPlayFromEmoId(int emo_id);
void RebuildReasons();
void PlayNewEmo(int emo_id,int err_id, bool is_gs=false/*is global stop on!*/, bool ignore_play_pos=false);

void DrawReasonToPlay(int reason_id);
void DrawEmoToPlay(int Emo_id);
void DrawEmoPlaying(int Emo_id);

#endif /* ATLANTIS_MUSIC_TREE_H_ */
