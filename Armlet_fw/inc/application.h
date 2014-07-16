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
#include "../MusicProcessor/atlantis_music_tree.h"
#include "kl_lib_f2xx.h"

#if 1 // ==== Timings ====
#define T_PILL_CHECK_MS         360  // Check if pill connected every TM_PILL_CHECK
#define T_MEASUREMENT_MS        5004 // Battery measurement
#endif

#if 1 // ============================== Pill ===================================
void TmrPillCheckCallback(void *p);
struct Pill_t {
    int32_t Type;
    int32_t ChargeCnt;
};
#define PILL_SZ     sizeof(Pill_t)
#define PILL_SZ32   (sizeof(Pill_t) / sizeof(int32_t))
// Inner Pill addresses
#define PILL_TYPE_ADDR          0
#define PILL_CHARGECNT_ADDR     4
#endif

enum AppState_t {asIdle, asCurrent};

class App_t {
public:
    uint32_t ID=0;
    Pill_t Pill;
    Thread *PThd;
    AppState_t State;
    // Timers
    VirtualTimer TmrPillCheck;
    void Init();
    void Task();
    void StopEverything();
    int on_run;
};

extern App_t App;

#endif /* APPLICATION_H_ */
