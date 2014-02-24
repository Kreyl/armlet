#include "atlantis_music_tree.h"
#include "emotions.h"
#include <stddef.h>
#include "ff.h"
#include "cmd_uart.h"

#include "kl_lib_f2xx.h" //random

#define MUSIC_FILE_EMO_INFO_SEPARATOR -
#define SEPARATOR_NOT_FOUND -3
#define EMO_INFO_NOT_FOUND -2

//buffer lengths defines
#define MAX_EMO_NAME_CHAR 20
//#define MAX_MUSIC_FILE_NAME_CHAR 50

emotionMusicNodeFiles emotionTreeMusicNodeFiles[MUSIC_ARRAY_SIZE];
//#define INI_BUF_SIZE    512
//static char IBuf2[512];
static char PrintFileToUARTbuffer[512];
static char EmoNamebuffer[MAX_EMO_NAME_CHAR];
static char MusicFileNamebuffer[MAX_MUSIC_FILENAME_CHAR_SIZE];

//������ ������ ������
Emotion * EmotionTreeGetParent(Emotion tree_node)
{
	if(tree_node.parent_indx==ROOT_ELEMENT_INDX)
		return NULL;
	Emotion * return_pointer=&(emotionTree[tree_node.parent_indx]);
	return return_pointer;
}
Emotion * EmotionTreeGetParent2(int tree_node_indx)
{
	if(emotionTree[tree_node_indx].parent_indx==ROOT_ELEMENT_INDX)
		return NULL;
	Emotion * return_pointer=&(emotionTree[emotionTree[tree_node_indx].parent_indx]);
	return return_pointer;
}



//�� ����� - �������� �����, ����� - ������ ������ �� ������� ������, ��������������� ������� �����
int GetEmoIndxFromFileString(char * string)
{
	//TODO weight auto rebalancer function to call here

	//char stremobuff[100];
	//int return_value=EMO_INFO_NOT_FOUND;
	//getting emo from filename
	int sep_id=-1;
	sep_id=strcspn(string,"_");
	if(sep_id<=0)
		return SEPARATOR_NOT_FOUND;
	strncpy(EmoNamebuffer,string,sep_id);
	//stremobuff=
	//str

	for(int i=0;i<EMOTIONTREE_ARRAY_SIZE;i++)
	{
		if(strncmp(EmoNamebuffer,emotionTree[i].name,sep_id)==0)
		//if(strcmp(strpbrk(string,"_"),emotionTree[i].name))
		{
			//Uart.Printf("GetEmoIndx string %s , EmoNamebuffer  %s, name %s  \r",string,EmoNamebuffer,emotionTree[i].name);
			return i;
		}
	}
	return EMO_INFO_NOT_FOUND;

}

//return random emo indx
int GetRandomEmoToPlay()
{
	int max_sum_weight=0;
	int min_sum_weight=0;
	for(int i=0;i<EMOTIONTREE_ARRAY_SIZE;i++)
		max_sum_weight+=emotionTree[i].weight;
	int rand_val=Random(max_sum_weight);
	for(int i=0;i<EMOTIONTREE_ARRAY_SIZE;i++)
	{
		min_sum_weight+=emotionTree[i].weight;
		if(min_sum_weight>=rand_val)
			return i;
	}
	return 0;
}
char * GetFileNameToPlayFromEmoId(int emo_id)
{
	//TODO error to log here!
	if(emo_id<0 || emo_id>EMOTIONTREE_ARRAY_SIZE)
		return NULL;

	//if no file available for this emo, play from top emo
	if(emotionTreeMusicNodeFiles[emo_id].num_files_at_node==0)
		return GetFileNameToPlayFromEmoId(emotionTreeMusicNodeFiles[emo_id].node_indx);

	//Random item from weights
	int max_sum_weight=0;
	int min_sum_weight=0;
	for(int i=0;i<emotionTreeMusicNodeFiles[emo_id].num_files_at_node;i++)
		max_sum_weight+=emotionTreeMusicNodeFiles[emo_id].float1000_file_weight[i];
	int rand_val=Random(max_sum_weight);
	for(int i=0;i<emotionTreeMusicNodeFiles[emo_id].num_files_at_node;i++)
	{
		min_sum_weight+=emotionTreeMusicNodeFiles[emo_id].float1000_file_weight[i];
		if(min_sum_weight>=rand_val)
			return emotionTreeMusicNodeFiles[emo_id].music_files[i].full_filename;
	}
	//else if smth wrong return first!
	//TODO log error here
	return emotionTreeMusicNodeFiles[emo_id].music_files[0].full_filename;
}

