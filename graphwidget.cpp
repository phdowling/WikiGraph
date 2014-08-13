#include "graphwidget.h"
#include "ui_graphwidget.h"


GraphWidget::GraphWidget(GraphLayout *layout, QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::GraphWidget),
    _graphLayout(layout)
{
//    _background = QBrush(Qt::white);
    this->lower();
    _scene = new QGraphicsScene(this);
    setScene(_scene);
    setSceneRect(-100000, -100000, 200000, 200000);
    setDragMode(QGraphicsView::ScrollHandDrag);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    setAttribute(Qt::WA_NoSystemBackground);
    centerOn(0, 0);

    _isCalculating = false;
    _frameCounter = 0;
    _attraction = 0.00001;//0.00018;
    _repulsion = 2.3;
    _damping = 0.98;
    _calculationIntervall = 10;
    _renderIntervall = 10;
    _slowUpdateIntervall = 70;
    _minimumKE = 1;
    _updateVelocity = true;
    _mousePressed = false;
    _zoomFactor = 1;
    _zoomSpeed = 1.2;
    _mouseCords = QPoint(0,0);
    _isRendering = true;
    ui->setupUi(this);
    updateZoom();

    _frameImage = QImage(1680, 1050, QImage::Format_ARGB32);
    _imagePainter = new QPainter(&_frameImage);

    _calculationThread = new QThread(this);
//    _graphLayout->moveToThread(_calculationThread);



    connect(_graphLayout, SIGNAL(updateFinished()), this, SLOT(updateGraph()));
    connect(this, SIGNAL(updateStart(QList<Node*>*,QList<NodeEdge*>*)), _graphLayout, SLOT(update(QList<Node*>*,QList<NodeEdge*>*)), Qt::QueuedConnection);

    startForceCalculation();
    startRendering();
}

GraphWidget::~GraphWidget()
{
    delete ui;
    delete _graphLayout;
}

void GraphWidget::startForceCalculation()
{
    setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    _isCalculating = true;
    updateGraph();
}

void GraphWidget::updateGraph()
{

    updateStart( &_nodeList, &_edgeList);
}

Node *GraphWidget::addNode(QPoint pos)
{
    Node *newNode = new Node;
    newNode->setPos(pos);
    _nodeList.append(newNode);
    newNode->setIndex(_nodeList.size());
    _scene->addItem(newNode);

    return newNode;
}

Node *GraphWidget::addNode(int x, int y)
{
    return addNode(QPoint(x, y));
}

void GraphWidget::addRandomNode()
{
    connectNodes(_nodeList[0], addNode(50, 50));
}

void GraphWidget::toggleForceCalculation()
{
    if(_isCalculating) {
        stopForceCalculation();
    } else {
        startForceCalculation();
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0) {
        // zoom in
        _zoomFactor *= _zoomSpeed;
    } else {
        // zoom out
        _zoomFactor /= _zoomSpeed;
    }
    updateZoom();
    updateGraph();
}

void GraphWidget::updateZoom()
{
    QMatrix matrix;
    matrix.scale(_zoomFactor, _zoomFactor);
    setMatrix(matrix);
}

void GraphWidget::connectNodes(Node *node1, Node *node2)
{
    NodeEdge *newEdge = new NodeEdge(node1, node2, _attraction);
    foreach(Edge *edge, _edgeList) {
        if(*newEdge == *edge) {
            delete newEdge;
            return;
        }
    }

    _edgeList.append(newEdge);
    node1->attachEdge(newEdge);
    node2->attachEdge(newEdge);
    _scene->addItem(newEdge);
}

void GraphWidget::showSize()
{
    QMessageBox::information(this, "Graph Size", QString::number(_nodeList.size()));
}

void GraphWidget::setUpdateVelocity(bool x){
    _updateVelocity = x;
}

void GraphWidget::toggleRendering()
{
    _isRendering = !_isRendering;
}

int GraphWidget::countNodes() const
{
    return _frameNodeList.size();
}

double GraphWidget::getRepulsion() const
{
    return _repulsion;
}

double GraphWidget::getDamping() const
{
    return _damping;
}

void GraphWidget::startRendering()
{
    connect(&_renderTimer, SIGNAL(timeout()), _scene, SLOT(update()));
    _renderTimer.start(20);
}

void GraphWidget::stopForceCalculation()
{
    disconnect(_graphLayout);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GraphWidget::stopRendering()
{
    _renderTimer.disconnect();
}

