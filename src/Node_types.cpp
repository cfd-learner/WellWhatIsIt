#include "Nodes.h"

#include <iostream>

/*
    0       1         ...  J-1
    J       J+1       ...  2J-1
    .       .         ...  .
    .       .         ...  .
    (I-1)J  (I-1)J+1  ...  IJ-1
*/

void NodeInternal::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    _Neighbors[1] = Nodes[_ID-1];
    _Neighbors[2] = Nodes[_ID+J];
    _Neighbors[3] = Nodes[_ID+1];
    _Neighbors[4] = Nodes[_ID-J];
    _Neighbors[5] = Nodes[_ID+J-1];
    _Neighbors[6] = Nodes[_ID+J+1];
    _Neighbors[7] = Nodes[_ID-J+1];
    _Neighbors[8] = Nodes[_ID-J-1];

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodeWall::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 1:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[5] = Nodes[_ID+J-1];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    case 2:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[5] = Nodes[_ID+J-1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 3:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[6] = Nodes[_ID+J+1];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 4:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodeWall::boundaryConditions(bool test){

    switch (_BSIDE) {
    case 1:
        _f[6] = _f[8] + 0.5*(_f[4]-_f[2]);
        _f[7] = _f[5] + 0.5*(_f[2]-_f[4]);
        _f[3] = _f[1];
        break;
    case 2:
        _f[7] = _f[5] + 0.5*(_f[1]-_f[3]);
        _f[8] = _f[6] + 0.5*(_f[3]-_f[1]);
        _f[4] = _f[2];
        break;
    case 3:
        _f[5] = _f[7] + 0.5*(_f[4]-_f[2]);
        _f[8] = _f[6] + 0.5*(_f[2]-_f[4]);
        _f[1] = _f[3];
        break;
    case 4:
        _f[6] = _f[8] + 0.5*(_f[1]-_f[3]);
        _f[5] = _f[7] + 0.5*(_f[3]-_f[1]);
        _f[2] = _f[4];
        break;
    }
}

void NodePressureEdge::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 1:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[5] = Nodes[_ID+J-1];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    case 3:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[6] = Nodes[_ID+J+1];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodePressureEdge::boundaryConditions(bool test){

    switch (_BSIDE) {
    case 1:
        _u = C*((_f[0]+_f[2]+_f[4]+2.*(_f[1]+_f[5]+_f[8]))/_rho-1.);
        _f[3] = _f[1] - 2./3.*_rho*_u/C;
        _f[6] = _f[8] + 0.5*(_f[4]-_f[2]) - 1./6.*_rho*_u/C;
        _f[7] = _f[5] + 0.5*(_f[2]-_f[4]) - 1./6.*_rho*_u/C;
        break;
    case 3:
        _f[5] = (_rho - _f[0] - 4.*_f[2] - 2.*_f[3] + 2.*_f[4] - 2.*_f[6] + 4.*_f[7])/6.;
        _f[8] = _f[2] + _f[5] + _f[6] - _f[4] - _f[7];
        _f[1] = 4.*_f[5] + 2.*_f[2] - 2.*_f[4] - 4.*_f[7] + _f[3];
        break;
    }
}

void NodePressureCorner::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 5:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[5] = Nodes[_ID+J-1];
        break;
    case 6:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 7:
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 8:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodePressureCorner::boundaryConditions(bool test){

    switch (_BSIDE) {
    case 5:
        _f[3] = _f[1];
        _f[4] = _f[2];
        _f[7] = _f[5];
        _f[6] = (_rho-_f[0]-_f[1]-_f[2]-_f[3]-_f[4]-_f[5]-_f[7])/2.;
        _f[8] = _f[6];
        break;
    case 6:
        _f[1] = _f[3];
        _f[4] = _f[2];
        _f[8] = _f[6];
        _f[5] = (_rho-_f[0]-_f[1]-_f[2]-_f[3]-_f[4]-_f[6]-_f[8])/2.;
        _f[7] = _f[5];
        break;
    case 7:
        _f[1] = _f[3];
        _f[2] = _f[4];
        _f[5] = _f[7];
        _f[8] = (_rho-_f[0]-_f[1]-_f[2]-_f[3]-_f[4]-_f[5]-_f[7])/2.;
        _f[6] = _f[8];
        break;
    case 8:
        _f[3] = _f[1];
        _f[2] = _f[4];
        _f[6] = _f[8];
        _f[7] = (_rho-_f[0]-_f[1]-_f[2]-_f[3]-_f[4]-_f[6]-_f[8])/2.;
        _f[5] = _f[7];
        break;
    }
}

void NodeInternalSeam::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 1:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[5] = Nodes[_ID+J-1];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    case 2:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[5] = Nodes[_ID+J-1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 3:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[6] = Nodes[_ID+J+1];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 4:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    case 5:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[5] = Nodes[_ID+J-1];
        break;
    case 6:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 7:
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 8:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodeWallSeam::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 5:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[5] = Nodes[_ID+J-1];
        break;
    case 6:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 7:
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 8:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodeWallSeam::boundaryConditions(bool test){

    switch (_BSIDE) {
    case 1:
        _f[6] = _f[8] + 0.5*(_f[4]-_f[2]);
        _f[7] = _f[5] + 0.5*(_f[2]-_f[4]);
        _f[3] = _f[1];
        break;
    case 2:
        _f[7] = _f[5] + 0.5*(_f[1]-_f[3]);
        _f[8] = _f[6] + 0.5*(_f[3]-_f[1]);
        _f[4] = _f[2];
        break;
    case 3:
        _f[5] = _f[7] + 0.5*(_f[4]-_f[2]);
        _f[8] = _f[6] + 0.5*(_f[2]-_f[4]);
        _f[1] = _f[3];
        break;
    case 4:
        _f[6] = _f[8] + 0.5*(_f[1]-_f[3]);
        _f[5] = _f[7] + 0.5*(_f[3]-_f[1]);
        _f[2] = _f[4];
        break;
    }
}

void NodePressureSeam::initNeighbors(vector<Node*>& Nodes, int J, bool test) {

    _Neighbors[0] = Nodes[_ID];
    switch (_NSIDE) {
    case 5:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[5] = Nodes[_ID+J-1];
        break;
    case 6:
        _Neighbors[2] = Nodes[_ID+J];
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[6] = Nodes[_ID+J+1];
        break;
    case 7:
        _Neighbors[3] = Nodes[_ID+1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[7] = Nodes[_ID-J+1];
        break;
    case 8:
        _Neighbors[1] = Nodes[_ID-1];
        _Neighbors[4] = Nodes[_ID-J];
        _Neighbors[8] = Nodes[_ID-J-1];
        break;
    }

    if (test) {
        for (int k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void NodePressureSeam::boundaryConditions(bool test){

    switch (_BSIDE) {
    case 1:
        _u = C*((_f[0]+_f[2]+_f[4]+2.*(_f[1]+_f[5]+_f[8]))/_rho-1.);
        _f[3] = _f[1] - 2./3.*_rho*_u/C;
        _f[6] = _f[8] + 0.5*(_f[4]-_f[2]) - 1./6.*_rho*_u/C;
        _f[7] = _f[5] + 0.5*(_f[2]-_f[4]) - 1./6.*_rho*_u/C;
        break;
    case 3:
        _f[5] = (_rho - _f[0] - 4.*_f[2] - 2.*_f[3] + 2.*_f[4] - 2.*_f[6] + 4.*_f[7])/6.;
        _f[8] = _f[2] + _f[5] + _f[6] - _f[4] - _f[7];
        _f[1] = 4.*_f[5] + 2.*_f[2] - 2.*_f[4] - 4.*_f[7] + _f[3];
        break;
    }
}