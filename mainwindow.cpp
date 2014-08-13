#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    BarnesHutLayout *bhl = new BarnesHutLayout;
    _graph = new WikipediaGraphWidget(bhl, this);
    this->setCentralWidget(_graph);
    _sliders = new SlidersWidget;
    connect(_sliders, SIGNAL(repulsionChange(double)), bhl, SLOT(setRepulsion(double)));
    connect(_sliders, SIGNAL(thetaChange(double)), bhl, SLOT(setTheta(double)));
    connect(_sliders, SIGNAL(attractionChange(double)), bhl, SLOT(setAttraction(double)));
    connect(_sliders, SIGNAL(dampingChange(double)), bhl, SLOT(setDamping(double)));
    _sliders->show();

    connect(ui->actionAdd_Node, SIGNAL(triggered()), _graph, SLOT(addRandomNode()));
    connect(ui->actionToggle_force_calculation, SIGNAL(triggered()), _graph, SLOT(toggleForceCalculation()));
    connect(ui->actionExpand_by_generation, SIGNAL(triggered()), _graph, SLOT(expandGeneration()));
    connect(ui->actionToggle_rendering, SIGNAL(triggered()), _graph, SLOT(toggleRendering()));
    connect(ui->actionShow_graph_size, SIGNAL(triggered()), _graph, SLOT(showSize()));


}

MainWindow::~MainWindow()
{
    delete ui;
}
