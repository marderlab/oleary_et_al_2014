//Abstract class for defining sensors
#ifndef SENSOR
#define SENSOR
#include "compstate.h"
#include <cmath>

class sensor {
protected:
    double* pvalue; //could be an array
    compstate* pcompartment_state;
public:
    sensor()
    {
        //null pointer for safety
        //pcompartment_state = 0;
        //pvalue = 0;
    }
    
    ~sensor() {}
    
    double* get_pvalue(void);
    
    // all of these must be defined and will be unique
    // for each sensor in general
    virtual int get_state_dim(void) =0;
    virtual int get_value_dim(void) =0;
    virtual void get_value(double*) =0;
    virtual void integrate(double) =0;
    virtual void connect(compstate*) =0;
    virtual void state2double(double*) =0;
    virtual void double2state(double*) =0;
};

double* sensor::get_pvalue(void)
{
    return pvalue;
}

#endif
