#include "PlotBloodOxygenWave1.h"
#include "MCustomAssembly.h"
#include "MCycleCurveData.h"
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
#include <qwt_picker_machine.h>
#include <windows.h>

void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

class PlotBloodOxygenWave1::PrivateData
{
public:
	QwtPlot             *pPlot = nullptr;
	MPlotMagnifier		*pMagnifier = nullptr;
    QwtPanner           *pPanner = nullptr;
	QwtPicker			*pPicker = nullptr;
	QwtPlotCurve		*pWaveCurve = nullptr;
	MCycleCurveData		*pCurveData = nullptr;

	QwtPlotDirectPainter	*pDirectPainter = nullptr;
    QwtInterval				interval;

    int						iDrawTimerId = 0;
	HANDLE					hTimer = NULL;
	QElapsedTimer			elapsed;
	unsigned				uPlotIntervalCount = 0;
	unsigned				uPlotCounter = 0;			//���Ƽ���
	EDrawStatus				eStatus = EDS_UNKNOWN;

	unsigned				uCachePointRatio = 66;		//��������
	unsigned				uCacheCheckCounter = 0;		//���������
	unsigned				uCacheCheckInterval = 30;	//������������

	~PrivateData()
    {
		if (pDirectPainter)
		{
			delete pDirectPainter;
			pDirectPainter = nullptr;
		}
	}
};

PlotBloodOxygenWave1::PlotBloodOxygenWave1(QwtPlot *plot) :
    QObject(plot)
{
	data = new PrivateData;
	data->pPlot = plot;
	data->pCurveData = new MCycleCurveData();
	data->pDirectPainter = new QwtPlotDirectPainter;
	data->interval.setMaxValue(5000);
	data->elapsed.start();
}

PlotBloodOxygenWave1::~PlotBloodOxygenWave1()
{
	delete data;
	data = nullptr;
}

void PlotBloodOxygenWave1::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
	data->pCurveData->SetParam(uSamplingRate, dXAxisWidth);
	dXAxisWidth *= 1000;
	if (dXAxisWidth > 1000 && dXAxisWidth != data->interval.width() )
	{
        data->interval.setMaxValue(data->interval.minValue() + dXAxisWidth);
        data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	}

    unsigned uIntervalTime = data->pCurveData->GetDrawTime();
    unsigned uPlotIntervalCount = round( uPlotIntervalTime / uIntervalTime);
    if(uPlotIntervalCount > 0 && uPlotIntervalCount != data->uPlotIntervalCount)
    {
        data->uPlotIntervalCount = uPlotIntervalCount;
    }

	if (data->hTimer)
    {
        ::ChangeTimerQueueTimer(NULL, data->hTimer, uIntervalTime, uIntervalTime);
    }

    if(data->iDrawTimerId)   //��������˻��ƶ�ʱ����������
    {
        killTimer(data->iDrawTimerId);
        data->iDrawTimerId = startTimer( uIntervalTime );
    }

    if (EDS_UNKNOWN != data->eStatus)
	{
		data->pPlot->replot();
	}
}

void PlotBloodOxygenWave1::BuildPlot()
{
    InitPlot();
    //AddPlotGrid();
//    AddLegend();
    //AddPanner();
	//AddPicker();
    AddWaveCurve();
    data->eStatus = EDS_STOP;
}

void PlotBloodOxygenWave1::Start()
{
    if (EDS_START != data->eStatus)
	{
        data->eStatus = EDS_START;
		data->elapsed.restart();
		unsigned uTimerInterval = data->pCurveData->GetDrawTime();
		if (!::CreateTimerQueueTimer(&data->hTimer, NULL, TimerCallback, this, uTimerInterval, uTimerInterval, WT_EXECUTEDEFAULT))
		{
            qDebug() << QString::fromLocal8Bit("����������ȡֵ��ʱ��ʧ�ܣ�");
			data->hTimer = nullptr;
		}
		data->iDrawTimerId = startTimer(uTimerInterval);
	}
}

void PlotBloodOxygenWave1::Stop()
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

void PlotBloodOxygenWave1::Reset()
{
	data->pCurveData->Reset();
	data->interval = data->pCurveData->GetXAxisInterval();
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
}

void PlotBloodOxygenWave1::AddWaveData(QList<unsigned char> list)
{
	if (EDS_START != data->eStatus) return;
	unsigned iSize = data->pCurveData->AddPendingValues(list);

    if (0 != data->uCacheCheckCounter++ % data->uCacheCheckInterval) return;			//30s���һ��
	double dValue = (double)iSize / data->pCurveData->GetSamplingRate();

    qDebug() << QString::fromLocal8Bit("��ǰ���������Ϊ:%1������ֵ:%2")
        .arg(iSize).arg(dValue);

	if (dValue < 1.5)	//ʵ��ֻʣ10������
	{
		if (1 != data->uCachePointRatio)
		{
			data->uCachePointRatio = 1;
            data->uCacheCheckInterval = 30;
			unsigned uTimeInterval = data->pCurveData->GetDrawTime() + 2;
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
			unsigned uTimeInterval = data->pCurveData->GetDrawTime() + 1;
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
			unsigned uTimeInterval = data->pCurveData->GetDrawTime();
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
			unsigned uTimeInterval = data->pCurveData->GetDrawTime() - 1;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
		}
	}
    else if (dValue < 5.1) //ʵ�ʴ���4������
	{
        if( 5 != data->uCachePointRatio)
        {
            data->uCachePointRatio = 5;
            data->uCacheCheckInterval = 30;
            unsigned uTimeInterval = data->pCurveData->GetDrawTime() - 2;
            ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
            qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
        }
	}
    else if(10 != data->uCachePointRatio)
    {
        data->uCachePointRatio = 10;
        data->uCacheCheckInterval = 6;
        unsigned uTimeInterval = data->pCurveData->GetDrawTime() - 10;
        ::ChangeTimerQueueTimer(NULL, data->hTimer, uTimeInterval, uTimeInterval);
        qDebug() << QString::fromLocal8Bit("��ǰȡֵ���Ϊ:%1").arg(uTimeInterval);
    }
}

