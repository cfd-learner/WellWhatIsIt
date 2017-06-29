#ifndef NODES_H
#define NODES_H

#include <vector>
using namespace std;

extern int K;
extern double C;
//const double NUL(-99);

//SIDE directions look towards walls, into corners.

class Node {
    public:
        Node(int ID, int NSIDE, int BSIDE, double rho, double x, double y);
        virtual void initNeighbors(vector<Node*>& Nodes, int J, bool test=false) = 0;
        void calcFeq(double C, vector<double>& W, vector<double>& Ex, vector<double>& Ey, bool test=false);
        void initF();
        void stream(int k, double TAU);
        virtual void boundaryConditions(bool test=false) = 0;
        void calcRho(bool test=false);
        void calcU(vector<double>& Ex, bool test=false);
        void calcV(vector<double>& Ey, bool test=false);
        void printInfo();

        void setF(int k, double f) {_f[k] = f;};
        double getF(int k) {return _f[k];};
        double getX() {return _x;};
        double getY() {return _y;};
        double getRho() {return _rho;};
        double getU() {return _u;};
        double getV() {return _v;};

        const int _ID, _NSIDE, _BSIDE;
        const double _x, _y;

    protected:
        double _rho, _u, _v;
        bool _ISFIXEDRHO, _ISFIXEDU, _ISFIXEDV;
        vector<double> _f, _feq;
        vector<Node*> _Neighbors;
};

class NodeInternal: public Node {
    public:
        NodeInternal(int ID, double rho, double x, double y): Node(ID, 0, 0, rho, x, y) {};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false) {};
};

class NodeWall: public Node {
    public:
        NodeWall(int ID, int SIDE, double rho, double x, double y): Node(ID, SIDE, SIDE, rho, x, y) {_ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureEdge: public Node {
    public:
        NodePressureEdge(int ID, int SIDE, double rho, double x, double y): Node(ID, SIDE, SIDE, rho, x, y) {_ISFIXEDRHO = true;};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureCorner: public Node {
    public:
        NodePressureCorner(int ID, int SIDE, double rho, double x, double y): Node(ID, SIDE, SIDE, rho, x, y) {_ISFIXEDRHO = true; _ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodeInternalSeam: public Node {
    public:
        NodeInternalSeam(int ID, int SIDE, double rho, double x, double y): Node(ID, SIDE, 0, rho, x, y) {};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false) {};
};

class NodeWallSeam: public Node {
    public:
        NodeWallSeam(int ID, int NSIDE, int BSIDE, double rho, double x, double y): Node(ID, NSIDE, BSIDE, rho, x, y) {_ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false);
};

class NodePressureSeam: public Node {
    public:
        NodePressureSeam(int ID, int NSIDE, int BSIDE, double rho, double x, double y): Node(ID, NSIDE, BSIDE, rho, x, y) {_ISFIXEDRHO = true; _ISFIXEDU = true; _ISFIXEDV = true;};
        void initNeighbors(vector<Node*>& Nodes, int J, bool test=false);
        void boundaryConditions(bool test=false);
};

#endif // NODES_H