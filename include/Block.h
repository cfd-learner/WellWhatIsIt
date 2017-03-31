#ifndef BLOCK_H
#define BLOCK_H

#include <Nodes.h>

class Block
{
    public:
        Block(unsigned ID, unsigned TYPE, unsigned L, unsigned I, unsigned J, double DX0, double DT0, double TAU, double RHOi, double RHOo);
        void initNeighbors(vector<Block*>& Blocks, unsigned I, unsigned J, bool test=false);
        void initW();
        void initE();
        void initNodes(unsigned TYPE, double RHOi, double RHOo, bool test=false);
        void allStream();
        void allSeams();
        void fillSeam(vector<Node*> Edge, vector<Node*> Edge2, int k1, int k2, int k3);
        void allBC() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->boundaryConditions();};
        void allRho() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcRho();};
        void allU() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcU(_Ex);};
        void allV() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcV(_Ey);};
        void allFeq() {for (unsigned n=0; n<_Nodes.size(); n++) _Nodes[n]->calcFeq(_C, _W, _Ex, _Ey);};
        Node* getNode(int n) {return _Nodes[n];};

        const unsigned _ID, _TYPE, _L, _I, _J, _N;

    private:
        double _DX, _DT, _C, _TAU;
        vector<double> _Ex, _Ey, _W;
        vector<Node*> _Nodes, _Edge1, _Edge2, _Edge3, _Edge4;
        vector<Block*> _Neighbors;
};

#endif // BLOCK_H