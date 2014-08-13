#include "wikinode.h"

WikiNode::WikiNode(WikipediaGraphWidget *owner, QGraphicsItem *parent) :
    Node(parent),
    _owner(owner)
{
}

void WikiNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    _owner->expand(this);
}

QList<QString> WikiNode::getLinkedPages() const
{
    return _linkedPages;
}

void WikiNode::setLinkedPages(const QList<QString> linkedPages)
{
    _linkedPages = linkedPages;
}

void WikiNode::addLinkedPage(const QString &linkedPage)
{
    _linkedPages.append(linkedPage);
}

int WikiNode::countLinkedPages() const
{
    return _linkedPages.size();
}
