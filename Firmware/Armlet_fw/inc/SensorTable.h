/*
 * SensorTable.h
 *
 *  Created on: 02 февр. 2014 г.
 *      Author: Kreyl
 */

#ifndef SENSORTABLE_H_
#define SENSORTABLE_H_

struct Row_t {
    uint8_t ID;
    uint8_t Level;
};

#define MAX_ROW_CNT     99
struct Table_t {
    uint32_t Size;
    Row_t Row[MAX_ROW_CNT];
};

class SnsTable_t {
private:
    Table_t ITbl[2], *PCurrTbl;
    void ISwitchTableI();
    Thread *IPThd;
public:
    void RegisterAppThd(Thread *PThd) { IPThd = PThd; }
    SnsTable_t() : PCurrTbl(&ITbl[0]), IPThd(nullptr), PTable(&ITbl[1]) {}
    Table_t *PTable;
    void PutSnsInfo(uint8_t ID, uint8_t Level);
    void SendEvtReady();
};

extern SnsTable_t SnsTable;

#endif /* SENSORTABLE_H_ */
