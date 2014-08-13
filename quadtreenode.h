#ifndef QUADTREENODE_H
#define QUADTREENODE_H
#include "node.h"
#include "newton.h"

class QuadTreeNode
{
public:
    QuadTreeNode(double w, double h, QPointF c, int level);
    ~QuadTreeNode();

    void insertNode(Node *n);
    QVector2D calculateNodeAcceleration(Node *n, double repulsion, double theta);
    void setNode(Node *n);
    Node* getNode();
    QuadTreeNode *getNodeQuadrant(Node *n);
//    int getNodeQuadrantNumber(Node *n);
private:
    double _width;
    double _height;
    QPointF _center;
    QPointF _weightCenter;
    int _depth;

    Node *_node;
    QPointF _massCenter;
    double _mass;
    int _numNodes;

    QList<QuadTreeNode*> _childQuadTreeNodes; //NE, NW, SE, SW


};

#endif // QUADTREENODE_H
