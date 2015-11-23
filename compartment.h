//single compartment class
#ifndef COMPARTMENT
#define COMPARTMENT
#include <cmath>
#include <vector>
#include "conductance.h"
#include "compstate.h"
#include "sensor.h"
#include "controller.h"
#include "mex.h"
#include "synapse.h"

using namespace std;

/* single compartment class (unit capacitance)
 * contains vector of conductances
 * integrates membrane equation after
 * integrating each conductance state equation */
class compartment
{
protected:
    double cm; // capacitance (not used)
    vector<conductance*> cond; // all conductances in compartment
    vector<controller*> cont;
    vector<sensor*> sens;
    vector<synapse*> syn;
    
    // voltage and other state variables (calcium, ..
    compstate state;    //pointer passed to membrane mechanisms (channels, sensors,..)
    double sum_g;
    double sum_ge;
    double v_inf; //put in compstate
    double v;           //voltage
    double Ca;           //calcium
    double Ca_inf; //put in compstate
    double tau_Ca;
    double i_ext, g_ext, ge_ext;
public:
    // constructor
    // should change argument to (const double &v_init) 
    compartment(double v_init)
    {
        state = compstate(v_init,0.2);
        tau_Ca = 20.0;
        i_ext = 0.0;
        g_ext = 0.0;
        ge_ext = 0.0;
    }
    
    // constructor
    compartment(double v_init, double Ca_init)
    {
        state = compstate(v_init,Ca_init);
        tau_Ca = 20.0;
        i_ext = 0.0;
        g_ext = 0.0;
        ge_ext = 0.0;
    }
    
    //pointer to state
    compstate* get_pstate(void);
    // return v_mem
    double get_v(void);
    double get_Ca(void);
    // integrate one timestep and return v_mem
    double integrate(double);
    // insert conductance
    void add_conductance(conductance*);
    void add_controller(controller*, sensor*, conductance*);
    void add_sensor(sensor*);
    void add_synapse(synapse*,compstate*);
    int get_cond_state_dim(void);
    int get_cont_state_dim(void);
    int get_sens_state_dim(void);
    int get_state_dim(void);
    int get_vcgs_dim(void);
    void get_state(double*);
    void set_state(double*);
    void get_vcgs(double*);
    void get_vc(double*);
    void input(double);
    void g_input(double, double);
    double get_cm(void);
    void set_cm(double);
};

compstate* compartment::get_pstate(void)
{
    return &state;
}

double compartment::get_v(void)
{
    return v;
}

double compartment::get_Ca(void)
{
    return Ca;
}

double compartment::integrate(double dt)
{
    int n_cond = (int) cond.size(); //conductances
    int n_cont = (int) cont.size(); //controllers
    int n_sens = (int) sens.size(); //sensors
    int n_syn = (int) syn.size(); //synapses
    // todo: ejuncs
    
    v = state.get_v();
    Ca = state.get_Ca();
    
    sum_g = 0.0;
    sum_ge = 0.0;
    state.set_i_Ca(0.0); //set ica to zero before summing contributions
    (void) state.calc_e_Ca(); //new reversal potential
    
    // integrate all conductances TODO use an iterator for this
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrate(dt);
        sum_g += cond[i]->get_g();
        sum_ge += cond[i]->get_ge();
    }
    
    // integrate all conductances TODO use an iterator for this
    for (int i=0; i<n_syn; i++)
    {
        syn[i]->integrate(dt);
        sum_g += syn[i]->get_g();
        sum_ge += syn[i]->get_ge();
    }
    
    sum_ge += i_ext + ge_ext;
    sum_g += g_ext;
    v_inf = sum_ge/sum_g;
    
    // integrate membrane equation (this could be method of compstate)
    state.set_v(v_inf + (v - v_inf)*exp(-dt*sum_g));
    
    Ca_inf = 0.05 + 0.94*state.get_i_Ca();
    
    // integrate Ca dynamics (this could be method of compstate)
    state.set_Ca(Ca_inf + (Ca - Ca_inf)*exp(-dt/tau_Ca));
    
    for (int i=0; i<n_sens; i++)
    {
        sens[i]->integrate(dt);
    }
    
    for (int i=0; i<n_cont; i++)
    {
        cont[i]->integrate(dt);
    }
    
    //mexPrintf("Ca = %f\n", Ca);
    
    return state.get_v();
}

// add conductance and provide pointer to compartment state
void compartment::add_conductance(conductance *cond_in)
{
    cond.push_back(cond_in);
    cond_in->connect(&state);
}

