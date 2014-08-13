#include "graphlayout.h"

GraphLayout::GraphLayout(QObject *parent) :
    QObject(parent)
{
}

void GraphLayout::update(const QList<Node *> *nodeList, const QList<NodeEdge *> *edgeList)
{
    calculateUpdate(nodeList, edgeList);
    updateFinished();
}

