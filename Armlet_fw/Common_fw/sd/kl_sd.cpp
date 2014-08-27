/*
 * kl_sd.cpp
 *
 *  Created on: 13.02.2013
 *      Author: kreyl
 */

#include "kl_sd.h"
#include "sdc.h"
#include "sdc_lld.h"
#include <string.h>
#include <stdlib.h>
#include "kl_lib_f2xx.h"
#include "cmd_uart.h"

sd_t SD;
extern Semaphore semSDRW;

void sd_t::Init() {
    IsReady = FALSE;
    // Bus pins
    PinSetupAlterFunc(GPIOC,  8, omPushPull, pudPullUp, AF12, ps50MHz); // DAT0
    PinSetupAlterFunc(GPIOC,  9, omPushPull, pudPullUp, AF12, ps50MHz); // DAT1
    PinSetupAlterFunc(GPIOC, 10, omPushPull, pudPullUp, AF12, ps50MHz); // DAT2
    PinSetupAlterFunc(GPIOC, 11, omPushPull, pudPullUp, AF12, ps50MHz); // DAT3
    PinSetupAlterFunc(GPIOC, 12, omPushPull, pudNone,   AF12, ps50MHz); // CLK
    PinSetupAlterFunc(GPIOD,  2, omPushPull, pudPullUp, AF12, ps50MHz); // CMD
    // Power pin
    PinSetupOut(GPIOC, 4, omPushPull, pudNone);
    PinClear(GPIOC, 4); // Power on
    chThdSleepMilliseconds(270);    // Let power to stabilize

    FRESULT err;
    sdcInit();
    sdcStart(&SDCD1, NULL);
    if(sdcConnect(&SDCD1)) {
        Uart.Printf("\rSD connect error");
        return;
    }
    else {
        Uart.Printf("\rSD capacity: %u", SDCD1.capacity);
    }
    err = f_mount(0, &SDC_FS);
    if (err != FR_OK) {
        Uart.Printf("\rSD mount error");
        sdcDisconnect(&SDCD1);
        return;
    }
    // Init RW semaphore
    chSemInit(&semSDRW, 1);
    IsReady = TRUE;
}

#if 1 // ====================== Get Filename In Folder =========================
// Get first file in folder
FRESULT sd_t::PrepareToReadDirs(MusList_t *PList) {
    // Setup variables
    IPList = PList;
    IPList->N = 0;
    // Open first dir
    char* DirPath = nullptr;
    if(IPList->GetCurrentDir(&DirPath) == OK) {
//        Uart.Printf("\r %S", DirPath);
        int i = strlen(DirPath) + 1;        // } Put filename in middle of buffer,
        FileInfo.lfname = &LongFileName[i]; // } saving space to copy path from beginning
        FileInfo.lfsize = MAX_NAME_LEN - i;
        return f_opendir(&Directory, DirPath); // Try to open the folder
    }
    else return FR_NO_FILE;
}

FRESULT sd_t::GetNext(char** PPName) {
    while(true) {   // Read everything within current dir until file found
        FRESULT r = f_readdir(&Directory, &FileInfo);
        if(r != FR_OK) return r;
        if((FileInfo.fname[0] == 0) and (FileInfo.lfname[0] == 0)) {
            // No files left, get next dir
            if(IPList == nullptr) return FR_INVALID_OBJECT;
            char* DirPath = nullptr;
            if(IPList->GetNextDir(&DirPath) == OK) {
//                Uart.Printf("\r %S", DirPath);
                int i = strlen(DirPath) + 1;
                FileInfo.lfname = &LongFileName[i];
                FileInfo.lfsize = MAX_NAME_LEN - i;
                r = f_opendir(&Directory, DirPath); // Try to open the folder
                if(r != FR_OK) return r;
            }
            else return FR_NO_FILE; // no more dirs left
        }
        else { // Filename ok, check if not dir
            *PPName = (FileInfo.lfname[0] == 0)? FileInfo.fname : FileInfo.lfname;
            if(!(FileInfo.fattrib & AM_DIR)) break;
        }
    } // while
    return FR_OK;
}

