#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QVector2D>
#include <QPoint>
#include <QSet>
#include <QFont>
#include <QFontMetricsF>

class NodeEdge;


class Node : public QGraphicsItem
{

public:
    Node(QGraphicsItem *parent = 0);
    void setIndex(int idx);
    int getIndex();
    void setText(const QString &text);
    QString getText() const;
    void setData(const QString &data);
    QString getData() const;
    void incrementWeight();
    void decrementWeight();
    int getWeight() const;
    QVector2D getVelocity() const;
    void setVelocity(QVector2D velocity);
    QVector2D getNetForce() const;
    void setNetForce(QVector2D netForce);
    void addToNetForce(QVector2D force);
    void resetNetForce();
    void calcGeomerty();
    QSet<Node*> getCalculatedNodes();
    void insertCalculatedNode(Node* node);
    void clearCalculatedNodes();
    void setExpanded();
    bool isExpanded() const;

    QSet<Node*>getAttractionCalculatedNodes();
    void insertAttractionCalculatedNode(Node* node);
    void clearAttractionCalculatedNodes();


    void attachEdge(NodeEdge* e);
    void detachEdge(NodeEdge* e);
    QList<NodeEdge*> getAttachedEdges();


    // graphics stuff
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int _index;
    bool _expanded;
    QSet<Node*> _calculatedNodes;
    QString _text;
    QString _data;
    QList<NodeEdge*> _attachedEdges;
    QSet<Node*> _calculatedAttractionNodes;
    int _weight;
    QVector2D _velocity;
    QVector2D _netForce;
    QFont _font;
    QRectF _boundingRect;
};

#endif // NODE_H
