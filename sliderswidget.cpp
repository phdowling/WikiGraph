#include "sliderswidget.h"
#include "ui_sliderswidget.h"

SlidersWidget::SlidersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlidersWidget)
{
    ui->setupUi(this);
}

SlidersWidget::~SlidersWidget()
{
    delete ui;
}

void SlidersWidget::on_repulsionSlider_valueChanged(int value)
{
   repulsionChange((double)value);
}

void SlidersWidget::on_dampingSlider_valueChanged(int value)
{
   dampingChange(/*100-*/(double)value/100);
}
void SlidersWidget::on_attractionSlider_valueChanged(int value)
{
   attractionChange((double)value/1000);
}
void SlidersWidget::on_thetaSlider_valueChanged(int value)
{
   thetaChange((double)value/100);
}
