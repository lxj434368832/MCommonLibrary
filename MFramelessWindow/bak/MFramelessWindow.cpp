#include "MFramelessWindow.h"
#include "ui_MFramelessWindow.h"

MFramelessWindow::MFramelessWindow(QWidget *parent) :
    MFramelessWidget(parent),
    ui(new Ui::MFramelessWindow)
{
    ui->setupUi(this);
}

MFramelessWindow::~MFramelessWindow()
{
    delete ui;
}

void MFramelessWindow::on_btnClose_clicked()
{
    close();
}
