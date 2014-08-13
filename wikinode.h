#ifndef WIKINODE_H
#define WIKINODE_H

#include "node.h"
#include "wikipediagraphwidget.h"

#include <QList>

class WikipediaGraphWidget;

class WikiNode : public Node
{
public:
    explicit WikiNode(WikipediaGraphWidget *owner, QGraphicsItem *parent = 0);
    QList<QString> getLinkedPages() const;
    void setLinkedPages(const QList<QString> linkedPages);
    void addLinkedPage(const QString &linkedPage);
    int countLinkedPages() const;

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    WikipediaGraphWidget *_owner;
    QList<QString> _linkedPages;
};

#endif // WIKINODE_H
