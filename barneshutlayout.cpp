#include "barneshutlayout.h"

BarnesHutLayout::BarnesHutLayout(QObject *parent) :
    GraphLayout(parent)
{
    _theta = 2;
    _attraction = 0.002;
    _repulsion = 550;
    _damping = 0.9;
    _rootNode = 0;
    quadrantList = QList<Quadrant*>();
    quadrantList.append(new Quadrant);
    quadListIterator = quadrantList.begin();
}

void BarnesHutLayout::calculateUpdate(const QList<Node *> *nodeList, const QList<NodeEdge *> *edgeList)
{

//    while(nodeList->length()!= quadrantList.length()){
//        if(nodeList->length()>quadrantList.length()){
//            quadrantList.append(new Quadrant);
//        }else{
//            quadrantList.removeLast();
//        }
//    }
    while(quadrantList.length() < 2000){
        quadrantList.append(new Quadrant);
    }
    quadListIterator = quadrantList.begin();
    _rootNode = getRootNode(nodeList);
    buildTree(nodeList);

    for(QList<Node *>::const_iterator it = nodeList->constBegin(); it != nodeList->constEnd(); ++it) {
        Node *node = *it;
        node->setNetForce(QVector2D());
        node->addToNetForce(calculateNodeAcceleration(_rootNode, node, _repulsion, _theta));  //tell the quadtree to recursively compute this attraction Node's force

        //added this here experimentally
//        node->setVelocity(_damping * (node->getVelocity() + Newton::calculateAcceleration(node->getNetForce(), node->getWeight())));
//        node->setPos(node->pos() + node->getVelocity().toPointF());
    }

    for(QList<NodeEdge *>::const_iterator it = edgeList->constBegin(); it != edgeList->constEnd(); ++it) {
        NodeEdge *edge = *it;
        Node *node1 = dynamic_cast<Node*>(edge->getItem1());
        Node *node2 = dynamic_cast<Node*>(edge->getItem2());
        QVector2D attractionForce = Newton::calculateAttractionForce(_attraction, QVector2D(node1->pos()), QVector2D(node2->pos()));
        node1->addToNetForce(attractionForce);
        node2->addToNetForce(-attractionForce);
    }



    for(QList<Node *>::const_iterator it = nodeList->constBegin(); it != nodeList->constEnd(); ++it) {
        Node *node = *it;
        node->setVelocity(_damping * (node->getVelocity() + Newton::calculateAcceleration(node->getNetForce(), node->getWeight())));
        node->setPos(node->pos() + node->getVelocity().toPointF());
    }

    for(QList<NodeEdge *>::const_iterator it = edgeList->constBegin(); it != edgeList->constEnd(); ++it) {
        NodeEdge *edge = *it;
        edge->updatePos();
    }
}

void BarnesHutLayout::setTheta(double t)
{
    _theta = t;
}

void BarnesHutLayout::setAttraction(double a)
{
    _attraction = a;
}

void BarnesHutLayout::setRepulsion(double r)
{
    _repulsion = r;
}

void BarnesHutLayout::setDamping(double d)
{
    _damping = d;
}

void BarnesHutLayout::buildTree(const QList<Node*> *nodeList)
{
    for(QList<Node *>::const_iterator it = nodeList->constBegin(); it != nodeList->constEnd(); ++it) {
        Node *node = *it;
        insertNode(_rootNode,node);
    }
}

BarnesHutLayout::Quadrant * BarnesHutLayout::getRootNode(const QList<Node *> *nodeList)
{

    quadListIterator = quadrantList.begin();

    int minX = nodeList->first()->pos().x();
    int minY = nodeList->first()->pos().y();
    int maxX = nodeList->first()->pos().x();
    int maxY = nodeList->first()->pos().y();
    for(int i = 0;i< nodeList->length(); i++){
        Node *n = nodeList->at(i);
        const int x = n->pos().x();
        const int y = n->pos().y();
        minX = x^((x^minX) & -(x > minX));
        maxX = x^((x^maxX) & -(x < maxX));
        minY = y^((y^minY) & -(y > minY));
        maxY = y^((y^maxY) & -(y < maxY));
    }
    double width = (maxX-minX)+2;
    double height = (maxY-minY)+2;
    QPointF center  = QPointF((maxX+minX)/2, (maxY+minY)/2);
    _boundingRect = QRect(minX, minY, width, height);

    Quadrant *bounding = *quadListIterator;    //not sure about syntax here
    ++quadListIterator;
    resetQuadrant(bounding);

    bounding->center = center;
    bounding->w = width;
    bounding->h = height;

    return bounding;
}

QRect BarnesHutLayout::getBoundingRect() const
{
    return _boundingRect;
}

