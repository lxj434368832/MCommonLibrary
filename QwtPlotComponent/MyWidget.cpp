#include "MyWidget.h"
#include "ui_MyWidget.h"
#include "MPlotMagnifier.h"
#include <qwt_legend.h>
#include <qwt_plot_panner.h>
#include <qwt_symbol.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <QTime>
#include <QTimer>
#include <QDebug>

#define SHOW_SECONDS 5

class MyTimeScaleDraw: public QwtScaleDraw
{
public:
	MyTimeScaleDraw()
    {
    }
    virtual QwtText label( double v ) const
    {
        QDateTime dt;
        dt.setTime_t((int)v);
        return dt.toString("yyyy-MM-dd\n hh:mm:ss");
    }
private:
    QDateTime baseTime;
};

MyWidget::MyWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    init();
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::init()
{
    //设置标题
   ui->qwtPlot->setTitle (QStringLiteral("按秒采集数据波形图"));
    //设置画图板的大小
    ui->qwtPlot->resize(500,300);
    //设置plot的背景
    ui->qwtPlot->setCanvasBackground(Qt::green);
    //设置坐标轴的名称
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, " System Uptime[yyyy-MM-dd\n hh:mm:ss]");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "V");

     ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom,new MyTimeScaleDraw());

    //设置坐标轴的范围
     ui->qwtPlot->setAxisScale(QwtPlot::xBottom, QDateTime::currentDateTime().toTime_t() - 5, xMaxScale = QDateTime::currentDateTime().toTime_t());
     ui->qwtPlot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 255);

    //设置右边标注(示例图示)
    ui->qwtPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    QwtScaleWidget *scaleWidget = ui->qwtPlot->axisWidget(QwtPlot::xBottom);//设置坐标边距
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);


    m_pMagnifier = new MPlotMagnifier(ui->qwtPlot->canvas());                  //使用滚轮放大/缩小

    QwtPlotPanner *qwtPanner = new QwtPlotPanner(ui->qwtPlot->canvas());       //使用鼠标左键平移
    connect(qwtPanner, SIGNAL(panned(int,int)), this, SLOT(slotCanvasMoved(int,int)));

    m_pPOCurve = new QwtPlotCurve(QStringLiteral("AD模拟采集"));    //线名
//    xs.append(0);
//    ys.append(0);
//    m_pPOCurve->setSamples(xs,ys);   //设置数据

    m_pPOCurve->setStyle(QwtPlotCurve::Lines);    //直线形式

    m_pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑

    m_pPOCurve->setPen(QPen(Qt::red));    //设置画笔
    //把曲线附加到plot
    m_pPOCurve->attach(ui->qwtPlot);

    //启动定时器，1秒响应，用于模拟产生实时数据
    randPlotTimer = new QTimer();
//    randPlotTimer->start(30);
    connect(randPlotTimer, SIGNAL(timeout()),this,SLOT(realTimePlot()));
}

void MyWidget::initPOWave()
{
}

void MyWidget::on_btnStart_clicked()
{
    if("stop" == ui->btnStart->text()  )
    {
        randPlotTimer->stop();
        ui->btnStart->setText("start");
    }
    else
    {
        randPlotTimer->start(40);
        ui->btnStart->setText("stop");
    }
}

void MyWidget::realTimePlot()
{
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, QDateTime::currentDateTime().toTime_t()-5, xMaxScale = QDateTime::currentDateTime().toTime_t());
        //qDebug()<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"sec";
        points << QPointF(QDateTime::currentDateTime().toTime_t(), rand()%255);
        m_pPOCurve->setSamples(points);
        m_pPOCurve->attach(ui->qwtPlot);
        ui->qwtPlot->replot();

}

void MyWidget::slotCanvasMoved(int dx, int dy)
{
    qDebug()<<"current dx:"<<dx<<" dy:"<<dy;
}
