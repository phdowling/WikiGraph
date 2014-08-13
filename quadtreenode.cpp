#include "quadtreenode.h"



/*
DEPRECATED

has been replaced with Quadrant struct in BarnesHutLayout, methods are also there now. To be deleted.

*/

QuadTreeNode::QuadTreeNode(double w, double h, QPointF c, int level)
{
    _depth = level;
    _width = w;
    _height = h;
    _center = c;
    _node = 0;
    _numNodes = 0;
    _mass = 0;
}

QuadTreeNode::~QuadTreeNode()
{
    foreach(QuadTreeNode *n, _childQuadTreeNodes){
            delete n;
    }
}

void QuadTreeNode::insertNode(Node *n)
{
    if(_numNodes > 1) {
        QuadTreeNode *quad = getNodeQuadrant(n);
        _childQuadTreeNodes.append(quad);
        quad->insertNode(n);
    } else {
        if(_numNodes == 1) {
            if(n==_node){
                return;
            }
            if(_node->pos() == n->pos()) {
                n->setPos(n->pos() + QPointF(-3, 0));
            }
            QuadTreeNode *quad1 = getNodeQuadrant(_node);
            _childQuadTreeNodes.append(quad1);
            quad1->insertNode(_node);

            QuadTreeNode *quad2 = getNodeQuadrant(n);
            _childQuadTreeNodes.append(quad2);
            quad2->insertNode(n);
        }
        if(_numNodes == 0){
            _node = n;
        }
    }
    ++_numNodes;
    _mass += n->getWeight();
    _weightCenter += n->getWeight() * n->pos();
    _massCenter = 1/_mass * _weightCenter;


}

QVector2D QuadTreeNode::calculateNodeAcceleration(Node *n, double repulsion, double theta)
{
    QVector2D force(0, 0);
    if(_numNodes == 1){
        if(n == _node) {
            return force;
        }

        if(n->pos() == _node->pos()){
            n->setPos(n->pos() + QPointF(-1, 0));
        }
        force += Newton::calculateRepulsionForce(repulsion, QVector2D(n->pos()), QVector2D(_node->pos()), n->getWeight(), _node->getWeight());

    } else {
        if( ( ((_width+_height)/2) / QVector2D(n->pos() - _massCenter).length() ) < theta){
        //if(  QVector2D(n->pos() - _massCenter).length() > (((( _width + _height) / 2 ) / theta) + QVector2D(_massCenter-n->pos()).length() )){
            force += Newton::calculateRepulsionForce(repulsion, QVector2D(n->pos()), QVector2D(_massCenter), n->getWeight(),_mass);
        } else {
            foreach(QuadTreeNode *qn, _childQuadTreeNodes){
                force += qn->calculateNodeAcceleration(n, repulsion, theta);
            }
        }
    }
    return force;
}

void QuadTreeNode::setNode(Node *n)
{
    _node = n;
}

Node* QuadTreeNode::getNode()
{
    return _node;
}

QuadTreeNode* QuadTreeNode::getNodeQuadrant(Node *n)
{
    if(n->pos().x()< _center.x()){
        //West
        if(n->pos().y() < _center.y()){
            return new QuadTreeNode(_width/2, _height/2, QPointF(_center.x()-_width/2, _center.y()-_height/2),_depth+1);  //NW
        }else{
            return new QuadTreeNode(_width/2,_height/2,QPointF(_center.x()-_width/2, _center.y()+_height/2), _depth+1); //SW
        }
    }else{
        //East
        if(n->pos().y() < _center.y()){
            return new QuadTreeNode(_width/2,_height/2,QPointF(_center.x()+_width/2, _center.y()-_height/2), _depth+1); //NE
        }else{
            return new QuadTreeNode(_width/2,_height/2,QPointF(_center.x()+_width/2, _center.y()+_height/2), _depth+1); //SE
        }
    }
}


