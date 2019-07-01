#include "PlotBloodOxygenWave.h"
#include "MCustomAssembly.h"
#include "MCycleCurveData.h"
#include "MPlotMagnifier.h"
#include "QwtPlotDefine.h"
#include "MPlotWaveCurve.h"
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
#include <qwt_picker_machine.h>
#include <windows.h>

void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

class PlotBloodOxygenWave::PrivateData
{
public:
	MPlotMagnifier		*pMagnifier = nullptr;
    QwtPanner                 *pPanner = nullptr;
	MPlotWaveCurve		*pPOCurve = nullptr;
	MCycleCurveData		*pWaveData = nullptr;

	QwtPlotDirectPainter	*pDirectPainter = nullptr;
    QwtInterval				interval;
	unsigned				m_uSamplingRate = 33;

    int						DrawTimerId = 0;
	HANDLE					hTimer = NULL;
	QElapsedTimer			elapsed;
	unsigned				uPlotIntervalCount = 0;
	EDrawStatus				eStatus = EDS_UNKNOWN;
	unsigned				uReservePointRatio = 66;

	~PrivateData()
    {
		if (pDirectPainter)
		{
			delete pDirectPainter;
			pDirectPainter = nullptr;
		}
	}
};

PlotBloodOxygenWave::PlotBloodOxygenWave(QwtPlot *plot) :
    QObject(plot)
{
    m_pPlot = plot;
	m_uDrawCounter = 0;
    m_uDataCheckCounter = 0;
    m_uDataCheckInterval = 30;
	data = new PrivateData;
	data->pWaveData = new MCycleCurveData();
	data->pDirectPainter = new QwtPlotDirectPainter;
	data->interval.setMaxValue(5000);
	data->elapsed.start();

    connect(this, SIGNAL(signalReplot()), m_pPlot, SLOT(replot()));
}

PlotBloodOxygenWave::~PlotBloodOxygenWave()
{
    m_pPlot = nullptr;
	delete data;
	data = nullptr;
}

void PlotBloodOxygenWave::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
	data->m_uSamplingRate = uSamplingRate;
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

	if (data->hTimer)
    {
        ::ChangeTimerQueueTimer(NULL, data->hTimer, uIntervalTime, uIntervalTime);
    }

    if(data->DrawTimerId)   //如果开启了绘制定时器，则重置
    {
        killTimer(data->DrawTimerId);
        data->DrawTimerId = startTimer( uIntervalTime );
    }

    if (EDS_UNKNOWN != data->eStatus)
	{
		m_pPlot->replot();
	}
}

void PlotBloodOxygenWave::BuildPlot()
{
    InitPlot();
    //AddPlotGrid();
//    AddLegend();
    //AddPanner();
	AddPicker();
    AddWaveCurve();
    data->eStatus = EDS_STOP;
}

void PlotBloodOxygenWave::Start()
{
    if (EDS_START != data->eStatus)
	{
        data->eStatus = EDS_START;
		data->elapsed.restart();
		unsigned uTimerInterval = data->pWaveData->GetTimeInterval();
		if (!::CreateTimerQueueTimer(&data->hTimer, NULL, TimerCallback, this, uTimerInterval, uTimerInterval, WT_EXECUTEDEFAULT))
		{
            qDebug() << QString::fromLocal8Bit("创建脉氧波取值定时器失败！");
			data->hTimer = nullptr;
		}
		data->DrawTimerId = startTimer(uTimerInterval);
	}
}

void PlotBloodOxygenWave::Stop()
{
    if (EDS_START == data->eStatus)
	{
        data->eStatus = EDS_STOP;
		if (data->hTimer)
			::DeleteTimerQueueTimer(nullptr, data->hTimer, INVALID_HANDLE_VALUE);
        data->hTimer = nullptr;

		killTimer(data->DrawTimerId);
		data->DrawTimerId = 0;
	}
}

void PlotBloodOxygenWave::Reset()
{
	data->pWaveData->Reset();
	data->interval = data->pWaveData->GetXAxisInterval();
	m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
}

