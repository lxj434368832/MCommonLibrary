#include "DialogTestMaximum.h"
#include "ui_DialogTestMaximum.h"

DialogTestMaximum::DialogTestMaximum(QWidget *parent) :
    MFramelessDialog(parent),
    ui(new Ui::DialogTestMaximum)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::Dialog);
}

DialogTestMaximum::~DialogTestMaximum()
{
    delete ui;
}
