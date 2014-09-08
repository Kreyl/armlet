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
#define REASON_FAIL_ENERGY_CHANGE -5
#define REASON_SUCESS_ENERGY_CHANGE 5
#define ENERGY_SEC_IGNORE_SHUTDOWN 10
#define ENERGY_VALUE_ON_DEATH_SHUTDOWN 10
#define ENERGY_SEC_SAFE_ON_DEATH_SHUTDOWN 60

#define ETS 50 //если это маленькое число - надо вводить вес для равномерного деления!
class Energy_t
{
private:
   int energy_lvl;

public:
   int human_support;
    Energy_t(){energy_lvl=START_ENERGY;is_default_cfg=false;};
    void AddEnergy(int value);
    bool is_default_cfg;
    //зависимость динамики персонажа от уровня энергии
    //[1-4]
  //  int GetEnergyScale(){return 1+ (energy_lvl)/50;};

    //TODO - make it smoother!!!
    //increase value due to energy
    int GetEnergyScaleValMoreDefault(int val_to_scale)
    {
        return val_to_scale*(ETS+ START_ENERGY)/ETS;
    }
    int GetEnergyScaleValLessDefault(int val_to_scale)
    {
        return val_to_scale*ETS/(ETS+START_ENERGY);
    }
    int GetEnergyScaleValMore(int val_to_scale,int UI_indx)
    {
        bool is_dependable= CheckEnergyDependency(UI_indx);
        int safe_energy=energy_lvl;

        if(!is_dependable)
            energy_lvl=START_ENERGY;

        if(human_support>4)
            human_support=4;
        int human_support_percentage=100;
        if(human_support>1)
            human_support_percentage+=(human_support-1)*15;//115 130 145

        int rval= val_to_scale*(ETS+ human_support_percentage*energy_lvl/100)/ETS;
        if(!is_dependable)
            energy_lvl=safe_energy;
        return rval;

        //return( (val_to_scale*100* (   1+ energy_lvl/50)  ) /100);
    }
    //reduce valuedue to energy
    int GetEnergyScaleValLess(int val_to_scale,int UI_indx)
    {
        if(human_support>4)
            human_support=4;
        int human_support_percentage=100;
        if(human_support_percentage>1)
            human_support_percentage+=(human_support-1)*15;//115 130 145

        return val_to_scale*ETS/(ETS+human_support_percentage*energy_lvl/100);
        //return( (val_to_scale*100)/( (   1+ energy_lvl/50)  *100));
    }
    int GetEnergy();//{return energy_lvl;}
    void SetEnergy(int val_in);
    bool CheckEnergyDependency(int UI_indx);

};


extern Energy_t Energy;

#endif /* ENERGY_H_ */
