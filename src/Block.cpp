#include "Block.h"

#include <iostream>
#include <cmath>

/*
    0       1         ...  J-1
    J       J+1       ...  2J-1
    .       .         ...  .
    .       .         ...  .
    (I-1)J  (I-1)J+1  ...  IJ-1
*/

Block::Block(unsigned ID, unsigned TYPE, unsigned L, unsigned I, unsigned J, double DX0, double DT0, double TAU, double RHOi, double RHOo):
    _ID(ID),
    _TYPE(TYPE),
    _L(L),
    _I(I),
    _J(J),
    _N(I*J),
    _DX(DX0/pow(2,L)),
    _DT(DT0/pow(2,L)),
    _C(DX0/DT0),
    _TAU(TAU),
    _Ex(K,0),
    _Ey(K,0),
    _W(K,0),
    _Neighbors(K)
    {
        initW();
        initE();
        initNodes(TYPE, RHOi, RHOo);
    }

void Block::initNeighbors(vector<Block*>& Blocks, unsigned I, unsigned J, bool test) {

    _Neighbors[0] = Blocks[_ID];
    if (_ID%J != 0) _Neighbors[1] = Blocks[_ID-1];
    if (_ID < (I-1)*J) _Neighbors[2] = Blocks[_ID+J];
    if ((_ID+1)%J != 0) _Neighbors[3] = Blocks[_ID+1];
    if (_ID >= J) _Neighbors[4] = Blocks[_ID-J];
    if (_Neighbors[1] && _Neighbors[2]) _Neighbors[5] = Blocks[_ID+J-1];
    if (_Neighbors[2] && _Neighbors[3]) _Neighbors[6] = Blocks[_ID+J+1];
    if (_Neighbors[3] && _Neighbors[4]) _Neighbors[7] = Blocks[_ID-J+1];
    if (_Neighbors[4] && _Neighbors[1]) _Neighbors[8] = Blocks[_ID-J-1];

    if (test) {
        for (unsigned k=0; k<K; k++) {
            if (_Neighbors[k] && _Neighbors[k]->_ID < 10) cout<<" "<<_Neighbors[k]->_ID<<" ";
            else if (_Neighbors[k]) cout<<_Neighbors[k]->_ID<<" ";
            else cout<<" _ ";
        }
        cout<<endl;
    }
}

void Block::initW() {
    _W[0] = 4/9.;
    for (int n=1; n<=4; n++) _W[n] = 1/9.;
    for (int n=5; n<=8; n++) _W[n] = 1/36.;
}

void Block::initE() {
    _Ex[1] = _C;
    _Ex[3] = -_C;
    _Ex[5] = _C;
    _Ex[6] = -_C;
    _Ex[7] = -_C;
    _Ex[8] = _C;

    _Ey[2] = _C;
    _Ey[4] = -_C;
    _Ey[5] = _C;
    _Ey[6] = _C;
    _Ey[7] = -_C;
    _Ey[8] = -_C;
}

void Block::initNodes(unsigned TYPE, double RHOi, double RHOo, bool test) {

    unsigned n(0);
    switch (TYPE) {
    case 0: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 6));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 7));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 2: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, RHOo, 6, 2));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 5, 2));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 7));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 4: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 6));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, RHOo, 7, 4));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 8, 4));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 11: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 6));
            else if (n == _J-1) _Nodes.push_back(new NodePressureSeam(n, RHOo, 5, 1));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 7));
            else if (n == _N-1) _Nodes.push_back(new NodePressureSeam(n, RHOo, 8, 1));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 13: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureSeam(n, RHOi, 6, 3));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureSeam(n, RHOi, 7, 3));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOi, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 15: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, RHOo, 6, 2));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 7));
            else if (n == _N-1) _Nodes.push_back(new NodePressureSeam(n, RHOo, 8, 1));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 16: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, RHOi, 6));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 5, 2));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureSeam(n, RHOi, 7, 3));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOi, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 17: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureSeam(n, RHOi, 6, 3));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, RHOi, 7));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 8, 4));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOi, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 18: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 6));
            else if (n == _J-1) _Nodes.push_back(new NodePressureSeam(n, RHOo, 5, 1));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, RHOo, 7, 4));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 24: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, RHOo, 6, 2));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 5, 2));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, RHOo, 7, 4));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 8, 4));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 111: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, RHOo, 6, 2));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, RHOo, 7, 4));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 333: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, RHOi, 6));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 5, 2));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, RHOi, 7));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, RHOo, 8, 4));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOi, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    case 999: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, RHOi, 6));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 5));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, RHOi, 7));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, RHOo, 8));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, RHOo, 2));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOi, 3));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, RHOo, 1));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    }

    for (unsigned n=0; n<_N; n++) {
        if (n%_J == 0) _Edge1.push_back(_Nodes[n]);
        if (n >= (_I-1)*_J) _Edge2.push_back(_Nodes[n]);
        if ((n+1)%_J == 0) _Edge3.push_back(_Nodes[n]);
        if (n < _J) _Edge4.push_back(_Nodes[n]);
    }

    for (unsigned n=0; n<_N; n++) {
        _Nodes[n]->initNeighbors(_Nodes, _J);
        _Nodes[n]->calcFeq(_C, _W, _Ex, _Ey);
        _Nodes[n]->initF();
    }

    if (test) {
        for (unsigned n=0; n<_Nodes.size(); n++) {
            cout<<"Node "<<_Nodes[n]->_ID<<endl;
            cout<<"  Address: "<<_Nodes[n]<<endl;
            cout<<"  Neighbs: ";
            for (int k=0; k<K; k++) {
                if (_Nodes[n]->_Neighbors[k]) cout<<_Nodes[n]->_Neighbors[k]->_ID<<" ";
                else cout<<"_ ";
            } cout<<endl;
            cout<<"  rho    : "<<_Nodes[n]->_rho<<endl;
            cout<<"  u      : "<<_Nodes[n]->_u<<endl;
            cout<<"  v      : "<<_Nodes[n]->_v<<endl;
            for (int k=0; k<K; k++) cout<<"  feq"<<k<<"   : "<<_Nodes[n]->_feq[k]<<endl;
            for (int k=0; k<K; k++) cout<<"  f"<<k<<"    : "<<_Nodes[n]->_f[k]<<endl;
        }
    }
}

