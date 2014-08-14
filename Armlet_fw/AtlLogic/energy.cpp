/*
 * energy.cpp
 *
 *  Created on: 24 θώνÿ 2014 γ.
 *      Author: USER
 */
#include "energy.h"

Energy_t Energy;

void Energy_t::AddEnergy(int value)
{
    if(value>ENERGY_MAX_CHANGE_STEP)
        energy_lvl+=ENERGY_MAX_CHANGE_STEP;
    else if(value<-1* ENERGY_MAX_CHANGE_STEP)
        energy_lvl+=-1* ENERGY_MAX_CHANGE_STEP;
    else
        energy_lvl+=value;
    if(energy_lvl<MIN_ENERGY_LVL)
        energy_lvl= MIN_ENERGY_LVL;
    if(energy_lvl>MAX_ENERGY_LVL)
        energy_lvl= MAX_ENERGY_LVL;
}
