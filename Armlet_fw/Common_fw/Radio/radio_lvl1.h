/*
 * radio_lvl1.h
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#ifndef RADIO_LVL1_H_
#define RADIO_LVL1_H_

#include "ch.h"
#include "kl_lib_f2xx.h"
#include "cc1101.h"
#include "mesh_params.h"

class rLevel1_t {
private:
    uint32_t LastTime;
public:
    void Init();
    // Inner use
    Thread *rThd;
    meshradio_t Valets; /* private for mesh */
    void ITask();
    void IMeshRx();
    void IIterateChannels();
};

extern rLevel1_t Radio;

#endif /* RADIO_LVL1_H_ */
