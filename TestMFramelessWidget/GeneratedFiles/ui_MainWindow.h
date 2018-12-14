/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionviewMenu;
    QAction *actionhideMenu;
    QAction *actionShowCameraWidget;
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 284);
        actionviewMenu = new QAction(MainWindow);
        actionviewMenu->setObjectName(QStringLiteral("actionviewMenu"));
        actionhideMenu = new QAction(MainWindow);
        actionhideMenu->setObjectName(QStringLiteral("actionhideMenu"));
        actionShowCameraWidget = new QAction(MainWindow);
        actionShowCameraWidget->setObjectName(QStringLiteral("actionShowCameraWidget"));
        actionShowCameraWidget->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(9, 9, 84, 16));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuFile->menuAction());
        menuView->addAction(actionShowCameraWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionviewMenu->setText(QApplication::translate("MainWindow", "\346\237\245\347\234\213\350\217\234\345\215\225", 0));
        actionhideMenu->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217\350\217\234\345\215\225", 0));
        actionShowCameraWidget->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\347\233\270\346\234\272\347\252\227\344\275\223", 0));
        label->setText(QApplication::translate("MainWindow", "\344\273\245\344\270\213\344\270\272\345\201\234\351\235\240\347\252\227\345\217\243", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "\346\237\245\347\234\213", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
