/*
 * sd.h
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#ifndef SD_H_
#define SD_H_

#include "ff.h"
#include "ch.h"
#include "hal.h"
#include "kl_lib_f2xx.h"

#define MAX_NAME_LEN    128

// See SDIO clock divider in halconf.h

// =========================== ini file operations =============================
/*
 * ini file has the following structure:
 *
 * # This is Comment: comment uses either '#' or ';' symbol
 * ; This is Comment too
 *
 * [Section]    ; This is name of section
 * Count=6      ; This is key with value of int32
 * Volume=-1    ; int32
 * SoundFileName=phrase01.wav   ; string
 *
 * [Section2]
 * Key1=1
 * ...
 */

#define SD_STRING_SZ    256 // for operations with strings

// ==== Music dir list ====
struct MusList_t {
    uint32_t Cnt, N;
    const char** Dir;
    uint8_t GetCurrentDir(char** PPDir) {
        if(N >= Cnt) return FAILURE;
        else {
            *PPDir = (char*)Dir[N];
            return OK;
        }
    }
    uint8_t GetNextDir(char** PPDir) {
        if(N+1 >= Cnt) return FAILURE;
        else {
            N++;
            *PPDir = (char*)Dir[N];
            return OK;
        }
    }
};

extern MusList_t MusList;

class sd_t {
private:
    FATFS SDC_FS;
    char LongFileName[MAX_NAME_LEN];
    FILINFO FileInfo;
    FIL IFile;  // Open and close inside one function, do not leave it opened
    char IStr[SD_STRING_SZ];
    MusList_t *IPList;
public:
    DIR Directory;
    FRESULT PrepareToReadDirs(MusList_t *PList);
    FRESULT GetNext(char** PPName); // Returns pointer to first filename (w/o path)
    // Returns pointer to Nth filename (WITH path)
    uint8_t GetNthFileByPrefix(MusList_t* PList, const char* Prefix, uint32_t N, char** PPName);
    bool IsReady;
    void Init();
    // ini file operations
    uint8_t iniReadString(const char *ASection, const char *AKey, const char *AFileName, char **PPOutput);
    uint8_t iniReadInt32 (const char *ASection, const char *AKey, const char *AFileName, int32_t *POutput);
};

extern sd_t SD;

#endif /* SD_H_ */
