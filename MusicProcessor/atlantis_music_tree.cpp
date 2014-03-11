#include "atlantis_music_tree.h"
#include "emotions.h"
#include "intention.h"
#include <stddef.h>
#include "ff.h"
#include "cmd_uart.h"
#include "kl_sd.h"
#include "Sound.h"
#include "kl_lib_f2xx.h" //random

#define MUSIC_FILE_EMO_INFO_SEPARATOR -
#define MUSIC_FILE_EMO_INFO_SEPARATOR_STRING "-"
#define SEPARATOR_NOT_FOUND -3
#define EMO_INFO_NOT_FOUND -2
#define NOEMOFILEREASONWEIGHT (-1)
//buffer lengths defines
#define MAX_EMO_NAME_CHAR 20
#define MAX_MUSIC_FILE_NAME_CHAR 128

//emotionMusicNodeFiles emotionTreeMusicNodeFiles[music_array_size];
//#define INI_BUF_SIZE    512
//static char IBuf2[512];
static char PrintFileToUARTbuffer[512];
static char GMFNFLbuffer[512];
static char emonamebuffer[129];
static char EmoNamebuffer[MAX_EMO_NAME_CHAR];
static char MusicFileNamebuffer[MAX_MUSIC_FILENAME_CHAR_SIZE];
static char PlayEmoBuffTmp[MAX_MUSIC_FILENAME_CHAR_SIZE];
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
    //strcpy(emonamebuffer,strcat((char*)emotions[emo_id].name,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING));
   // Uart.Printf("GetMusicFileNameFromList1\r");
    strcpy(emonamebuffer,(char*)emotions[emo_id].name);
   // Uart.Printf("GetMusicFileNameFromList2\r");
    strcat(emonamebuffer,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING);
  //  Uart.Printf("GetMusicFileNameFromList3\r");
   // Uart.Printf("emonamebuffer %s , emotions[emo_id].name  %s \r",emonamebuffer,emotions[emo_id].name);
    //SD.GetNthFileByPrefix(emonamebuffer,file_num,GMFNFLbuffer);
    if(SD.GetNthFileByPrefix(emonamebuffer,file_num,GMFNFLbuffer)==FR_OK)
    {
        Uart.Printf("GetNthFileByPrefix prefix: %s,fnum %d result: %s \r",emonamebuffer,file_num,GMFNFLbuffer);
        return GMFNFLbuffer;
    }
    //TODO error!
    return nullptr;
}

