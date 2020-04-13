#include "MCustomTabStyle.h"
#include "MTabWidgetV.h"
#include "ui_MTabWidgetV.h"

MTabWidgetV::MTabWidgetV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidgetV)
{
    ui->setupUi(this);

    MCustomTabBarStyle *style = new MCustomTabBarStyle(120, 39);
    style->setColor(0x646464, 0x323232, 0x1d1d1d);
    ui->tabWidget->tabBar()->setStyle(style);
    ui->tabWidget->setTabIcon(0, QIcon(":/Resource/appoint_flag.png"));
}

MTabWidgetV::~MTabWidgetV()
{
    delete ui;
}
