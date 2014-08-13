#include "nodeedge.h"



NodeEdge::NodeEdge(Node *node1, Node *node2, double attraction) :
    Edge(node1, node2),
    _attraction(attraction),
    _item1(node1),
    _item2(node2)
{
    _item1->incrementWeight();
    _item2->incrementWeight();
}

NodeEdge::~NodeEdge()
{
    _item1->decrementWeight();
    _item2->decrementWeight();
}

QVector2D NodeEdge::getAttractionForceFor(const Node *node) const
{
    if(node == _item1) {
        return(QVector2D( _item2->pos() - _item1->pos()) * (_item1->getWeight() + _item2->getWeight()) * _attraction);
    } else if(node == _item2) {
        return(QVector2D(_item1->pos() - _item2->pos()) * (_item1->getWeight() + _item2->getWeight()) * _attraction);
    }
    return QVector2D();
    // currentNode->getWeight() + connctedNode->getWeight()) * _attraction * (connctedNode->pos() - currentNode->pos()
}



