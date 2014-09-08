#include "atlantis_music_tree.h"
#include "emotions.h"
#include "intention.h"
#include <stddef.h>
#include "ff.h"
#include "cmd_uart.h"
#include "kl_sd.h"
#include "Sound.h"
#include "kl_lib_f2xx.h" //random
#include"energy.h"
#include "lcd2630.h"

#define MUSIC_FILE_EMO_INFO_SEPARATOR -
#define MUSIC_FILE_EMO_INFO_SEPARATOR_STRING "-"
#define SEPARATOR_NOT_FOUND -3
#define EMO_INFO_NOT_FOUND -2
#define NOEMOFILEREASONWEIGHT (-1)
//buffer lengths defines
#define MAX_EMO_NAME_CHAR 20
#define MAX_MUSIC_FILE_NAME_CHAR 128

#define MID_SEEK_SUPPORT

//#define UART_EMOTREE_DEBUG

static const char* FDirList[] = {
        "music",
        "music/common",
};

MusList_t MusList = {
        sizeof(FDirList)/sizeof(FDirList[0]),
        0,
        FDirList
};


//emotionMusicNodeFiles emotionTreeMusicNodeFiles[music_array_size];
//#define INI_BUF_SIZE    512
static char PrintFileToUARTbuffer[512];
//static char GMFNFLbuffer[512];
static char emonamebuffer[129];
static char EmoNamebuffer[MAX_EMO_NAME_CHAR];
static char MusicFileNamebuffer[MAX_NAME_LEN];
static char PlayEmoBuffTmp[MAX_NAME_LEN];
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

char * GetMusicFileNameFromList(int emo_id, int file_num) {
    //strcpy(emonamebuffer,strcat((char*)emotions[emo_id].name,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING));
    Uart.Printf("\r%S: %d %d", __FUNCTION__, emo_id, file_num);
    strcpy(emonamebuffer, (char*)emotions[emo_id].name);
    strcat(emonamebuffer, MUSIC_FILE_EMO_INFO_SEPARATOR_STRING);
    Uart.Printf("\rEmonamebuffer: %s; emotions[emo_id].name: %s",emonamebuffer,emotions[emo_id].name);
    char *S = nullptr;
    SD.GetNthFileByPrefix(&MusList,  emonamebuffer, file_num, &S);
    Uart.Printf("\r%S; Filename = %S", __FUNCTION__, S);
    return S;
}

