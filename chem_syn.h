// chemical synapse with boltzman activation
#ifndef CHEM_SYN
#define CHEM_SYN
#include "synapse.h"
#include <cmath>

//inherit conductance class spec
class chem_syn: public synapse {
protected:
    double v_half, delta; //half-activation, slope
public:
    //constructors - must set gbar!    
    // gbar only
    chem_syn(double g_in, double e_in, double vh_in, double d_in)
    {
        gbar = g_in;
        state_dim = 0;
        e_rev = e_in;
        v_half = vh_in;
        delta = d_in;
    }
    
    double activation(double);
    double i(void);
    int get_state_dim(void);
    void integrate(double);
    void state2double(double*);
    void double2state(double*);
};

double chem_syn::activation(double pre_v)
{
    return 1.0/(1.0+exp((v_half-pre_v)/delta));
}

double chem_syn::i(void)
{
    return gbar*activation(pre_vm())*(e_rev - post_vm());
}

int chem_syn::get_state_dim(void)
{
    return 0;
}

void chem_syn::integrate(double dt)
{
    g = gbar*activation(pre_vm());
}

void chem_syn::state2double(double* st) { }

void chem_syn::double2state(double*) { }

#endif
