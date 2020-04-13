#include "MTabWidget.h"
#include "MTabWidgetV.h"
#include "MTabWidgetLR.h"
#include "FormTestTabWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormTestTabWidget widget;
    /*QObject::connect(&widget, &MTabWidgetV::currentChanged, [=](int idx){
        qDebug()<<"current index changed:"<<idx;
    });
    int idx = widget.addTab(new QWidget(), QString::fromLocal8Bit("放肌肤抵抗力"));
    qDebug()<<"widget index:"<<idx;
    idx = widget.addTab(new QWidget(), QString::fromLocal8Bit("发送记录了"));
    qDebug()<<"widget index:"<<idx;
    idx = widget.addTab(new QLabel(QString::fromLocal8Bit("收到了尽大方大方快")), QString::fromLocal8Bit("收代收代付到了尽快"));
    qDebug()<<"widget index:"<<idx;
    widget.setCurrentIndex(2);*/
    widget.show();

    return a.exec();
}

/*QTabWidget stylesheet
QTabWidget::pane{
    border:0px;
    border-radius:2px;
    background-color:#0e0e0f;
    border-top: 4px solid #C2C7CB;
}

QTabWidget::tab-bar{
    left: 0px;
}

QTabBar::tab{
    background:rgb(100, 100, 100);
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
    border:none;
    color:white;
    font-family:"Microsoft YaHei";
    padding:2px;
}
QTabBar::tab:hover{
    background:rgb(50, 50, 50);
}

QTabBar::tab:selected{
    background:rgb(0, 0, 0);
    border-color: #9B9B9B;
  border-bottom-color: #C2C7CB;
}

QTabBar::tab:!selected{
    border-left: 1px solid black;
    border-right: 1px solid black;
    margin-top: 2px;
}*/
