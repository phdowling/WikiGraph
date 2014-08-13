#include "directforcelayout.h"

DirectForceLayout::DirectForceLayout(QObject *parent) :
    GraphLayout(parent)
{
    _attraction = 0.004;
    _repulsion = 500;
    _damping = 0.92;
}

void DirectForceLayout::update(QList<Node *> nodeList, QList<NodeEdge *> edgeList)
{
    foreach(Node *node, nodeList) {
        node->setNetForce(QVector2D());
    }

    foreach(NodeEdge *edge, edgeList) {
        Node *node1 = dynamic_cast<Node*>(edge->getItem1());
        Node *node2 = dynamic_cast<Node*>(edge->getItem2());
        QVector2D attractionForce = Newton::calculateAttractionForce(_attraction, QVector2D(node1->pos()), QVector2D(node2->pos()));
        node1->addToNetForce(attractionForce);
        node2->addToNetForce(-attractionForce);
    }

    foreach(Node *node, nodeList) {
        foreach(Node *otherNode, nodeList) {
            if(node == otherNode) {
                continue;
            } else {
                QVector2D repulsionForce = Newton::calculateRepulsionForce(_repulsion, QVector2D(node->pos()), QVector2D(otherNode->pos()), node->getWeight(), otherNode->getWeight());
                node->addToNetForce(repulsionForce);
            }
        }
    }

    foreach(Node* node, nodeList) {
        node->setVelocity(_damping * (node->getVelocity() + Newton::calculateAcceleration(node->getNetForce(), node->getWeight())));
        node->setPos(node->pos() + node->getVelocity().toPointF());
    }

    // update edges
    foreach(Edge *edge, edgeList) {
        edge->updatePos();
    }

}
