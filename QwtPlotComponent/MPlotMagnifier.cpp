#include "MPlotMagnifier.h"
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_scale_div.h>

MPlotMagnifier::MPlotMagnifier(QWidget *widget) :
    QwtPlotMagnifier(widget)
{
    m_iCount = 0;
}

void MPlotMagnifier::rescale(double factor)
{
//    qDebug()<<"current factor:"<<factor;

    QwtPlot *plot = QwtPlotMagnifier::plot();
    const QwtScaleDiv &xScale = plot->axisScaleDiv(QwtPlot::xBottom);
    const QwtScaleDiv &yScale = plot->axisScaleDiv(QwtPlot::yLeft);
    double dxScaleLowerBound  = xScale.lowerBound();
    double dxScaleUpperBound  = xScale.upperBound();
    double dyScaleLowerBound  = yScale.lowerBound();
    double dyScaleUpperBound  = yScale.upperBound();

    /*qDebug()<<"xScale lowerBound:"<< dxScaleLowerBound
           <<"xScale upperBound:"<<dxScaleUpperBound;
    qDebug()<<"yScale lowerBound:"<< dyScaleLowerBound
           <<"yScale upperBound:"<<dyScaleUpperBound;*/

    if(m_iCount == 0)
    {
        m_dxScaleLowerBound = dxScaleLowerBound;
        m_dxScaleUpperBound = dxScaleUpperBound;
        m_dyScaleLowerBound = dyScaleLowerBound;
        m_dyScaleUpperBound = dyScaleUpperBound;
        m_iCount ++;
    }
    else if(factor > 1.0 &&
            dxScaleLowerBound <= m_dxScaleLowerBound &&
            dxScaleUpperBound >= m_dxScaleUpperBound &&
            dyScaleLowerBound <= m_dyScaleLowerBound &&
            dyScaleUpperBound >= m_dyScaleUpperBound)
    {
        //qDebug()<<"has zoom in mininus";
        return;
    }

    QwtPlotMagnifier::rescale(factor);
}
