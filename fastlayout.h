#ifndef FASTLAYOUT_H
#define FASTLAYOUT_H

#include "graphlayout.h"
#include "fieldmap.h"
#include "newton.h"

class FastLayout : public GraphLayout
{
    Q_OBJECT
public:
    explicit FastLayout(QObject *parent = 0);
    virtual void update(QList<Node *> nodeList, QList<NodeEdge *> edgeList);

private:
    double _attraction;
    double _repulsion;
    double _damping;

};


#endif // FASTLAYOUT_H
