#include "Simulation.h"

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>

Simulation::Simulation(string geo, unsigned STEPS, double DX0, double DT0, double TAU, double RHOi, double RHOo):
    _STEPS(STEPS),
    _step(0),
    _DX0(DX0),
    _DT0(DT0),
    _TAU(TAU),
    _RHOi(RHOi),
    _RHOo(RHOo)
    {
        whatAreYouCasul(geo, true);
        becomeUnstoppable();
        theLegendNeverDies();
    }

void Simulation::whatAreYouCasul(string geometry_path, bool test) {

    //open geometry.txt
    cout<<"Opening geometry file: "<<geometry_path<<endl;
    ifstream Fin(geometry_path.c_str());

    Fin>>_I>>_J; //get I and J from first line
    _N = _I * _J;

    unsigned n(0), type, level, i, j;
    while (Fin>>type>>level>>i>>j) {
        _Blocks.push_back(new Block(n, type, level, i, j, _DX0, _DT0, _TAU, _RHOi, _RHOo));
        if (test) {
            cout<<endl<<"Block "<<n<<endl<<"Type "<<type<<endl<<"Level "<<level<<endl<<"I "<<i<<endl<<"J "<<j;
            cin.get();
        }
        if (level == 0) _BL0.push_back(_Blocks[n]);
        else if (level == 1) _BL1.push_back(_Blocks[n]);
        else if (level == 2) _BL2.push_back(_Blocks[n]);
        else if (level == 3) _BL3.push_back(_Blocks[n]);
        n++;
    }

    for (unsigned n=0; n<_N; n++) _Blocks[n]->initNeighbors(_Blocks, _I, _J);

    if (test) {
        cout<<endl<<_I<<" rows, "<<_J<<" columns, "<<_N<<" total blocks."<<endl;
        cin.get();
        cout<<"Block types:"<<endl;
        for (unsigned i=0; i<_I; i++)
        {
            for (unsigned j=0; j<_J; j++)
            {
                if (_Blocks[i*_J+j]->_TYPE < 10) cout<<" ";
                cout<<_Blocks[i*_J+j]->_TYPE<<" ";
            }
            cout<<endl;
        }
        cin.get();
        cout<<"Block levels:"<<endl;
        for (unsigned i=0; i<_I; i++)
        {
            for (unsigned j=0; j<_J; j++)
            {
                if (_Blocks[i*_J+j]->_L < 10) cout<<" ";
                cout<<_Blocks[i*_J+j]->_L<<" ";
            }
            cout<<endl;
        }
        cin.get();
        cout<<"Level 0:"; if (_BL0.size() == 0) cout<<" none"; cout<<endl;
        for (unsigned n=0; n<_BL0.size(); n++) cout<<_BL0[n]->_ID<<endl;
        cin.get();
        cout<<"Level 1:"; if (_BL1.size() == 0) cout<<" none"; cout<<endl;
        for (unsigned n=0; n<_BL1.size(); n++) cout<<_BL1[n]->_ID<<endl;
        cin.get();
        cout<<"Level 2:"; if (_BL2.size() == 0) cout<<" none"; cout<<endl;
        for (unsigned n=0; n<_BL2.size(); n++) cout<<_BL2[n]->_ID<<endl;
        cin.get();
        cout<<"Level 3:"; if (_BL3.size() == 0) cout<<" none"; cout<<endl;
        for (unsigned n=0; n<_BL3.size(); n++) cout<<_BL3[n]->_ID<<endl;
        cin.get();
    }
}

void Simulation::becomeUnstoppable() {
    while (_step < _STEPS) {
        giantsGiantsGiants(0);
        _step++;
        cout<<_step<<endl;
    }
}

void Simulation::giantsGiantsGiants(unsigned level) {

    switch (level) {
    case 0:
        giantsGiantsGiants(1);
        for (unsigned n0=0; n0<_BL0.size(); n0++) wellWhatIsIt(_BL0[n0]);
        break;
    case 1:
        giantsGiantsGiants(2);
        for (unsigned n1=0; n1<_BL1.size(); n1++) wellWhatIsIt(_BL1[n1]);
        giantsGiantsGiants(2);
        for (unsigned n1=0; n1<_BL1.size(); n1++) wellWhatIsIt(_BL1[n1]);
        break;
    case 2:
        giantsGiantsGiants(3);
        for (unsigned n2=0; n2<_BL2.size(); n2++) wellWhatIsIt(_BL2[n2]);
        giantsGiantsGiants(3);
        for (unsigned n2=0; n2<_BL2.size(); n2++) wellWhatIsIt(_BL2[n2]);
        break;
    case 3:
        for (unsigned n3=0; n3<_BL3.size(); n3++) wellWhatIsIt(_BL3[n3]);
        for (unsigned n3=0; n3<_BL3.size(); n3++) wellWhatIsIt(_BL3[n3]);
        break;
    }
}

void Simulation::wellWhatIsIt(Block* Blockjawn, bool test) {
    Blockjawn->allStream();
    Blockjawn->allSeams();
    Blockjawn->allBC();
    Blockjawn->allRho();
    Blockjawn->allU();
    Blockjawn->allV();
    Blockjawn->allFeq();

    if (test) {
        cout<<"Updated Block "<<Blockjawn->_ID;
        cin.get();
    }
}

void Simulation::theLegendNeverDies() {
    ofstream fout("output.txt");
    double y = 0.;
    for (unsigned n=_Blocks[1]->_J/2; n<_Blocks[1]->_N; n+=_Blocks[1]->_J) {fout<<y<<" "<<_Blocks[1]->_Nodes[n]->_u<<endl; y += _Blocks[1]->_DX;}
    for (unsigned n=_Blocks[4]->_J+_J/2; n<_Blocks[4]->_N; n+=_Blocks[4]->_J) {fout<<y<<" "<<_Blocks[4]->_Nodes[n]->_u<<endl; y += _Blocks[4]->_DX;}
    for (unsigned n=_Blocks[7]->_J+_J/2; n<_Blocks[7]->_N; n+=_Blocks[7]->_J) {fout<<y<<" "<<_Blocks[7]->_Nodes[n]->_u<<endl; y += _Blocks[7]->_DX;}
}