uint8_t sd_t::GetNthFileByPrefix(MusList_t* PList, const char* Prefix, uint32_t N, char** PPName) {
    uint32_t Len = strlen(Prefix);
    FRESULT r = PrepareToReadDirs(PList);
    if(r != FR_OK) return r;
    char *S = nullptr;
    while(GetNext(&S) == FR_OK) {
        // Check if name begins with prefix
        if(strncmp(S, Prefix, Len) == 0) {  // Prefix found
            if(N == 0) {                    // Required number of files found
                // if filename is short (==stored in short charbuf), copy it
                if(S == FileInfo.fname) strcpy(FileInfo.lfname, S);    // Copy short filename to long buf
                // Copy dir path
                char *DirPath = nullptr;
                IPList->GetCurrentDir(&DirPath);
                strcpy(LongFileName, DirPath);
                // Add '/' between path and name
                int Len = strlen(DirPath);
                LongFileName[Len] = '/';
                *PPName = LongFileName; // return Path + Filename
                return OK;
            }
            else N--;
        } // if prefix
    } // while
    Uart.Printf("\nGetNthFileByPrefix FALURE");
    return FAILURE;
}
#endif

#if 1 // ======================= ini file operations ===========================
// ==== Inner use ====
static inline char* skipleading(char *S) {
    while (*S != '\0' && *S <= ' ') S++;
    return S;
}
static inline char* skiptrailing(char *S, const char *base) {
    while ((S > base) && (*(S-1) <= ' ')) S--;
    return S;
}
static inline char* striptrailing(char *S) {
    char *ptr = skiptrailing(strchr(S, '\0'), S);
    *ptr='\0';
    return S;
}

uint8_t sd_t::iniReadString(const char *ASection, const char *AKey, const char *AFileName, char **PPOutput) {
    FRESULT rslt;
    // Open file
    rslt = f_open(&IFile, AFileName, FA_READ+FA_OPEN_EXISTING);
    if(rslt != FR_OK) {
        if (rslt == FR_NO_FILE) Uart.Printf("\r%S: not found", AFileName);
        else Uart.Printf("\r%S: openFile error: %u", AFileName, rslt);
        return FAILURE;
    }
    // Check if zero file
    if(IFile.fsize == 0) {
        f_close(&IFile);
        Uart.Printf("\rEmpty file");
        return FAILURE;
    }
    // Move through file one line at a time until a section is matched or EOF.
    char *StartP, *EndP = nullptr;
    int32_t len = strlen(ASection);
    do {
        if(f_gets(IStr, SD_STRING_SZ, &IFile) == nullptr) {
            Uart.Printf("\riniNoSection %S", ASection);
            f_close(&IFile);
            return FAILURE;
        }
        StartP = skipleading(IStr);
        if((*StartP != '[') or (*StartP == ';') or (*StartP == '#')) continue;
        EndP = strchr(StartP, ']');
        if((EndP == NULL) or ((int32_t)(EndP-StartP-1) != len)) continue;
    } while (strnicmp(StartP+1, ASection, len) != 0);

    // Section found, find the key
    len = strlen(AKey);
    do {
        if (!f_gets(IStr, SD_STRING_SZ, &IFile) or *(StartP = skipleading(IStr)) == '[') {
            Uart.Printf("\riniNoKey");
            f_close(&IFile);
            return FAILURE;
        }
        StartP = skipleading(IStr);
        if((*StartP == ';') or (*StartP == '#')) continue;
        EndP = strchr(StartP, '=');
        if(EndP == NULL) continue;
    } while(((int32_t)(skiptrailing(EndP, StartP)-StartP) != len or strnicmp(StartP, AKey, len) != 0));
    f_close(&IFile);

    // Process Key's value
    StartP = skipleading(EndP + 1);
    // Remove a trailing comment
    uint8_t isstring = 0;
    for(EndP = StartP; (*EndP != '\0') and (((*EndP != ';') and (*EndP != '#')) or isstring) and ((uint32_t)(EndP - StartP) < SD_STRING_SZ); EndP++) {
        if (*EndP == '"') {
            if (*(EndP + 1) == '"') EndP++;     // skip "" (both quotes)
            else isstring = !isstring; // single quote, toggle isstring
        }
        else if (*EndP == '\\' && *(EndP + 1) == '"') EndP++; // skip \" (both quotes)
    } // for
    *EndP = '\0';   // Terminate at a comment
    striptrailing(StartP);
    *PPOutput = StartP;
    return OK;
}

uint8_t sd_t::iniReadInt32(const char *ASection, const char *AKey, const char *AFileName, int32_t *POutput) {
    char *S = nullptr;
    if(iniReadString(ASection, AKey, AFileName, &S) == OK) {
        *POutput = strtol(S, NULL, 10);
        return OK;
    }
    else return FAILURE;
}
#endif

// ============================== Hardware =====================================
#ifdef __cplusplus
extern "C" {
#endif

bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) { return TRUE; }

bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) { return FALSE; }

#ifdef __cplusplus
}
#endif
