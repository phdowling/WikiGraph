#ifndef BARNESHUTLAYOUT_H
#define BARNESHUTLAYOUT_H
#include "graphlayout.h"
#include "newton.h"

class BarnesHutLayout : public GraphLayout
{
    Q_OBJECT
public:
    explicit BarnesHutLayout(QObject *parent = 0);
    QRect getBoundingRect() const;
    struct Quadrant{
        int pos;    //0 -> NW, 1 -> NE, 2->SW, 3->SE
        Quadrant* childNodes[4];
        Node* heldNode;
        QPointF center;
        QPointF massCenter;
        QPointF massSum;
        double w;
        double h;
        int numNodes;
        double mass;
    };

private:
    virtual void calculateUpdate(const QList<Node *> *nodeList, const QList<NodeEdge *> *edgeList);
    double _theta;
    double _attraction;
    double _repulsion;
    double _damping;

    QRect _boundingRect;

    void buildTree(const QList<Node*> *nodeList);

    QList<Node*>::iterator nodeIterator;


    Quadrant* _rootNode;
    QList<Quadrant*> quadrantList;
    QList<Quadrant*>::const_iterator quadListIterator;

    Quadrant *getRootNode(const QList<Node *> *nodeList);
    void insertNode(Quadrant* quadrant, Node *node);
    QVector2D calculateNodeAcceleration(Quadrant* quadrant, Node *n, double repulsion, double theta);
    void setNode(Quadrant *quadrant, Node *n);
    Node* getNode(Quadrant* quadrant);
    Quadrant *getNodeQuadrant(Quadrant* quadrant, Node *n);

    void resetQuadrant(Quadrant *q);


signals:
    void updateFinished();

public slots:
    void update(QList<Node *> *nodeList, QList<NodeEdge *> *edgeList);
    void setTheta(double t);
    void setAttraction(double a);
    void setRepulsion(double r);
    void setDamping(double d);
};

#endif // BARNESHUTLAYOUT_H