void BarnesHutLayout::insertNode(BarnesHutLayout::Quadrant *quadrant, Node *n)
{
    if(quadrant->numNodes > 1) {
        Quadrant *quad = getNodeQuadrant(quadrant, n);
        quadrant->childNodes[quad->pos] = quad;
        insertNode(quad, n);
    } else {
        if(quadrant->numNodes == 1) {
            //quadrantList.append(new Quadrant);
            if(n == quadrant->heldNode){
                return;
            }
            if( quadrant->heldNode->pos() == n->pos()) {
                n->setPos(n->pos() + QPointF(-3, 0));
            }
            Quadrant *quad1 = getNodeQuadrant( quadrant, quadrant->heldNode);
            quadrant->childNodes[quad1->pos] = quad1;
            insertNode( quad1, quadrant->heldNode);

            Quadrant *quad2 = getNodeQuadrant( quadrant, n);
            quadrant->childNodes[quad2->pos] = quad2;
            insertNode(quad2, n);
        }
        if(quadrant->numNodes == 0){
            quadrant->heldNode = n;
        }
    }
    quadrant->numNodes++;
    quadrant->mass += n->getWeight();
    quadrant->massSum += n->getWeight() * n->pos();
    quadrant->massCenter =  quadrant->massSum / quadrant->mass;


}

BarnesHutLayout::Quadrant * BarnesHutLayout::getNodeQuadrant(BarnesHutLayout::Quadrant *quadrant, Node *n)
{
    Quadrant *result = 0;
    if(n->pos().x()< quadrant->center.x()){
        //West
        if(n->pos().y() < quadrant->center.y()){     //NW
            if(quadrant->childNodes[0] == 0){

                result = *quadListIterator++; //using the interator so we don't create new instances al the time
                resetQuadrant(result);              //make sure the quadrant we're editing isn't holding data
                result->pos = 0;
                result->center = QPointF(quadrant->center.x()-quadrant->w/2, quadrant->center.y()-quadrant->h/2);
                result->w = quadrant->w/2;
                result->h = quadrant->h/2;
            }else{
                return quadrant->childNodes[0];
            }
        }else{                              //SW
            if(quadrant->childNodes[2] == 0){
                result = *quadListIterator++;
                resetQuadrant(result);
                result->pos = 2;
                result->center = QPointF(quadrant->center.x()-quadrant->w/2, quadrant->center.y()+quadrant->h/2);
                result->w = quadrant->w/2;
                result->h = quadrant->h/2;
            }else{
                return quadrant->childNodes[2];
            }
        }
    }else{
        //East
        if(n->pos().y() < quadrant->center.y()){     //NE
            if(quadrant->childNodes[1] == 0){
                result = *quadListIterator++;
                resetQuadrant(result);
                result->pos = 1;
                result->center = QPointF(quadrant->center.x()+quadrant->w/2, quadrant->center.y()-quadrant->h/2);
                result->w = quadrant->w/2;
                result->h = quadrant->h/2;
            }else{
                return quadrant->childNodes[1];
            }
        }else{                              //SE
            if(quadrant->childNodes[3] == 0){
                result = *quadListIterator++;
                resetQuadrant(result);
                result->pos = 3;
                result->center = QPointF(quadrant->center.x()+quadrant->w/2, quadrant->center.y()+quadrant->h/2);
                result->w = quadrant->w/2;
                result->h = quadrant->h/2;
            }else{
                return quadrant->childNodes[3];
            }
        }
    }

    return result;
}

QVector2D BarnesHutLayout::calculateNodeAcceleration(BarnesHutLayout::Quadrant *quadrant, Node *n, double repulsion, double theta)
{
    QVector2D force(0, 0);
    QPointF nodePos = n->pos();
    if(quadrant->numNodes == 1){
        if(n == quadrant->heldNode) {
            return force;
        }

        if(n->pos() == quadrant->heldNode->pos()){
            n->setPos(nodePos + QPointF(-1, 0));
        }
        force += Newton::calculateRepulsionForce(repulsion, QVector2D(nodePos), QVector2D(quadrant->heldNode->pos()), n->getWeight(), quadrant->heldNode->getWeight());

    } else {
        if( ( ((quadrant->w + quadrant->h) / 2) / QVector2D( nodePos - quadrant->massCenter).length() ) < theta){
        //if(  QVector2D(n->pos() - _massCenter).length() > (((( _width + _height) / 2 ) / theta) + QVector2D(_massCenter-n->pos()).length() )){
            force += Newton::calculateRepulsionForce(repulsion, QVector2D(nodePos), QVector2D( quadrant->massCenter), n->getWeight(),quadrant->mass);
        } else {
            for( int i = 0; i<4; i++){
                if( quadrant->childNodes[i] != 0)
                    force += calculateNodeAcceleration(quadrant->childNodes[i], n, repulsion, theta);
            }
        }
    }
    return force;
}

void BarnesHutLayout::resetQuadrant(BarnesHutLayout::Quadrant *q) //reset Quadrant for reconfiguration
{
    q->pos = 0;
    q->heldNode = 0;
    q->numNodes = 0;

    q->mass = 0;
    q->massCenter = QPoint(0,0);
    q->massSum = QPoint(0,0);
    for(int i = 0; i < 4; i++){
        q->childNodes[i] = 0;
    }

}

Node * BarnesHutLayout::getNode(BarnesHutLayout::Quadrant *quadrant)
{
    return quadrant->heldNode;
}

void BarnesHutLayout::setNode(BarnesHutLayout::Quadrant *quadrant, Node *n)
{
    quadrant->heldNode = n;
}

void BarnesHutLayout::update(QList<Node *> *nodeList, QList<NodeEdge *> *edgeList)
{
    calculateUpdate(nodeList,edgeList);
    updateFinished();
}

