#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QMouseEvent>
#include <QTimer>
#include <QVector2D>
#include <QtOpenGL/QGLWidget>
#include <QMutex>
#include <QPainter>
#include <QMessageBox>
#include <QThread>
#include <QGLFormat>

#include "node.h"
#include "nodeedge.h"
#include "graphlayout.h"

class ForceCalculator;

namespace Ui {
    class GraphWidget;
}

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    explicit GraphWidget(GraphLayout *layout, QWidget *parent = 0);
    QPoint mousePos;
    QPoint lastMousePos;
    ~GraphWidget();

    virtual Node *addNode(QPoint pos);
    virtual Node *addNode(int x, int y);
    virtual void connectNodes(Node *node1, Node *node2);
    bool _updateVelocity;
    double _minimumKE;
    void setUpdateVelocity(bool x);
    int countNodes() const;
    double getRepulsion() const;
    double getDamping() const;

public slots:
    void updateGraph();
    void addRandomNode();
    void toggleForceCalculation();
    void toggleRendering();
    void startForceCalculation();
    void startRendering();
    void stopForceCalculation();
    void stopRendering();

private:

    Ui::GraphWidget *ui;

protected:
    virtual void wheelEvent(QWheelEvent *event);
    void updateZoom();
    void setupThreads();
    QList<Node*> _nodeList;
    QList<Node*> _frameNodeList;
    QList<NodeEdge*> _edgeList;
    QTimer _renderTimer;
    bool _mousePressed;
    Node *_pressedNode;
    double _attraction;
    int _repulsion;
    double _damping;
    int _calculationIntervall;
    int _renderIntervall;
    int _slowUpdateIntervall;
    bool _isRendering;
    QPoint _mouseCords;
    qreal _zoomFactor;
    qreal _zoomSpeed;
    bool _updating;
    int _forceCalculations;
    QGraphicsScene *_scene;
    int _calculatePos;
    GraphLayout *_graphLayout;
    int _frameCounter;
    QImage _frameImage;
    QPainter *_imagePainter;
    QThread *_calculationThread;
    bool _isCalculating;

private slots:
    void showSize();

signals:
    void nodeDoubleClicked(Node *node);
    void calculateNode(Node *node);
    void updateStart(QList<Node *> *nodeList, QList<NodeEdge *> *edgeList);
};

#endif // GRAPHWIDGET_H
