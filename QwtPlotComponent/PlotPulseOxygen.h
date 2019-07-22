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

	//��Ӳ�������
	void AddCurve() override;

	//������ʷ����
	void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse);

	//����¼����
	void UpdateEventMarker(int Id, quint64 ulTime, std::string strText);

	//�Ƴ��¼����
	void RemoveEventMarker(int Id);

signals:
	void signalMarkerPosition(double xPos);

protected:
	bool eventFilter(QObject *, QEvent *) override;

private:
	QMap<int, QwtPlotMarker*> m_mapEventMarker;

};

