#include "MainWindow.h"
#include "cameras_widget/cameras_widget.h"
#include "tk_signals_widget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cameras_widget w;
    w.show();
    w.add_user(12, 12, "yuannei1");
//    QObject::connect(&w, &cameras_widget::signal_position_changed, [=](int px, int py){
//        qDebug()<<"current point:("<<px<<","<<py<<").";
//    });

//    QObject::connect(&w, &cameras_widget::signal_size_changed, [=](int width, int height){
//        qDebug()<<"current size:("<<width<<","<<height<<").";
//    });

//    QObject::connect(&w, &cameras_widget::signal_camera_widget_size_change, [=](int width, int height){
//        qDebug()<<"current camera_widget size:("<<width<<","<<height<<").";
//    });

//    w.add_user(13, 13, "yuannei2");
//    MainWindow main;
//    main.slot_actionShowCameraWidget_checked(true);
//    tk_signals_widget w;
//    w.add_widget(1,"±¨¸æ", &main);
//    w.show();
//    w.CreateDockWindows();
//    main.show();
    return a.exec();
}