//на входе - название файла, выход - индекс эмоции из массива эмоций, соответствующей данному файлу
int GetEmoIndxFromFileString(char * string)
{
	//TODO weight auto rebalancer function to call here

	//char stremobuff[100];
	//int return_value=EMO_INFO_NOT_FOUND;
	//getting emo from filename
	int sep_id=-1;
	sep_id=strcspn(string,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING);
	Uart.Printf("sepid=%d\r", sep_id);
	if(sep_id > 20) return -3;
	if(sep_id<=0)
		return SEPARATOR_NOT_FOUND;
	strncpy(EmoNamebuffer,string,sep_id);
	//stremobuff=
	//str

	for(int i=0;i<emotions_number;i++)
	{
		//if(strncmp(EmoNamebuffer,emotions[i].name,sep_id)==0)
        if((uint32_t)sep_id== strlen(emotions[i].name))
        {
            if(strncmp(EmoNamebuffer,emotions[i].name,sep_id)==0)
            //if(strcmp(strpbrk(string,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING),emotions[i].name))
            {
            //	Uart.Printf("GetEmoIndx string %s , EmoNamebuffer  %s, name %s  \r",string,EmoNamebuffer,emotions[i].name);
                  //  Uart.Printf("String_size %d %d, ",sep_id,strlen(emotions[i].name));
                return i;
            }
        }
	}
	return EMO_INFO_NOT_FOUND;

}
//recalculate reasons tree to remove zero file numbers on emo tree to call
void RebuildReasons()
{
    for(int i=0;i<reasons_number;i++)
    {
        while(emotions[reasons[i].eID].numTracks==0)
        {
            //переносим эмоцию на родителя
            if(emotions[reasons[i].eID].parent!= ROOT)
                reasons[i].eID=emotions[reasons[i].eID].parent;
            reasons[i].weight+= NOEMOFILEREASONWEIGHT;
        }
    }
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
		Uart.Printf("GetFileNameToPlayFromEmoId emo_id out of range\r");
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
	if(emotions[emo_id].numTracks ==1)
	{
		Uart.Printf("GetFileNameToPlayFromEmoId only one file for emo %s \r", emotions[emo_id]);
		return GetMusicFileNameFromList(emo_id,0);//"TODO get full filename";// emotionTreeMusicNodeFiles[emo_id].music_files[0].full_filename;
	}
	else
		Uart.Printf("GetFileNameToPlayFromEmoId found %d files for for emo %s \r",emotions[emo_id].numTracks, emotions[emo_id]);
	// если треков 2 и более
	int rand_val=Random(emotions[emo_id].numTracks-2)+1; //рандомное число от максимум треков +1
	if(emotions[emo_id].numTracks==0)
	    Uart.Printf("GetFileNameToPlayFromEmoId smth wrong\r");

	//Uart.Printf("emotions[emo_id].lastPlayedTrack %d",emotions[emo_id].lastPlayedTrack);
	int track_num_calculated;
	if(emotions[emo_id].lastPlayedTrack==-1)
	    track_num_calculated=Random(emotions[emo_id].numTracks-1);
	// к текущему номе ру ртека прибавляем рандомное число по модулю числа треков - получаем номер другого трека
	else
	 track_num_calculated=(rand_val+emotions[emo_id].lastPlayedTrack) % emotions[emo_id].numTracks;

	//Uart.Printf("GetFileNameToPlayFromEmoId result: rand_val %d, track_num %d, last played %d \r", rand_val,track_num_calculated,emotions[emo_id].lastPlayedTrack);
	emotions[emo_id].lastPlayedTrack=track_num_calculated;
//	Uart.Printf("GetFileNameToPlayFromEmoId result: new last played %d\r",emotions[emo_id].lastPlayedTrack);
	    return GetMusicFileNameFromList(emo_id,track_num_calculated);
}
void PlayNewEmo(int emo_id,int err_id)
{
    //если гавно, то фон
    if(emo_id<0)
    {
        Uart.Printf("emo_id <0 %d\r", emo_id);
        emo_id =0;
    }
    //Uart.Printf("")
    SICD.last_played_emo=emo_id;
    char * fname = GetFileNameToPlayFromEmoId(SICD.last_played_emo);
    if(fname != nullptr)
    {
       strcpy(PlayEmoBuffTmp,fname);
       Sound.Play(PlayEmoBuffTmp);
       Uart.Printf(PlayEmoBuffTmp);
       Uart.Printf("\r");

    }
    else
    {
        Uart.Printf("PlayNewEmo GetFileNameToPlayFromEmoId nullptr!!! smth wrong. emo_id %d, err_id%d \r",emo_id,err_id);

    }
}
int Init_emotionTreeMusicNodeFiles_FromFile(const char * filename)
{
	//init zero state
	for(int i=0;i<music_array_size;i++)
	{
		emotions[i].numTracks=0;
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
		 Uart.Printf("emo id found1: %d \r",emo_id);
		 if(emo_id>=0)
		 {	//собственно инициализация
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

int Init_emotionTreeMusicNodeFiles_FromFileIterrator()
{
    //init zero state
    for(int i=0;i<music_array_size;i++)
    {
        emotions[i].numTracks=0;
    }
    if(SD.GetFirst("/")==FR_OK)
    {
        int emo_id=GetEmoIndxFromFileString(SD.Filename);
      //
        if(emo_id>=0)
                 {  //собственно инициализация
                     //увеличиваем счетчик файлов
                    Uart.Printf("emo id found2: %d \r",emo_id);
                     emotions[emo_id].numTracks++;
                 }
                 else
                     Uart.Printf("cannot find emotion for file1 %s \r",SD.Filename);//MusicFileNamebuffer);
    }

    while(SD.GetNext()==FR_OK)
    {
        int emo_id=GetEmoIndxFromFileString(SD.Filename);

        if(emo_id>=0)
        {  //собственно инициализация
            //увеличиваем счетчик файлов
            Uart.Printf("emo id found3: %d, filename %s \r",emo_id,SD.Filename);
            emotions[emo_id].numTracks++;
        }
        else
            Uart.Printf("cannot find emotion for file2 %s \r",SD.Filename);//MusicFileNamebuffer);
    }
    //TODO critical error, stop working

    // while(f_eof(&file)==0)
    // {
    //     f_gets(MusicFileNamebuffer, MAX_MUSIC_FILENAME_CHAR_SIZE, &file);
   //      int emo_id=GetEmoIndxFromFileString(MusicFileNamebuffer);
    //     Uart.Printf("emo id found: %d \r",emo_id);
    //     if(emo_id>=0)
   //      {  //собственно инициализация
    //         //увеличиваем счетчик файлов
    //         emotions[emo_id].numTracks++;
    //     }
    //     else
    //         Uart.Printf("cannot find emotion for file %s",MusicFileNamebuffer);
    // }
    // Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile end \r");
     return 0;
}
void Print_emotionTreeMusicNodeFiles_ToUART()
{
	 for (int i = 0; i < music_array_size; i++)
		 for(int j=0;j<emotions[i].numTracks;j++)
		 {
		     Uart.Printf(" emo_indx = %d,fileindx= %d\r", i,j);
		    // GetMusicFileNameFromList(i,j);
			 Uart.Printf(" emo_indx = %d,fileindx= %d,filename= %s   \r",
					 i,j, GetMusicFileNameFromList(i,j)); //emotionTreeMusicNodeFiles[i].music_files[j].full_filename);
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
