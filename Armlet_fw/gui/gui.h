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

#define MAX_GUI_NEIGHBOR_LINES  9

struct gui_t {
private:
    uint8_t RxTable_lines;
public:
    void draw_RxTable(RxTable_t * P);
    void clean_RxTable();
    void draw_Location(uint8_t LocAddr);
};


extern gui_t GUI;


#endif /* GUI_H_ */
