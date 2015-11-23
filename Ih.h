// LEAK CONDUCTANCE
#ifndef IH
#define IH
#include "conductance.h"

//inherit conductance class spec
class Ih: public conductance {
protected:
    double m;
public:
    //constructors - must set gbar!    
    // gbar only
    Ih(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -20.0;
    }
    
    //specify both gbar and erev
    Ih(double g_in, double e_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        e_rev = e_in;
    }
    
    //required by abstract class
    int get_state_dim(void);
    double i(void);
    void integrate(double dt);
    void connect(compstate *s_in);
    void state2double(double*);
    void double2state(double*);
    
    //specific to this class
    double m_inf(void);
    double tau_m(void);
};

int Ih::get_state_dim(void)
{
    return 1;
}

double Ih::i(void)
{
    return gbar*m*(e_rev - v_m());
}

void Ih::integrate(double dt)
{
    m = m_inf() + (m - m_inf())*exp(-dt/tau_m());
    g = gbar*m;
}

void Ih::connect(compstate *s_in)
{
    compartment_state = s_in;
    m = m_inf();
}

double Ih::m_inf(void) {return 1.0/(1.0+exp((v_m()+70.0)/6.0));}
double Ih::tau_m(void) {return (272.0 + 1499.0/(1.0+exp((v_m()+42.2)/-8.73)));}

void Ih::state2double(double* st)
{
    st[0] = m;
}

void Ih::double2state(double* st)
{
    m = st[0];
}

#endif
