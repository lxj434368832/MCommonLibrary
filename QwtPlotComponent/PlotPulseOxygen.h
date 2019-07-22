#pragma once
#include "PlotCurveBase.h"
#include <QMap>

class QwtPlotMarker;

class PlotPulseOxygen : public PlotCurveBase
{
	Q_OBJECT
public:
	PlotPulseOxygen(QwtPlot *plot);
	~PlotPulseOxygen();

	void BuildPlot() override;

	//添加波形曲线
	void AddCurve() override;

	//绘制历史数据
	void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse);

	//添加事件标记
	void UpdateEventMarker(int Id, quint64 ulTime, std::string strText);

	//移除事件标记
	void RemoveEventMarker(int Id);

signals:
	void signalMarkerPosition(double xPos);

protected:
	bool eventFilter(QObject *, QEvent *) override;

private:
	QMap<int, QwtPlotMarker*> m_mapEventMarker;

};

