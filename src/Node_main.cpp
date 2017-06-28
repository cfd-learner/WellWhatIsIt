#include "Nodes.h"

#include <iostream>
#include <iomanip>
#include <cmath>

/*
    0       1         ...  J-1
    J       J+1       ...  2J-1
    .       .         ...  .
    .       .         ...  .
    (I-1)J  (I-1)J+1  ...  IJ-1
*/

Node::Node(int ID, int NSIDE, int BSIDE, double rho, double x, double y):
    _ID(ID),
    _NSIDE(NSIDE),
    _BSIDE(BSIDE),
    _rho(rho),
    _u(0),
    _v(0),
    _x(x),
    _y(y),
    _ISFIXEDRHO(false),
    _ISFIXEDU(false),
    _ISFIXEDV(false),
    _f(K,0.),
    _feq(K,0.),
    _Neighbors(K)
    {};

void Node::calcFeq(double C, vector<double>& W, vector<double>& Ex, vector<double>& Ey, bool test) {
    double edotu;
    double _v2 = _u*_u + _v*_v;

    for (int k=0; k<K; k++) {
        edotu = Ex[k]*_u + Ey[k]*_v; // dot product of e and velocity
        _feq[k] = W[k]*_rho*(1. + 3.*edotu/pow(C, 2.) + 4.5*pow(edotu, 2.)/pow(C, 4.) - 1.5*_v2/pow(C, 2.));
    }

    if (test) {
        cout<<_ID;
        for (int k=0; k<K; k++) cout<<" "<<fixed<<setprecision(4)<<_feq[k];
        cout<<endl;
    }
}

void Node::initF() {
    for (int k=0; k<K; k++) _f[k] = _feq[k];
}

void Node::stream(int k, double TAU) {
    if (_Neighbors[k]) _f[k] = _Neighbors[k]->_f[k] - (_Neighbors[k]->_f[k]-_Neighbors[k]->_feq[k])/TAU;
}

void Node::calcRho(bool test) {
    if (!_ISFIXEDRHO) {
        _rho = 0.;
        for (int k=0; k<K; k++) _rho += _f[k];
    }

    if (test) {
        cout<<_ID<<" "<<_rho<<endl;
    }
}

void Node::calcU(vector<double>& Ex, bool test) {
    if (!_ISFIXEDU) {
        _u = 0.;
        for (int k=0; k<K; k++) _u += _f[k]*Ex[k];
        _u /= _rho;
    }

    if (test) {
        cout<<_ID<<" "<<_u<<endl;
    }
}

void Node::calcV(vector<double>& Ey, bool test) {
    if (!_ISFIXEDV) {
        _v = 0.;
        for (int k=0; k<K; k++) _v += _f[k]*Ey[k];
        _v /= _rho;
    }

    if (test) {
        cout<<_ID<<" "<<_v<<endl;
    }
}

void Node::printInfo() {
    cout<<"Node "<<_ID<<endl;
    cout<<"  Address: "<<this<<endl;
    cout<<"   Coords: "<<_x<<", "<<_y<<endl;
    cout<<"  Neighbs: ";
    for (int k=0; k<K; k++) {
        if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
        else cout<<"_ ";
    } cout<<endl;
    cout<<"  rho    : "<<_rho<<endl;
    cout<<"  u      : "<<_u<<endl;
    cout<<"  v      : "<<_v<<endl;
    for (int k=0; k<K; k++) cout<<"  feq"<<k<<"   : "<<_feq[k]<<endl;
    for (int k=0; k<K; k++) cout<<"  f"<<k<<"    : "<<_f[k]<<endl;
}