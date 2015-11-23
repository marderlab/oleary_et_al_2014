// Integral controller
#ifndef CONTROLLER
#define CONTROLLER
#include "conductance.h"
#include "sensor.h"
#include <cmath>
//#include "mex.h"

class controller {
protected:
    double* pgbar;
    double* psensor_value;
public:
    controller()
    {
        pgbar = 0;
        psensor_value = 0;
    }
    
    ~controller() {}
    
    void connect(sensor*, conductance*);
    
    virtual void integrate(double) =0;
    virtual int get_state_dim(void) =0;
    virtual void state2double(double*) =0;
    virtual void double2state(double*) =0;
};

void controller::connect(sensor* psensor, conductance* pg)
{
    pgbar = pg->get_pgbar();
    psensor_value = psensor->get_pvalue();
}

#endif
