#pragma once

#include <qwt_plot_curve.h>

class MPlotWaveCurve : public QwtPlotCurve
{
public:
	explicit MPlotWaveCurve(const QString &title = QString::null);
	explicit MPlotWaveCurve(const QwtText &title);
	~MPlotWaveCurve();

	void SetCurrentPoint(int);

protected:
	virtual void drawCurve(QPainter *painter, int style,
		const QwtScaleMap &xMap, const QwtScaleMap &yMap,
		const QRectF &canvasRect, int from, int to) const;

private:
	int	m_iCurrentPoint;
};

