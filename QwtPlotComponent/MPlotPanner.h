/*
 * function:    自定义平移器，用于实现平移不能移出边界
 * author:      明巧文
 * corporation:
 * datetime:    2017-03-23
*/
#ifndef MPLOTPANNER_H
#define MPLOTPANNER_H

#include "qwt_plot_panner.h"
#include "qwt_plot.h"

class MPlotPanner : public QwtPlotPanner
{
public:
    MPlotPanner(QWidget *widget);

    void setAxisRange(int axisId, double dMinValue, double dMaxValue);
    void setDivFactor(unsigned int unDivFactor = 50);

protected Q_SLOTS:
    virtual void moveCanvas( int dx, int dy );

private:
    struct SRange
    {
        double dLowerBound;
        double dUpperBound;
		void checkValue(double &dMin, double &dMax, unsigned int unFactor) const;
		void checkBorder(double &dMin, double &dMax, unsigned int unFactor) const;
    };
    unsigned int    m_unDivFactor;
    SRange  m_aAxisRange[QwtPlot::axisCnt];
};

#endif // MPLOTPANNER_H
