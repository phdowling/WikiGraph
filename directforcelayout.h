#ifndef DIRECTFORCELAYOUT_H
#define DIRECTFORCELAYOUT_H

#include <QVector2D>

#include "graphlayout.h"
#include "newton.h"

class DirectForceLayout : public GraphLayout
{
    Q_OBJECT
public:
    explicit DirectForceLayout(QObject *parent = 0);

    virtual void update(QList<Node *> nodeList, QList<NodeEdge *> edgeList);
    
private:
    double _attraction;
    double _repulsion;
    double _damping;
    
};

#endif // DIRECTFORCELAYOUT_H
