/*
 * energy.cpp
 *
 *  Created on: 24 θώνÿ 2014 γ.
 *      Author: USER
 */
#include "energy.h"
#include "cmd_uart.h"
Energy_t Energy;
int Energy_t::GetEnergy()
{
    if(energy_lvl<MIN_ENERGY_LVL)
    {
        Uart.Printf("\r GetEnergy E<MIN_ENERGY_LVL ERROR SET E=MIN_ENERGY_LVL %d",energy_lvl);
        energy_lvl=MIN_ENERGY_LVL;

    }
    if(energy_lvl>MAX_ENERGY_LVL)
    {
        Uart.Printf("\r GetEnergy E>MAX_ENERGY_LVL ERROR SET E=MAX_ENERGY_LVL %d",energy_lvl);
        energy_lvl=MAX_ENERGY_LVL;
    }
    return energy_lvl;
}
void Energy_t::SetEnergy(int val_in){

    Uart.Printf("\r ENERGY CURRENT SET %d PROTECTED",val_in);
    if(val_in>MAX_ENERGY_LVL)
        energy_lvl=MAX_ENERGY_LVL;
    else if(val_in<MIN_ENERGY_LVL)
        energy_lvl=MIN_ENERGY_LVL;

    energy_lvl=val_in;}
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

    Uart.Printf("\r ENERGY CURRENT %d",energy_lvl);
}
