#include "MMainWindow.h"
#include "WidgetMain.h"
#include "ui_MMainWindow.h"

MMainWindow::MMainWindow(QWidget *parent) :
    MFramelessWidget(parent),
    ui(new Ui::MMainWindow)
{
    ui->setupUi(this);
    m_widgetMain = new WidgetMain;
    ui->layoutWidgetmain->addWidget(m_widgetMain);
}

MMainWindow::~MMainWindow()
{
    delete ui;
}

void MMainWindow::on_btnClose_clicked()
{
    close();
}
