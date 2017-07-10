#include "Simulation.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

Simulation::Simulation(string geo, int STEPS, int SNAPSHOT, double DX0, double DT0, double TAU, double RHOi, double RHOo):
    _STEPS(STEPS),
    _SNAPSHOT(SNAPSHOT),
    _step(0),
    _DX0(DX0),
    _DT0(DT0),
    _TAU(TAU),
    _RHOi(RHOi),
    _RHOo(RHOo),
    _Uall(2, vector<double>(0, 0)),
    _Vall(2, vector<double>(0, 0))
    {
        whatAreYouCasul(geo, true);
        becomeUnstoppable();
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

    vector<int> type(_N), level(_N), i(_N), j(_N);
    int n = 0;
    while (getline(Geo_in, linejawn)) {
        typejawn.clear(); leveljawn.clear(); ijawn.clear(); jjawn.clear();
        c = 0;

        while (linejawn[c] != ',') {typejawn += linejawn[c]; c++;} c++;
        stringstream(typejawn)>>type[n];
        while (linejawn[c] != ',') {leveljawn += linejawn[c]; c++;} c++;
        stringstream(leveljawn)>>level[n];
        while (linejawn[c] != ',') {ijawn += linejawn[c]; c++;} c++;
        stringstream(ijawn)>>i[n];
        while (c < linejawn.size()) {jjawn += linejawn[c]; c++;}
        stringstream(jjawn)>>j[n];
        n++;
    }

    //find width and height of each block in lattice units
    vector<double> width(_N), height(_N);
    double totalwidth = 0;
    for (int n=0; n<_N; n++) {
        width[n] = (j[n]-1) * _DX0 / pow(2, level[n]);
        height[n] = (i[n]-1) * _DX0 / pow(2, level[n]);
        if (n < _J) totalwidth += width[n];
    }

    //use width, height, and totalwidth to determine X0 and Y0 (X-Y coordinates of Node 0 in block)
    vector<double> x0(_N), y0(_N);
    double x = 0, y = 0;
    for (int n=_N-1; n>=0; n--) {
        if ((n+1)%_J == 0) {y += height[n]; x = totalwidth;}
        x -= width[n];
        x0[n] = x; y0[n] = y;
    }

    for (int n=0; n<_N; n++) {

        _Blocks.push_back(new Block(n, type[n], level[n], i[n], j[n], x0[n], y0[n], _DX0, _DT0, _TAU, _RHOi, _RHOo));
        if (test) {
            cout<<endl<<"Block "<<n<<", Type "<<type[n]<<", Level "<<level[n]<<", I "<<i[n]<<", J "<<j[n]<<", X0 "<<x0[n]<<", Y0 "<<y0[n];
        }
        if (level[n] == 0) _BL0.push_back(_Blocks[n]);
        else if (level[n] == 1) _BL1.push_back(_Blocks[n]);
        else if (level[n] == 2) _BL2.push_back(_Blocks[n]);
        else if (level[n] == 3) _BL3.push_back(_Blocks[n]);
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
        cout<<"Level 0:"<<endl; if (_BL0.size() == 0) cout<<"none"<<endl;
        for (unsigned n=0; n<_BL0.size(); n++) cout<<_BL0[n]->_ID<<endl;
        cin.get();
        cout<<"Level 1:"<<endl; if (_BL1.size() == 0) cout<<"none"<<endl;
        for (unsigned n=0; n<_BL1.size(); n++) cout<<_BL1[n]->_ID<<endl;
        cin.get();
        cout<<"Level 2:"<<endl; if (_BL2.size() == 0) cout<<"none"<<endl;
        for (unsigned n=0; n<_BL2.size(); n++) cout<<_BL2[n]->_ID<<endl;
        cin.get();
        cout<<"Level 3:"<<endl; if (_BL3.size() == 0) cout<<"none";
        for (unsigned n=0; n<_BL3.size(); n++) cout<<_BL3[n]->_ID<<endl;
        cin.get();
        cout<<endl<<"Neighbors:"<<endl;
        for (unsigned n=0; n<_Blocks.size(); n++) {
            for (int k=0; k<K; k++) {
                if (_Blocks[n]->getNeighbor(k)) cout<<_Blocks[n]->getNeighbor(k)->_ID<<" ";
                else cout<<"_ ";
            }
            cout<<endl;
        }
        cin.get();
    }
}

void Simulation::becomeUnstoppable() {
    while (_step < _STEPS) {
        if (_step % _SNAPSHOT == 0) theLegendNeverDies();
        giantsGiantsGiants(0);
        _step++;
        cout<<_step<<endl;
    }
    theLegendNeverDies();
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
    ostringstream fileNameStream;
    string fileNameString;
    ofstream Rhoout, Uout, Vout, Udiffout("output/Udiff.csv", ios::app), Vdiffout("output/Vdiff.csv", ios::app);

    fileNameStream<<"output/rho/"<<_step<<".csv";
    fileNameString = fileNameStream.str();
    fileNameStream.str("");
    Rhoout.open(fileNameString.c_str());

    fileNameStream<<"output/u/"<<_step<<".csv";
    fileNameString = fileNameStream.str();
    fileNameStream.str("");
    Uout.open(fileNameString.c_str());

    fileNameStream<<"output/v/"<<_step<<".csv";
    fileNameString = fileNameStream.str();
    fileNameStream.str("");
    Vout.open(fileNameString.c_str());

    int n = 0; double udiff = 0, vdiff = 0;
    for (int nb=0; nb<_N; nb++) {
        for (int nn=0; nn<_Blocks[nb]->_N; nn++) {
            Rhoout<<_Blocks[nb]->getX(nn)<<","<<_Blocks[nb]->getY(nn)<<","<<_Blocks[nb]->getRho(nn)<<endl;
            Uout<<_Blocks[nb]->getX(nn)<<","<<_Blocks[nb]->getY(nn)<<","<<_Blocks[nb]->getU(nn)<<endl;
            Vout<<_Blocks[nb]->getX(nn)<<","<<_Blocks[nb]->getY(nn)<<","<<_Blocks[nb]->getV(nn)<<endl;

            if (_step == 0) {
                _Uall[0].push_back(0.);
                _Vall[0].push_back(0.);
                _Uall[1].push_back(_Blocks[nb]->getU(nn));
                _Vall[1].push_back(_Blocks[nb]->getV(nn));
            } else {
                _Uall[0][n] = _Uall[1][n];
                _Vall[0][n] = _Vall[1][n];
                _Uall[1][n] = _Blocks[nb]->getU(nn);
                _Vall[1][n] = _Blocks[nb]->getV(nn);
                udiff += abs(_Uall[1][n] - _Uall[0][n]);
                vdiff += abs(_Vall[1][n] - _Vall[0][n]);
            }
            n++;
        }
    }
    if (_step > 0) {Udiffout<<_step<<","<<udiff<<endl; Vdiffout<<_step<<","<<vdiff<<endl;}

    Rhoout.close(); Uout.close(); Vout.close();
}