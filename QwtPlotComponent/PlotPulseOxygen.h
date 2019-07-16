#pragma once
#include "PlotCurveBase.h"

class PlotPulseOxygen : public PlotCurveBase
{
	Q_OBJECT
public:
	PlotPulseOxygen(QwtPlot *plot);
	~PlotPulseOxygen();

	void BuildPlot() override;

	//��Ӳ�������
	void AddCurve() override;

	//������ʷ����
	void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse);

signals:
	void signalMarkerPosition(double xPos);

protected:
	bool eventFilter(QObject *, QEvent *) override;
};

