#include "grid/cartesian1D.h"
#include "run.h"
#include <string.h>

scalar n[];
scalar dnx[];
scalar dux[];
scalar dnx2[];
scalar dux2[];
scalar u[];
double dt;

int main(){
    // define grid size and time step
    L0 = 10;
    X0 = 0;
    N = 100;
    DT = (L0/N)*(L0/N)/2*(L0/N)*(L0/N)/2;
    // define boundary conditions
    n[left] = neumann(0);
    n[right] = dirichlet(0.5);
    u[left] = dirichlet(0);
    u[right] = dirichlet(0.5);
    // define parameters
#define tau 0.1
#define tau_1 1e-2
#define a 0.01
#define b 1e-3
    run();
}

event init (t = 0){
    // define initial conditions
    foreach()
        n[] = 0.5+0.2*(10-x);
    boundary({n});

    foreach()
        u[] = 0.1*exp(0.1*x);
    boundary({u});
}

event integration (i++){
    double dt = DT;
    dt = dtnext(dt);
    
    // first order approximation for n_x
    foreach()
        //dnx[] = (n[1,0]-n[0,0])/Delta;
        //dnx[] = (n[0,0]-n[-1,0])/Delta;
        dnx[] = (n[1,0]-n[-1,0])/(2.*Delta);
    
    // first order approximation for u_x
    foreach()
        //dux[] = (u[1,0]-u[0,0])/Delta;
        //dux[]=(u[0,0]-u[-1,0])/Delta;
        dux[] = (u[1,0]-u[-1,0])/(2.*Delta);    

    foreach()
        dux2[] = (u[-1,0] + 2*u[0,0] + u[1,0])/Delta/Delta;
    
    // updates n
    foreach()
        n[] += dt*(dnx2[] + a*dux[] + (tau-1)*n[]);
    boundary({n});
    
    // updates u 
    foreach()
        u[] = b*dux2[] + tau_1*dnx[];
    boundary({u});
}

event graphs(i += 1000){
    char s[40];
    sprintf(s, "%d", i/1000);
    char const *file_name = strcat(s, ".dat");

    FILE *file=fopen(file_name, "w");

    foreach()
        fprintf(file, "%g %g %g %g\n", t, x, n[], u[]);

    fclose(file);
}

/*event debug(i=0;i<500;i+=100){
    char d[40];
    sprintf(d, "%d", i);
    char const *debug = strcat(d, ".txt");

    FILE *debug_log=fopen(debug, "w");

    foreach()
        fprintf(debug_log, "%g %g %g %g\n", t, x, n[], u[]);

    fclose(debug_log);
}*/

event end(t=8){}
