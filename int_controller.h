// FSD Integral controller
#ifndef INT_CONTROLLER
#define INT_CONTROLLER
#include <cmath>
//#include "mex.h"

class int_controller: public controller {
protected:
    double Tu1;
    double tau_g, Tg;
    double target;
    double m, Tm;
public:
    int_controller()
    {
        target = 0.0;
        Tu1 = 0.0;
        Tg = 0.0;        
    }
    
    int_controller(double t, double tu1, double tg, double tm, double m0)
    {
        target = t;
        m = m0;
        Tu1 = -tu1;
        Tm = 1.0/tm;
        Tg = 1.0/tg;
    }
    
    ~int_controller() {}
    
    void integrate(double);
    int get_state_dim(void);
    void state2double(double*);
    void double2state(double*);
//    void intialize(void);
};

void int_controller::integrate(double dt)
{
    double old_m = m;
    
    (*pgbar) += dt*Tg*(m - (*pgbar));
    m += dt*Tm*(Tu1*(psensor_value[0] - target));
    
    if ((*pgbar) < 0.0)
    {
        (*pgbar) = 0.0;
        m = old_m;
    }
       
//    (*pgbar) += dt*Tg*Tu1*(m - (*pgbar));
//    m += dt*Tm*((target - psensor_value[0]));
}

int int_controller::get_state_dim(void) 
{
    return 0;
}

void int_controller::state2double(double* st)
{
    st = 0;
}

void int_controller::double2state(double* uin)
{ }

/*
void int_controller::intialize(void)
{
}
*/
#endif
