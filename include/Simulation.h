#ifndef SIMULATION_H
#define SIMULATION_H

#include <Block.h>

#include <string>

class Simulation
{
    public:
        Simulation(string geo, unsigned STEPS, double DX0, double DT0, double TAU, double RHOi, double RHOo);

    private:
        void whatAreYouCasul(string geometry_path, bool test=false);
        void becomeUnstoppable();
        void giantsGiantsGiants(unsigned level);
        void wellWhatIsIt(Block* Blockjawn, bool test=false);
        void theLegendNeverDies();

        const unsigned int _STEPS;
        unsigned int _step, _I, _J, _N;
        double _DX0, _DT0, _TAU, _RHOi, _RHOo;
        vector<Block*> _Blocks, _BL0, _BL1, _BL2, _BL3;
};

#endif // SIMULATION_H
