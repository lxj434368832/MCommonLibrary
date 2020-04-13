#include "FormTestTabWidget.h"
#include "ui_FormTestTabWidget.h"
#include <QtDebug>

FormTestTabWidget::FormTestTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTestTabWidget)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
}

FormTestTabWidget::~FormTestTabWidget()
{
    delete ui;
}

void FormTestTabWidget::on_btnHideSecondTab_clicked()
{
    style()->unpolish(ui->tabWidget->tabBar());
    ui->tabWidget->setTabEnabled(1, false);
    /*ui->tabWidget->tabBar()->setTabText(1, "");
    QWidget *wnd = ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide);
    if(wnd)
    {
        wnd->hide();
    }
    else
    {
        qDebug()<<"widget is nullptr.";
    }*/
    style()->polish(ui->tabWidget->tabBar());
}
