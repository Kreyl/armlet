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
    chThdSleepMicroseconds(450);

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
    IsReady = TRUE;
}

#if 1 // ====================== Get Filename In Folder =========================
// Get first file in folder
FRESULT sd_t::GetFirst(const char* DirPath) {
    FRESULT r = f_opendir(&Directory, DirPath); // Try to open the folder
    if(r != FR_OK) return r;
    FileInfo.lfname = LongFileName;
    FileInfo.lfsize = MAX_NAME_LEN;
    while(true) {   // Read everything until file found
        r = f_readdir(&Directory, &FileInfo);
        if(r != FR_OK) return r;
        Filename = (FileInfo.lfname[0] == 0)? FileInfo.fname : FileInfo.lfname;
        if(Filename[0] == 0) return FR_NO_FILE;
        if(!(FileInfo.fattrib & AM_DIR)) return FR_OK;
    }
    return FR_INT_ERR;
}

FRESULT sd_t::GetNext() {
    while(true) {
        FRESULT r = f_readdir(&Directory, &FileInfo);
        if(r != FR_OK) return r;
        Filename = (FileInfo.lfname[0] == 0)? FileInfo.fname : FileInfo.lfname;
        if(Filename[0] == 0) return FR_NO_FILE;
        if(!(FileInfo.fattrib & AM_DIR)) return FR_OK;
    }
    return FR_INT_ERR;
}

uint8_t sd_t::GetNthFileByPrefix(const char* DirPath, const char* Prefix, uint32_t N, char* PName) {
    uint32_t Len = strlen(Prefix);
    FRESULT r = GetFirst(DirPath);
    while(r == FR_OK) {
        // Check if name begins with prefix
        if(strncmp(Filename, Prefix, Len) == 0) {   // Prefix found
            if(N == 0) {                            // Required number of files found
                strcpy(PName, Filename);            // Copy name
                return OK;
            }
            else N--;
        }
        r = GetNext();  // Find next file
    }
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

uint8_t sd_t::iniReadString(const char *ASection, const char *AKey, const char *AFileName, char *POutput) {
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
    POutput = StartP;
    return OK;
}

uint8_t sd_t::iniReadInt32(const char *ASection, const char *AKey, const char *AFileName, int32_t *POutput) {
    char *S = nullptr;
    if(iniReadString(ASection, AKey, AFileName, S) == OK) {
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
