#include "FormImageViewer.h"
#include "ui_FormImageViewer.h"

FormImageViewer::FormImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImageViewer)
{
    ui->setupUi(this);
}

FormImageViewer::~FormImageViewer()
{
    delete ui;
}
