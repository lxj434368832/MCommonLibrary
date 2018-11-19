#include "MTabWidget.h"
#include "ui_MTabWidget.h"
#include "CustomTabStyle.h"

MTabWidget::MTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidget)
{
    ui->setupUi(this);

    CustomTabStyle *style = new CustomTabStyle(150, 60);
    style->setColor(0x646464, 0x323232, 0x101010);
    ui->tabWidget->tabBar()->setStyle(style);
}

MTabWidget::~MTabWidget()
{
    delete ui;
}
