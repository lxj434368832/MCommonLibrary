#pragma once

#include "PlotCurveBase.h"

class PlotBloodOxygenWave : public PlotCurveBase
{
	Q_OBJECT
public:
	PlotBloodOxygenWave(QwtPlot *plot);
	~PlotBloodOxygenWave();

	void BuildPlot() override;

	//绘制历史数据
	void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list) override;

	//更新画布
	void UpdateCurve() override;


public slots:
	void slotMarkerPosition(double xPos);

};

