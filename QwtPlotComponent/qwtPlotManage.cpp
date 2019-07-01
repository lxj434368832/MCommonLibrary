#include "qwtPlotManage.h"
#include "MCustomAssembly.h"
#include "MCurveData.h"
#include "MPlotMagnifier.h"
#include "QwtPlotDefine.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QTimerEvent>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_symbol.h>


class qwtPlotManage::PrivateData
{
public:
	MPlotMagnifier		*pMagnifier = nullptr;
	QwtPlotCurve		*pPOCurve = nullptr;
	MCurveData			*pWaveData = nullptr;

	QwtPlotDirectPainter	*pDirectPainter = nullptr;
    QwtInterval				interval;

    int						timerId = 0;
	QElapsedTimer			elapsed;
	unsigned				uPlotIntervalCount = 0;
	EDrawStatus				eStatus = EDS_UNKNOWN;

	~PrivateData()
    {
		if (pDirectPainter)
		{
			delete pDirectPainter;
			pDirectPainter = nullptr;
		}
	}
};

qwtPlotManage::qwtPlotManage(QwtPlot *plot) :
    QObject(plot)
{
    m_pPlot = plot;
	data = new PrivateData;
	data->pWaveData = new MCurveData();
	data->pDirectPainter = new QwtPlotDirectPainter;
	data->interval.setMaxValue(5000);
	data->elapsed.start();
}

qwtPlotManage::~qwtPlotManage()
{
    m_pPlot = nullptr;
	delete data;
	data = nullptr;
}

void qwtPlotManage::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
	data->pWaveData->SetParam(uSamplingRate, dXAxisWidth);
	dXAxisWidth *= 1000;
	if (dXAxisWidth > 1000 && dXAxisWidth != data->interval.width() )
	{
        data->interval.setMaxValue(data->interval.minValue() + dXAxisWidth);
        m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	}

    unsigned uIntervalTime = data->pWaveData->GetTimeInterval();
    unsigned uPlotIntervalCount = round( uPlotIntervalTime / uIntervalTime);
    if(uPlotIntervalCount > 0 && uPlotIntervalCount != data->uPlotIntervalCount)
    {
        data->uPlotIntervalCount = uPlotIntervalCount;
    }

    if(data->timerId)   //如果开启了定时器，则重置
    {
        data->elapsed.restart();
        killTimer(data->timerId);
        data->timerId = startTimer( uIntervalTime );
    }
	
    if (EDS_UNKNOWN != data->eStatus)
	{
		m_pPlot->replot();
	}
}

void qwtPlotManage::BuildPlot()
{
    InitPlot();
    //AddPlotGrid();
    //AddLegend();
    //AddPanner();
    AddWaveCurve();
    data->eStatus = EDS_STOP;
}

void qwtPlotManage::Start()
{
    if (EDS_START != data->eStatus)
	{
        data->eStatus = EDS_START;
		data->elapsed.restart();
		data->timerId = startTimer(data->pWaveData->GetTimeInterval());
	}
}

void qwtPlotManage::Stop()
{
    if (EDS_STOP != data->eStatus)
	{
        data->eStatus = EDS_STOP;
		killTimer(data->timerId);
		data->timerId = 0;
		data->pWaveData->Reset();
	}
}

void qwtPlotManage::AddWaveData(QList<unsigned char> list)
{
    data->pWaveData->AddPendingValues(list);
}

void qwtPlotManage::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	data->eStatus = EDS_HISTORY;
    data->pWaveData->AddHistoryData(ulStartTime, list);
    //刷新坐标轴
    QwtInterval interval =  data->pWaveData->GetXAxisInterval();

    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
    //重绘
    m_pPlot->replot();
}

