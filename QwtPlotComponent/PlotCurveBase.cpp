#include "PlotCurveBase.h"
#include "MCustomAssembly.h"
#include "MCurveData.h"
#include "MPlotMagnifier.h"
#include "QwtPlotDefine.h"
#include <QDebug>
#include <QTimerEvent>
#include <QDateTime>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>

void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

PlotCurveBase::PlotCurveBase(QwtPlot *plot) :
    QObject(plot)
{
	data = new QwtPlotData;
	data->pPlot = plot;
	data->pDirectPainter = new QwtPlotDirectPainter;
	data->interval.setMaxValue(5000);
	data->elapsed.start();
}

PlotCurveBase::~PlotCurveBase()
{
	delete data;
	data = nullptr;
}

void PlotCurveBase::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
	for (MCurveData *curveData : data->vctCurveData)
	{
		curveData->SetParam(uSamplingRate, dXAxisWidth);
	}

	dXAxisWidth *= 1000;
	if (dXAxisWidth > 1000 && dXAxisWidth != data->interval.width())
	{
		data->interval.setMaxValue(data->interval.minValue() + dXAxisWidth);
		data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	}

	unsigned uIntervalTime = data->vctCurveData[0]->GetDrawTime();
	unsigned uPlotIntervalCount = round(uPlotIntervalTime / uIntervalTime);
	if (uPlotIntervalCount > 0 && uPlotIntervalCount != data->uPlotIntervalCount)
	{
		data->uPlotIntervalCount = uPlotIntervalCount;
	}

	if (data->hTimer)
	{
		::ChangeTimerQueueTimer(NULL, data->hTimer, uIntervalTime, uIntervalTime);
	}

	if (data->iDrawTimerId)   //��������˻��ƶ�ʱ����������
	{
		killTimer(data->iDrawTimerId);
		data->iDrawTimerId = startTimer(uIntervalTime);
	}

	if (EDS_UNKNOWN != data->eStatus)
	{
		data->pPlot->replot();
	}
}

void PlotCurveBase::BuildPlot()
{
    InitPlot();
    //AddPlotGrid();
    //AddLegend();
    //AddPanner();
    AddCurve();
    data->eStatus = EDS_STOP;
}

void PlotCurveBase::Start()
{
	if (EDS_START != data->eStatus)
	{
		data->eStatus = EDS_START;
		data->elapsed.restart();
		unsigned uTimerInterval = data->vctCurveData[0]->GetDrawTime();
		if (!::CreateTimerQueueTimer(&data->hTimer, NULL, TimerCallback, this, uTimerInterval, uTimerInterval, WT_EXECUTEDEFAULT))
		{
			qDebug() << QString::fromLocal8Bit("����������ȡֵ��ʱ��ʧ�ܣ�");
			data->hTimer = nullptr;
		}
		data->iDrawTimerId = startTimer(uTimerInterval);
	}
}

void PlotCurveBase::Stop()
{
	if (EDS_START == data->eStatus)
	{
		data->eStatus = EDS_STOP;
		if (data->hTimer)
			::DeleteTimerQueueTimer(nullptr, data->hTimer, INVALID_HANDLE_VALUE);
		data->hTimer = nullptr;

		killTimer(data->iDrawTimerId);
		data->iDrawTimerId = 0;
	}
}

void PlotCurveBase::Reset()
{
	for (MCurveData *curveData : data->vctCurveData)
		curveData->Reset();

	data->interval = data->vctCurveData[0]->GetXAxisInterval();
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
}

