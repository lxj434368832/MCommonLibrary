#include "MCustomAssembly.h"
#include <QTime>

QwtText TimeScaleDraw::label(double v) const
{
	//        v /= 1000;
	//        return QString("%1s").arg(QString::number(v , 'f', 0));
	qint64 lCount = v;
	//        QTime time(0,0);
	//        time = time.addMSecs(iCount);
	QDateTime dt = QDateTime::fromMSecsSinceEpoch(lCount);
	return dt.toString("HH:mm:ss");
}

MCanvas::MCanvas(QwtPlot * plot) :
	QwtPlotCanvas(plot)
{
	// The backing store is important, when working with widget
	// overlays ( f.e rubberbands for zooming ).
	// Here we don't have them and the internal
	// backing store of QWidget is good enough.

	setPaintAttribute(QwtPlotCanvas::BackingStore, false);
	//        setBorderRadius( 10 );

	if (QwtPainter::isX11GraphicsSystem())
	{
#if QT_VERSION < 0x050000
		// Even if not liked by the Qt development, Qt::WA_PaintOutsidePaintEvent
		// works on X11. This has a nice effect on the performance.

		setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif

		// Disabling the backing store of Qt improves the performance
		// for the direct painter even more, but the canvas becomes
		// a native window of the window system, receiving paint events
		// for resize and expose operations. Those might be expensive
		// when there are many points and the backing store of
		// the canvas is disabled. So in this application
		// we better don't disable both backing stores.

		if (testPaintAttribute(QwtPlotCanvas::BackingStore))
		{
			setAttribute(Qt::WA_PaintOnScreen, true);
			setAttribute(Qt::WA_NoSystemBackground, true);
		}
	}

	setupPalette();
}

void MCanvas::setupPalette()
{
	QPalette pal = palette();

#if QT_VERSION >= 0x040400
	QLinearGradient gradient;
	gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
	gradient.setColorAt(0.0, QColor(0, 49, 110));
	gradient.setColorAt(1.0, QColor(0, 87, 174));

	pal.setBrush(QPalette::Window, QBrush(gradient));
#else
	pal.setBrush(QPalette::Window, QBrush(color));
#endif

	// QPalette::WindowText is used for the curve color
	pal.setColor(QPalette::WindowText, Qt::green);

	setPalette(pal);
}

QwtText MPlotPicker::trackerTextF(const QPointF & pos) const
{
	QwtText trackerText;
	QString text;
	QDateTime dt;
	QColor bkColor;
	switch (rubberBand())
	{
	case HLineRubberBand:
		text.sprintf("%.4f", pos.y());
		break;
	case VLineRubberBand:
		dt = QDateTime::fromMSecsSinceEpoch(pos.x());
		text.append(dt.toString("HH:mm:ss zzz"));
		//text.append(dt.toString("HH:mm:ss zzz, ")).append(QString::number(pos.y(), 'f', 4));
		bkColor = trackerPen().color();
		bkColor.setAlpha(60);
		trackerText.setBackgroundBrush(bkColor);
		//trackerText.setBackgroundBrush(QColor("#acc4ef"));
		trackerText.setBorderRadius(10);
		break;
	default:
		text.sprintf("%.4f, %.4f", pos.x(), pos.y());
	}

	trackerText.setText(text);
	return trackerText;
}

QRect MPlotPicker::trackerRect(const QFont & font) const
{
	QRect r = QwtPlotPicker::trackerRect(font);
	r += QMargins(6, 0, 6, 0);
	return r;
}

void MPlotMarker::drawLabel(QPainter * painter, const QRectF & canvasRect, const QPointF & pos) const
{
	if (label().isEmpty())
		return;

	Qt::Alignment align = labelAlignment();
	QPointF alignPos = pos;

	QSizeF symbolOff(0, 0);

	switch (lineStyle())
	{
	case QwtPlotMarker::VLine:
	{
		// In VLine-style the y-position is pointless and
		// the alignment flags are relative to the canvas

		if (labelAlignment() & Qt::AlignTop)
		{
			alignPos.setY(canvasRect.top());
			align &= ~Qt::AlignTop;
			align |= Qt::AlignBottom;
		}
		else if (labelAlignment() & Qt::AlignBottom)
		{
			// In HLine-style the x-position is pointless and
			// the alignment flags are relative to the canvas

			alignPos.setY(canvasRect.bottom() - 1);
			align &= ~Qt::AlignBottom;
			align |= Qt::AlignTop;
		}
		else
		{
			alignPos.setY(canvasRect.center().y());
		}
		break;
	}
	case QwtPlotMarker::HLine:
	{
		if (labelAlignment() & Qt::AlignLeft)
		{
			alignPos.setX(canvasRect.left());
			align &= ~Qt::AlignLeft;
			align |= Qt::AlignRight;
		}
		else if (labelAlignment() & Qt::AlignRight)
		{
			alignPos.setX(canvasRect.right() - 1);
			align &= ~Qt::AlignRight;
			align |= Qt::AlignLeft;
		}
		else
		{
			alignPos.setX(canvasRect.center().x());
		}
		break;
	}
	default:
	{
		if (symbol() &&
			(symbol()->style() != QwtSymbol::NoSymbol))
		{
			symbolOff = symbol()->size() + QSizeF(1, 1);
			symbolOff /= 2;
		}
	}
	}

	qreal pw2 = linePen().widthF() / 2.0;
	if (pw2 == 0.0)
		pw2 = 0.5;

	const int spacing = this->spacing();

	const qreal xOff = qMax(pw2, symbolOff.width());
	const qreal yOff = qMax(pw2, symbolOff.height());

	const QSizeF textSize = label().textSize(painter->font()) + QSizeF(12, 0);

	if (align & Qt::AlignLeft)
	{
		alignPos.rx() -= xOff + spacing;
		if (labelOrientation() == Qt::Vertical)
			alignPos.rx() -= textSize.height();
		else
			alignPos.rx() -= textSize.width();
	}
	else if (align & Qt::AlignRight)
	{
		alignPos.rx() += xOff + spacing;
	}
	else
	{
		if (labelOrientation() == Qt::Vertical)
			alignPos.rx() -= textSize.height() / 2;
		else
			alignPos.rx() -= textSize.width() / 2;
	}

	if (align & Qt::AlignTop)
	{
		alignPos.ry() -= yOff + spacing;
		if (labelOrientation() != Qt::Vertical)
			alignPos.ry() -= textSize.height();
	}
	else if (align & Qt::AlignBottom)
	{
		alignPos.ry() += yOff + spacing;
		if (labelOrientation() == Qt::Vertical)
			alignPos.ry() += textSize.width();
	}
	else
	{
		if (labelOrientation() == Qt::Vertical)
			alignPos.ry() += textSize.width() / 2;
		else
			alignPos.ry() -= textSize.height() / 2;
	}

	painter->translate(alignPos.x(), alignPos.y());
	if (labelOrientation() == Qt::Vertical)
		painter->rotate(-90.0);

	const QRectF textRect(0, 0, textSize.width(), textSize.height());
	label().draw(painter, textRect);
}

MPlotWaveCurve::MPlotWaveCurve(const QString & title) :
	QwtPlotCurve(title)
{
	m_iCurrentPoint = 0;
}

MPlotWaveCurve::MPlotWaveCurve(const QwtText & title) :
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
