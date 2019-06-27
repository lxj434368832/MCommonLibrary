#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <qwt_plot_curve.h>

namespace Ui {
class MyWidget;
}

class MPlotMagnifier;

class MyWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    void init();
    void initPOWave();

private slots:
    void on_btnStart_clicked();
    void realTimePlot();
    void slotCanvasMoved(int dx, int dy);
private:
    Ui::MyWidget *ui;
    QVector<double> xs;
    QVector<double> ys;
    QwtPlotCurve    *m_pPOCurve;

    MPlotMagnifier  *m_pMagnifier;
    QTimer *randPlotTimer;
    QVector<QPointF>  points;
    int xMaxScale;
};

#endif // MAINWINDOW_H
