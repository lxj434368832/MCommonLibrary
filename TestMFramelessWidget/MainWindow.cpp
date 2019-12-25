#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "cameras_widget/cameras_widget.h"
#include "DialogTestMaximum.h"

#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::CustomizeWindowHint);
    ui->setupUi(this);
    connect(ui->actionShowCameraWidget, SIGNAL(triggered(bool)), this,SLOT(slot_actionShowCameraWidget_checked(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Customers"),this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget *customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
                           <<"jfwoefjdlsfjfjdasfjoajfdosjafojueworumfdas"
                           <<"fjeowfdadlsajfd fdjasofjkdas jlfkdjasoirj fdas"
                           <<"fjdasoruiewkfdjasfoqewjfjfjaojfdlasfdkslajf fafds");
    dock->setWidget(customerList);
//    ui->verticalLayout->addWidget(dock);
    addDockWidget(Qt::BottomDockWidgetArea,dock);
//    ui->menuView->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Paragraphs"), this);
    QListWidget *paragraphList = new QListWidget(dock);
    paragraphList->addItems(QStringList()
                            <<"fjowejrfldsajfdklsa fqewo fjdlas jafjdlsajf"
                            <<"fjdsoau jfodajfk ewoqfjcdas jfoiqjwro324"
                            <<"fjoiqa rj0235r90mfdas;l mr903eiqrjfkldsaj rfdsla");
    dock->setWidget(paragraphList);
//    ui->verticalLayout->addWidget(dock);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
//    ui->menuView->addAction(dock->toggleViewAction());

}

void MainWindow::slot_actionShowCameraWidget_checked(bool bcheck)
{
    if(NULL == m_cam_widget)
    {
        m_cam_widget = new cameras_widget(this);
        m_cam_widget->set_camera_start_position(2739,581);
        m_cam_widget->set_camera_widget_size(553 , 391);
    }

    if(bcheck)
    {
        m_cam_widget->on_camera_list_switch(true);
            m_cam_widget->add_user(12, 12, "yuannei1");
            m_cam_widget->add_user(13, 13, "yuannei2");
    }
    else
        m_cam_widget->on_camera_list_switch(false);
}

void MainWindow::on_btnOpenDialog_clicked()
{
    DialogTestMaximum dlg;
    dlg.exec();
}
