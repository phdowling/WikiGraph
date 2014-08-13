#include "wikipediagraphwidget.h"

WikipediaGraphWidget::WikipediaGraphWidget(GraphLayout *layout, QWidget *parent) :
    GraphWidget(layout, parent)
{
    _expandTimer.setInterval(100);
    connect(&_expandTimer, SIGNAL(timeout()), this, SLOT(expandNext()));
    makeTestScene();
    startForceCalculation();
}

Node *WikipediaGraphWidget::addNode(QPointF pos)
{
    Node *newNode = new WikiNode(this);
    newNode->setPos(pos);
    newNode->setIndex(_nodeList.size());
    _nodeList.append(newNode);
    _scene->addItem(newNode);
    return newNode;
}

Node *WikipediaGraphWidget::addNode(int x, int y)
{
    return addNode(QPoint(x, y));
}

void WikipediaGraphWidget::expand(Node *node)
{
    if(node->isExpanded()){
        return;
    }
    node->setExpanded();
    PageLoader *loader = new PageLoader(node->getData(), (void*)node, this);
    connect(loader, SIGNAL(finished(PageLoader*)), this, SLOT(parseLinks(PageLoader*)));
}

void WikipediaGraphWidget::parseLinks(PageLoader *pageLoader)
{
    QString pageString(pageLoader->getReply()->readAll());
    WikiNode *expandedNode = (WikiNode*)pageLoader->getItem();
    pageLoader->deleteLater();

    int start = pageString.indexOf("<div id=\"bodyContent\">");
    int end = pageString.indexOf("<ol class=\"references\">")+23 -pageString.indexOf("<div id=\"bodyContent\">");
    pageString = pageString.mid(start,end);
    int linkStartIndex;
    int linkEndIndex;
    QString workingString;
    QString result;
    for(int idx = 0; pageString.contains("<a href=\"/wiki/"); idx++)
    {

        linkStartIndex = pageString.indexOf("<a href=\"/wiki/")+15;
        workingString = pageString.mid(linkStartIndex,
                                        pageString.indexOf("<ol class=\"references\">")+23-linkStartIndex);
        linkEndIndex = workingString.indexOf("\" ");
        if(workingString.mid(0,linkEndIndex).contains(":") ||  expandedNode->countLinkedPages() >= _maxLinks){
            pageString = pageString.mid(linkStartIndex,pageString.indexOf("<ol class=\"references\">")+23-linkStartIndex);
            continue;
        }
        else{
            result = "http://en.wikipedia.org/wiki/"+workingString.mid(0,linkEndIndex);
            pageString = pageString.mid(linkStartIndex,pageString.indexOf("<ol class=\"references\">")+23-linkStartIndex);
            expandedNode->addLinkedPage(result);
        }
    }

    int numNodes = expandedNode->countLinkedPages();
    int idx = 0;
    bool connected;
    QList<QString> linkedPages = expandedNode->getLinkedPages();
    foreach(QString page, linkedPages) {
        connected = false;
        foreach(Node *otherNode, _nodeList){
            if(otherNode->getData()==page) {
                connectNodes(expandedNode, otherNode);
                connected = true;
                idx++;
                break;
            }
        }
        if(! connected){

            Node *newNode = addNode(expandedNode->x(), expandedNode->y());
            newNode->setData(page);
            newNode->setText(page.mid(page.lastIndexOf("/") + 1));
            newNode->setPos(QPoint(expandedNode->pos().x()+ qSin(2*3.14159/numNodes*idx)*50,
                          expandedNode->pos().y()+ qCos(2*3.14159/numNodes*idx)*50));
            connectNodes(expandedNode, newNode);
            idx ++;
        }
    }
    this->setUpdateVelocity(true);
}



void WikipediaGraphWidget::makeTestScene()
{
    Node *test = addNode(10, 10);
    QString text = QInputDialog::getText(this,
                                         tr("WikiGraph"),
                                         tr("Enter page to display"),QLineEdit::Normal,
                                         "http://en.wikipedia.org/wiki/Quicksort");
    test->setText(text.mid(text.lastIndexOf("/")+1));
    test->setData(text);//"http://en.wikipedia.org/wiki/Quicksort"
    int links = QInputDialog::getInt(this,
                                         tr("WikiGraph"),
                                         tr("Enter maximum number of pagelinks per article"),QLineEdit::Normal,
                                         5);
    _maxLinks = links;
    expand(test);
}

//void WikipediaGraphWidget::expandGeneration()
//{
//    foreach(Node *node, _nodeList){
//        expand(node);
//    }
//}

void WikipediaGraphWidget::setMaxLinks(int links)
{
    _maxLinks = links;
}
void WikipediaGraphWidget::expandGeneration()
{
    _expandPos = 0;
    _expandNum = 0;
    foreach(Node* node, _nodeList){
        if(!node->isExpanded()){
            _expandNum++;
        }
    }
    _expandTimer.start();
}

void WikipediaGraphWidget::expandNext()
{
    if(_expandPos  < _expandNum) {
        expand(_nodeList[_expandPos]);
        ++_expandPos;
    }
}




