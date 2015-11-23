//just run to get membrane trace etc, no regulation
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

using namespace std;

// simple max function
int max(int x, int y)
{
  return (x > y) ? x: y;
}

// usage [output] = f([dt tstop res],[gbars]
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *output;
    double *simparams, *gmax;
    double *inj_times, *inj_values;
    int nits = 10, res = 1;
    double dt, tstop;
    double gbar_leak, gbar_na, gbar_kd, gbar_ka, gbar_kca, gbar_cas, gbar_cat, gbar_h, e_leak;
    int state_dim = 0, full_state_dim = 0;
    double *state, *full_state;
    int trows, tcols, vrows, vcols, nsteps;
    int inj_step = 0;
    
    //input + params
    if(!(nrhs==4)) {
        mexErrMsgTxt("Wrong number of inputs");
    }
    // 1st input - simulation parameters
    simparams = mxGetPr(prhs[0]);
    dt = simparams[0];
    tstop = simparams[1];
    res = simparams[2];

    // 2nd input - conductance parameters
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
    
    // 3rd & 4th inputs - current injection
    // array of times and values
    inj_times = mxGetPr(prhs[2]);
    inj_values = mxGetPr(prhs[3]);
    
    trows = mxGetM(prhs[2]);
    tcols = mxGetN(prhs[2]);
    vrows = mxGetM(prhs[3]);
    vcols = mxGetN(prhs[3]);
    
    if( !((trows==1 || tcols==1) && (vrows==1 || vcols==1)) ||
        !(max(trows,tcols)==max(vrows,vcols)) ) {
        mexErrMsgTxt("Injection arrays must be 1xN or Nx1 and equal in length");
    }
    
    nsteps = max(trows,tcols);
    //mexPrintf("dim = %d\n", nsteps);
    
    //make conductances
    leak gleak(gbar_leak,e_leak);
    NaV gna(gbar_na,50);
    Kdr gkdr(gbar_kd,-80.0);
    Ka gka(gbar_ka,-80.0);
    KCa gkca(gbar_kca,-80.0);
    CaS gcas(gbar_cas,120.0);
    CaT gcat(gbar_cat,120.0);
    Ih gh(gbar_h,-20.0);
    
    //make compartment
    compartment cell(-50);
    cell.add_conductance(&gna);
    cell.add_conductance(&gcat);
    cell.add_conductance(&gcas);
    cell.add_conductance(&gka);
    cell.add_conductance(&gkca);
    cell.add_conductance(&gkdr);
    cell.add_conductance(&gh);
    cell.add_conductance(&gleak);
    
    nits = (int) floor(tstop/dt);
    
    state_dim = cell.get_vcgs_dim();
    state = new double[state_dim];
    //full_state_dim = cell.get_state_dim();
    //full_state = new double[full_state_dim];
    
    plhs[0] = mxCreateDoubleMatrix(state_dim, ((int)nits)/((int)res), mxREAL);
    //plhs[0] = mxCreateDoubleMatrix(state_dim, nits, mxREAL);
    output = mxGetPr(plhs[0]);
    
    if (inj_times[0] < dt)
    {
        cell.input(inj_values[0]);
        inj_step = 1;
    }
    
    //integration loop
    for(int I = 0, i=0; i<nits; i++)
    //for(int i=0; i<nits; i++)
    {
        (void) cell.integrate(dt);
        if (i%res == 0)
        {
            cell.get_vcgs(state);
            //cell.get_state(full_state);
            for(int j=0; j<state_dim; j++)
            {
                output[state_dim*I+j] = state[j];
            }
            //output[(state_dim+1)*I+state_dim] = full_state[full_state_dim - 1];
            
            if ((inj_step < nsteps) && (((double) i)*dt > inj_times[inj_step]))
            {
                cell.input(inj_values[inj_step]);
                //mexPrintf("dim = %f\n", inj_values[inj_step]);
                inj_step++;
            }
            
            I++;
        }
    }

}