void PlotCurveBase::AddWaveData(QList<unsigned char> list)
{
	if (EDS_START != data->eStatus) return;
	unsigned iSize = data->vctCurveData[0]->AddPendingValues(list);

	if (0 != data->uCacheCheckCounter++ % data->uCacheCheckInterval) return;			//30s���һ��
	double dValue = (double)iSize / data->vctCurveData[0]->GetSamplingRate();

	qDebug() << QString::fromLocal8Bit("��ǰ���������Ϊ:%1������ֵ:%2")
		.arg(iSize).arg(dValue);

	if (dValue < 1.5)	//ʵ��ֻʣ10������
	{
		if (1 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 1;
			data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime() + 2;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
			qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 2.1) //ֻ��1����������
	{
		if (2 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 2;
			data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime() + 1;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
			qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 3.1) //ֻ��2����������
	{
		if (3 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 3;
			data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime();
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
			qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 4.1) //ֻ��3����������
	{
		if (4 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 4;
			data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime() - 1;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
			qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 5.1) //ʵ�ʴ���4������
	{
		if (5 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 5;
			data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime() - 2;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
			qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (10 != data->uCachePointRatio)
	{
		data->uCachePointRatio = 10;
		data->uCacheCheckInterval = 6;
		unsigned uTimeInterval = data->vctCurveData[0]->GetDrawTime() - 10;
		::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
		qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
	}
}

void PlotCurveBase::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	data->eStatus = EDS_HISTORY;
	data->vctCurveData[0]->AddHistoryData(ulStartTime, list);

	//ˢ��������
	QwtInterval interval = data->vctCurveData[0]->GetXAxisInterval();
	data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());

	if (data->pMarker && !data->interval.contains(data->pMarker->xValue()))
	{
		data->pMarker->setXValue((data->interval.minValue() + data->interval.maxValue()) / 2);
	}

	if (data->pMagnifier)
		data->pMagnifier->setXAxisRange(interval.minValue(), interval.maxValue());
	if (data->pPanner)
		data->pPanner->setAxisRange(QwtPlot::xBottom, interval.minValue(), interval.maxValue());

	//�ػ�
	data->pPlot->replot();
}

void PlotCurveBase::InitPlot()
{
	//���ñ���
//    m_pPlot->setTitle("My Project");
	//���û����򱳾�
	data->pPlot->setCanvas(new MCanvas());
	data->pPlot->setCanvasBackground(Qt::white);
	//���������������
//    data->pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X��"));
//    data->pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y��"));

	//�����������߻���
	data->pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

	//����������ķ�Χ
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	//	data->pPlot->setAxisAutoScale(QwtPlot::xBottom);
	//data->pPlot->enableAxis(QwtPlot::xBottom, false);

	data->pPlot->setAxisScale(QwtPlot::yLeft, 0, 250);
	//data->pPlot->enableAxis(QwtPlot::yLeft, false);
}

void PlotCurveBase::AddPlotGrid()
{
    // ��������
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( true );
    //    grid->setMajorPen(Qt::black,0,Qt::SolidLine);
    grid->attach(data->pPlot);
}

void PlotCurveBase::AddLegend()
{
	data->pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotCurveBase::AddMagnifier()
{
	data->pMagnifier = new MPlotMagnifier(data->pPlot->canvas());
	data->pMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
}

void PlotCurveBase::SetMagnifierEnabled(bool on)
{
	data->pMagnifier->setEnabled(on);
}

void PlotCurveBase::AddPanner()
{
	data->pPanner = new MPlotPanner(data->pPlot->canvas());       //ʹ��������ƽ��
	data->pPanner->setOrientations(Qt::Horizontal);
}

void PlotCurveBase::SetPannerEnabled(bool on)
{
	data->pPanner->setEnabled(on);
}

void PlotCurveBase::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer(data->pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void PlotCurveBase::AddMarker()
{
	data->pMarker = new MPlotMarker;
	data->pMarker->setRenderHint(QwtPlotItem::RenderAntialiased);
	data->pMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
	data->pMarker->setLabelOrientation(Qt::Horizontal);
	data->pMarker->setLineStyle(QwtPlotMarker::VLine);
	data->pMarker->setLinePen(QPen(QColor("#658bfe"), 2, Qt::SolidLine));
	data->pMarker->setSpacing(6);
	QwtInterval xAxisInterval = data->vctCurveData[0]->GetXAxisInterval();
	SetMarkerValue((xAxisInterval.minValue() + xAxisInterval.maxValue()) / 2.0, 60);

	data->pMarker->attach(data->pPlot);
}

void PlotCurveBase::SetMarkerValue(double x, double y)
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

void PlotCurveBase::SetMarkerEnabled(bool on)
{
	if (on)
		data->pMarker->attach(data->pPlot);
	else
		data->pMarker->detach();
	//data->pPlot->replot();
}

void PlotCurveBase::AddPicker()
{
	data->pPicker = new MPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn,
		data->pPlot->canvas());
	data->pPicker->setTrackerPen(QPen(QColor("#658bfe")));
	data->pPicker->setTrackerFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
	//ֻ�ж������� �������ö�Ӧ����ʽ ����������RectRubberBand������setStateMachine( new QwtPickerRectPointMachine() );
	data->pPicker->setStateMachine(new QwtPickerDragPointMachine());
	data->pPicker->setRubberBandPen(QPen(QColor("#658bfe"), 2));
}

void PlotCurveBase::AddCurve()
{
	QwtPlotCurve *curve = new MPlotWaveCurve("DescribeWave");    //������������
	curve->setStyle(QwtPlotCurve::Lines);		  //ֱ����ʽ
	//curve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬
	curve->setPen(QPen(Qt::red, 2));    //���û���
	curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	//curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
	curve->setData(data->vctCurveData[0]);
	//�����߸��ӵ�plot
	curve->attach(data->pPlot);
	data->vctCurve.append(curve);
}

void PlotCurveBase::UpdateCurve()
{
	//��������
	if (false == data->vctCurveData[0]->UpdateCurveData(data->elapsed.elapsed())) return;

	data->interval = data->vctCurveData[0]->GetXAxisInterval();
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
}

static void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	PlotCurveBase* pThis = (PlotCurveBase*)lpParameter;
	pThis->UpdateCurve();
}

void PlotCurveBase::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == data->iDrawTimerId &&
		(0 == data->uPlotCounter++ % data->uPlotIntervalCount))
	{
		//QwtPlot�ػ�
		data->pPlot->replot();
		/*
			const bool doClip = !data->pPlot->canvas()->testAttribute( Qt::WA_PaintOnScreen );
			if ( doClip )
			{
				const QwtScaleMap xMap = data->pPlot->canvasMap( data->pPOCurve->xAxis() );
				const QwtScaleMap yMap = data->pPlot->canvasMap( data->pPOCurve->yAxis() );

				QRectF br = qwtBoundingRect( *data->pWaveData, 0, data->pWaveData->size() - 1 );

				const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
				data->pDirectPainter->setClipRegion( clipRect );
			}

		//data->pDirectPainter->drawSeries( data->pPOCurve,  0, data->pWaveData->size() - 1 );*/
	}
}
