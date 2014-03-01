/*
 * rlvl2.h
 *
 *  Created on: 01 марта 2014 г.
 *      Author: Kreyl
 */

#ifndef RLVL2_H_
#define RLVL2_H_

#include "kl_lib_f2xx.h"

// Single entry of ID Table
struct IdTableRow_t {
    uint8_t ID;
    uint8_t Level;  // Percent
} __attribute__((__packed__));

#define IDTABLE_ROW_CNT     252

struct IdTable_t {
    uint32_t RowCnt;
    IdTableRow_t Rows[IDTABLE_ROW_CNT];
};

class rLevel2_t {
private:
    IdTable_t ITbl[2], *PInTbl;
public:
    IdTable_t *PTable;
    void Init();
    void ITask();
};

extern rLevel2_t RLvl2;

#endif /* RLVL2_H_ */
