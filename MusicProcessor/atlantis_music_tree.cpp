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

emotionMusicNodeFiles emotionTreeMusicNodeFiles[music_array_size];
//#define INI_BUF_SIZE    512
//static char IBuf2[512];
static char PrintFileToUARTbuffer[512];
static char EmoNamebuffer[MAX_EMO_NAME_CHAR];
static char MusicFileNamebuffer[MAX_MUSIC_FILENAME_CHAR_SIZE];

//полное дерево музыки
Emotion * EmotionTreeGetParent(Emotion tree_node)
{
	if(tree_node.parent==ROOT_ELEMENT_INDX)
		return NULL;
	Emotion * return_pointer=&(emotions[tree_node.parent]);
	return return_pointer;
}
Emotion * EmotionTreeGetParent2(int tree_node_indx)
{
	if(emotions[tree_node_indx].parent==ROOT_ELEMENT_INDX)
		return NULL;
	Emotion * return_pointer=&(emotions[emotions[tree_node_indx].parent]);
	return return_pointer;
}

char * GetMusicFileNameFromList(int emo_id, int file_num)
{
    return "TODO";
    }

//на входе - название файла, выход - индекс эмоции из массива эмоций, соответствующей данному файлу
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

	for(int i=0;i<emotions_number;i++)
	{
		if(strncmp(EmoNamebuffer,emotions[i].name,sep_id)==0)
		//if(strcmp(strpbrk(string,"_"),emotions[i].name))
		{
			//Uart.Printf("GetEmoIndx string %s , EmoNamebuffer  %s, name %s  \r",string,EmoNamebuffer,emotions[i].name);
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
	for(int i=0;i<emotions_number;i++)
		max_sum_weight+=emotions[i].weight;
	int rand_val=Random(max_sum_weight);
	for(int i=0;i<emotions_number;i++)
	{
		min_sum_weight+=emotions[i].weight;
		if(min_sum_weight>=rand_val)
			return i;
	}
	return 0;
}
char * GetFileNameToPlayFromEmoId(int emo_id)
{
	//TODO error to log here!
	if(emo_id<0 || emo_id>emotions_number)
	{
		Uart.Printf("GetFileNameToPlayFromEmoId emo_id out of range");
		return NULL;
	}
	//if no file available for this emo, play from top emo
	if(emotions[emo_id].numTracks==0)
	{
		Uart.Printf("no files for emo %s \r", emotions[emo_id]);
		return GetFileNameToPlayFromEmoId(emotions[emo_id].parent);
	}
	//Random item from weights
	//if 1 file, always use it.
	if(emotionTreeMusicNodeFiles[emo_id].num_files_at_node ==1)
	{
		Uart.Printf("only one file for emo %s \r", emotions[emo_id]);
		return "TODO get full filename";// emotionTreeMusicNodeFiles[emo_id].music_files[0].full_filename;
	}
	else
		Uart.Printf("found %d files for for emo %s \r",emotions[emo_id].numTracks, emotions[emo_id]);

	int max_sum_weight=0;
	int min_sum_weight=0;
	for(int i=0;i<emotions[emo_id].numTracks;i++)
		max_sum_weight+=emotions[emo_id].weight[i];
	int rand_val=Random(emotions[emo_id].numTracks);
	//веса у файлов убрали
	/*for(int i=0;i<emotionTreeMusicNodeFiles[emo_id].num_files_at_node;i++)
	{
		min_sum_weight+=emotionTreeMusicNodeFiles[emo_id].float1000_file_weight[i];
		if(min_sum_weight>=rand_val)
		{
			Uart.Printf("play %d file for for emo %s \r",i, emotions[emo_id]);
			return emotionTreeMusicNodeFiles[emo_id].music_files[i].full_filename;
		}
	}*/
	Uart.Printf("GetFileNameToPlayFromEmoId smth wrong");
	//else if smth wrong return first!
	//TODO log error here
	return emotionTreeMusicNodeFiles[emo_id].music_files[0].full_filename;
}

int Init_emotionTreeMusicNodeFiles_FromFile(char * filename)
{
	//init zero state
	for(int i=0;i<music_array_size;i++)
	{
		emotions[i].numTracks=0;
		emotions[i].weight=1000;
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
		 Uart.Printf("emo id found: %d \r",emo_id);
		 if(emo_id>=0)
		 {	//собственно инициализация
			// Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile write file  %s ,on emo_id  %d  \r",PrintFileToUARTbuffer,emo_id);

			//записываем имя файла
		    //не записываем - будет геттер по номеру эмоции
			// strcpy(emotionTreeMusicNodeFiles[emo_id].music_files[ emotionTreeMusicNodeFiles[emo_id].num_files_at_node].full_filename,
			//		 MusicFileNamebuffer);

			//REMOVED emotionTreeMusicNodeFiles[emo_id].float1000_file_weight[emotionTreeMusicNodeFiles[emo_id].num_files_at_node]=1000;
			 //увеличиваем счетчик файлов
			 emotions[emo_id].numTracks++;
		 }
		 else
			 Uart.Printf("cannot find emotion for file %s",MusicFileNamebuffer);
	 }
	// Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile end \r");
	 f_close(&file);
	 return 0;
}
void Print_emotionTreeMusicNodeFiles_ToUART()
{
	 for (int i = 0; i < music_array_size; i++)
		 for(int j=0;j<emotions[i].numTracks;j++)
		 {
		     //TODO - тут нужно вызывать геттер
			// Uart.Printf("emotion tree, emo_indx = %d,fileindx= %d,filename= %s   \r",
			//		 i,j,emotionTreeMusicNodeFiles[i].music_files[j].full_filename);
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
