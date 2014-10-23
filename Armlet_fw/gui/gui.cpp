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

void gui_t::RxTableParse(RxTable_t *P) {
    for(uint8_t vert_str=0; vert_str < P->PTable->Size; vert_str++) {
        if((P->PTable->Row[vert_str].ID >= PERSON_ID_START) && (PERSON_ID_END >= P->PTable->Row[vert_str].ID)) {
            NeighbourTbl.PutNeighbor(P->PTable->Row[vert_str].ID, P->PTable->Row[vert_str].Level);
        } // if reason is human number
    } // for string in RxTable
    NeighbourTbl.PrepareDisplayBuf();
    NeighbourTbl.DisplayNeighbors();
}


#if 1 // Integlral Neighbours
void NeighbourTbl_t::PutNeighbor(uint16_t ID, uint8_t Level) {
    if(LineCnt < MAX_NEIGHBOR_SIZE) { NeighbourBuf[LineCnt].Put(ID, Level); LineCnt++; }
}

void NeighbourTbl_t::PrepareDisplayBuf() {
    SortByPower();
    CleanUpDisplayBuf();
    if(LineCnt > GUI_NEIGHBOR_LINES) { // All data is freshy
        for(uint8_t i=0; i<GUI_NEIGHBOR_LINES; i++) {
            DisplayBuf[i] = NeighbourBuf[i];
            DisplayBufSz = GUI_NEIGHBOR_LINES;
        }
    }
    else { // should check the data and fill the display information correctly
        NeighbourLine_t *P = NeighbourBuf;
        while(LineCnt-- > 0) {
            uint8_t Line = WRONG_PARAM;
            for(uint8_t i=0; i<DisplayBufSz; i++) {
                if(P->ID == DisplayBuf[i].ID) Line = i;
            }
            if(Line != WRONG_PARAM) DisplayBuf[Line] = *P++;
            else if(DisplayBufSz < GUI_NEIGHBOR_LINES) {
                    DisplayBuf[DisplayBufSz] = *P++;
                    DisplayBufSz++;
            } else { // Find more relevant information
                Line = FindPlace();
                if(Line != WRONG_PARAM) DisplayBuf[Line] = *P++;
            }
        } // while LineCnt
    }
}

void NeighbourTbl_t::DisplayNeighbors() {
    Cls();
    if(DisplayBufSz == 0) {
        GUI.draw_EmptyLine(REASON_NAME_START_Y, clOrangeClr, clBlack);
    }
    else {
        for(uint8_t i=0; i<DisplayBufSz; i++) {
            GUI.draw_Line(REASON_NAME_START_Y + (i*REASON_NAME_HEIGHT), clOrangeClr, clBlack, DisplayBuf[i].ID, DisplayBuf[i].Level);
            DisplayBuf[i].LifeTime += 1;
            DisplayBuf[i].Level = 0;
        }
    }
    LineCntToDelete = DisplayBufSz;
    LineCnt = 0;
}

void NeighbourTbl_t::Cls() {
    for(uint8_t i=0; i<LineCntToDelete; i++) GUI.erase_Line(REASON_NAME_START_Y + (i*REASON_NAME_HEIGHT), clOrangeClr, clBlack);
}

#endif
void gui_t::draw_Location(uint8_t LocAddr, uint8_t Power) {
    if(LocAddr == 0) {
        draw_EmptyLine(LOCATION_START_Y, clWhiteClr, clBlackClr);
        return;
    }
    draw_locationI(LOCATION_START_Y, clWhiteClr, clBlackClr, LocAddr, Power);
}

void gui_t::draw_Time() {
    char timechar[5];
    Mesh.GetAstronomicTime(timechar, 6);
    Lcd.Printf(CourierNew, TIME_X, TIME_Y, clBlackClr, clOrangeClr, "%s" ,timechar);
}

void gui_t::draw_SelfID() {
    uint8_t Spacing = ID_START_X;
    if(App.SelfID < 10) Spacing = 15;
    if((App.SelfID > 9) && (App.SelfID < 100)) Spacing = LOCATION_ID_X;
    Lcd.Printf(CourierNew, Spacing, ID_START_Y, clBlackClr, clOrangeClr, "%u", App.SelfID);
}

void gui_t::draw_SelfName() {
    Lcd.Printf(CourierNew, NAME_START_X, NAME_START_Y, clBlackClr, clOrangeClr, "%s", reasons[App.SelfID].name);
}

