/********************************************************************************
** Form generated from reading UI file 'ReadWriteExcel.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_READWRITEEXCEL_H
#define UI_READWRITEEXCEL_H

#include <ActiveQt/QAxWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReadWriteExcel
{
public:
    QWidget *centralWidget;
    QPushButton *btnReadExcel;
    QAxWidget *axWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ReadWriteExcel)
    {
        if (ReadWriteExcel->objectName().isEmpty())
            ReadWriteExcel->setObjectName(QStringLiteral("ReadWriteExcel"));
        ReadWriteExcel->resize(400, 300);
        centralWidget = new QWidget(ReadWriteExcel);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btnReadExcel = new QPushButton(centralWidget);
        btnReadExcel->setObjectName(QStringLiteral("btnReadExcel"));
        btnReadExcel->setGeometry(QRect(30, 20, 91, 23));
        axWidget = new QAxWidget(centralWidget);
        axWidget->setObjectName(QStringLiteral("axWidget"));
        axWidget->setProperty("geometry", QVariant(QRect(190, 10, 80, 70)));
        ReadWriteExcel->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ReadWriteExcel);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        ReadWriteExcel->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ReadWriteExcel);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ReadWriteExcel->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ReadWriteExcel);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ReadWriteExcel->setStatusBar(statusBar);

        retranslateUi(ReadWriteExcel);

        QMetaObject::connectSlotsByName(ReadWriteExcel);
    } // setupUi

    void retranslateUi(QMainWindow *ReadWriteExcel)
    {
        ReadWriteExcel->setWindowTitle(QApplication::translate("ReadWriteExcel", "ReadWriteExcel", 0));
        btnReadExcel->setText(QApplication::translate("ReadWriteExcel", "\350\257\273\345\217\226excel", 0));
    } // retranslateUi

};

namespace Ui {
    class ReadWriteExcel: public Ui_ReadWriteExcel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_READWRITEEXCEL_H