int Init_emotionTreeMusicNodeFiles_FromFile(char * filename)
{
	//init zero state
	for(int i=0;i<MUSIC_ARRAY_SIZE;i++)
	{
		emotionTreeMusicNodeFiles[i].num_files_at_node=0;
		emotionTreeMusicNodeFiles[i].node_weight=1000;
		//node indx ���� ������ � ������ � ����� ������, ���� ���� ���, ������ -1
		if(i<EMOTIONTREE_ARRAY_SIZE)
			emotionTreeMusicNodeFiles[i].node_indx=i;
		else
			emotionTreeMusicNodeFiles[i].node_indx=-1;
	}
	//fopen routine
	 FIL file;
	// char Init_emotionTreeMusicNodeFiles_FromFile_buffer[512];
	 int open_code= f_open (
	   	&file,
	   	filename,
	    FA_READ//+FA_OPEN_EXISTING
	 );
	 if(open_code!=0)
	 {
		  Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile error on filename %s , code from f_open  %d  \r",filename,open_code);
		 return open_code;
	 }
	 if(file.fsize == 0) {
	        f_close(&file);
	        Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile Empty file\r");
	        return -1;
	    }
	 while(f_eof(&file)==0)
	 {
		 f_gets(MusicFileNamebuffer, MAX_MUSIC_FILENAME_CHAR_SIZE, &file);
		 int emo_id=GetEmoIndxFromFileString(MusicFileNamebuffer);
		 if(emo_id>=0)
		 {	//���������� �������������
			// Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile write file  %s ,on emo_id  %d  \r",PrintFileToUARTbuffer,emo_id);
			 	//���������� ��� �����
			 strcpy(emotionTreeMusicNodeFiles[emo_id].music_files[ emotionTreeMusicNodeFiles[emo_id].num_files_at_node].full_filename,
					 MusicFileNamebuffer);

			 emotionTreeMusicNodeFiles[emo_id].float1000_file_weight[emotionTreeMusicNodeFiles[emo_id].num_files_at_node]=1000;
			 //����������� ������� ������
			 emotionTreeMusicNodeFiles[emo_id].num_files_at_node++;
		 }
		 else
			 Uart.Printf("cannot find emotion for file %s",MusicFileNamebuffer);
	 }
	 f_close(&file);
	 return 0;
}
void Print_emotionTreeMusicNodeFiles_ToUART()
{
	 for (int i = 0; i < MUSIC_ARRAY_SIZE; i++)
		 for(int j=0;j<emotionTreeMusicNodeFiles[i].num_files_at_node;j++)
		 {
			 Uart.Printf("emotion tree, emo_indx = %d,fileindx= %d,filename= %s   \r",
					 i,j,emotionTreeMusicNodeFiles[i].music_files[j].full_filename);
		 }

}
//TODO move this to better place
void CreateFileWithString(char* filename, char * filestring,int string_size, BYTE open_mode)
{
	 FIL file;
	 int open_code= f_open (
			 &file,
	   	filename,
	   	open_mode
	 );
	 if(open_code!=0)
		 return;
	 for(int i=0;i<string_size;i++)
	 {
		 UINT bw;
		 f_write(&file, filestring, string_size, &bw);
	 }
	 f_close(&file);
}
char filedata[4][512]={"fon_1_music1.mp3","fon_2_music2.mp3","ravnodushie_1_music_3.mp3","zlost_1_music_4.mp3'"};
void CreateFileWithStringArray(char* filename,int string_size, BYTE open_mode )
{
	 FIL file;
	 int open_code= f_open (
		&file,
	   	filename,
	   	FA_CREATE_ALWAYS | FA_WRITE// 	open_mode
	 );
	 if(open_code!=0)
	 {
		 Uart.Printf("CreateFileWithStringArray error on filename %s , code from f_open  %d  \r",filename,open_code);
		 return;
	 }
	 for(int i=0;i<string_size;i++)
	 {
		 int string_size=strlen(filedata[i]);
		 UINT bw;
		 f_write(&file, filedata[i], string_size, &bw);
	 }
	 f_close(&file);
}


int PrintFileToUART(char* filename)
{

	 FIL file;
	 int open_code= f_open (
	   	&file,
	   	filename,
	    FA_READ//+FA_OPEN_EXISTING
	 );
	 if(open_code!=0)
	 {
		  Uart.Printf("PrintFileToUART error on filename %s , code from f_open  %d  \r",filename,open_code);
		 return open_code;
	 }
	 if(file.fsize == 0) {
	        f_close(&file);
	        Uart.Printf("PrintFileToUART Empty file\r");
	        return -1;
	    }
	 Uart.Printf("print content of %s \r",filename);
	 int string_id=0;
	 while(f_eof(&file)==0)
	 {
		 f_gets(PrintFileToUARTbuffer, 512, &file);
		 Uart.Printf("string %d: %s \r",string_id,PrintFileToUARTbuffer);
		 string_id++;
	 }
	 Uart.Printf("last string %d \r",string_id);
	 f_close(&file);
	 return 1;
}