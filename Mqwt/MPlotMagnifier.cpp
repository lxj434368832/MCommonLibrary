#include "MPlotMagnifier.h"
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_scale_div.h>

MPlotMagnifier::MPlotMagnifier(QWidget *widget) :
    QwtPlotMagnifier(widget)
{
    m_iInitFlag = 0;
}

void MPlotMagnifier::setXAxisRange(double dMin, double dMax)
{
    if(dMax - dMin > 0)
    {
        m_dXAxisMin = dMin;
        m_dXAxisMax = dMax;
        m_iInitFlag ++;
    }
}

void MPlotMagnifier::setYAxisRange(double dMin, double dMax)
{
    if(dMax - dMin > 0)
    {
        m_dYAxisMin = dMin;
        m_dYAxisMax = dMax;
        m_iInitFlag ++;
    }
}

void MPlotMagnifier::resetMagnifierStatus()
{
    m_iInitFlag = 0;
}

void MPlotMagnifier::rescale(double factor)
{
    QwtPlot *plot = QwtPlotMagnifier::plot();
    const QwtScaleDiv &xScale = plot->axisScaleDiv(QwtPlot::xBottom);
    const QwtScaleDiv &yScale = plot->axisScaleDiv(QwtPlot::yLeft);
    double dxScaleLowerBound  = xScale.lowerBound();
    double dxScaleUpperBound  = xScale.upperBound();
    double dyScaleLowerBound  = yScale.lowerBound();
    double dyScaleUpperBound  = yScale.upperBound();


    bool bRescale = true;
    if(0 == m_iInitFlag )
    {
        m_dXAxisMin = dxScaleLowerBound;
        m_dXAxisMax = dxScaleUpperBound;
        m_dYAxisMin = dyScaleLowerBound;
        m_dYAxisMax = dyScaleUpperBound;
        m_iInitFlag ++;
    }
    else if(factor > 1.0)           //代表缩小
    {
        bool bXAsixSatisfy = false;

        if( !isAxisEnabled(QwtPlot::xBottom) || !isAxisEnabled(QwtPlot::xTop)||
                ( dxScaleLowerBound <= m_dXAxisMin &&
                  m_dXAxisMax <= dxScaleUpperBound ) )
        {
            bXAsixSatisfy = true;
        }

        bool bYAsixSatisfy = false;
        if(!isAxisEnabled(QwtPlot::yLeft) || !isAxisEnabled(QwtPlot::yRight)||
                ( dyScaleLowerBound <= m_dYAxisMin &&
                  m_dYAxisMax <= dyScaleUpperBound ))
        {
            bYAsixSatisfy = true;
        }

        if(bXAsixSatisfy && bYAsixSatisfy)          //两个坐标轴同时缩小到范围内，则停止缩小
            bRescale = false;
    }

    if( bRescale)
        QwtPlotMagnifier::rescale(factor);
    else
    {
//        qDebug()<< "has zoom minimus value" ;
//        if(m_bEnableXAsix) qDebug()<<"init X Asix range:"<<m_dXAsixMin<< " -- "<< m_dXAsixMax;
//        if(m_bEnableYAsix) qDebug()<<"init Y Asix range:"<<m_dYAsixMin<< " -- "<< m_dYAsixMax;
    }

}
