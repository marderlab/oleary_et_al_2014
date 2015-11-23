//class for storing state of a compartment
#define R_F 8.6174e-005 
#ifndef COMPSTATE
#define COMPSTATE
#include <cmath>

class compstate
{
protected:
    double v;
    double Ca;     //calcium concentration (uM)
    double e_leak;
    double e_K;
    double e_Na;
    double e_Ca;
    double e_h;
    double i_Ca; //Ca current (for liu sensor)
    double T; //temperature (deg C)
public:
    compstate() {}
    ~compstate() {}
    
    compstate(double v_in, double c_in)
    {
        v = v_in;
        Ca = c_in;
        //default reversal potentials
        e_leak = -60.0;
        e_K = -80.0;
        e_Na = 50.0;
        e_Ca = 120.0;
        T = 10.0;
    }
    
    void set_v(double);
    void set_Ca(double);
    void set_e_leak(double);
    void set_e_K(double);
    void set_e_Na(double);
    void set_e_Ca(double);
    void set_e_h(double);
    void set_i_Ca(double);

    double get_v(void);
    double get_Ca(void);
    double get_e_leak(void);
    double get_e_Na(void);
    double get_e_K(void);
    double get_e_Ca(void);    
    double get_e_h(void);    
    double get_i_Ca(void);
    
    double calc_e_Ca(void);
};

// setters
void compstate::set_v(double v_in)      {v = v_in;}
void compstate::set_Ca(double c_in)     {Ca = c_in;}
void compstate::set_e_leak(double e_in) {e_leak = e_in;}
void compstate::set_e_K(double e_in)    {e_K = e_in;}
void compstate::set_e_Na(double e_in)   {e_Na = e_in;}
void compstate::set_e_Ca(double e_in)   {e_Ca = e_in;}
void compstate::set_e_h(double e_in)    {e_h = e_in;}
void compstate::set_i_Ca(double iCa_in) {i_Ca = iCa_in;}

// getters
double compstate::get_v(void)       {return v;}
double compstate::get_Ca(void)      {return Ca;}
double compstate::get_e_leak(void)  {return e_leak;}
double compstate::get_e_K(void)     {return e_K;}
double compstate::get_e_Na(void)    {return e_Na;}
double compstate::get_e_Ca(void)    {return e_Ca;}
double compstate::get_e_h(void)     {return e_h;}
double compstate::get_i_Ca(void)    {return i_Ca;}

//set reversal according to Nernst equation
double compstate::calc_e_Ca(void)
{
    e_Ca = 500.0*R_F*(T + 273.15)*log(3000.0/Ca);
    return e_Ca;
}
#endif
