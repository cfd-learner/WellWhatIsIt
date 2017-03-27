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

Node::Node(unsigned int ID, unsigned int NSIDE, unsigned int BSIDE, double rho):
    _ID(ID),
    _NSIDE(NSIDE),
    _BSIDE(BSIDE),
    _rho(rho),
    _u(0),
    _v(0),
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

    for (unsigned int k=0; k<K; k++) {
        edotu = Ex[k]*_u + Ey[k]*_v; // dot product of e and velocity
        _feq[k] = W[k]*_rho*(1. + 3.*edotu/pow(C, 2.) + 4.5*pow(edotu, 2.)/pow(C, 4.) - 1.5*_v2/pow(C, 2.));
    }

    if (test) {
        cout<<_ID;
        for (unsigned int k=0; k<K; k++) cout<<" "<<fixed<<setprecision(4)<<_feq[k];
        cout<<endl;
    }
}

void Node::initF() {
    for (unsigned int k=0; k<K; k++) _f[k] = _feq[k];
}

void Node::stream(unsigned int k, double TAU) {
    if (_Neighbors[k]) _f[k] = _Neighbors[k]->_f[k] - (_Neighbors[k]->_f[k]-_Neighbors[k]->_feq[k])/TAU;
}

void Node::calcRho(bool test) {
    if (!_ISFIXEDRHO) {
        _rho = 0.;
        for (unsigned int k=0; k<K; k++) _rho += _f[k];
    }

    if (test) {
        cout<<_ID<<" "<<_rho<<endl;
    }
}

void Node::calcU(vector<double>& Ex, bool test) {
    if (!_ISFIXEDU) {
        _u = 0.;
        for (unsigned int k=0; k<K; k++) _u += _f[k]*Ex[k];
        _u /= _rho;
    }

    if (test) {
        cout<<_ID<<" "<<_u<<endl;
    }
}

void Node::calcV(vector<double>& Ey, bool test) {
    if (!_ISFIXEDV) {
        _v = 0.;
        for (unsigned int k=0; k<K; k++) _v += _f[k]*Ey[k];
        _v /= _rho;
    }

    if (test) {
        cout<<_ID<<" "<<_v<<endl;
    }
}