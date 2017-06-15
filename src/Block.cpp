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

Block::Block(int ID, int TYPE, int L, int I, int J, double DX0, double DT0, double TAU, double RHOi, double RHOo):
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

void Block::initNeighbors(vector<Block*>& Blocks, int I, int J, bool test) {

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
        for (int k=0; k<K; k++) {
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

void Block::initNodes(int TYPE, double RHOi, double RHOo, bool test) {

    int n = 0;
    switch (TYPE) {
        
    //internal seams on all sides
    case 0: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, 6, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 7, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, RHOo, 4));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along top edge, internal otherwise
    case 2: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, 6, 2, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, 5, 2, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 7, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along bottom edge, internal otherwise
    case 4: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, 6, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, 7, 4, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, 8, 4, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //pressure outlet along right edge, internal otherwise
    case 11: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, 6, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodePressureSeam(n, 5, 1, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 7, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodePressureSeam(n, 8, 1, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //pressure inlet along left edge, internal otherwise
    case 13: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureSeam(n, 6, 3, RHOi));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureSeam(n, 7, 3, RHOi));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 3, RHOi));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along top, pressure outlet along right, internal otherwise
    case 15: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, 6, 2, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 7, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodePressureSeam(n, 8, 1, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along top, pressure inlet along left, internal otherwise
    case 16: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, 6, RHOi));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, 5, 2, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureSeam(n, 7, 3, RHOi));
            else if (n == _N-1) _Nodes.push_back(new NodeInternalSeam(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeInternalSeam(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 3, RHOi));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along bottom, pressure inlet along left, internal otherwise
    case 17: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureSeam(n, 6, 3, RHOi));
            else if (n == _J-1) _Nodes.push_back(new NodeInternalSeam(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, 7, RHOi));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, 8, 4, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 3, RHOi));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //wall along bottom, pressure outlet along right, internal otherwise
    case 18: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeInternalSeam(n, 6, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodePressureSeam(n, 5, 1, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, 7, 4, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeInternalSeam(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //walls along top and bottom, internal otherwise
    case 24: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, 6, 2, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, 5, 2, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, 7, 4, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, 8, 4, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //walls along top and bottom, pressure outlet along right, internal left
    case 111: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodeWallSeam(n, 6, 2, RHOo));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodeWallSeam(n, 7, 4, RHOo));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 3, RHOo));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //walls along top and bottom, pressure inlet along left, internal right
    case 333: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, 6, RHOi));
            else if (n == _J-1) _Nodes.push_back(new NodeWallSeam(n, 5, 2, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, 7, RHOi));
            else if (n == _N-1) _Nodes.push_back(new NodeWallSeam(n, 8, 4, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 3, RHOi));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodeInternalSeam(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
        
    //walls along top and bottom, pressure inlet left, pressure outlet right
    case 999: while (n < _N) {
            if (n == 0) _Nodes.push_back(new NodePressureCorner(n, 6, RHOi));
            else if (n == _J-1) _Nodes.push_back(new NodePressureCorner(n, 5, RHOo));
            else if (n == (_I-1)*_J) _Nodes.push_back(new NodePressureCorner(n, 7, RHOi));
            else if (n == _N-1) _Nodes.push_back(new NodePressureCorner(n, 8, RHOo));
            else if (n < _J-1) _Nodes.push_back(new NodeWall(n, 2, RHOo));
            else if (n > (_I-1)*_J) _Nodes.push_back(new NodeWall(n, 4, RHOo));
            else if (n%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 3, RHOi));
            else if ((n+1)%_J == 0) _Nodes.push_back(new NodePressureEdge(n, 1, RHOo));
            else _Nodes.push_back(new NodeInternal(n, RHOo));
            n++;
        } break;
    }

    for (int n=0; n<_N; n++) {
        if (n%_J == 0) _Edge1.push_back(_Nodes[n]);
        if (n >= (_I-1)*_J) _Edge2.push_back(_Nodes[n]);
        if ((n+1)%_J == 0) _Edge3.push_back(_Nodes[n]);
        if (n < _J) _Edge4.push_back(_Nodes[n]);
    }

    for (int n=0; n<_N; n++) {
        _Nodes[n]->initNeighbors(_Nodes, _J);
        _Nodes[n]->calcFeq(_C, _W, _Ex, _Ey);
        _Nodes[n]->initF();
    }

    if (test) for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->printInfo();
