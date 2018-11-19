#include "MTabWidget.h"
#include "ui_MTabWidget.h"
#include "CustomTabStyle.h"

MTabWidget::MTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidget)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
}

MTabWidget::~MTabWidget()
{
    delete ui;
}
