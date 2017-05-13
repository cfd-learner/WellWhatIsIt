#include "Simulation.h"

#include <iostream>
#include <fstream>
#include <sstream>

Simulation::Simulation(string geo, int STEPS, double DX0, double DT0, double TAU, double RHOi, double RHOo):
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

    cout<<"Opening geometry file: "<<geometry_path<<endl;
    ifstream Geo_in(geometry_path.c_str());
    string linejawn, typejawn, leveljawn, ijawn, jjawn;
    unsigned c = 0;

    //get I and J from first line
    getline(Geo_in, linejawn);
    while (linejawn[c] != ',') {ijawn += linejawn[c]; c++;} c++;
    stringstream(ijawn)>>_I;
    while (c < linejawn.size()) {jjawn += linejawn[c]; c++;}
    stringstream(jjawn)>>_J;
    _N = _I * _J;

    int n = 0, type, level, i, j;
    while (getline(Geo_in, linejawn)) {
        typejawn.clear(); leveljawn.clear(); ijawn.clear(); jjawn.clear();
        c = 0;
        while (linejawn[c] != ',') {typejawn += linejawn[c]; c++;} c++;
        stringstream(typejawn)>>type;
        while (linejawn[c] != ',') {leveljawn += linejawn[c]; c++;} c++;
        stringstream(leveljawn)>>level;
        while (linejawn[c] != ',') {ijawn += linejawn[c]; c++;} c++;
        stringstream(ijawn)>>i;
        while (c < linejawn.size()) {jjawn += linejawn[c]; c++;}
        stringstream(jjawn)>>j;

        _Blocks.push_back(new Block(n, type, level, i, j, _DX0, _DT0, _TAU, _RHOi, _RHOo));
        if (test) {
            cout<<endl<<"Block "<<n<<", Type "<<type<<", Level "<<level<<", I "<<i<<", J "<<j;
        }
        if (level == 0) _BL0.push_back(_Blocks[n]);
        else if (level == 1) _BL1.push_back(_Blocks[n]);
        else if (level == 2) _BL2.push_back(_Blocks[n]);
        else if (level == 3) _BL3.push_back(_Blocks[n]);
        n++;
    }

    for (int n=0; n<_N; n++) _Blocks[n]->initNeighbors(_Blocks, _I, _J);

    if (test) {
        cout<<endl<<_I<<" rows, "<<_J<<" columns, "<<_N<<" total blocks."<<endl;
        cin.get();
        cout<<"Block types:"<<endl;
        for (int i=0; i<_I; i++)
        {
            for (int j=0; j<_J; j++)
            {
                if (_Blocks[i*_J+j]->_TYPE < 10) cout<<" ";
                cout<<_Blocks[i*_J+j]->_TYPE<<" ";
            }
            cout<<endl;
        }
        cin.get();
        cout<<"Block levels:"<<endl;
        for (int i=0; i<_I; i++)
        {
            for (int j=0; j<_J; j++)
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

void Simulation::giantsGiantsGiants(int level) {

    switch (level) {
    case 0:
        giantsGiantsGiants(1);
        for (unsigned n0=0; n0<_BL0.size(); n0++) everythingYouNeed(_BL0[n0]);
        break;
    case 1:
        giantsGiantsGiants(2);
        for (unsigned n1=0; n1<_BL1.size(); n1++) everythingYouNeed(_BL1[n1]);
        giantsGiantsGiants(2);
        for (unsigned n1=0; n1<_BL1.size(); n1++) everythingYouNeed(_BL1[n1]);
        break;
    case 2:
        giantsGiantsGiants(3);
        for (unsigned n2=0; n2<_BL2.size(); n2++) everythingYouNeed(_BL2[n2]);
        giantsGiantsGiants(3);
        for (unsigned n2=0; n2<_BL2.size(); n2++) everythingYouNeed(_BL2[n2]);
        break;
    case 3:
        for (unsigned n3=0; n3<_BL3.size(); n3++) everythingYouNeed(_BL3[n3]);
        for (unsigned n3=0; n3<_BL3.size(); n3++) everythingYouNeed(_BL3[n3]);
        break;
    }
}

void Simulation::everythingYouNeed(Block* Blockjawn, bool test) {
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
    ofstream fout("output.csv");
    double y = 0.;
//    for (int n=_Blocks[1]->_J/2; n<_Blocks[1]->_N; n+=_Blocks[1]->_J) {fout<<y<<" "<<_Blocks[1]->getU(n)<<endl; y += _Blocks[1]->_DX;}
//    for (int n=_Blocks[4]->_J+_J/2; n<_Blocks[4]->_N; n+=_Blocks[4]->_J) {fout<<y<<" "<<_Blocks[4]->getU(n)<<endl; y += _Blocks[4]->_DX;}
//    for (int n=_Blocks[7]->_J+_J/2; n<_Blocks[7]->_N; n+=_Blocks[7]->_J) {fout<<y<<" "<<_Blocks[7]->getU(n)<<endl; y += _Blocks[7]->_DX;}
    for (int n=_Blocks[0]->_J/2; n<_Blocks[0]->_N; n+=_Blocks[0]->_J) {fout<<y<<","<<_Blocks[0]->getU(n)<<endl; y += _Blocks[0]->_DX;}
}