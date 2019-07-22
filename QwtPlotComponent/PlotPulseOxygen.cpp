#include "PlotPulseOxygen.h"
#include "QwtPlotDefine.h"
#include <QMouseEvent>
#include <qwt_symbol.h>

struct SEventMarker
{
	int Id = -1;
	quint64 ulTime = 0;
	std::string strText;
};

PlotPulseOxygen::PlotPulseOxygen(QwtPlot *plot) : 
	PlotCurveBase(plot)
{
	data->vctCurveData.append(new MCurveData());	
	data->vctCurveData.append(new MCurveData());
}

PlotPulseOxygen::~PlotPulseOxygen()
{
}

void PlotPulseOxygen::BuildPlot()
{
	InitPlot();
	AddPlotGrid();
	AddLegend();
	AddPanner();
	AddMagnifier();
	AddPicker();
	AddMarker();
	AddCurve();
	data->eStatus = EDS_STOP;

    QwtInterval xAxisInterval = data->vctCurveData[0]->GetXAxisInterval();
    SetMarkerValue((xAxisInterval.minValue() + xAxisInterval.maxValue()) / 2.0, 60);

	data->pPlot->setAxisScale(QwtPlot::yLeft, 0, 120);
	data->pPlot->canvas()->installEventFilter(this);
}

void PlotPulseOxygen::AddCurve()
{

	QwtPlotCurve *curve = new QwtPlotCurve("SPO2");    //设置曲线名称
	curve->setStyle(QwtPlotCurve::Lines);    //直线形式
	//curve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
	curve->setPen(QColor("#f40200"), 3);    //设置画笔
	curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
	curve->setData(data->vctCurveData[0]);
	//把曲线附加到plot
	curve->attach(data->pPlot);
	data->vctCurve.append(curve);

	curve = new QwtPlotCurve("PR");    //设置曲线名称
	curve->setStyle(QwtPlotCurve::Lines);    //直线形式
	//curve->setCurveAttribute(QwtPlotCurve::Fitted, true);    //让曲线更光滑
	curve->setPen(QColor("#6abf31"), 3);    //设置画笔
	curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
	curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
	curve->setData(data->vctCurveData[1]);
	//把曲线附加到plot
	curve->attach(data->pPlot);
	data->vctCurve.append(curve);
}

void PlotPulseOxygen::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse)
{
	data->eStatus = EDS_HISTORY;
	data->vctCurveData[0]->AddHistoryData(ulStartTime, vctSPO2);
	data->vctCurveData[1]->AddHistoryData(ulStartTime, vctPulse);

	//刷新坐标轴
	data->interval = data->vctCurveData[0]->GetXAxisInterval();
	data->pPlot->setAxisScale(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());
	if (data->pMarker && !data->interval.contains(data->pMarker->xValue()))
	{
		SetMarkerValue((data->interval.minValue() + data->interval.maxValue()) / 2, 60);
	}

	if (data->pMagnifier)
		data->pMagnifier->setXAxisRange(data->interval.minValue(), data->interval.maxValue());
	if (data->pPanner)
		data->pPanner->setAxisRange(QwtPlot::xBottom, data->interval.minValue(), data->interval.maxValue());

	//重绘
	data->pPlot->replot();
}

bool PlotPulseOxygen::eventFilter(QObject *watched, QEvent *event)
{
	if (data && watched == data->pPlot->canvas())
	{
		if (QEvent::MouseButtonDblClick == event->type())
		{
			const QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

			const QwtScaleMap xMap = data->pPlot->canvasMap(data->vctCurve[0]->xAxis());
			const QwtScaleMap yMap = data->pPlot->canvasMap(data->vctCurve[0]->yAxis());

			const QPointF pt = QwtScaleMap::invTransform(xMap, yMap, mouseEvent->pos());

			emit signalMarkerPosition(pt.x());
			SetMarkerValue(pt.x(), 60);
			data->pPlot->replot();
		}

		return false;
	}
	else
		return QObject::eventFilter(watched, event);
}

void PlotPulseOxygen::UpdateEventMarker(int Id, quint64 ulTime, std::string strText)
{
	QwtPlotMarker *pMarker = nullptr;
	auto iter = m_mapEventMarker.find(Id);
	if (iter == m_mapEventMarker.end())		//不存在则新建Marker
	{
		pMarker = new QwtPlotMarker();
		pMarker->setItemAttribute(QwtPlotItem::AutoScale);
		pMarker->setRenderHint(QwtPlotItem::RenderAntialiased, true);
		QwtSymbol *smb = new QwtSymbol;
		QPixmap px(QString("D:\\Myprogram\\WPOClient\\MainClient\\Resource\\MonitorCenter\\MarkerIcon.png"));
		smb->setPixmap(px);
		smb->setSize(px.size());
		smb->setPinPoint(QPointF(px.width() / 2, px.height()));
		pMarker->setSymbol(smb);
		pMarker->setSpacing(16);

		const QwtScaleMap yMap = data->pPlot->canvasMap(QwtPlot::yLeft);
		double dy = yMap.invTransform(px.height());
		pMarker->setValue(ulTime, 0);
		QwtText txt;
		txt.setText(QString::fromLocal8Bit(strText.c_str()));
		txt.setFont(QFont("Microsoft YaHei UI", 12, QFont::Bold));
		txt.setColor(QColor("#658bfe"));
		pMarker->setLabel(txt);
		pMarker->setLabelAlignment(Qt::AlignTop);
		pMarker->attach(data->pPlot);
	}
	else
	{
		pMarker = iter.value();
		pMarker->setXValue(ulTime);
		pMarker->label().setText(QString::fromLocal8Bit(strText.c_str()));
	}

	data->pPlot->replot();
}

void PlotPulseOxygen::RemoveEventMarker(int Id)
{
	QwtPlotMarker *pMarker = nullptr;
	auto iter = m_mapEventMarker.find(Id);
	if (iter != m_mapEventMarker.end())
    {
        m_mapEventMarker.erase(iter);
        pMarker = iter.value();
        pMarker->detach();
        delete pMarker;
	}
	data->pPlot->replot();
}
