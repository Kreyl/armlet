/*
 * gui.cpp
 *
 *  Created on: 29 сент. 2014 г.
 *      Author: r.leonov
 */


#include "gui.h"
#include "application.h"
#include "reasons.h"
#include "mesh_lvl.h"

gui_t GUI;

void gui_t::Init() {
    for(uint8_t i=0; i<LCD_W; i++) {
        Lcd.Printf(B52, i, 0, clTopClr, clBack, "%c", ' ');
        chThdSleepMilliseconds(4);
    }
    draw_SelfID();
    draw_SelfName();
    Lcd.DrawBatteryBounds();
}

void gui_t::draw_RxTable(RxTable_t *P) {
    // clean RxTable lines
    RxTable_lines = MIN(P->PTable->Size, MAX_GUI_NEIGHBOR_LINES);
}

void gui_t::clean_RxTable() {

}


void gui_t::draw_Location(uint8_t LocAddr, uint8_t Power) {
    if(LocAddr == 0) {
        Lcd.Printf(LOCATION_ID_X, LOCATION_START_Y, clLocClr, clTopClr, "|");
        Lcd.Printf(LOCATION_POWER_X, LOCATION_START_Y, clLocClr, clTopClr, "|");
        return;
    }
    Lcd.Printf(LOCATION_ID_X, LOCATION_START_Y, clLocClr, clTopClr, "%u", LocAddr);
    char* Name[MAX_REASON_BUF_SIZE];
    uint8_t Len = strlen(reasons[LocAddr].name);
    Len = MIN(MAX_REASON_BUF_SIZE-1, Len);
    memcpy(Name, reasons[LocAddr].name, Len);
    Lcd.Printf(LOCATION_START_X, LOCATION_START_Y, clLocClr, clTopClr, reasons[LocAddr].name);
    Lcd.Printf(LOCATION_POWER_X, LOCATION_START_Y, clLocClr, clTopClr, "%u", Power);
}

void gui_t::draw_Time() {
    char timechar[5];
    Mesh.GetAstronomicTime(timechar, 6);
    Lcd.Printf(B52, TIME_X, TIME_Y, clTopClr, clBack, "%s" ,timechar);
}

void gui_t::draw_SelfID() {
    Lcd.Printf(B52, ID_START_X, ID_START_Y, clTopClr, clBack, "%u", App.SelfID);
}

void gui_t::draw_SelfName() {
    char* Name[MAX_REASON_BUF_SIZE];
    uint8_t Len = strlen(reasons[App.SelfID].name);
    Len = MIN(MAX_REASON_BUF_SIZE-1, Len);
    memcpy(Name, reasons[App.SelfID].name, Len);
    Name[Len] = '\0';
    Lcd.Printf(B52,NAME_START_X, NAME_START_Y, clTopClr, clBack, "%s", Name);
}

