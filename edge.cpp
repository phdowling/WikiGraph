#include "edge.h"

Edge::Edge(QGraphicsItem *item1, QGraphicsItem *item2, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _item1(item1),
    _item2(item2)
{
    setZValue(-1);
}

void Edge::updatePos()
{
    setPos(topLeft());
}

QGraphicsItem *Edge::getItem1() const
{
    return _item1;
}

QGraphicsItem *Edge::getItem2() const
{
    return _item2;
}

QLineF Edge::toLine() const
{
    QPoint p1, p2;
    if(_item1->x() > _item2->x()) {
        if(_item1->y() > _item2->y()) {
            p1.setX(boundingRect().width());
            p1.setY(boundingRect().height());
            p2.setX(0);
            p2.setY(0);
        } else {
            p1.setX(boundingRect().width());
            p1.setY(0);
            p2.setX(0);
            p2.setY(boundingRect().height());
        }
    } else {
        if(_item1->y() > _item2->y()) {
            p1.setX(0);
            p1.setY(boundingRect().height());
            p2.setX(boundingRect().width());
            p2.setY(0);
        } else {
            p1.setX(0);
            p1.setY(0);
            p2.setX(boundingRect().width());
            p2.setY(boundingRect().height());
        }
    }


    return QLineF(p1, p2);
}

bool Edge::has(QGraphicsItem *item) const
{
    return(_item1 == item || _item2 == item);
}

QPointF Edge::topLeft() const
{
    return QPointF(min(_item1->pos().x(), _item2->pos().x()),
                   min(_item1->pos().y(), _item2->pos().y()));
}

QPointF Edge::bottomRight() const
{
    return QPointF(max(_item1->pos().x(), _item2->pos().x()),
                   max(_item1->pos().y(), _item2->pos().y()));
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter->drawLine(toLine());
}

QRectF Edge::boundingRect() const
{
    return QRectF(0.0, 0.0, abs(_item1->pos().x() - _item2->pos().x()), abs(_item1->pos().y() - _item2->pos().y()));
}

qreal Edge::max(qreal a, qreal b) const
{
    return (a > b ? a : b);
}

qreal Edge::min(qreal a, qreal b) const
{
    return (a < b ? a : b);
}

qreal Edge::abs(qreal num) const
{
    return (num > 0 ? num : -num);
}
