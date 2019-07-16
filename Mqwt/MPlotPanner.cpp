#include "MPlotPanner.h"
#include "qwt_plot.h"
#include "qwt_scale_div.h"

void MPlotPanner::SRange::checkValue(double &dMin, double &dMax, unsigned int unFactor) const
{
    if(dUpperBound > dLowerBound + 0.001)
    {
        double dRange = dMax - dMin;
        double dMargin = dRange / unFactor;

        if(dMax < dLowerBound + dMargin)
        {
            dMax = dLowerBound + dMargin;
            dMin = dMax - dRange;
        }

        if(dMin > dUpperBound - dMargin)
        {
            dMin = dUpperBound - dMargin;
            dMax = dMin + dRange;
        }
    }
}

MPlotPanner::MPlotPanner(QWidget *widget) :
    QwtPlotPanner(widget),
    m_unDivFactor(50)
{
    memset(m_aAxisRange, 0, sizeof(m_aAxisRange));
}

void MPlotPanner::setAxisRange(int axisId, double dMinValue, double dMaxValue)
{
    if(axisId < QwtPlot::axisCnt)
    {
        m_aAxisRange[axisId].dLowerBound = dMinValue;
        m_aAxisRange[axisId].dUpperBound = dMaxValue;
    }
}

void MPlotPanner::setDivFactor(unsigned int unDivFactor)
{
    m_unDivFactor = unDivFactor;
}

void MPlotPanner::moveCanvas(int dx, int dy)
{
    if ( dx == 0 && dy == 0 )
        return;

    QwtPlot *plot = this->plot();
    if ( plot == NULL )
        return;

    const bool doAutoReplot = plot->autoReplot();
    plot->setAutoReplot( false );

    for( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
    {
        if ( !isAxisEnabled(axis))
            continue;

        const QwtScaleMap map = plot->canvasMap( axis );

        const double p1 = map.transform( plot->axisScaleDiv( axis ).lowerBound() );
        const double p2 = map.transform( plot->axisScaleDiv( axis ).upperBound() );

        double d1, d2;
        if ( axis == QwtPlot::xBottom || axis == QwtPlot::xTop )
        {
            d1 = map.invTransform( p1 - dx );
            d2 = map.invTransform( p2 - dx );
        }
        else
        {
            d1 = map.invTransform( p1 - dy );
            d2 = map.invTransform( p2 - dy );
        }
        m_aAxisRange[axis].checkValue(d1, d2, m_unDivFactor);
        plot->setAxisScale( axis, d1, d2 );
    }

    plot->setAutoReplot( doAutoReplot );
    plot->replot();
}
