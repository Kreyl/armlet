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
#define ETS 50 //если это маленькое число - надо вводить вес для равномерного деления!
class Energy_t
{
private:
   int energy_lvl;
public:
   int human_support;
    Energy_t(){energy_lvl=START_ENERGY;};
    void AddEnergy(int value);

    //зависимость динамики персонажа от уровня энергии
    //[1-4]
  //  int GetEnergyScale(){return 1+ (energy_lvl)/30;};

    //TODO - make it smoother!!!
    //increase value due to energy
    int GetEnergyScaleValMore(int val_to_scale)
    {
        if(human_support>4)
            human_support=4;
        int human_support_percentage=100;
        if(human_support_percentage>1)
            human_support_percentage+=(human_support-1)*15;//115 130 145

        return val_to_scale*(ETS+ human_support_percentage*energy_lvl/100)/ETS;

        //return( (val_to_scale*100* (   1+ energy_lvl/50)  ) /100);
    }
    //reduce valuedue to energy
    int GetEnergyScaleValLess(int val_to_scale)
    {
        if(human_support>4)
            human_support=4;
        int human_support_percentage=100;
        if(human_support_percentage>1)
            human_support_percentage+=(human_support-1)*15;//115 130 145

        return val_to_scale*ETS/(ETS+human_support_percentage*energy_lvl/100);
        //return( (val_to_scale*100)/( (   1+ energy_lvl/50)  *100));
    }
};


extern Energy_t Energy;

#endif /* ENERGY_H_ */