//на входе - название файла, выход - индекс эмоции из массива эмоций, соответствующей данному файлу
int GetEmoIndxFromFileString(char * string)
{
	//TODO weight auto rebalancer function to call here

	//getting emo from filename
	int sep_id=-1;
	sep_id=strcspn(string,MUSIC_FILE_EMO_INFO_SEPARATOR_STRING);
//	Uart.Printf("sepid=%d\r", sep_id);
	if(sep_id > 20) return -3;
	if(sep_id<=0)
		return SEPARATOR_NOT_FOUND;
	strncpy(EmoNamebuffer,string,sep_id);
	//stremobuff=
	//str

	for(int i=0;i<NUMBER_OF_EMOTIONS;i++)
	{
        if((uint32_t)sep_id== strlen(emotions[i].name))
        {
            if(strncmp(EmoNamebuffer,emotions[i].name,sep_id)==0)
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
    for(int i=0;i<NUMBER_OF_REASONS;i++)
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
	for(int i=0;i<NUMBER_OF_EMOTIONS;i++)
		max_sum_weight+=emotions[i].weight;
	int rand_val=Random(max_sum_weight);
	for(int i=0;i<NUMBER_OF_EMOTIONS;i++)
	{
		min_sum_weight+=emotions[i].weight;
		if(min_sum_weight>=rand_val)
			return i;
	}
	return 0;
}
int GetFileNumerForEmoToPlay(int emo_id)
{
    if(emo_id<0 || emo_id>NUMBER_OF_EMOTIONS)
    {
        Uart.Printf("\rGetFileNumerForEmoToPlay emo_id out of range\r");
        return -1;
    }
    //if no file available for this emo, play from top emo
    if(emotions[emo_id].numTracks==0)
    {
        Uart.Printf("\rno files for emo %s \r", emotions[emo_id]);
        return GetFileNumerForEmoToPlay(emotions[emo_id].parent);
    }
    else
        return emotions[emo_id].numTracks;
}
int GetRealEmoForEmoToPlay(int emo_id)
{
    if(emo_id<0 || emo_id>NUMBER_OF_EMOTIONS)
    {
        Uart.Printf("\r GetRealEmoForEmoToPlay emo_id out of range\r");
        return -1;
    }
    //if no file available for this emo, play from top emo
    if(emotions[emo_id].numTracks==0)
    {
        Uart.Printf("\r GetRealEmoForEmoToPlayno files for emo %s \r", emotions[emo_id]);
        return GetRealEmoForEmoToPlay(emotions[emo_id].parent);
    }
    else
        return emo_id;
}
char * GetFileNameToPlayFromEmoId(int emo_id) {
	//TODO error to log here!
	if(emo_id < 0 || emo_id > NUMBER_OF_EMOTIONS) {
		Uart.Printf("\rGetFileNameToPlayFromEmoId emo_id out of range");
		return NULL;
	}


	//if no file available for this emo, play from top emo
	if(emotions[emo_id].numTracks==0) {
		Uart.Printf("\rNo files for emo %s", emotions[emo_id]);
		return GetFileNameToPlayFromEmoId(emotions[emo_id].parent);
	}
	//Random item from weights
	//if 1 file, always use it.
	if(emotions[emo_id].numTracks == 1) {
#ifdef UART_EMOTREE_DEBUG
		Uart.Printf("GetFileNameToPlayFromEmoId only one file for emo %s \r", emotions[emo_id]);
#endif
#ifdef MID_SEEK_SUPPORT
       SRPFESingleton.last_played_emo_imdx=emo_id;
       SRPFESingleton.last_played_file_indx=0;
#endif
		return GetMusicFileNameFromList(emo_id, 0);
	}
#ifdef UART_EMOTREE_DEBUG
	else
		Uart.Printf("GetFileNameToPlayFromEmoId found %d files for emo %s \r",emotions[emo_id].numTracks, emotions[emo_id]);
#endif
	// если треков 2 и более
	int rand_val=Random(emotions[emo_id].numTracks-2)+1; //рандомное число от максимум треков +1
	if(emotions[emo_id].numTracks==0) Uart.Printf("GetFileNameToPlayFromEmoId smth wrong\r");

	//Uart.Printf("emotions[emo_id].lastPlayedTrack %d",emotions[emo_id].lastPlayedTrack);
	int track_num_calculated;
	if(emotions[emo_id].lastPlayedTrack == -1) track_num_calculated=Random(emotions[emo_id].numTracks-1);
	// к текущему номе ру ртека прибавляем рандомное число по модулю числа треков - получаем номер другого трека
	else track_num_calculated=(rand_val+emotions[emo_id].lastPlayedTrack) % emotions[emo_id].numTracks;

	//Uart.Printf("GetFileNameToPlayFromEmoId result: rand_val %d, track_num %d, last played %d \r", rand_val,track_num_calculated,emotions[emo_id].lastPlayedTrack);
	emotions[emo_id].lastPlayedTrack = track_num_calculated;
//	Uart.Printf("GetFileNameToPlayFromEmoId result: new last played %d\r",emotions[emo_id].lastPlayedTrack);


#ifdef MID_SEEK_SUPPORT
       SRPFESingleton.last_played_emo_imdx=emo_id;
       SRPFESingleton.last_played_file_indx=track_num_calculated;
#endif
	return GetMusicFileNameFromList(emo_id, track_num_calculated);
}

void PlayNewEmo(int emo_id, int err_id, bool is_gs, bool ignore_play_pos)
{
    if(SICD.is_global_stop_active && !is_gs)
    {
        Uart.Printf("PlayNewEmo called on globalstop,playing same emo");
        emo_id=SICD.last_played_emo;
    }
    //обработка снижений важности резонов:
    if(!is_gs)
    if(SRD.reduced_reason_id!=-1 && SRD.reduced_reason_id!=SICD.last_intention_index_winner)
    {
        if(SRD.is_reason_changed==false)
        {
            SRD.is_reason_changed=true;//опознали смену музыки по новому резону. старый снижен, но еще не выключен
            SRD.overthrower_reason_id=SICD.last_intention_index_winner;
        }
        else if(SICD.last_intention_index_winner != SRD.overthrower_reason_id)
        {
            SRD.is_reason_changed=false;// музыка поменялась второй раз - ок! теперь сбрасываем счетчик прошедших мелодий по несниженному резону - снова все резоны нормальные
            SRD.reduced_reason_id=-1;
            SRD.overthrower_reason_id=-1;
            SRD.weight_reduced=0;
        }
    }
    //если гавно, то фон
    if(emo_id < 0) {
        Uart.Printf("\rEmo_id <0 %d", emo_id);
        emo_id =0;
    }
    //Uart.Printf("")
#ifdef MID_SEEK_SUPPORT
    //get old position
    int old_pos=Sound.GetPosition();
    if(emo_id!=SRPFESingleton.last_played_emo_imdx)
        Uart.Printf("\rMID_SEEK_SUPPORT warning! possible differ: emo play %d, rmo stored: %d",emo_id,SRPFESingleton.last_played_emo_imdx);
    //записали старые emo id file id
    SRPFESingleton.OnCallStopPlay(SRPFESingleton.last_played_emo_imdx,SRPFESingleton.last_played_file_indx,old_pos);

#endif
    SICD.last_played_emo=emo_id;
#ifdef BRACELET_TEST_MODE_VALS
    DrawEmoToPlay(SICD.last_played_emo);
#endif
    char * fname = GetFileNameToPlayFromEmoId(SICD.last_played_emo);
    if(fname != nullptr) {
       strcpy(PlayEmoBuffTmp,fname);
#ifdef MID_SEEK_SUPPORT
       //проверили новые emo id file id
       Uart.Printf("\rMID_SEEK_SUPPORT CheckIfRecent: emo indx %d, file_indx: %d",SRPFESingleton.last_played_emo_imdx,SRPFESingleton.last_played_file_indx);
       int seek_pos_old=SRPFESingleton.CheckIfRecent(SRPFESingleton.last_played_emo_imdx,SRPFESingleton.last_played_file_indx);
       if(ignore_play_pos)
       {
           Uart.Printf("\r PlayNewEmo SEEKPOS IGNORE");
           seek_pos_old=0;
       }
       Sound.Play(PlayEmoBuffTmp,seek_pos_old);
       Uart.Printf(PlayEmoBuffTmp);
       Uart.Printf("\r");
       Uart.Printf("\r PlayNewEmo SEEKPOS %d errid%d",seek_pos_old,err_id);

#else
       Sound.Play(PlayEmoBuffTmp);
       Uart.Printf(PlayEmoBuffTmp);
       Uart.Printf("\r");

#endif

    }
    else {
        Uart.Printf("\rPlayNewEmo GetFileNameToPlayFromEmoId nullptr!!! smth wrong. emo_id %d, err_id%d",emo_id,err_id);
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
//		  Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile error on filename %s , code from f_open  %d  \r",filename,open_code);
		 return open_code;
	 }
	 if(file.fsize == 0) {
	        f_close(&file);
//	        Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile Empty file\r");
	        return -1;
	    }
	 while(f_eof(&file)==0)
	 {
		 f_gets(MusicFileNamebuffer, MAX_NAME_LEN, &file);
		 int emo_id=GetEmoIndxFromFileString(MusicFileNamebuffer);
//		 Uart.Printf("emo id found1: %d \r",emo_id);
		 if(emo_id>=0)
		 {	//собственно инициализация
			 //увеличиваем счетчик файлов
			 emotions[emo_id].numTracks++;
		 }
//		 else Uart.Printf("cannot find emotion for file %s",MusicFileNamebuffer);
	 }
	// Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFile end \r");
	 f_close(&file);
	 return 0;
}

void Init_emotionTreeMusicNode() {
    if(SD.PrepareToReadDirs(&MusList) == FR_OK) {
        // Count files available
        char *S = nullptr;
        while(SD.GetNext(&S) == FR_OK) {
            Uart.Printf("\rFilename: %S,",S);
            int emo_id = GetEmoIndxFromFileString(S);
            if(emo_id >= 0)
                {
                emotions[emo_id].numTracks++;
                Uart.Printf("\rFilename: %S, emo id %d, NumTracks %d", S,emo_id, emotions[emo_id].numTracks);
                }
           // else
           // Uart.Printf("\rFilename: %S, emo id %d", S,emo_id);
        } // if GetNext
    } // if PrepareToReadDirs
}

void Init_emotionTreeMusicNodeFiles_FromFileIterrator() {
    // init zero state
    for(int i=0;i<music_array_size;i++)
        {
            emotions[i].numTracks=0;
            //Uart.Printf("Init_emotionTreeMusicNodeFiles_FromFileIterrator emo_id %d, trnum %d",i,emotions[i].numTracks )
        }
    Init_emotionTreeMusicNode();
}

void Print_emotionTreeMusicNodeFiles_ToUART()
{
	 for (int i = 0; i < music_array_size; i++)
		 for(int j=0;j<emotions[i].numTracks;j++)
		 {
//		     Uart.Printf(" emo_indx = %d,fileindx= %d\r", i,j);
		    // GetMusicFileNameFromList(i,j);
//			 Uart.Printf(" emo_indx = %d,fileindx= %d,filename= %s   \r", i,j, GetMusicFileNameFromList(i,j)); //emotionTreeMusicNodeFiles[i].music_files[j].full_filename);
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
//		  Uart.Printf("PrintFileToUART error on filename %s , code from f_open  %d  \r",filename,open_code);
		 return open_code;
	 }
	 if(file.fsize == 0) {
	        f_close(&file);
//	        Uart.Printf("PrintFileToUART Empty file\r");
	        return -1;
	    }
//	 Uart.Printf("print content of %s \r",filename);
	 int string_id=0;
	 while(f_eof(&file)==0)
	 {
		 f_gets(PrintFileToUARTbuffer, 512, &file);
//		 Uart.Printf("string %d: %s \r",string_id,PrintFileToUARTbuffer);
		 string_id++;
	 }
//	 Uart.Printf("last string %d \r",string_id);
	 f_close(&file);
	 return 1;
}

void DrawReasonToPlay(int reason_id){

    if(reason_id>0 && reason_id<NUMBER_OF_REASONS)
    Lcd.Printf(B52,15, 20, clAtltopstr, clAtlBack, "%s",reasons[reason_id].name);
};
void DrawEmoToPlay(int Emo_id){
    if(Emo_id>0 && Emo_id<NUMBER_OF_EMOTIONS)
    Lcd.Printf(B52,15, 30, clAtltopstr, clAtlBack, "%s",emotions[Emo_id].name);
};
void DrawEmoPlaying(int Emo_id){
    if(Emo_id>0 && Emo_id<NUMBER_OF_EMOTIONS )
    Lcd.Printf(B52,15, 40, clAtltopstr, clAtlBack, "%s",emotions[Emo_id].name);
};
