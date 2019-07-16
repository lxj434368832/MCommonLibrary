#pragma once

#include <qwt_plot.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_canvas.h>
#include <qwt_painter.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

class TimeScaleDraw: public QwtScaleDraw
{
public:
	QwtText label(double v) const override;
};

class MCanvas: public QwtPlotCanvas
{
public:
	MCanvas(QwtPlot *plot = nullptr);

private:
	void setupPalette();
};

class MPlotPicker : public QwtPlotPicker
{
public:
	explicit MPlotPicker(QWidget *canvas) : 
		QwtPlotPicker(canvas){}
	explicit MPlotPicker(int xAxis, int yAxis, QWidget *parent) : 
		QwtPlotPicker(xAxis, yAxis, parent){}
	explicit MPlotPicker(int xAxis, int yAxis,
		RubberBand rubberBand, DisplayMode trackerMode, QWidget *parent):
		QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, parent) {}

protected:
	QwtText trackerTextF(const QPointF &pos) const override;
	QRect trackerRect(const QFont& font) const;
};

class MPlotMarker :public QwtPlotMarker
{
public:
	explicit MPlotMarker(const QString &title = QString::null) :
		QwtPlotMarker(title){}
protected:
	void drawLabel(QPainter *painter, const QRectF &canvasRect, const QPointF &pos) const override;
};


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