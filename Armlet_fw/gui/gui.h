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

#define TIME_X                  104
#define TIME_Y                  0

#define SELF_NAME       "RedArrow\0"
#define MAX_REASON_BUF_SIZE     12


#define GUI_NEIGHBOR_LINES      8
#define MAX_NEIGHBOR_SIZE       100
#define MAX_POWER               100

#define ID_START_X              1
#define ID_START_Y              0

#define NAME_START_X            30
#define NAME_START_Y            0

#define LOCATION_START_X        30
#define LOCATION_START_Y        12
#define LOCATION_ID_X           8
#define LOCATION_POWER_X        145

#define REASON_NAME_START_Y     24
#define REASON_NAME_HEIGHT      12

#define NEIGHBOR_LIFETIME       3
#define WRONG_PARAM             0xFF

struct NeighbourLine_t {
public:
    uint16_t ID;
    uint8_t Level;
    uint8_t  LifeTime;
    void Put(uint16_t PutID, uint8_t PutLevel) {
        ID = PutID;
        Level = PutLevel;
        LifeTime = 0;
    }
};

class NeighbourTbl_t {
private:
    NeighbourLine_t NeighbourBuf[MAX_NEIGHBOR_SIZE], DisplayBuf[GUI_NEIGHBOR_LINES];
    uint8_t LineCnt, DisplayBufSz, LineCntToDelete, LineCntToFill;
    void SortByPower() {
        for(uint8_t i = 0; i < LineCnt - 1; i++) {
            for(uint8_t j = 0; j < LineCnt - i - 1; j++) {
                if(NeighbourBuf[j].Level < NeighbourBuf[j+1].Level) {
                    NeighbourLine_t tmp = NeighbourBuf[j];
                    NeighbourBuf[j] = NeighbourBuf[j+1];
                    NeighbourBuf[j+1] = tmp;
                }
            } // for j
        } // for i
    }
    void CleanUpDisplayBuf() {
        uint8_t Sz = DisplayBufSz;
        for(uint8_t i=0; i < Sz; i++) {
            if(DisplayBuf[i].LifeTime > NEIGHBOR_LIFETIME) {
                Uart.Printf("\rDelete %u", DisplayBuf[i].ID);
                DisplayBufSz--;
                for(uint8_t j=i; j < Sz-1; j++) {
                    NeighbourLine_t tmp = DisplayBuf[j];
                    DisplayBuf[j] = DisplayBuf[j+1];
                    DisplayBuf[j+1] = tmp;
                }
            } // if 0
        } // for
    }
    uint8_t FindPlace() {
        uint8_t RetLine = WRONG_PARAM;
        uint8_t LowLifeTime = 0;
        for(uint8_t i = 0; i < DisplayBufSz; i++) {
            if(DisplayBuf[i].LifeTime > LowLifeTime) {
                LowLifeTime = DisplayBuf[i].LifeTime;
                RetLine = i;
            } // if 0
        } // for
        return RetLine;
    }
public:
    void PutNeighbor(uint16_t ID, uint8_t Level);
    void DisplayNeighbors();
    void PrepareDisplayBuf();
    void Cls();
};


struct gui_t {
private:
    NeighbourTbl_t NeighbourTbl;
    uint8_t RxTable_lines;
    void draw_locationI(uint8_t y, Color_t Text, Color_t Bg, uint16_t ID, uint8_t Pwr) {
        erase_Line(y, clBlackClr, clBlackClr);
        Lcd.Printf(CourierNew, LOCATION_ID_X, y, Text, Bg, "%u ", ID);
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, reasons[ID].name);
        Pwr = (Pwr == 100)? Pwr-1: Pwr;
        Lcd.DrawSignal(LOCATION_POWER_X, y, Text, Bg, Pwr);
    }
public:
    void Init();
    void RxTableParse(RxTable_t *P);
    void draw_Line(uint8_t y, Color_t Text, Color_t Bg, uint16_t ID, uint8_t Pwr) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "%u ", ID);
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, reasons[ID].name);
        Pwr = (Pwr == 100)? Pwr-1: Pwr;
        Lcd.DrawSignal(LOCATION_POWER_X, y, Text, Bg, Pwr);
    }
    void erase_Line(uint8_t y, Color_t Text, Color_t Bg) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "   ");
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, "             ");
        Lcd.Printf(CourierNew, LOCATION_POWER_X, y, Text, Bg, "  ");
    }
    void draw_EmptyLine(uint8_t y, Color_t Text, Color_t Bg) {
        Lcd.Printf(CourierNew, ID_START_X, y, Text, Bg, "---");
        Lcd.Printf(CourierNew, LOCATION_START_X, y, Text, Bg, "--");
        Lcd.Printf(CourierNew, LOCATION_POWER_X, y, Text, Bg, "--");
    }
    void draw_Location(uint8_t LocAddr, uint8_t Power);
    void draw_Time();
    void draw_SelfID();
    void draw_SelfName();
    void draw_BatteryBorder();
    friend class NeighborTbl_t;
};


extern gui_t GUI;


#endif /* GUI_H_ */
