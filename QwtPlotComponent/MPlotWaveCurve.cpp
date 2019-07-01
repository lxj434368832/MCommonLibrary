#include "MPlotWaveCurve.h"
#include "qglobal.h"

MPlotWaveCurve::MPlotWaveCurve(const QString & title):
	QwtPlotCurve(title)
{
	m_iCurrentPoint = 0;
}

MPlotWaveCurve::MPlotWaveCurve(const QwtText & title):
	QwtPlotCurve(title)
{
}

MPlotWaveCurve::~MPlotWaveCurve()
{
}

void MPlotWaveCurve::SetCurrentPoint(int iCurrentPoint)
{
	m_iCurrentPoint = iCurrentPoint;
}

void MPlotWaveCurve::drawCurve(QPainter * painter, int style,
	const QwtScaleMap & xMap, const QwtScaleMap & yMap, 
	const QRectF & canvasRect, int from, int to) const
{
	switch (style)
	{
	case Lines:
		if (testCurveAttribute(Fitted))
		{
			// we always need the complete
			// curve for fitting
			from = 0;
			to = dataSize() - 1;
		}
        if (m_iCurrentPoint > from && m_iCurrentPoint < to - 6)
		{
            drawLines(painter, xMap, yMap, canvasRect, from, m_iCurrentPoint);
            drawLines(painter, xMap, yMap, canvasRect, m_iCurrentPoint + 5, to);
		}
		else
		{
			drawLines(painter, xMap, yMap, canvasRect, from, to);
		}
		break;
	case Sticks:
		drawSticks(painter, xMap, yMap, canvasRect, from, to);
		break;
	case Steps:
		drawSteps(painter, xMap, yMap, canvasRect, from, to);
		break;
	case Dots:
		drawDots(painter, xMap, yMap, canvasRect, from, to);
		break;
	case NoCurve:
	default:
		break;
	}
}