void compartment::add_controller(controller* cont_in, sensor* sens_in, conductance* cond_in)
{
    cont.push_back(cont_in);
    cont_in->connect(sens_in, cond_in);
}

void compartment::add_sensor(sensor* sens_in)
{
    sens.push_back(sens_in);
    sens_in->connect(&state);
}

void compartment::add_synapse(synapse* syn_in, compstate* pre_comp)
{
    syn.push_back(syn_in);
    syn_in->connect(pre_comp,&state);
}

int compartment::get_cond_state_dim(void)
{
    int n_cond = (int) cond.size();
    int st_dim = 0;
    
    for (int i=0; i<n_cond; i++)     // count states
    {
        st_dim += cond[i]->get_state_dim();   
    }
    
    return st_dim;
}

int compartment::get_cont_state_dim(void)
{
    int n_cont = (int) cont.size();
    int st_dim = 0;
    
    for (int i=0; i<n_cont; i++)     // count states
    {
        st_dim += cont[i]->get_state_dim();   
    }
    
    return st_dim;
}

int compartment::get_sens_state_dim(void)
{
    int n_sens = (int) sens.size();
    int st_dim = 0;
    
    for (int i=0; i<n_sens; i++)     // count states
    {
        st_dim += sens[i]->get_state_dim();   
    }
    
    return st_dim;
}

int compartment::get_state_dim(void)
{
    return 2 + get_cond_state_dim() + get_cont_state_dim() + get_sens_state_dim();
}

int compartment::get_vcgs_dim(void)
{
    int n_cond = (int) cond.size();
    int n_sens = (int) sens.size();
    int ind = 0;
    
    for (int i=0; i<n_sens; i++)
    {
        ind += sens[i]->get_value_dim();
    }
    
    return 2 + n_cond + ind;
}

// get voltage, [Ca], gbars, sensor vals
void compartment::get_vcgs(double* out)
{
    int n_cond = (int) cond.size();
    int n_sens = (int) sens.size();
    
    int ind = 2;
    
    out[0] = state.get_v();
    out[1] = state.get_Ca();
    
    for (int i=0; i<n_cond; i++)
    {
        out[ind++] = cond[i]->get_gbar();
    }
    
    for (int i=0; i<n_sens; i++)
    {
        sens[i]->get_value(&out[ind]);
        ind += sens[i]->get_value_dim();
    }
}

// get voltage, [Ca], gbars, sensor vals
void compartment::get_vc(double* out)
{   
    out[0] = state.get_v();
    out[1] = state.get_Ca();
}

void compartment::get_state(double* out)
{
    int n_cond = (int) cond.size();
    int n_cont = (int) cont.size();
    int n_sens = (int) sens.size();
    
    int ind = 2;
    
    out[0] = state.get_v();
    out[1] = state.get_Ca();
    
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->state2double(&out[ind]);
        ind += cond[i]->get_state_dim();
    }
    
    for (int i=0; i<n_cont; i++)
    {
        cont[i]->state2double(&out[ind]);
        ind += cont[i]->get_state_dim();
    }
    
    for (int i=0; i<n_sens; i++)
    {
        sens[i]->state2double(&out[ind]);
        ind += sens[i]->get_state_dim();
    }
}

void compartment::set_state(double* st_in)
{
    int n_cond = (int) cond.size();
    int n_cont = (int) cont.size();
    int n_sens = (int) sens.size();
    int ind = 2;
    
    state.set_v(st_in[0]);
    state.set_Ca(st_in[1]);
    
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->double2state(&st_in[ind]);
        ind += cond[i]->get_state_dim();
    }
    
    for (int i=0; i<n_cont; i++)
    {
        cont[i]->double2state(&st_in[ind]);
        ind += cont[i]->get_state_dim();
    }
    
    for (int i=0; i<n_sens; i++)
    {
        sens[i]->double2state(&st_in[ind]);
        ind += sens[i]->get_state_dim();
    }    
}

void compartment::input(double i_inj)
{
    i_ext = i_inj;
    //mexPrintf("received inj = %f\n", i_ext);
}

void compartment::g_input(double g_bar, double e_rev)
{
    g_ext = g_bar;
    ge_ext = g_bar*e_rev;
    //mexPrintf("received inj = %f\n", i_ext);
}

double compartment::get_cm(void)
{
    return cm;
}

void compartment::set_cm(double cm_in)
{
    cm = cm_in;
}

#endif

//mexPrintf("integrating, tau = %e\n", tau_v);

