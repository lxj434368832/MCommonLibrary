#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class cameras_widget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CreateDockWindows();

public slots:
    void slot_actionShowCameraWidget_checked(bool bcheck);

private:
    Ui::MainWindow *ui;
    cameras_widget *m_cam_widget = NULL;
};

#endif // MAINWINDOW_H
