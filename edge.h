#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QLine>
#include <QPainter>

class Edge : public QGraphicsItem
{
    friend bool operator ==(const Edge &first, const Edge &second);

public:
    Edge(QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent = 0);
    void updatePos();
    QGraphicsItem *getItem1() const;
    QGraphicsItem *getItem2() const;
    QLineF toLine() const;
    bool has(QGraphicsItem *item) const;
    QPointF topLeft() const;
    QPointF bottomRight() const;

    // graphics stuff
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    qreal max(qreal a, qreal b) const;
    qreal min(qreal a, qreal b) const;
    qreal abs(qreal num) const;


    QGraphicsItem *_item1;
    QGraphicsItem *_item2;
};



inline bool operator ==(const Edge &first, const Edge &second)
{
    return( (first._item1 == second._item1 && first._item2 == second._item2) ||
            (first._item1 == second._item2 && first._item2 == second._item1) );
}


#endif // EDGE_H
