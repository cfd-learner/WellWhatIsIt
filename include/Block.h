#ifndef BLOCK_H
#define BLOCK_H

#include <Nodes.h>

class Block
{
    public:
        Block(int ID, int TYPE, int L, int I, int J, double X0, double Y0, double DX0, double DT0, double TAU, double RHOi, double RHOo);
        void initNeighbors(vector<Block*>& Blocks, int I, int J, bool test=false);
        void initW();
        void initE();
        void initNodes(int TYPE, double RHOi, double RHOo, bool test=false);
        void allStream();
        void allSeams();
        void fillSeam(vector<Node*> Edge, vector<Node*> Edge2, int k1, int k2, int k3);
        void allBC() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->boundaryConditions();};
        void allRho() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcRho();};
        void allU() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcU(_Ex);};
        void allV() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcV(_Ey);};
        void allFeq() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcFeq(_C, _W, _Ex, _Ey);};

        Node* getNode(int n) {return _Nodes[n];};
        Block* getNeighbor(int k) {return _Neighbors[k];};
        double getX(int n) {return _Nodes[n]->getX();};
        double getY(int n) {return _Nodes[n]->getY();};
        double getRho(int n) {return _Nodes[n]->getRho();};
        double getU(int n) {return _Nodes[n]->getU();};
        double getV(int n) {return _Nodes[n]->getV();};

        const int _ID, _TYPE, _L, _I, _J, _N;
        const double _X0, _Y0, _DX, _DT, _C, _TAU;

    private:
        vector<double> _Ex, _Ey, _W;
        vector<Node*> _Nodes, _Edge1, _Edge2, _Edge3, _Edge4;
        vector<Block*> _Neighbors;
};

#endif // BLOCK_H