void Block::allStream() {

    for (unsigned n=0; n<_N; n++) {
        _Nodes[n]->stream(0, _TAU);
        _Nodes[n]->stream(2, _TAU);
        _Nodes[n]->stream(3, _TAU);
        _Nodes[n]->stream(5, _TAU);
        _Nodes[n]->stream(6, _TAU);
    }
    for (unsigned n=_N; n>=1; n--) {
        _Nodes[n-1]->stream(1, _TAU);
        _Nodes[n-1]->stream(4, _TAU);
        _Nodes[n-1]->stream(7, _TAU);
        _Nodes[n-1]->stream(8, _TAU);
    }
}

void Block::allSeams() {

    if (_Neighbors[1]) fillSeam(_Edge1, _Neighbors[1]->_Edge3, 1, 5, 8);
    if (_Neighbors[2]) fillSeam(_Edge2, _Neighbors[2]->_Edge4, 2, 5, 6);
    if (_Neighbors[3]) fillSeam(_Edge3, _Neighbors[3]->_Edge1, 3, 6, 7);
    if (_Neighbors[4]) fillSeam(_Edge4, _Neighbors[4]->_Edge2, 4, 7, 8);

    if (_Neighbors[5]) _Edge1.back()->_f[5] = _Neighbors[5]->_Edge3[0]->_f[5];
    if (_Neighbors[6]) _Edge3.back()->_f[6] = _Neighbors[6]->_Edge1[0]->_f[6];
    if (_Neighbors[7]) _Edge3[0]->_f[7] = _Neighbors[7]->_Edge1.back()->_f[7];
    if (_Neighbors[8]) _Edge1[0]->_f[8] = _Neighbors[8]->_Edge3.back()->_f[8];

}

void Block::fillSeam(vector<Node*> Edge, vector<Node*> Edge2, int k1, int k2, int k3) {

    unsigned nratio;
    double j1, j2; unsigned jn; //junk variables
    if (Edge.size() <= Edge2.size()) { //receiving edge has less/same # of nodes
        nratio = (Edge2.size()-1) / (Edge.size()-1);
        for (unsigned n=0; n<Edge.size(); n++) {
            Edge[n]->_f[k1] = Edge2[n*nratio]->_f[k1];
            Edge[n]->_f[k2] = Edge2[n*nratio]->_f[k2];
            Edge[n]->_f[k3] = Edge2[n*nratio]->_f[k3];
        }
    } else { //receiving edge has more nodes
        nratio = (Edge.size()-1) / (Edge2.size()-1);
        for (unsigned n=0; n<Edge.size(); n++) {
            jn = n/nratio;
            if (n%nratio != 0) {
                j2 = double(n%nratio)/nratio;
                j1 = 1 - j2;
                Edge[n]->_f[k1] = j1*Edge2[jn]->_f[k1] + j2*Edge2[jn+1]->_f[k1];
                Edge[n]->_f[k2] = j1*Edge2[jn]->_f[k2] + j2*Edge2[jn+1]->_f[k2];
                Edge[n]->_f[k3] = j1*Edge2[jn]->_f[k3] + j2*Edge2[jn+1]->_f[k3];
            } else {
                Edge[n]->_f[k1] = Edge2[jn]->_f[k1];
                Edge[n]->_f[k2] = Edge2[jn]->_f[k2];
                Edge[n]->_f[k3] = Edge2[jn]->_f[k3];
            }
        }
    }
}