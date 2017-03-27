#ifndef NODES_H
#define NODES_H

#include <vector>
using namespace std;

extern unsigned int K;
extern double C;
//const double NUL(-99);

//SIDE directions look towards walls, into corners.

class Node
{
    public:
        Node(unsigned int ID, unsigned int NSIDE, unsigned int BSIDE, double rho);
        virtual void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false) = 0;
        void calcFeq(double C, vector<double>& W, vector<double>& Ex, vector<double>& Ey, bool test=false);
        void initF();
        void stream(unsigned int k, double TAU);
        virtual void boundaryConditions(bool test=false) = 0;
        void calcRho(bool test=false);
        void calcU(vector<double>& Ex, bool test=false);
        void calcV(vector<double>& Ey, bool test=false);

        double getF(unsigned int k) {return _f[k];};

    public:
        unsigned int _ID, _NSIDE, _BSIDE;
        double _rho;
        double _u, _v;
        bool _ISFIXEDRHO, _ISFIXEDU, _ISFIXEDV;
        vector<double> _f, _feq;
        vector<Node*> _Neighbors;
};

class NodeInternal: private Node
{
    public:
        NodeInternal(unsigned int ID, double rho): Node(ID, 0, 0, rho) {};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false) {};
};

class NodeWall: private Node
{
    public:
        NodeWall(unsigned int ID, unsigned int SIDE, double rho): Node(ID, SIDE, SIDE, rho) {_ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureEdge: private Node
{
    public:
        NodePressureEdge(unsigned int ID, unsigned int SIDE, double rho): Node(ID, SIDE, SIDE, rho) {_ISFIXEDRHO = true;};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureCorner: private Node
{
    public:
        NodePressureCorner(unsigned int ID, unsigned int SIDE, double rho): Node(ID, SIDE, SIDE, rho) {_ISFIXEDRHO = true; _ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodeInternalSeam: private Node
{
    public:
        NodeInternalSeam(unsigned int ID, unsigned int SIDE, double rho): Node(ID, SIDE, 0, rho) {};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false) {};
};

class NodeWallSeam: private Node
{
    public:
        NodeWallSeam(unsigned int ID, unsigned int NSIDE, unsigned int BSIDE, double rho): Node(ID, NSIDE, BSIDE, rho) {_ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureSeam: private Node
{
    public:
        NodePressureSeam(unsigned int ID, unsigned int NSIDE, unsigned int BSIDE, double rho): Node(ID, NSIDE, BSIDE, rho) {_ISFIXEDRHO = true; _ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, unsigned int J, bool test=false);
        void boundaryConditions(bool test=false);
};

#endif // NODES_H