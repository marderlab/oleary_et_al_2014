// Sodium CONDUCTANCE
#ifndef NAV
#define NAV
#include "conductance.h"

//inherit conductance class spec
class NaV: public conductance {
protected:
    double m;
    double h;
public:
    //constructors - must set gbar!    
    // gbar only
    NaV(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        // default if unspecified
        e_rev = 50.0;
    }
    
    //specify both gbar and erev
    NaV(double g_in, double e_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        e_rev = e_in;
    }
    
    //return current
    double i(void);
    void integrate(double dt);
    //let conductance access cell/compartment state
    int get_state_dim(void);
    void connect(compstate *s_in);
    double m_inf(void);
    double h_inf(void);
    double tau_m(void);
    double tau_h(void);
    void state2double(double*);
    void double2state(double*);    
};

int NaV::get_state_dim(void)
{
    return 2;
}

double NaV::i(void)
{
    return gbar*m*m*m*h*(e_rev - v_m());
}

// can maybe speed things up by overwriting get_g to return gbar & making integrate empty.
void NaV::integrate(double dt)
{
    //mexPrintf("integrating, dt = %e\n", dt);
    m = m_inf() + (m - m_inf())*exp(-dt/tau_m());
    h = h_inf() + (h - h_inf())*exp(-dt/tau_h());
    g = gbar*m*m*m*h;
    //default: g is fixed
}

void NaV::connect(compstate *s_in)
{
    compartment_state = s_in;
    m = m_inf();
    h = h_inf();
    //mexPrintf("state ptr val: %f\n",compartment_state->get_v());
    //mexPrintf("state ptr: %d\n",compartment_state);
}

double NaV::m_inf(void) {return 1.0/(1.0+exp((v_m()+25.5)/-5.29));}
double NaV::h_inf(void) {return 1.0/(1.0+exp((v_m()+48.9)/5.18));}
double NaV::tau_m(void) {return 1.32 - 1.26/(1+exp((v_m()+120.0)/-25.0));}
double NaV::tau_h(void) {return (0.67/(1.0+exp((v_m()+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((v_m()+34.9)/3.6)));}

void NaV::state2double(double* st)
{
    st[0] = m;
    st[1] = h;
}

void NaV::double2state(double* st)
{
    m = st[0];
    h = st[1];
}

#endif
