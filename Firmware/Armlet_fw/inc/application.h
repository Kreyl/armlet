/*
 * application.h
 *
 *  Created on: 16.01.2013
 *      Author: kreyl
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "ch.h"

class App_t {
public:
    Thread *PThd;
    void Init();
//    void StopEverything();
};

extern App_t App;

#endif /* APPLICATION_H_ */
