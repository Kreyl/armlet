/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_
/*
void AppInit();
*/
#include "kl_lib_f2xx.h"

#define START_VOLUME    180
#define LCD_PRINT_SZ    19

enum AppState_t {asIdle, asCurrent};

class App_t {
public:
    //bool UseQuartz;
    Thread *PThd;
    AppState_t State;
    void Init();
    void Task();
    void StopEverything();

    bool IsPlay;
    int16_t Loudness;
    uint8_t *PFileName;
    uint32_t FileLen;
};

extern App_t App;

#endif /* APPLICATION_H_ */
