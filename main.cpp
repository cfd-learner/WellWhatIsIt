#include "Simulation.h"

int K = 9;
double C = 1.;

int main()
{
    int STEPS = 1000;
    double DX0 = 1.0;
    double DT0 = 1.0;
    double TAU = 0.6;
    double RHOi= 1.1;
    double RHOo= 1.0;

    Simulation S = Simulation("geo.csv", STEPS, DX0, DT0, TAU, RHOi, RHOo);

    return 0;
}