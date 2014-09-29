/*
 * gui.h
 *
 *  Created on: 29 сент. 2014 г.
 *      Author: r.leonov
 */

#ifndef GUI_H_
#define GUI_H_


#include "kl_lib_f2xx.h"
#include "cmd_uart.h"
#include "RxTable.h"
#include "color.h"
#include "Fonts.h"
#include "lcd2630.h"

#define clFontClr   ((Color_t){0xFF, 0x15, 0x00}) // Orange
#define clLocClr    ((Color_t){0xFF, 0xFF, 0xFF}) // White ()
#define clTopClr    ((Color_t){0x00, 0x00, 0x00}) // Black
#define clBack      ((Color_t){0xFF, 0x15, 0x00})


#define TIME_X                  100
#define TIME_Y                  0

#define SELF_NAME       "RedArrow\0"
#define MAX_REASON_BUF_SIZE     14


#define MAX_GUI_NEIGHBOR_LINES  9
#define ID_START_X              1
#define ID_START_Y              0

#define NAME_START_X            30
#define NAME_START_Y            0

#define LOCATION_START_X        30
#define LOCATION_START_Y        12
#define LOCATION_ID_X           10
#define LOCATION_POWER_X        142

struct gui_t {
private:
    uint8_t RxTable_lines;
public:
    void Init();
    void draw_RxTable(RxTable_t *P);
    void clean_RxTable();
    void draw_Location(uint8_t LocAddr, uint8_t Power);
    void draw_Time();
    void draw_SelfID();
    void draw_SelfName();
    void draw_BatteryBorder();
};


extern gui_t GUI;


#endif /* GUI_H_ */
