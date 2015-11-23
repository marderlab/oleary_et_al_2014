// CALCIUM CONDUCTANCE (slow)
#ifndef CAS
#define CAS
#include "conductance.h"

//inherit conductance class spec
class CaS: public conductance {
protected:
    double m;
    double h;
    double i_Ca;
    double v;
public:
    //constructors - must set gbar!    
    // gbar only
    CaS(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        // default if unspecified
        e_rev = 120.0;
    }
    
    //specify both gbar and erev
    CaS(double g_in, double e_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        e_rev = e_in;
    }
    
    int get_state_dim(void);
    double i(void);
    void integrate(double dt);
    //let conductance access cell/compartment state
    void connect(compstate *s_in);
    double m_inf(void);
    double h_inf(void);
    double tau_m(void);
    double tau_h(void);
    void state2double(double*);
    void double2state(double*);    
};

int CaS::get_state_dim(void)
{
    return 2;
}

double CaS::i(void)
{
    return gbar*m*m*m*h*(e_rev - v_m());
}

// can maybe speed things up by overwriting get_g to return gbar & making integrate empty.
void CaS::integrate(double dt)
{
    i_Ca = compartment_state->get_i_Ca();
    e_rev = compartment_state->get_e_Ca();

    m = m_inf() + (m - m_inf())*exp(-dt/tau_m());
    h = h_inf() + (h - h_inf())*exp(-dt/tau_h());
    g = gbar*m*m*m*h;
    
    compartment_state->set_i_Ca(g*(e_rev - v_m()) + i_Ca);
}

void CaS::connect(compstate *s_in)
{
    compartment_state = s_in;
    m = m_inf();
    h = h_inf();
}

double CaS::m_inf(void) {return 1.0/(1.0+exp((v_m()+33.0)/-8.1));}
double CaS::h_inf(void) {return 1.0/(1.0+exp((v_m()+60.0)/6.2));}
double CaS::tau_m(void) {return 1.4 + 7.0/(exp((v_m()+27.0)/10.0) + exp((v_m()+70.0)/-13.0));}
double CaS::tau_h(void) {return 60.0 + 150.0/(exp((v_m()+55.0)/9.0) + exp((v_m()+65.0)/-16.0));}

void CaS::state2double(double* st)
{
    st[0] = m;
    st[1] = h;
}

void CaS::double2state(double* st)
{
    m = st[0];
    h = st[1];
}

#endif
