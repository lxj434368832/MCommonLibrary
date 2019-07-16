#include "PlotPulseOxygen1.h"
#include "MCustomAssembly.h"
#include "MCurveData.h"
#include "MPlotMagnifier.h"
#include "MPlotPanner.h"
#include "QwtPlotDefine.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QDateTime>
#include <qwt_plot.h>
#include <qwt_symbol.h>


class PlotPulseOxygen1::PrivateData
{
public:
	MPlotMagnifier		*pMagnifier = nullptr;
	MPlotPanner			*pPanner = nullptr;
	QwtPicker			*pPicker = nullptr;
	QwtPlotMarker		*pMarker = nullptr;

    QwtPlotCurve		*pSPO2Curve = nullptr;      //血氧饱和度曲线
    MCurveData			*pSPO2Data = nullptr;
    QwtPlotCurve		*pPulseCurve = nullptr;      //脉搏曲线
    MCurveData			*pPulseData = nullptr;

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

PlotPulseOxygen1::PlotPulseOxygen1(QwtPlot *plot) :
    QObject(plot)
{
    m_pPlot = plot;
	data = new PrivateData;
    data->pSPO2Data = new MCurveData();
    data->pPulseData = new MCurveData();
	data->pDirectPainter = new QwtPlotDirectPainter;
	data->interval.setMaxValue(5000);
	data->elapsed.start();
}

PlotPulseOxygen1::~PlotPulseOxygen1()
{
    m_pPlot = nullptr;
	delete data;
	data = nullptr;
}

void PlotPulseOxygen1::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
    data->pSPO2Data->SetParam(uSamplingRate, dXAxisWidth);
    data->pPulseData->SetParam(uSamplingRate, dXAxisWidth);
	dXAxisWidth *= 1000;
	if (dXAxisWidth > 1000 && dXAxisWidth != data->interval.width() )
	{
        data->interval.setMaxValue(data->interval.minValue() + dXAxisWidth);
        m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	}

    unsigned uIntervalTime = data->pSPO2Data->GetDrawTime();
    unsigned uPlotIntervalCount = round( uPlotIntervalTime / uIntervalTime);
    if(uPlotIntervalCount > 0 && uPlotIntervalCount != data->uPlotIntervalCount)
    {
        data->uPlotIntervalCount = uPlotIntervalCount;
    }

    if(data->timerId)   //如果开启了定时器，则重置
    {
        data->elapsed.restart();
        killTimer(data->timerId);
        data->timerId = startTimer( data->pSPO2Data->GetDrawTime() );
    }
	
    if (EDS_UNKNOWN != data->eStatus)
	{
		m_pPlot->replot();
	}
}

void PlotPulseOxygen1::BuildPlot()
{
    InitPlot();
    AddPlotGrid();
    AddLegend();
    AddPanner();
    AddMagnifier();
    AddPicker();
	AddMarker();
    AddWaveCurve();
    data->eStatus = EDS_STOP;

	m_pPlot->canvas()->installEventFilter(this);
}

void PlotPulseOxygen1::Start()
{
    if (EDS_START != data->eStatus)
	{
        data->eStatus = EDS_START;
		data->elapsed.restart();
        data->timerId = startTimer(data->pSPO2Data->GetDrawTime());
	}
}

void PlotPulseOxygen1::Stop()
{
    if (EDS_STOP != data->eStatus)
	{
        data->eStatus = EDS_STOP;
		killTimer(data->timerId);
		data->timerId = 0;
        data->pSPO2Data->Reset();
	}
}

void PlotPulseOxygen1::AddWaveData(QList<unsigned char> list)
{
    data->pSPO2Data->AddPendingValues(list);
}

void PlotPulseOxygen1::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse)
{
	data->eStatus = EDS_HISTORY;
    data->pSPO2Data->AddHistoryData(ulStartTime, vctSPO2);
    data->pPulseData->AddHistoryData(ulStartTime, vctPulse);
    //刷新坐标轴
    QwtInterval interval =  data->pSPO2Data->GetXAxisInterval();

//    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
    data->interval = interval;
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	if (data->pMarker && !data->interval.contains(data->pMarker->xValue()))
	{
		SetMarkerValue((data->interval.minValue() + data->interval.maxValue()) / 2, 60);
	}

	if (data->pMagnifier) 
		data->pMagnifier->setXAxisRange(interval.minValue(), interval.maxValue());
	if (data->pPanner) 
		data->pPanner->setAxisRange(QwtPlot::xBottom, interval.minValue(), interval.maxValue());

	//重绘
	m_pPlot->replot();
}

void PlotPulseOxygen1::InitPlot()
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

    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, 120);
}

void PlotPulseOxygen1::AddPlotGrid()
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

