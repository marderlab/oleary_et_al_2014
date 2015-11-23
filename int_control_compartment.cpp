#include <cmath>
#include <vector>
#include "mex.h"
#include "compartment.h"
#include "NaV.h"
#include "CaT.h"
#include "CaS.h"
#include "Ka.h"
#include "KCa.h"
#include "Kdr.h"
#include "Ih.h"
#include "leak.h"
#include "int_controller.h"
#include "calciumsensor.h"

using namespace std;

// usage [output] = f([dt tstop res],[gbars]
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *output;
    double *simparams, *gmax, *regparams;
    int nits = 10, res = 1;
    double dt, tstop, taug, taum, targ;
    double gbar_leak, gbar_na, gbar_kd, gbar_ka, gbar_kca, gbar_cas, gbar_cat, gbar_h;
    double e_leak = -50.0, e_na = 50.0, e_k = -80.0, e_h = -20.0, v0 = -60.0;
    int state_dim = 0;
    double* state;
    
    //input + params
    simparams = mxGetPr(prhs[0]);
    dt = simparams[0];
    tstop = simparams[1];
    res = (int) simparams[2];
    v0 = simparams[3];
    
    gmax = mxGetPr(prhs[1]);
    gbar_na = gmax[0];
    gbar_cat = gmax[1];
    gbar_cas = gmax[2];
    gbar_ka = gmax[3];
    gbar_kca = gmax[4];
    gbar_kd = gmax[5];
    gbar_h = gmax[6];
    gbar_leak = gmax[7];
    e_leak = gmax[8];
    e_na = gmax[9];
    e_k = gmax[10];
    e_h = gmax[11];
    
    simparams = mxGetPr(prhs[2]);
    taug = simparams[0];
    taum = simparams[1];
    targ = simparams[2];
    
    //make conductances
    leak gleak(gbar_leak,e_leak); //change back to -50
    NaV gna(gbar_na,e_na);
    Kdr gkdr(gbar_kd,e_k);
    Ka gka(gbar_ka,e_k);
    KCa gkca(gbar_kca,e_k);
    CaS gcas(gbar_cas,120.0);
    CaT gcat(gbar_cat,120.0);
    Ih gh(gbar_h,e_h);
    
    //make compartment
    compartment cell(v0);
    cell.add_conductance(&gna);
    cell.add_conductance(&gcat);
    cell.add_conductance(&gcas);
    cell.add_conductance(&gka);
    cell.add_conductance(&gkca);
    cell.add_conductance(&gkdr);
    cell.add_conductance(&gh);
    cell.add_conductance(&gleak);
    
    //make sensor
    calciumsensor ca_sens;
    cell.add_sensor(&ca_sens);
    
    //make controller
    int_controller con_gna(targ,  simparams[3], taug, taum, gbar_na);
    int_controller con_gcat(targ, simparams[4], taug, taum, gbar_cat);
    int_controller con_gcas(targ, simparams[5], taug, taum, gbar_cas);
    int_controller con_gka(targ,  simparams[6], taug, taum, gbar_ka);
    int_controller con_gkca(targ, simparams[7], taug, taum, gbar_kca);
    int_controller con_gkdr(targ, simparams[8], taug, taum, gbar_kd);
    int_controller con_gh(targ,   simparams[9], taug, taum, gbar_h);
    
    cell.add_controller(&con_gna, &ca_sens, &gna);
    cell.add_controller(&con_gkdr, &ca_sens, &gkdr);
    cell.add_controller(&con_gka, &ca_sens, &gka);
    cell.add_controller(&con_gkca, &ca_sens, &gkca);
    cell.add_controller(&con_gcas, &ca_sens, &gcas);
    cell.add_controller(&con_gcat, &ca_sens, &gcat);
    cell.add_controller(&con_gh, &ca_sens, &gh);
    
    nits = (int) floor(tstop/dt);
    
    state_dim = cell.get_vcgs_dim();
    state = new double[state_dim];
    
    plhs[0] = mxCreateDoubleMatrix(state_dim, ((int)nits)/((int)res), mxREAL);
    output = mxGetPr(plhs[0]);
    
    //integration loop
    for(int I = 0, i=0; i<nits; i++)
    {
        (void) cell.integrate(dt);
        if (i%res == 0)
        {
            cell.get_vcgs(state);
            for(int j=0; j<state_dim; j++)
            {
                output[state_dim*I+j] = state[j];
            }
            I++;
        }
    }
}
