// LEAK CONDUCTANCE
#ifndef KA
#define KA
#include "conductance.h"

//inherit conductance class spec
class Ka: public conductance {
protected:
    double m;
    double h;
public:
    //constructors - must set gbar!    
    // gbar only
    Ka(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        // default if unspecified
        e_rev = -80.0;
    }
    
    //specify both gbar and erev
    Ka(double g_in, double e_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        e_rev = e_in;
    }
    
    //return current
    double i(void);
    int get_state_dim(void);
    void integrate(double dt);
    //let conductance access cell/compartment state
    void connect(compstate *s_in);
    double m_inf(void);
    double tau_m(void);
    double h_inf(void);
    double tau_h(void);
    void state2double(double*);
    void double2state(double*);    
};

int Ka::get_state_dim(void)
{
    return 2;
}

//membrane current
double Ka::i(void)
{
    return gbar*m*m*m*h*(e_rev - v_m());
}

// can maybe speed things up by overwriting get_g to return gbar & making integrate empty.
void Ka::integrate(double dt)
{
    //mexPrintf("integrating, dt = %e\n", dt);
    m = m_inf() + (m - m_inf())*exp(-dt/tau_m());
    h = h_inf() + (h - h_inf())*exp(-dt/tau_h());
    g = gbar*m*m*m*h;
    //default: g is fixed
}

void Ka::connect(compstate *s_in)
{
    compartment_state = s_in;
    m = m_inf();
    h = h_inf();
    //mexPrintf("state ptr val: %f\n",compartment_state->get_v());
    //mexPrintf("state ptr: %d\n",compartment_state);
}

double Ka::m_inf(void)
{
    return 1.0/(1.0+exp((v_m()+27.2)/-8.7));
}

double Ka::h_inf(void)
{
    return 1.0/(1.0+exp((v_m()+56.9)/4.9));
}

double Ka::tau_m(void) 
{
    return 11.6 - 10.4/(1.0+exp((v_m()+32.9)/-15.2));
}

double Ka::tau_h(void) 
{
    return 38.6 - 29.2/(1.0+exp((v_m()+38.9)/-26.5));
}

void Ka::state2double(double* st)
{
    st[0] = m;
    st[1] = h;
}

void Ka::double2state(double* st)
{
    m = st[0];
    h = st[1];
}

#endif
