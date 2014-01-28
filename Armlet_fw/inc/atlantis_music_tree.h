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
//#define EMOTIONTREE_ARRAY_SIZE 5
#define ROOT_ELEMENT_INDX -1
#define MAX_MUSIC_FILENAME_CHAR_SIZE 100
#define MAX_NUMBER_MUSIC_FILES_ON_NODE 30
//структура определ€юща€ всю музыку под один нод, если таковой имеетс€
//float1000 - используетс€ инк как флоат, с точностью 1/1000

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
	char full_filename[MAX_MUSIC_FILENAME_CHAR_SIZE];
	}emotionMusicFile;

typedef struct emotionMusicNodeFiles {
	int node_indx;
	int node_weight;
	int num_files_at_node;
	emotionMusicFile music_files[MAX_NUMBER_MUSIC_FILES_ON_NODE];
	int float1000_file_weight[MAX_NUMBER_MUSIC_FILES_ON_NODE];
} emotionMusicNodeFiles;

//в +1 будут хранитьс€ файлы, дл€ которых не нашлось соответстви€.
//в последствии надо будет завести место под служебные файлы музыки

extern struct emotionMusicNodeFiles emotionTreeMusicNodeFiles[MUSIC_ARRAY_SIZE];

void Print_emotionTreeMusicNodeFiles_ToUART();
int Init_emotionTreeMusicNodeFiles_FromFile(char * filename);
void CreateFileWithString(char* filename, char * filestring,int string_size, BYTE open_mode);
void CreateFileWithStringArray(char* filename, int string_size, BYTE open_mode );
int PrintFileToUART(char* filename);
#endif /* ATLANTIS_MUSIC_TREE_H_ */
