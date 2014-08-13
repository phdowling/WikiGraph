#include "fastlayout.h"

FastLayout::FastLayout(QObject *parent) :
    GraphLayout(parent)
{
    _attraction = 0.001;
    _repulsion = 500;
    _damping = 0.9;
}

void FastLayout::update(QList<Node *> nodeList, QList<NodeEdge *> edgeList)
{
    // reset forces
    foreach(Node *node, nodeList) {
        node->setNetForce(QVector2D());
    }

    // calculate edge forces
    foreach(NodeEdge *edge, edgeList) {
        Node *node1 = dynamic_cast<Node*>(edge->getItem1());
        Node *node2 = dynamic_cast<Node*>(edge->getItem2());

        // edge attraction
        QVector2D edgeAttractionForce = Newton::calculateAttractionForce(_attraction, QVector2D(node1->pos()), QVector2D(node2->pos()));
        node1->addToNetForce(edgeAttractionForce);
        node2->addToNetForce(-edgeAttractionForce);

        // edge repulsion
        QVector2D edgeRepulsionForce = Newton::calculateRepulsionForce(_repulsion, QVector2D(node1->pos()), QVector2D(node2->pos()), node1->getWeight(), node2->getWeight());
        if(QVector2D(node1->pos() - node2->pos()).length() < 5) {
            node2->setPos(node2->pos() + QPointF(10, 10));
        }
        node1->addToNetForce(edgeRepulsionForce);
        node2->addToNetForce(-edgeRepulsionForce);
    }

    // create repulsion fields
    FieldMap<Node> repulsionMap;
    foreach(Node *node, nodeList) {
        repulsionMap.add(node, node->pos());
    }

    //calculate repulsion
    QList< QPair<Node*, Node*> > influencingNodes = repulsionMap.getInfluencingElements();
    QPair<Node*, Node*> influencingPair;
    foreach(influencingPair, influencingNodes) {
        Node *node1 = influencingPair.first;
        Node *node2 = influencingPair.second;
        if(QVector2D(node1->pos() - node2->pos()).length() < 5) {
            node2->setPos(node2->pos() + QPointF(10, 10));
        }
        QVector2D repulsionForce = Newton::calculateRepulsionForce(_repulsion, QVector2D(node1->pos()), QVector2D(node2->pos()), node1->getWeight(), node2->getWeight());
        influencingPair.first->addToNetForce(repulsionForce);
        influencingPair.second->addToNetForce(-repulsionForce);
    }

    // calculate velocities and move nodes
    foreach(Node* node, nodeList) {
        node->setVelocity(node->getVelocity() + Newton::calculateAcceleration(node->getNetForce(), node->getWeight()));
        node->setPos(node->pos() + node->getVelocity().toPointF());
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

