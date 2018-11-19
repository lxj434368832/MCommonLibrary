#include "WidgetVolumeSlider.h"
#include "ui_WidgetVolumeSlider.h"

WidgetVolumeSlider::WidgetVolumeSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetVolumeSlider)
{
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
}

WidgetVolumeSlider::~WidgetVolumeSlider()
{
    delete ui;
}

void WidgetVolumeSlider::on_sliderVolumeValue_valueChanged(int value)
{
    ui->lbVolumeValue->setText(QString::number(value).append("%"));
    emit signalVolumeChanged(value);
}
