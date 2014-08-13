#ifndef GRAPHLAYOUT_H
#define GRAPHLAYOUT_H

#include <QObject>
#include <QList>
#include <QRect>

#include "node.h"
#include "nodeedge.h"

class GraphWidget;

class GraphLayout : public QObject
{
    Q_OBJECT
public:
    explicit GraphLayout(QObject *parent = 0);
    virtual QRect getBoundingRect() const = 0;

private:
    virtual void calculateUpdate(const QList<Node *> *nodeList, const QList<NodeEdge *> *edgeList) = 0;

signals:
    void updateFinished();

public slots:
    void update(const QList<Node *> *nodeList, const QList<NodeEdge *> *edgeList);

};

#endif // GRAPHLAYOUT_H