void PlotBloodOxygenWave::AddWaveData(QList<unsigned char> list)
{
	if (EDS_START != data->eStatus) return;
	unsigned iSize = data->pWaveData->AddPendingValues(list);

    if (0 != m_uDataCheckCounter++ % m_uDataCheckInterval) return;			//30s检测一次
	double dValue = (double)iSize / data->m_uSamplingRate;
    qDebug() << QString::fromLocal8Bit("当前缓存的数据为:%1个，比值:%2")
        .arg(iSize).arg(dValue);

	if (dValue < 1.5)	//实际只剩10条数据
	{
		if (1 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 1;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() + 2;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 2.1) //只有1秒以内数据
	{
		if (2 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 2;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() + 1;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 3.1) //只有2秒以内数据
	{
		if (3 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 3;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval();
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 4.1) //只有3秒以内数据
	{
		if (4 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 4;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 1;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
		}
	}
    else if (dValue < 5.1) //实际大于4秒数据
	{
        if( 5 != data->uReservePointRatio)
        {
            data->uReservePointRatio = 5;
            m_uDataCheckInterval = 30;
            unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 2;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
        }
	}
    else if(10 != data->uReservePointRatio)
    {
        data->uReservePointRatio = 10;
        m_uDataCheckInterval = 6;
        unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 10;
        ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
        qDebug() << QString::fromLocal8Bit("当前取值间隔为:%1").arg(uTimeInterval);
    }
}

void PlotBloodOxygenWave::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	data->eStatus = EDS_HISTORY;
    data->pWaveData->AddHistoryData(ulStartTime, list);
    //刷新坐标轴
    QwtInterval interval = data->pWaveData->GetXAxisInterval();

    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	data->pPOCurve->SetCurrentPoint(-1);
    //重绘
    m_pPlot->replot();
}

void PlotBloodOxygenWave::InitPlot()
{
    //设置标题
//    m_pPlot->setTitle("My Project");
    //设置画布或背景
    m_pPlot->setCanvas( new MCanvas() );
    m_pPlot->setCanvasBackground(Qt::white);
    //设置坐标轴的名称
//    m_pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X轴"));
//    m_pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y轴"));

    //设置坐标轴标尺绘制
    m_pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

    //设置坐标轴的范围
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
//	m_pPlot->setAxisAutoScale(QwtPlot::xBottom);
    //m_pPlot->enableAxis(QwtPlot::xBottom, false);

    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, 250);
    m_pPlot->enableAxis(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave::AddPlotGrid()
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

void PlotBloodOxygenWave::AddPlotMarker()
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

void PlotBloodOxygenWave::AddLegend()
{
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotBloodOxygenWave::AddMagnifier()
{
    data->pMagnifier = new MPlotMagnifier(m_pPlot->canvas());
    data->pMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave::SetMagnifierEnabled(bool on)
{
    data->pMagnifier->setEnabled(on);
}

void PlotBloodOxygenWave::AddPanner()
{
    data->pPanner = new QwtPlotPanner(m_pPlot->canvas());       //使用鼠标左键平移
    data->pPanner->setOrientations(Qt::Horizontal);
}

void PlotBloodOxygenWave::SetPannerEnabled(bool on)
{
    data->pPanner->setEnabled(on);
}

void PlotBloodOxygenWave::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( m_pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void PlotBloodOxygenWave::AddPicker()
{
	QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn,
		m_pPlot->canvas());
	picker->setTrackerPen(QPen(Qt::green));
	//只有定义了它 才能设置对应的样式 比如想设置RectRubberBand必须先setStateMachine( new QwtPickerRectPointMachine() );
	picker->setStateMachine(new QwtPickerDragPointMachine());
	picker->setRubberBandPen(QColor(Qt::green));               //拾取器点击后样式颜色 
}

void PlotBloodOxygenWave::AddWaveCurve()
{
    data->pPOCurve = new MPlotWaveCurve("DescribeWave");    //设置曲线名称
    data->pPOCurve->setStyle(QwtPlotCurve::Lines);		  //直线形式
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
    data->pPOCurve->setPen(QPen(Qt::red, 2));    //设置画笔
    data->pPOCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    //data->pPOCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pPOCurve->setData(data->pWaveData);
    //把曲线附加到plot
    data->pPOCurve->attach(m_pPlot);
}

void PlotBloodOxygenWave::UpdateCurve()
{
    //加载数据
    if(false == data->pWaveData->UpdateCurveData(data->elapsed.elapsed())) return;

	data->interval = data->pWaveData->GetXAxisInterval();
	m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	data->pPOCurve->SetCurrentPoint(data->pWaveData->GetCurrentPoint());
}

void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	PlotBloodOxygenWave* pThis = (PlotBloodOxygenWave*)lpParameter;
	pThis->UpdateCurve();
}

void PlotBloodOxygenWave::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == data->DrawTimerId &&
            (0 == m_uDrawCounter++ % data->uPlotIntervalCount))
    {
        //QwtPlot重绘
        m_pPlot->replot();
        /*
            const bool doClip = !m_pPlot->canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                const QwtScaleMap xMap = m_pPlot->canvasMap( data->pPOCurve->xAxis() );
                const QwtScaleMap yMap = m_pPlot->canvasMap( data->pPOCurve->yAxis() );

                QRectF br = qwtBoundingRect( *data->pWaveData, 0, data->pWaveData->size() - 1 );

                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                data->pDirectPainter->setClipRegion( clipRect );
            }

        //data->pDirectPainter->drawSeries( data->pPOCurve,  0, data->pWaveData->size() - 1 );*/
    }
}
