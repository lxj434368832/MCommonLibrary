#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QElapsedTimer>
#include <memory>

namespace Ui {
class MainWindow;
}

class PlotBloodOxygenWave;
class MSamplingThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();
    void on_btnConfirmModify_clicked();

	void on_btnDrawHistory_clicked();//��������֪ͨ

private:
    Ui::MainWindow *ui;
    PlotBloodOxygenWave					*m_pPlotMng;
    std::shared_ptr<MSamplingThread>	m_thSampling;
};

#endif // MAINWINDOW_H
