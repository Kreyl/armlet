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
#include "reasons.h"

#define clOrangeClr     ((Color_t){0xFF, 0x15, 0x00}) // Orange
#define clWhiteClr      ((Color_t){0xFF, 0xFF, 0xFF}) // White ()
#define clBlackClr      ((Color_t){0x00, 0x00, 0x00}) // Black


#define TIME_X                  104
#define TIME_Y                  0

#define SELF_NAME       "RedArrow\0"
#define MAX_REASON_BUF_SIZE     14


#define MAX_GUI_NEIGHBOR_LINES  9
#define MAX_POWER               100

#define ID_START_X              1
#define ID_START_Y              0

#define NAME_START_X            30
#define NAME_START_Y            0

#define LOCATION_START_X        30
#define LOCATION_START_Y        12
#define LOCATION_ID_X           10
#define LOCATION_POWER_X        142

#define REASON_NAME_START_Y     24
#define REASON_NAME_HEIGHT      12

struct gui_t {
private:
    uint8_t RxTable_lines;
    void draw_Line(uint8_t y, Color_t Text, Color_t Bg, uint16_t ID, uint8_t Pwr) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "%u ", ID);
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, reasons[ID].name);
        Pwr = (Pwr == 100)? Pwr-1: Pwr;
        Lcd.Printf(CourierNew, LOCATION_POWER_X, y, Text, Bg, "%u", Pwr);
    }
    void draw_EmptyLine(uint8_t y, Color_t Text, Color_t Bg) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "--");
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, "--");
        Lcd.Printf(CourierNew, LOCATION_POWER_X, y, Text, Bg, "--");
    }
    void erase_Line(uint8_t y, Color_t Text, Color_t Bg) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "   ");
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, "             ");
        Lcd.Printf(CourierNew, LOCATION_POWER_X, y, Text, Bg, "  ");
    }
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
