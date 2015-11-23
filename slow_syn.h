// slow chemical synapse with boltzman activation
#ifndef SLOW_SYN
#define SLOW_SYN
#include "synapse.h"
#include <cmath>

//inherit conductance class spec
class slow_syn: public synapse {
protected:
    double v_half, delta, tau, m, m_inf; //half-activation, slope, timeconst
public:
    //constructors - must set gbar!    
    // gbar only
    slow_syn(double g_in, double e_in, double vh_in, double d_in, double tau_in)
    {
        gbar = g_in;
        state_dim = 0;
        e_rev = e_in;
        v_half = vh_in;
        delta = d_in;
        tau = tau_in;
        m = 0.0;
        m_inf = 0.0;
    }
    
    double activation(double);
    double i(void);
    int get_state_dim(void);
    void integrate(double);
    void state2double(double*);
    void double2state(double*);
};

double slow_syn::activation(double pre_v)
{
    return 1.0/(1.0+exp((v_half-pre_v)/delta));
}

double slow_syn::i(void)
{
    return gbar*activation(pre_vm())*(e_rev - post_vm());
}

int slow_syn::get_state_dim(void)
{
    return 0;
}

void slow_syn::integrate(double dt)
{
    m_inf = activation(pre_vm());
    m = m_inf + (m - m_inf)*exp(-dt/tau);
    g = gbar*m;
}

void slow_syn::state2double(double* st) { }

void slow_syn::double2state(double*) { }

#endif
