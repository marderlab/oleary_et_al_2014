//Simple calcium conc sensor!
#ifndef CALCIUMSENSOR
#define CALCIUMSENSOR
#include "sensor.h"

//just a copy of calcium concentration!
class calciumsensor: public sensor {
protected:
//    double value;
public:
    calciumsensor() 
    {
        pvalue = new double(0.0); //could be array
    }
    
    ~calciumsensor() {}
    
    int get_state_dim(void);
    int get_value_dim(void);
    void get_value(double*);
    void integrate(double);
    void connect(compstate*);
    void state2double(double*);
    void double2state(double*);
};

int calciumsensor::get_state_dim(void)
{
    return 0;
}

int calciumsensor::get_value_dim(void)
{
    return 1;
}

void calciumsensor::get_value(double* val)
{
    val[0] = pcompartment_state->get_Ca();
}

void calciumsensor::integrate(double dt)
{
    *pvalue = pcompartment_state->get_Ca();
}

void calciumsensor::connect(compstate* cs)
{
    pcompartment_state = cs;
    *pvalue = pcompartment_state->get_Ca();
}

void calciumsensor::state2double(double* st)
{ }

void calciumsensor::double2state(double* s_in)
{ }

#endif
