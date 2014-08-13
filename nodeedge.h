#ifndef NODEEDGE_H
#define NODEEDGE_H

#include <QVector2D>

#include "edge.h"
#include "node.h"

class NodeEdge : public Edge
{
public:
    NodeEdge(Node *node1, Node *node2, double attraction = 0.01);
    ~NodeEdge();
    QVector2D getAttractionForceFor(const Node *node) const;

private:
    double _attraction;
    Node *_item1;
    Node *_item2;
};

#endif // NODEEDGE_H
