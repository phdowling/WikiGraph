#ifndef SLIDERSWIDGET_H
#define SLIDERSWIDGET_H

#include <QWidget>

namespace Ui {
    class SlidersWidget;
}

class SlidersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SlidersWidget(QWidget *parent = 0);
    ~SlidersWidget();

private:
    Ui::SlidersWidget *ui;

signals:
    void repulsionChange(double newRepulsion);
    void thetaChange(double newTheta);
    void attractionChange(double newAttraction);
    void dampingChange(double newDamping);
private slots:
    void on_repulsionSlider_valueChanged(int value);
    void on_attractionSlider_valueChanged(int value);
    void on_thetaSlider_valueChanged(int value);
    void on_dampingSlider_valueChanged(int value);
};

#endif // SLIDERSWIDGET_H
