#ifndef WIKIPEDIAGRAPHWIDGET_H
#define WIKIPEDIAGRAPHWIDGET_H

#include <QInputDialog>

#include "graphwidget.h"
#include "wikinode.h"
#include "qmath.h"
#include "pageloader.h"

class WikipediaGraphWidget : public GraphWidget
{
    Q_OBJECT
public:
    explicit WikipediaGraphWidget(GraphLayout *layout, QWidget *parent = 0);
    virtual Node *addNode(QPointF pos);
    virtual Node *addNode(int x, int y);
    void expand(Node *node);
    void setMaxLinks(int links);

private:
    QList<QString> getLinkedPages(QString address);
    void makeTestScene();
    void performCriticalExpansionTasks(void *arg);
    int _maxLinks;
    int _expandPos;
    int _expandNum;
    QTimer _expandTimer;

signals:
    
private slots:
    void parseLinks(PageLoader *pageLoader);

public slots:
    void expandGeneration();
    void expandNext();
    
};

#endif // WIKIPEDIAGRAPHWIDGET_H
