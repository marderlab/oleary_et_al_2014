// LEAK CONDUCTANCE
#ifndef GAP
#define GAP
#include "synapse.h"

//inherit conductance class spec
class gap: public synapse {
public:
    //constructors - must set gbar!    
    // gbar only
    gap(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 0;
        // default if unspecified
        e_rev = -50.0;
    }
        
    //double i(void);
    double get_ge(void); // for Dayan-Abbot integration
    int get_state_dim(void);
    void integrate(double);
    void state2double(double*);
    void double2state(double*);
};

int gap::get_state_dim(void)
{
    return 0;
}

double gap::i(void)
{
    return gbar*(e_rev - compartment_state->get_v());
}

void gap::integrate(double dt)
{
    g = gbar;
}

void gap::state2double(double* st) { }

void gap::double2state(double*) { }

#endif
