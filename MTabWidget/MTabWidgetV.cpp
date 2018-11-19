#include "CustomTabStyle.h"
#include "MTabWidgetV.h"
#include "ui_MTabWidgetV.h"

MTabWidgetV::MTabWidgetV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidgetV)
{
    ui->setupUi(this);

    CustomTabStyle *style = new CustomTabStyle(150, 60);
    style->setColor(0x646464, 0x323232, 0x101010);
    ui->tabWidget->tabBar()->setStyle(style);
}

MTabWidgetV::~MTabWidgetV()
{
    delete ui;
}
