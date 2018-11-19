#include "WidgetMain.h"
#include "ui_WidgetMain.h"

WidgetMain::WidgetMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WidgetMain)
{
    setWindowFlags(Qt::Widget);
    ui->setupUi(this);

}

WidgetMain::~WidgetMain()
{
    delete ui;
}
