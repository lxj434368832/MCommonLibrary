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

    if(data->DrawTimerId)   //��������˻��ƶ�ʱ����������
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
            qDebug() << QString::fromLocal8Bit("����������ȡֵ��ʱ��ʧ�ܣ�");
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

    if (0 != m_uDataCheckCounter++ % m_uDataCheckInterval) return;			//30s���һ��
	double dValue = (double)iSize / data->m_uSamplingRate;
    qDebug() << QString::fromLocal8Bit("��ǰ���������Ϊ:%1������ֵ:%2")
        .arg(iSize).arg(dValue);

	if (dValue < 1.5)	//ʵ��ֻʣ10������
	{
		if (1 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 1;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() + 2;
			::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 2.1) //ֻ��1����������
	{
		if (2 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 2;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() + 1;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 3.1) //ֻ��2����������
	{
		if (3 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 3;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval();
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
	else if (dValue < 4.1) //ֻ��3����������
	{
		if (4 != data->uReservePointRatio)
		{
			data->uReservePointRatio = 4;
            m_uDataCheckInterval = 30;
			unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 1;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
    else if (dValue < 5.1) //ʵ�ʴ���4������
	{
        if( 5 != data->uReservePointRatio)
        {
            data->uReservePointRatio = 5;
            m_uDataCheckInterval = 30;
            unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 2;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
        }
	}
    else if(10 != data->uReservePointRatio)
    {
        data->uReservePointRatio = 10;
        m_uDataCheckInterval = 6;
        unsigned uTimeInterval = data->pWaveData->GetTimeInterval() - 10;
        ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
        qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
    }
}

void PlotBloodOxygenWave::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	data->eStatus = EDS_HISTORY;
    data->pWaveData->AddHistoryData(ulStartTime, list);
    //ˢ��������
    QwtInterval interval = data->pWaveData->GetXAxisInterval();

    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	data->pPOCurve->SetCurrentPoint(-1);
    //�ػ�
    m_pPlot->replot();
}

void PlotBloodOxygenWave::InitPlot()
{
    //���ñ���
//    m_pPlot->setTitle("My Project");
    //���û����򱳾�
    m_pPlot->setCanvas( new MCanvas() );
    m_pPlot->setCanvasBackground(Qt::white);
    //���������������
//    m_pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X��"));
//    m_pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y��"));

    //�����������߻���
    m_pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

    //����������ķ�Χ
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
//	m_pPlot->setAxisAutoScale(QwtPlot::xBottom);
    //m_pPlot->enableAxis(QwtPlot::xBottom, false);

    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, 250);
    m_pPlot->enableAxis(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave::AddPlotGrid()
{
    // ��������
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
    data->pPanner = new QwtPlotPanner(m_pPlot->canvas());       //ʹ��������ƽ��
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
	//ֻ�ж������� �������ö�Ӧ����ʽ ����������RectRubberBand������setStateMachine( new QwtPickerRectPointMachine() );
	picker->setStateMachine(new QwtPickerDragPointMachine());
	picker->setRubberBandPen(QColor(Qt::green));               //ʰȡ���������ʽ��ɫ 
}

void PlotBloodOxygenWave::AddWaveCurve()
{
    data->pPOCurve = new MPlotWaveCurve("DescribeWave");    //������������
    data->pPOCurve->setStyle(QwtPlotCurve::Lines);		  //ֱ����ʽ
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬
    data->pPOCurve->setPen(QPen(Qt::red, 2));    //���û���
    data->pPOCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    //data->pPOCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pPOCurve->setData(data->pWaveData);
    //�����߸��ӵ�plot
    data->pPOCurve->attach(m_pPlot);
}

void PlotBloodOxygenWave::UpdateCurve()
{
    //��������
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
        //QwtPlot�ػ�
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
