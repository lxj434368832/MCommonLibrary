#include "MTabWidget.h"
#include "ui_MTabWidget.h"

MTabWidget::MTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidget)
{
    ui->setupUi(this);
}

MTabWidget::~MTabWidget()
{
    delete ui;
}