void PlotPulseOxygen1::AddLegend()
{
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotPulseOxygen1::AddMagnifier()
{
    data->pMagnifier = new MPlotMagnifier(m_pPlot->canvas());
    data->pMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
}

void PlotPulseOxygen1::AddPanner()
{
	data->pPanner = new MPlotPanner(m_pPlot->canvas());       //使用鼠标左键平移
	data->pPanner->setOrientations(Qt::Horizontal);
}

void PlotPulseOxygen1::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( m_pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void PlotPulseOxygen1::AddMarker()
{
	data->pMarker = new MPlotMarker;
	data->pMarker->setRenderHint(QwtPlotItem::RenderAntialiased);
	data->pMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
	data->pMarker->setLabelOrientation(Qt::Horizontal);
	data->pMarker->setLineStyle(QwtPlotMarker::VLine);
	data->pMarker->setLinePen(QPen(QColor("#658bfe"), 2, Qt::SolidLine));
	data->pMarker->setSpacing(6);
	QwtInterval xAxisInterval = data->pSPO2Data->GetXAxisInterval();
	SetMarkerValue((xAxisInterval.minValue() + xAxisInterval.maxValue()) / 2.0, 60);

	data->pMarker->attach(m_pPlot);
}

void PlotPulseOxygen1::SetMarkerValue(double x, double y)
{
	data->pMarker->setValue(x, y);
	QDateTime dt = QDateTime::fromMSecsSinceEpoch(x);

	QwtText txt;
	txt.setText(dt.toString("HH:mm:ss zzz"));
	txt.setFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
	QColor bkColor = data->pMarker->linePen().color();
	bkColor.setAlpha(60);
	txt.setBackgroundBrush(bkColor);
	txt.setBorderRadius(10);

	data->pMarker->setLabel(txt);
}

void PlotPulseOxygen1::AddPicker()
{
	data->pPicker = new MPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn,
		m_pPlot->canvas());
    data->pPicker->setTrackerPen(QPen(QColor("#658bfe")));
    data->pPicker->setTrackerFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
	//只有定义了它 才能设置对应的样式 比如想设置RectRubberBand必须先setStateMachine( new QwtPickerRectPointMachine() );
	data->pPicker->setStateMachine(new QwtPickerDragPointMachine());
	data->pPicker->setRubberBandPen(QPen(QColor("#658bfe"), 2));
}

void PlotPulseOxygen1::AddWaveCurve()
{
    data->pSPO2Curve = new QwtPlotCurve("SPO2");    //设置曲线名称
    data->pSPO2Curve->setStyle(QwtPlotCurve::Lines);    //直线形式
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
    data->pSPO2Curve->setPen(QColor("#f40200") ,3);    //设置画笔
    data->pSPO2Curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    data->pSPO2Curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pSPO2Curve->setData(data->pSPO2Data);
    //把曲线附加到plot
    data->pSPO2Curve->attach(m_pPlot);

    data->pPulseCurve = new QwtPlotCurve("PR");    //设置曲线名称
    data->pPulseCurve->setStyle(QwtPlotCurve::Lines);    //直线形式
    //data->pPulseCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
    data->pPulseCurve->setPen(QColor("#6abf31"), 3);    //设置画笔
    data->pPulseCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    data->pPulseCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pPulseCurve->setData(data->pPulseData);
    //把曲线附加到plot
    data->pPulseCurve->attach(m_pPlot);
}

void PlotPulseOxygen1::UpdateCurve(qint64 elapsed)
{
    static unsigned uCount = 0;

    //重新加载数据
    if(false == data->pSPO2Data->UpdateCurveData(elapsed)) return;
	//刷新坐标轴
    data->interval = data->pSPO2Data->GetXAxisInterval();
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
    //m_pPlot->updateAxes();

    //QwtPlot重绘
    if(0 == uCount++ % data->uPlotIntervalCount)
    {
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

        data->pDirectPainter->drawSeries( data->pPOCurve,  0, data->pWaveData->size() - 1 );*/
    }
}

bool PlotPulseOxygen1::eventFilter(QObject *watched, QEvent *event)
{
	if (m_pPlot && watched == m_pPlot->canvas())
	{
		if (QEvent::MouseButtonDblClick == event->type())
		{
			const QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

			const QwtScaleMap xMap = m_pPlot->canvasMap(data->pSPO2Curve->xAxis());
			const QwtScaleMap yMap = m_pPlot->canvasMap(data->pSPO2Curve->yAxis());

			const QPointF pt = QwtScaleMap::invTransform(xMap, yMap, mouseEvent->pos());

			emit signalMarkerPosition(pt.x());
			SetMarkerValue(pt.x(), 60);
			m_pPlot->replot();
		}
		
		return false;
	}
	else
		return QObject::eventFilter(watched, event);
}

void PlotPulseOxygen1::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == data->timerId )
    {
        UpdateCurve(data->elapsed.elapsed());
        return;
    }

    QObject::timerEvent( event );
}