//    if (test) {
//        cout<<"Edge 1 nodes:"<<endl;
//        for (unsigned n=0; n<_Edge1.size(); n++) cout<<_Edge1[n]->_ID<<endl;
//        cout<<"Edge 2 nodes:"<<endl;
//        for (unsigned n=0; n<_Edge2.size(); n++) cout<<_Edge2[n]->_ID<<endl;
//        cout<<"Edge 3 nodes:"<<endl;
//        for (unsigned n=0; n<_Edge3.size(); n++) cout<<_Edge3[n]->_ID<<endl;
//        cout<<"Edge 4 nodes:"<<endl;
//        for (unsigned n=0; n<_Edge4.size(); n++) cout<<_Edge4[n]->_ID<<endl;
//    }
}

void Block::allStream() {

    for (int n=0; n<_N; n++) {
        _Nodes[n]->stream(0, _TAU);
        _Nodes[n]->stream(2, _TAU);
        _Nodes[n]->stream(3, _TAU);
        _Nodes[n]->stream(5, _TAU);
        _Nodes[n]->stream(6, _TAU);
    }
    for (int n=_N-1; n>=0; n--) {
        _Nodes[n]->stream(1, _TAU);
        _Nodes[n]->stream(4, _TAU);
        _Nodes[n]->stream(7, _TAU);
        _Nodes[n]->stream(8, _TAU);
    }
}

void Block::allSeams() {

    if (_Neighbors[1]) fillSeam(_Edge1, _Neighbors[1]->_Edge3, 1, 5, 8);
    if (_Neighbors[2]) fillSeam(_Edge2, _Neighbors[2]->_Edge4, 2, 5, 6);
    if (_Neighbors[3]) fillSeam(_Edge3, _Neighbors[3]->_Edge1, 3, 6, 7);
    if (_Neighbors[4]) fillSeam(_Edge4, _Neighbors[4]->_Edge2, 4, 7, 8);

    if (_Neighbors[5]) _Edge1.back()->setF( 5, _Neighbors[5]->_Edge3[0]->getF(5) );
    if (_Neighbors[6]) _Edge3.back()->setF( 6, _Neighbors[6]->_Edge1[0]->getF(6) );
    if (_Neighbors[7]) _Edge3[0]->setF( 7, _Neighbors[7]->_Edge1.back()->getF(7) );
    if (_Neighbors[8]) _Edge1[0]->setF( 8, _Neighbors[8]->_Edge3.back()->getF(8) );

}

void Block::fillSeam(vector<Node*> Edge, vector<Node*> Edge2, int k1, int k2, int k3) {

    int nratio;
    double jawn1, jawn2; int njawn; //junk variables
    if (Edge.size() <= Edge2.size()) { //receiving edge has less/same # of nodes
        nratio = (Edge2.size()-1) / (Edge.size()-1);
        for (unsigned n=0; n<Edge.size(); n++) {
            Edge[n]->setF( k1, Edge2[n*nratio]->getF(k1) );
            Edge[n]->setF( k2, Edge2[n*nratio]->getF(k2) );
            Edge[n]->setF( k3, Edge2[n*nratio]->getF(k3) );
        }
    } else { //receiving edge has more nodes
        nratio = (Edge.size()-1) / (Edge2.size()-1);
        for (unsigned n=0; n<Edge.size(); n++) {
            njawn = n/nratio;
            if (n%nratio != 0) {
                jawn2 = double(n%nratio)/nratio;
                jawn1 = 1 - jawn2;
                Edge[n]->setF( k1, jawn1*Edge2[njawn]->getF(k1) + jawn2*Edge2[njawn+1]->getF(k1) );
                Edge[n]->setF( k2, jawn1*Edge2[njawn]->getF(k2) + jawn2*Edge2[njawn+1]->getF(k2) );
                Edge[n]->setF( k3, jawn1*Edge2[njawn]->getF(k3) + jawn2*Edge2[njawn+1]->getF(k3) );
            } else {
                Edge[n]->setF( k1, Edge2[njawn]->getF(k1) );
                Edge[n]->setF( k2, Edge2[njawn]->getF(k2) );
                Edge[n]->setF( k3, Edge2[njawn]->getF(k3) );
            }
        }
    }
}