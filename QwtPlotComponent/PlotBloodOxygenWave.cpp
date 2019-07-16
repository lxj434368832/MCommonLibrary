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
	QwtInterval interval = data->pPlot->axisInterval(QwtPlot::xBottom);	//��ȡx��ʵ����ʾ�ķ�Χ
	double dMin = xPos - interval.width() / 2;
	double dMax = dMin + interval.width();

	interval = data->vctCurveData[0]->GetXAxisInterval();
	if (dMin > interval.maxValue() || dMax < interval.minValue())	//����������������᷶Χ����Ϊ����Ч��
	{
		qDebug() << QString::fromLocal8Bit("��ǰֵ:%1,���������᷶Χ��%2--%3.").
			arg(xPos).arg(interval.minValue()).arg(interval.maxValue());
		return;
	}
	
	data->pMarker->setXValue(xPos);
	data->pPlot->setAxisScale(QwtPlot::xBottom, dMin, dMax);

	data->pPlot->replot();
}
