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
#include "kl_lib_f2xx.h"

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
        Uart.Printf("SD connect error\r");
        return;
    }
    else {
        Uart.Printf("SD capacity: %u\r", SDCD1.capacity);
    }
    err = f_mount(0, &SDC_FS);
    if (err != FR_OK) {
        Uart.Printf("SD mount error\r");
        sdcDisconnect(&SDCD1);
        return;
    }
    IsReady = TRUE;
}

// Get first file in folder
FRESULT sd_t::GetFirst(const char* DirPath) {
    FRESULT r = f_opendir(&Directory, DirPath); // Try to open the folder
    if(r != FR_OK) return r;
    FileInfo.lfname = LongFileName;
    FileInfo.lfsize = MAX_NAME_LEN;
//    Uart.Printf("lfn: %u\r", Directory.lfn_idx);
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
    strcpy(PrevFn, Filename);
    while(true) {
        FRESULT r = f_readdir(&Directory, &FileInfo);
        if(r != FR_OK) return r;
//        Uart.Printf("lfn: %u\r", Directory.lfn_idx);
        Filename = (FileInfo.lfname[0] == 0)? FileInfo.fname : FileInfo.lfname;
        if(Filename[0] == 0) return FR_NO_FILE;
        if(!(FileInfo.fattrib & AM_DIR)) {
            return FR_OK;
        }
    }
    return FR_INT_ERR;
}

uint8_t sd_t::GetPrevious() {
//    Uart.Printf("FN %S\r", Filename);
    strcpy(TmpFn, PrevFn);
//    Uart.Printf("searchFN %S\r", TmpFn);
    FRESULT r = GetFirst("/");
    while(r == FR_OK) {
//        Uart.Printf("currFN %S\r", Filename);
        if(strcmp(Filename, TmpFn) == 0) {
//            Uart.Printf("1\r");
            return OK;
        }
//        else Uart.Printf("2\r");
        r = GetNext();
    }
    return FAILURE;
}


uint8_t sd_t::GetNthFileByPrefix(const char* Prefix, uint32_t N, char* PName) {
    uint32_t Len = strlen(Prefix);
    FRESULT r = GetFirst("/");
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

// ========================== ini files operations =============================
#ifdef USE_INI_FILES
#include <stdlib.h>

#define INI_BUF_SIZE    512
static char IBuf[INI_BUF_SIZE];
static char FBuf[64];

// ==== Inner use ====
static inline char* skipleading(char *S) {
    while (*S != '\0' && *S <= ' ') S++;
    return (char*)S;
}
static inline char* skiptrailing(char *S, const char *base) {
    while ((S > base) && (*(S-1) <= ' ')) S--;
    return (char*)S;
}
static inline char* striptrailing(char *S) {
    char *ptr = skiptrailing(strchr(S, '\0'), S);
    *ptr='\0';
    return (char*)S;
}

// ======================== Implementation =====================================
uint8_t iniReadString(const char *ASection, const char *AKey, const char *AFileName, char *POutput, uint32_t AMaxLen) {
    FRESULT rslt;
    // Open file
    rslt = f_open(&SD.File, AFileName, FA_READ+FA_OPEN_EXISTING);
    if(rslt != FR_OK) {
        Uart.Printf(AFileName);
        if (rslt == FR_NO_FILE) Uart.Printf(": file not found\r");
        else Uart.Printf(": openFile error: %u", rslt);
        return FAILURE;
    }
    // Check if zero file
    if(SD.File.fsize == 0) {
        f_close(&SD.File);
        Uart.Printf("Empty file\r");
        return FAILURE;
    }
    //Uart.Printf("%S\r", FBuf);
    // Move through file one line at a time until a section is matched or EOF.
    char *StartP, *EndP;
    uint32_t len = strlen(ASection);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &SD.File)) {
            Uart.Printf("Section Read Err\r");
            f_close(&SD.File);
            return FAILURE;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, ']');
    } while ((*StartP != '[') or (EndP == NULL) or ((uint32_t)(EndP-StartP-1) != len or strnicmp(StartP+1, ASection, len) != 0));

    // Section found, find the key
    len = strlen(AKey);
    do {
        if (!f_gets(IBuf, INI_BUF_SIZE, &SD.File) or *(StartP = skipleading(IBuf)) == '[') {
            Uart.Printf("Key Read Err\r");
            f_close(&SD.File);
            return FAILURE;
        }
        StartP = skipleading(IBuf);
        EndP = strchr(StartP, '='); /* Parse out the equal sign */
    } while ((*StartP == ';') or (*StartP == '#') or (EndP == NULL) or ((uint32_t)(skiptrailing(EndP, StartP)-StartP) != len or strnicmp(StartP, AKey, len) != 0));

    // Copy up to ALength chars to AOutput
    StartP = skipleading(EndP + 1);
    // Remove a trailing comment
    uint8_t isstring = 0;
    for (EndP = StartP; (*EndP != '\0') and (((*EndP != ';') and (*EndP != '#')) or isstring) and ((uint32_t)(EndP - StartP) < AMaxLen); EndP++) {
        if (*EndP == '"') {
            if (*(EndP + 1) == '"') EndP++;     // skip "" (both quotes)
            else isstring = !isstring; // single quote, toggle isstring
        }
        else if (*EndP == '\\' && *(EndP + 1) == '"') EndP++; // skip \" (both quotes)
    } // for
    *EndP = '\0';   // Terminate at a comment
    striptrailing(StartP);
    strcpy(POutput, StartP);
    f_close(&SD.File);
    return OK;
}

uint8_t iniReadInt32(const char *ASection, const char *AKey, const char *AFileName, int32_t *POutput) {
    if(iniReadString(ASection, AKey, AFileName, FBuf, 64) == OK) {
        *POutput = strtol(FBuf, NULL, 10);
        return OK;
    }
    else return FAILURE;
}
uint8_t iniReadUint32(const char *ASection, const char *AKey, const char *AFileName, uint32_t *POutput) {
    if(iniReadString(ASection, AKey, AFileName, FBuf, 64) == OK) {
        *POutput = strtol(FBuf, NULL, 10);
        return OK;
    }
    else return FAILURE;
}

#endif

// ============================== Hardware =====================================
#ifdef __cplusplus
extern "C" {
#endif

bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {
    (void)sdcp;
    return TRUE;
}

bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {
    (void)sdcp;
    return FALSE;
}

#ifdef __cplusplus
}
#endif
