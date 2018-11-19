#include "MTabWidget.h"
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget widget;
    widget.resize(800,600);
    QBoxLayout *layout =new QVBoxLayout(&widget);

    MTabWidget *tabWidget =new MTabWidget(&widget);
    tabWidget->setTabBarParam(26, 12);          //此为关键，设置tabBar字体的大小，和边框的大小
    tabWidget->setMinimumSize(403,300);
//    tabWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    tabWidget->setIconSize(QSize(20,20));
//    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->addTab(new QWidget(tabWidget), QIcon("D:\\ProgramSrc\\MCommonLibrary\\MFramelessWindow\\Resource\\NormalIcon.png"), "tab1");
    tabWidget->addTab(new QWidget(tabWidget), QIcon("D:\\ProgramSrc\\MCommonLibrary\\MFramelessWindow\\Resource\\NormalIcon.png"),
                      QString::fromLocal8Bit("tab2大家说法理解对方考虑"));
    tabWidget->addTab(new QWidget(tabWidget), QString::fromLocal8Bit("tab3范德萨发大水快啦"));
    layout->addWidget(tabWidget);

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
