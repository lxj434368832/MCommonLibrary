#include "PlotBloodOxygenWave.h"
#include "QwtPlotDefine.h"
#include "MCycleCurveData.h"
#include "MCustomAssembly.h"



PlotBloodOxygenWave::PlotBloodOxygenWave(QwtPlot *plot):
	PlotCurveBase(plot)
{
	data->vctCurveData.append(new MCycleCurveData());
}

PlotBloodOxygenWave::~PlotBloodOxygenWave()
{
}

void PlotBloodOxygenWave::BuildPlot()
{
	InitPlot();
	AddMarker();
	AddCurve();
	data->eStatus = EDS_STOP;

	data->pMarker->setLabel(QString());
	data->pPlot->enableAxis(QwtPlot::xBottom, false);
	data->pPlot->enableAxis(QwtPlot::yLeft, false);
}

void PlotBloodOxygenWave::DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list)
{
	PlotCurveBase::DrawHistoryData(ulStartTime, list);
	dynamic_cast<MPlotWaveCurve*>(data->vctCurve[0])->SetCurrentPoint(-1);
}

void PlotBloodOxygenWave::UpdateCurve()
{
	PlotCurveBase::UpdateCurve();

	unsigned uCurrentPoint = dynamic_cast<MCycleCurveData*>(data->vctCurveData[0])->GetCurrentPoint();
	dynamic_cast<MPlotWaveCurve*>(data->vctCurve[0])->SetCurrentPoint(uCurrentPoint);
}

void PlotBloodOxygenWave::slotMarkerPosition(double xPos)
{
	if (EDS_START == data->eStatus) return;
	QwtInterval interval = data->pPlot->axisInterval(QwtPlot::xBottom);	//获取x轴实际显示的范围
	double dMin = xPos - interval.width() / 2;
	double dMax = dMin + interval.width();

	interval = data->vctCurveData[0]->GetXAxisInterval();
	if (dMin > interval.maxValue() || dMax < interval.minValue())	//如果超出整个坐标轴范围，认为是无效的
	{
		qDebug() << QString::fromLocal8Bit("当前值:%1,超过坐标轴范围：%2--%3.").
			arg(xPos).arg(interval.minValue()).arg(interval.maxValue());
		return;
	}
	
	data->pMarker->setXValue(xPos);
	data->pPlot->setAxisScale(QwtPlot::xBottom, dMin, dMax);

	data->pPlot->replot();
}
