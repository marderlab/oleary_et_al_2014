// Electrical junction (e.g. gap junction, inter-compartment link..)
#ifndef EJUNC
#define EJUNC

class ejunc {
protected:
    double g;
    compstate *post_comp_state;
    compstate *pre_comp_state;
public:
    //constructors - must set gbar!    
    // gbar only
    ejunc(double g_in)//: conductance(g_in)
    {
        g = g_in;
    }
    
    double i(void);
    double get_ge(void); // for Dayan-Abbot integration
    int get_state_dim(void);
    void integrate(double);
    void state2double(double*);
    void double2state(double*);
};

double ejunc::i(void)
{
    return gbar*(pre_comp_state->get_v() - pos_comp_state->get_v());
}

double ejunc:get_ge(void)
{
    return gbar*(pre_comp_state->get_v());
}

int ejunc::get_state_dim(void)
{
    return 0;
}

void ejunc::integrate(double dt)
{
    g = gbar;
}

void ejunc::state2double(double* st) { }

void ejunc::double2state(double*) { }

#endif
