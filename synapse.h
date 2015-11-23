//Abstract class for defining synapses
#ifndef SYNAPSE
#define SYNAPSE
#include "compstate.h"
#include <cmath>

class synapse {
protected:
    double gbar;
    double g;
    double e_rev;
    int state_dim;
    compstate* post_comp_state;
    compstate* pre_comp_state;
public:
    synapse()
    {
        //null pointer for safety
        post_comp_state = 0;
        pre_comp_state = 0;
        state_dim = 0;
    }
    
    ~synapse() {}
    
    void connect(compstate*,compstate*);
    double post_vm(void); //return postsynaptic compartment voltage
    double pre_vm(void); //return presynaptic compartment voltage
    double get_gbar(void);
    double* get_pgbar(void);
    double get_g(void);
    double get_ge(void); // for Dayan-Abbot integration
    
    // all of these must be defined and will be unique
    // for each synapse in general
    virtual double i(void) =0; //consider removing this
    virtual int get_state_dim(void) =0;
    virtual void integrate(double) =0;
    virtual void state2double(double*) =0;
    virtual void double2state(double*) =0;
};

void synapse::connect(compstate* pre, compstate* post)
{
    pre_comp_state = pre;
    post_comp_state = post;
}

double synapse::post_vm(void)
{
    return post_comp_state->get_v();
}

double synapse::pre_vm(void)
{
    return pre_comp_state->get_v();
}

double synapse::get_gbar(void)
{
    return gbar;
}

double* synapse::get_pgbar(void)
{
    return &gbar;
}

double synapse::get_g(void)
{
    return g;
}

double synapse::get_ge(void)
{
    return g*e_rev;
}


#endif
