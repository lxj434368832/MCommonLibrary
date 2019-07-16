#pragma once

#include "PlotCurveBase.h"

class PlotBloodOxygenWave : public PlotCurveBase
{
	Q_OBJECT
public:
	PlotBloodOxygenWave(QwtPlot *plot);
	~PlotBloodOxygenWave();

	void BuildPlot() override;

	//������ʷ����
	void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list) override;

	//���»���
	void UpdateCurve() override;


public slots:
	void slotMarkerPosition(double xPos);

};

