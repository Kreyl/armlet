/*
 * energy.h
 *
 *  Created on: 24 июня 2014 г.
 *      Author: USER
 */

#ifndef ENERGY_H_
#define ENERGY_H_
#define MIN_ENERGY_LVL 5
#define MAX_ENERGY_LVL 100
#define ENERGY_MAX_CHANGE_STEP 10
#define SEC_TO_SELF_REDUCE 5700
#define START_ENERGY 50
class Energy_t
{
private:
   int energy_lvl;
public:
    Energy_t(){energy_lvl=START_ENERGY;};
    void AddEnergy(int value);

    //зависимость динамики персонажа от уровня энергии
    //[1-4]
  //  int GetEnergyScale(){return 1+ (energy_lvl)/30;};

    //TODO - make it smoother!!!
    //increase value due to energy
    int GetEnergyScaleValMore(int val_to_scale)
    {
        return( (val_to_scale*100* (   1+ energy_lvl/50)  ) /100);
    }
    //reduce valuedue to energy
    int GetEnergyScaleValLess(int val_to_scale)
    {
        return( (val_to_scale*100)/( (   1+ energy_lvl/50)  *100));
    }
};


extern Energy_t Energy;

#endif /* ENERGY_H_ */
