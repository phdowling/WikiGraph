#include "node.h"
#include "nodeedge.h"

Node::Node(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _weight(100)
{
    calcGeomerty();
    _expanded = false;
    setFlags(ItemIsMovable);
    _velocity = QVector2D(0,0);
}

void Node::setText(const QString &text)
{
    calcGeomerty();
    _text = text;
}

QString Node::getText() const
{
    return _text;
}

void Node::incrementWeight()
{
    ++_weight;
    calcGeomerty();
}

void Node::decrementWeight()
{
    --_weight;
    calcGeomerty();
}

QVector2D Node::getVelocity() const
{
    return _velocity;
}

void Node::setVelocity(QVector2D velocity)
{
    _velocity = velocity;
}

QVector2D Node::getNetForce() const
{
    return _netForce;
}

void Node::setNetForce(QVector2D netForce)
{
    _netForce = netForce;
}

int Node::getWeight() const
{
    return _weight;
}

void Node::setData(const QString &data)
{
    _data = data;
}

QString Node::getData() const
{
    return _data;
}

QSet<Node *> Node::getCalculatedNodes()
{
    return _calculatedNodes;
}

void Node::insertCalculatedNode(Node *node)
{
    _calculatedNodes.insert(node);
}

void Node::clearCalculatedNodes()
{
    _calculatedNodes.clear();
}

void Node::setExpanded()
{
    _expanded = true;
}

bool Node::isExpanded() const
{
    return _expanded;
}

QRectF Node::boundingRect() const
{
    return _boundingRect;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setFont(_font);
//    if(_weight>40){
//        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine));
//    } else{
        painter->setPen(QPen(Qt::black, 3, Qt::SolidLine));
//    }
    painter->setBrush(Qt::gray);



//    painter->drawEllipse(boundingRect());
//    QRect textRect(QPoint(-_r*0.7071, -_r*0.7071),
//                   QPoint(_r*0.7071, _r*0.7071));
//    painter->drawText(/*textRect,*/ Qt::AlignCenter | Qt::TextWrapAnywhere, _text);
    //painter->drawText(_boundingRect, _text);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addRect(_boundingRect);
    return path;
}

void Node::attachEdge(NodeEdge *e)
{
    _attachedEdges.append(e);
    _weight++;
}

void Node::detachEdge(NodeEdge *e)
{
    _attachedEdges.removeOne(e);
}

QList<NodeEdge *> Node::getAttachedEdges()
{
    return _attachedEdges;
}

QSet<Node *> Node::getAttractionCalculatedNodes()
{
    return _calculatedAttractionNodes;
}

void Node::insertAttractionCalculatedNode(Node *node)
{
    _calculatedAttractionNodes.insert(node);
}

void Node::clearAttractionCalculatedNodes()
{
    _calculatedAttractionNodes.clear();
}

void Node::setIndex(int idx)
{
    _index = idx;
}

int Node::getIndex()
{
    return _index;
}

void Node::addToNetForce(QVector2D force)
{
    _netForce+=force;
}

void Node::resetNetForce()
{
    _netForce = QVector2D();
}

void Node::calcGeomerty()
{
    _font = QFont("Arial", 6+(_weight*4));
    QFontMetricsF fontMetrics(_font);
    _boundingRect = fontMetrics.boundingRect(_text);
    _boundingRect.moveLeft(-_boundingRect.width() / 2);
    _boundingRect.moveTop(-_boundingRect.height() / 2);
}
