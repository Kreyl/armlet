/*
 * gui.cpp
 *
 *  Created on: 29 сент. 2014 г.
 *      Author: r.leonov
 */


#include "gui.h"
#include "application.h"
#include "mesh_lvl.h"

gui_t GUI;

void gui_t::Init() {
    for(uint8_t i=0; i<LCD_W; i++) {
        Lcd.Printf(B52, i, 0, clBlackClr, clOrangeClr, "%c", ' ');
        chThdSleepMilliseconds(4);
    }
    draw_SelfID();
    draw_SelfName();
    Lcd.DrawBatteryBounds();
}

void gui_t::draw_RxTable(RxTable_t *P) {
    clean_RxTable();
//    if(P->PTable->Size == 0) {
//        draw_EmptyLine(REASON_NAME_START_Y, clOrangeClr, clBlackClr);
//    }
//    else {
        for(uint8_t vert_str=0; vert_str < P->PTable->Size; vert_str++) {
            if((P->PTable->Row[vert_str].ID >= PERSON_ID_START) && (PERSON_ID_END >= P->PTable->Row[vert_str].ID)) {
                if(RxTable_lines < MAX_GUI_NEIGHBOR_LINES) draw_Line(REASON_NAME_START_Y + (RxTable_lines*REASON_NAME_HEIGHT), clOrangeClr, clBlackClr, P->PTable->Row[vert_str].ID, P->PTable->Row[vert_str].Level);
                RxTable_lines++;
            } // if reason is human number
        } // for string in RxTable
//    }
}

void gui_t::clean_RxTable() {
    if(RxTable_lines != 0) {
        do {
            RxTable_lines--;
            erase_Line(REASON_NAME_START_Y + (RxTable_lines*REASON_NAME_HEIGHT), clOrangeClr, clBlackClr);
        } while(RxTable_lines != 0);
    }
    draw_EmptyLine(REASON_NAME_START_Y, clOrangeClr, clBlackClr);
}


void gui_t::draw_Location(uint8_t LocAddr, uint8_t Power) {
    if(LocAddr == 0) {
        draw_EmptyLine(LOCATION_START_Y, clWhiteClr, clBlackClr);
        return;
    }
    if(Power == 0) return;
    draw_Line(LOCATION_START_Y, clWhiteClr, clBlackClr, LocAddr, Power);
}

void gui_t::draw_Time() {
    char timechar[5];
    Mesh.GetAstronomicTime(timechar, 6);
    Lcd.Printf(CourierNew, TIME_X, TIME_Y, clBlackClr, clOrangeClr, "%s" ,timechar);
}

void gui_t::draw_SelfID() {
    Lcd.Printf(CourierNew, ID_START_X, ID_START_Y, clBlackClr, clOrangeClr, "%u", App.SelfID);
}

void gui_t::draw_SelfName() {
    char* Name[MAX_REASON_BUF_SIZE];
    uint8_t Len = strlen(reasons[App.SelfID].name);
    Len = MIN(MAX_REASON_BUF_SIZE-1, Len);
    memcpy(Name, reasons[App.SelfID].name, Len);
    Name[Len] = '\0';
    Lcd.Printf(CourierNew, NAME_START_X, NAME_START_Y, clBlackClr, clOrangeClr, "%s", Name);
}

