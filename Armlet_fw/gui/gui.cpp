/*
 * gui.cpp
 *
 *  Created on: 29 ����. 2014 �.
 *      Author: r.leonov
 */


#include "gui.h"

void gui_t::draw_RxTable(RxTable_t * P) {
    // clean RxTable lines
    RxTable_lines = MIN(P->PTable->Size, MAX_GUI_NEIGHBOR_LINES);
}

void gui_t::clean_RxTable() {

}


void gui_t::draw_Location(uint8_t LocAddr) {

}