void qwtPlotManage::InitPlot()
{
    //设置标题
//    m_pPlot->setTitle("My Project");
    //设置画布或背景
    //    m_pPlot->setCanvas( new MCanvas() );
    m_pPlot->setCanvasBackground(Qt::white);
    //设置坐标轴的名称
//    m_pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X轴"));
//    m_pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y轴"));

    //设置坐标轴标尺绘制
    m_pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

    //设置坐标轴的范围
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
//	m_pPlot->setAxisAutoScale(QwtPlot::xBottom);
    m_pPlot->enableAxis(QwtPlot::xBottom, false);

    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, 255);
    m_pPlot->enableAxis(QwtPlot::yLeft, false);
}

void qwtPlotManage::AddPlotGrid()
{
    // 设置网格
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( true );
    //    grid->setMajorPen(Qt::black,0,Qt::SolidLine);
    grid->attach(m_pPlot);
}

void qwtPlotManage::AddPlotMarker()
{
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = PI/2"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Horizontal);
    mX->setLineStyle( QwtPlotMarker::Cross );
    mX->setLinePen(QPen(Qt::black, 1, Qt::DashDotDotLine));
    QwtInterval xAxisInterval = data->pWaveData->GetXAxisInterval();
    mX->setValue( (xAxisInterval.minValue() + xAxisInterval.maxValue()) /  2.0, 255.0 / 2 );
    mX->attach(m_pPlot);
}

void qwtPlotManage::AddLegend()
{
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void qwtPlotManage::AddMagnifier()
{
    data->pMagnifier = new MPlotMagnifier(m_pPlot->canvas());
}

void qwtPlotManage::AddPanner()
{
    QwtPlotPanner *qwtPanner = new QwtPlotPanner(m_pPlot->canvas());       //使用鼠标左键平移
    qwtPanner->setOrientations(Qt::Horizontal);
    connect(qwtPanner, SIGNAL(panned(int,int)), this, SLOT(slotCanvasMoved(int,int)));
}

void qwtPlotManage::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( m_pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void qwtPlotManage::AddWaveCurve()
{
    data->pPOCurve = new QwtPlotCurve("DescribeWave");    //设置曲线名称
    data->pPOCurve->setStyle(QwtPlotCurve::Lines);    //直线形式
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
    data->pPOCurve->setPen(QPen(Qt::red));    //设置画笔
    data->pPOCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    //data->pPOCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pPOCurve->setData(data->pWaveData);
    //把曲线附加到plot
    data->pPOCurve->attach(m_pPlot);
}

void qwtPlotManage::UpdateCurve(qint64 elapsed)
{
    static unsigned uCount = 0;

    //重新加载数据
    if(false == data->pWaveData->UpdateCurveData(elapsed)) return;

    //QwtPlot重绘
    if(0 == uCount++ % data->uPlotIntervalCount)
	{	
		//刷新坐标轴
		data->interval = data->pWaveData->GetXAxisInterval();
		m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
		//m_pPlot->updateAxes();
        m_pPlot->replot();
		//qDebug() << QString("current interval minValue:%1 max:%2 width:%3").
		//	arg(data->interval.minValue()).arg(data->interval.maxValue()).arg(data->interval.width());
        /*
        const bool doClip = !m_pPlot->canvas()->testAttribute( Qt::WA_PaintOnScreen );
        if ( doClip )
        {
            const QwtScaleMap xMap = m_pPlot->canvasMap( data->pPOCurve->xAxis() );
            const QwtScaleMap yMap = m_pPlot->canvasMap( data->pPOCurve->yAxis() );

            QRectF br = qwtBoundingRect( *data->pWaveData, 0, data->pWaveData->size() - 1 );

            const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
            data->pDirectPainter->setClipRegion( clipRect );
        }*/

        //data->pDirectPainter->drawSeries( data->pPOCurve,  0, data->pWaveData->size() - 1 );
    }
}

void qwtPlotManage::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == data->timerId )
    {
        UpdateCurve(data->elapsed.elapsed());
        return;
    }

    QObject::timerEvent( event );
}

void qwtPlotManage::slotCanvasMoved(int dx, int dy)
{
    qDebug()<<"current dx:"<<dx<<" dy:"<<dy;
}
