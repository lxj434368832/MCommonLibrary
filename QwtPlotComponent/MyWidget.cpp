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
    //���ñ���
   ui->qwtPlot->setTitle (QStringLiteral("����ɼ����ݲ���ͼ"));
    //���û�ͼ��Ĵ�С
    ui->qwtPlot->resize(500,300);
    //����plot�ı���
    ui->qwtPlot->setCanvasBackground(Qt::green);
    //���������������
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, " System Uptime[yyyy-MM-dd\n hh:mm:ss]");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "V");

     ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom,new MyTimeScaleDraw());

    //����������ķ�Χ
     ui->qwtPlot->setAxisScale(QwtPlot::xBottom, QDateTime::currentDateTime().toTime_t() - 5, xMaxScale = QDateTime::currentDateTime().toTime_t());
     ui->qwtPlot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom);
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 255);

    //�����ұ߱�ע(ʾ��ͼʾ)
    ui->qwtPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    QwtScaleWidget *scaleWidget = ui->qwtPlot->axisWidget(QwtPlot::xBottom);//��������߾�
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);


    m_pMagnifier = new MPlotMagnifier(ui->qwtPlot->canvas());                  //ʹ�ù��ַŴ�/��С

    QwtPlotPanner *qwtPanner = new QwtPlotPanner(ui->qwtPlot->canvas());       //ʹ��������ƽ��
    connect(qwtPanner, SIGNAL(panned(int,int)), this, SLOT(slotCanvasMoved(int,int)));

    m_pPOCurve = new QwtPlotCurve(QStringLiteral("ADģ��ɼ�"));    //����
//    xs.append(0);
//    ys.append(0);
//    m_pPOCurve->setSamples(xs,ys);   //��������

    m_pPOCurve->setStyle(QwtPlotCurve::Lines);    //ֱ����ʽ

    m_pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬

    m_pPOCurve->setPen(QPen(Qt::red));    //���û���
    //�����߸��ӵ�plot
    m_pPOCurve->attach(ui->qwtPlot);

    //������ʱ����1����Ӧ������ģ�����ʵʱ����
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