void PlotBloodOxygenWave1::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	data->eStatus = EDS_HISTORY;
    data->pCurveData->AddHistoryData(ulStartTime, list);
    //ˢ��������
    QwtInterval interval = data->pCurveData->GetXAxisInterval();

    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	dynamic_cast<MPlotWaveCurve*>(data->pWaveCurve)->SetCurrentPoint(-1);
    //�ػ�
	data->pPlot->replot();

	//�������ŵķ�Χ
	if(data->pMagnifier)
		data->pMagnifier->setXAxisRange(interval.minValue(), interval.maxValue());
}

void PlotBloodOxygenWave1::InitPlot()
{
    //���ñ���
//    m_pPlot->setTitle("My Project");
    //���û����򱳾�
	data->pPlot->setCanvas( new MCanvas() );
	data->pPlot->setCanvasBackground(Qt::white);
    //���������������
//    data->pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X��"));
//    data->pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y��"));

    //�����������߻���
	data->pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

    //����������ķ�Χ
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
//	data->pPlot->setAxisAutoScale(QwtPlot::xBottom);
	data->pPlot->enableAxis(QwtPlot::xBottom, false);

	data->pPlot->setAxisScale(QwtPlot::yLeft, 0, 250);
	data->pPlot->enableAxis(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave1::AddPlotGrid()
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

void PlotBloodOxygenWave1::AddPlotMarker()
{
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = PI/2"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Horizontal);
    mX->setLineStyle( QwtPlotMarker::Cross );
    mX->setLinePen(QPen(Qt::black, 1, Qt::DashDotDotLine));
    QwtInterval xAxisInterval = data->pCurveData->GetXAxisInterval();
    mX->setValue( (xAxisInterval.minValue() + xAxisInterval.maxValue()) /  2.0, 255.0 / 2 );
    mX->attach(data->pPlot);
}

void PlotBloodOxygenWave1::AddLegend()
{
	data->pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotBloodOxygenWave1::AddMagnifier()
{
    data->pMagnifier = new MPlotMagnifier(data->pPlot->canvas());
    data->pMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave1::SetMagnifierEnabled(bool on)
{
    data->pMagnifier->setEnabled(on);
}

void PlotBloodOxygenWave1::AddPanner()
{
    data->pPanner = new QwtPlotPanner(data->pPlot->canvas());       //ʹ��������ƽ��
    data->pPanner->setOrientations(Qt::Horizontal);
}

void PlotBloodOxygenWave1::SetPannerEnabled(bool on)
{
    data->pPanner->setEnabled(on);
}

void PlotBloodOxygenWave1::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer(data->pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void PlotBloodOxygenWave1::AddPicker()
{
	data->pPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
		QwtPlotPicker::VLineRubberBand, QwtPicker::AlwaysOn,
		data->pPlot->canvas());
	data->pPicker->setTrackerPen(QPen(Qt::green));
	//ֻ�ж������� �������ö�Ӧ����ʽ ����������RectRubberBand������setStateMachine( new QwtPickerRectPointMachine() );
	data->pPicker->setStateMachine(new QwtPickerDragPointMachine());
	data->pPicker->setRubberBandPen(QColor(Qt::green));               //ʰȡ���������ʽ��ɫ 
}

void PlotBloodOxygenWave1::AddWaveCurve()
{
    data->pWaveCurve = new MPlotWaveCurve("DescribeWave");    //������������
    data->pWaveCurve->setStyle(QwtPlotCurve::Lines);		  //ֱ����ʽ
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬
    data->pWaveCurve->setPen(QPen(Qt::red, 2));    //���û���
    data->pWaveCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    //data->pPOCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pWaveCurve->setData(data->pCurveData);
    //�����߸��ӵ�plot
    data->pWaveCurve->attach(data->pPlot);
}

void PlotBloodOxygenWave1::UpdateCurve()
{
    //��������
    if(false == data->pCurveData->UpdateCurveData(data->elapsed.elapsed())) return;

	data->interval = data->pCurveData->GetXAxisInterval();
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	dynamic_cast<MPlotWaveCurve*>(data->pWaveCurve)->SetCurrentPoint(data->pCurveData->GetCurrentPoint());
}

void PlotBloodOxygenWave1::slotMarkerPosition(double xPos)
{
	if (EDS_START == data->eStatus) return;
	QwtInterval interval = data->pPlot->axisInterval(QwtPlot::xBottom);	//��ȡx��ʵ����ʾ�ķ�Χ
	double dMin = xPos - interval.width() / 2;
	double dMax = dMin + interval.width();

	interval = data->pCurveData->GetXAxisInterval();
	if (dMin > interval.maxValue() || dMax < interval.minValue())	//����������������᷶Χ����Ϊ����Ч��
	{
		qDebug() << QString::fromLocal8Bit("��ǰֵ:%1,���������᷶Χ��%2--%3.").
			arg(xPos).arg(interval.minValue()).arg(interval.maxValue());
		//return;
	}

	data->pPlot->setAxisScale(QwtPlot::xBottom, dMin, dMax);
	data->pPlot->replot();
}

static void CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	PlotBloodOxygenWave1* pThis = (PlotBloodOxygenWave1*)lpParameter;
	pThis->UpdateCurve();
}

void PlotBloodOxygenWave1::timerEvent(QTimerEvent *event)
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
