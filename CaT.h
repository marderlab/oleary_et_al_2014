// CALCIUM CONDUCTANCE (fast)
#ifndef CAT
#define CAT
#include "conductance.h"

//inherit conductance class spec
class CaT: public conductance {
protected:
    double m;
    double h;
    double i_Ca;
    double v;
public:
    //constructors - must set gbar!    
    // gbar only
    CaT(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        // default if unspecified
        e_rev = 120.0;
    }
    
    //specify both gbar and erev
    CaT(double g_in, double e_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        e_rev = e_in;
    }
    
    double i(void);
    void integrate(double dt);
    //let conductance access cell/compartment state
    void connect(compstate *s_in);
    int get_state_dim(void);
    double m_inf(void);
    double h_inf(void);
    double tau_m(void);
    double tau_h(void);
    void state2double(double*);
    void double2state(double*);    
};

int CaT::get_state_dim(void)
{
    return 2;
}

double CaT::i(void)
{
    return gbar*m*m*m*h*(e_rev - v_m());
}

// can maybe speed things up by overwriting get_g to return gbar & making integrate empty.
void CaT::integrate(double dt)
{
    i_Ca = compartment_state->get_i_Ca();
    e_rev = compartment_state->get_e_Ca();

    m = m_inf() + (m - m_inf())*exp(-dt/tau_m());
    h = h_inf() + (h - h_inf())*exp(-dt/tau_h());
    g = gbar*m*m*m*h;
    
    compartment_state->set_i_Ca(g*(e_rev - v_m()) + i_Ca);
}

void CaT::connect(compstate *s_in)
{
    compartment_state = s_in;
    m = m_inf();
    h = h_inf();
}

double CaT::m_inf(void) {return 1.0/(1.0 + exp((v_m()+27.1)/-7.2));}
double CaT::h_inf(void) {return 1.0/(1.0 + exp((v_m()+32.1)/5.5));}
double CaT::tau_m(void) {return 21.7 - 21.3/(1.0 + exp((v_m()+68.1)/-20.5));}
double CaT::tau_h(void) {return 105.0 - 89.8/(1.0 + exp((v_m()+55.0)/-16.9));}

void CaT::state2double(double* st)
{
    st[0] = m;
    st[1] = h;
}

void CaT::double2state(double* st)
{
    m = st[0];
    h = st[1];
}

#endif
