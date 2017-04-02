#include "Simulation.h"

unsigned K = 9;
double C = 1.;

int main()
{
    Simulation S = Simulation("geo.txt", 2000, 1., 1., 0.6, 1.1, 1.);
    return 0;
}