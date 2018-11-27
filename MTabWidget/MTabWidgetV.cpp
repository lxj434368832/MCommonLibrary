#include "MCustomTabStyle.h"
#include "MTabWidgetV.h"
#include "ui_MTabWidgetV.h"

MTabWidgetV::MTabWidgetV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidgetV)
{
    ui->setupUi(this);

    MCustomTabStyle *style = new MCustomTabStyle(120, 39);
    style->setColor(0x646464, 0x323232, 0x101010);
    ui->tabWidget->tabBar()->setStyle(style);
}

MTabWidgetV::~MTabWidgetV()
{
    delete ui;
}
