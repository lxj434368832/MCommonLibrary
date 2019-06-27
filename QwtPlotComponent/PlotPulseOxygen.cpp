#include "PlotPulseOxygen.h"
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


class PlotPulseOxygen::PrivateData
{
public:
	MPlotMagnifier		*pMagnifier = nullptr;
    QwtPlotCurve		*pSPO2Curve = nullptr;      //Ѫ�����Ͷ�����
    MCurveData		*pSPO2Data = nullptr;
    QwtPlotCurve		*pPulseCurve = nullptr;      //��������
    MCurveData		*pPulseData = nullptr;

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

PlotPulseOxygen::PlotPulseOxygen(QwtPlot *plot) :
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

PlotPulseOxygen::~PlotPulseOxygen()
{
    m_pPlot = nullptr;
	delete data;
	data = nullptr;
}

void PlotPulseOxygen::SetParam(unsigned uSamplingRate, double dXAxisWidth, unsigned uPlotIntervalTime)
{
    data->pSPO2Data->SetParam(uSamplingRate, dXAxisWidth);
    data->pPulseData->SetParam(uSamplingRate, dXAxisWidth);
	dXAxisWidth *= 1000;
	if (dXAxisWidth > 1000 && dXAxisWidth != data->interval.width() )
	{
        data->interval.setMaxValue(data->interval.minValue() + dXAxisWidth);
        m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	}

    unsigned uIntervalTime = data->pSPO2Data->GetTimeInterval();
    unsigned uPlotIntervalCount = round( uPlotIntervalTime / uIntervalTime);
    if(uPlotIntervalCount > 0 && uPlotIntervalCount != data->uPlotIntervalCount)
    {
        data->uPlotIntervalCount = uPlotIntervalCount;
    }

    if(data->timerId)   //��������˶�ʱ����������
    {
        data->elapsed.restart();
        killTimer(data->timerId);
        data->timerId = startTimer( data->pSPO2Data->GetTimeInterval() );
    }
	
    if (EDS_UNKNOWN != data->eStatus)
	{
		m_pPlot->replot();
	}
}

void PlotPulseOxygen::BuildPlot()
{
    InitPlot();
    AddPlotGrid();
    AddLegend();
    AddPanner();
    AddMagnifier();
    AddWaveCurve();
    data->eStatus = EDS_STOP;
}

void PlotPulseOxygen::Start()
{
    if (EDS_START != data->eStatus)
	{
        data->eStatus = EDS_START;
		data->elapsed.restart();
        data->timerId = startTimer(data->pSPO2Data->GetTimeInterval());
	}
}

void PlotPulseOxygen::Stop()
{
    if (EDS_STOP != data->eStatus)
	{
        data->eStatus = EDS_STOP;
		killTimer(data->timerId);
		data->timerId = 0;
        data->pSPO2Data->Reset();
	}
}

void PlotPulseOxygen::AddWaveData(QList<unsigned char> list)
{
    data->pSPO2Data->AddPendingValues(list);
}

void PlotPulseOxygen::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse)
{
	data->eStatus = EDS_HISTORY;
    data->pSPO2Data->AddHistoryData(ulStartTime, vctSPO2);
    data->pPulseData->AddHistoryData(ulStartTime, vctPulse);
    //ˢ��������
    QwtInterval interval =  data->pSPO2Data->GetXAxisInterval();

//    data->interval = QwtInterval(interval.maxValue() - data->interval.width(), interval.maxValue());
    data->interval = QwtInterval(interval.minValue(), interval.maxValue());
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
    //�ػ�
    m_pPlot->replot();
}

void PlotPulseOxygen::InitPlot()
{
    //���ñ���
//    m_pPlot->setTitle("My Project");
    //���û����򱳾�
    //    m_pPlot->setCanvas( new MCanvas() );
    m_pPlot->setCanvasBackground(Qt::white);
    //���������������
//    m_pPlot->setAxisTitle(QwtPlot::xBottom, QStringLiteral("X��"));
//    m_pPlot->setAxisTitle(QwtPlot::yLeft, QStringLiteral("Y��"));

    //�����������߻���
    m_pPlot->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());

    //����������ķ�Χ
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());

    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, 120);
}

void PlotPulseOxygen::AddPlotGrid()
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

void PlotPulseOxygen::AddPlotMarker()
{
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = PI/2"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Horizontal);
    mX->setLineStyle( QwtPlotMarker::Cross );
    mX->setLinePen(QPen(Qt::black, 1, Qt::DashDotDotLine));
    QwtInterval xAxisInterval = data->pSPO2Data->GetXAxisInterval();
    mX->setValue( (xAxisInterval.minValue() + xAxisInterval.maxValue()) /  2.0, 255.0 / 2 );
    mX->attach(m_pPlot);
}

void PlotPulseOxygen::AddLegend()
{
    m_pPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotPulseOxygen::AddMagnifier()
{
    data->pMagnifier = new MPlotMagnifier(m_pPlot->canvas());
    data->pMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
}

void PlotPulseOxygen::AddPanner()
{
    QwtPlotPanner *qwtPanner = new QwtPlotPanner(m_pPlot->canvas());       //ʹ��������ƽ��
    qwtPanner->setOrientations(Qt::Horizontal);
    connect(qwtPanner, SIGNAL(panned(int,int)), this, SLOT(slotCanvasMoved(int,int)));
}

void PlotPulseOxygen::AddZoomer()
{
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( m_pPlot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
}

void PlotPulseOxygen::AddWaveCurve()
{
    data->pSPO2Curve = new QwtPlotCurve("SPO2");    //������������
    data->pSPO2Curve->setStyle(QwtPlotCurve::Lines);    //ֱ����ʽ
    //data->pPOCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬
    data->pSPO2Curve->setPen(QPen("#f40200"));    //���û���
    data->pSPO2Curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    data->pSPO2Curve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pSPO2Curve->setData(data->pSPO2Data);
    //�����߸��ӵ�plot
    data->pSPO2Curve->attach(m_pPlot);

    data->pPulseCurve = new QwtPlotCurve("PR");    //������������
    data->pPulseCurve->setStyle(QwtPlotCurve::Lines);    //ֱ����ʽ
    //data->pPulseCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //�����߸��⻬
    data->pPulseCurve->setPen(QPen("#6abf31"));    //���û���
    data->pPulseCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    data->pPulseCurve->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
    data->pPulseCurve->setData(data->pPulseData);
    //�����߸��ӵ�plot
    data->pPulseCurve->attach(m_pPlot);
}

void PlotPulseOxygen::UpdateCurve(qint64 elapsed)
{
    static unsigned uCount = 0;

    //���¼�������
    if(false == data->pSPO2Data->UpdateCurveData(elapsed)) return;
	//ˢ��������
    data->interval = data->pSPO2Data->GetXAxisInterval();
    m_pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
    //m_pPlot->updateAxes();

    //QwtPlot�ػ�
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

void PlotPulseOxygen::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == data->timerId )
    {
        UpdateCurve(data->elapsed.elapsed());
        return;
    }

    QObject::timerEvent( event );
}

void PlotPulseOxygen::slotCanvasMoved(int dx, int dy)
{
//    qDebug()<<"current dx:"<<dx<<" dy:"<<dy;
}
