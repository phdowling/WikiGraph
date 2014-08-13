#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "graphwidget.h"
#include "wikipediagraphwidget.h"
#include "fastlayout.h"
#include "directforcelayout.h"
#include "barneshutlayout.h"
#include "sliderswidget.h"



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GraphWidget *_graph;
    QGridLayout *_layout;
    SlidersWidget *_sliders;
};

#endif // MAINWINDOW_H
