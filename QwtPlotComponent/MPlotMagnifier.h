#ifndef MPLOTMAGNIFIER_H
#define MPLOTMAGNIFIER_H

#include <qwt_plot_magnifier.h>
#include <QRect>

class MPlotMagnifier : public QwtPlotMagnifier
{
public:
    MPlotMagnifier(QWidget *widget);
protected:
    virtual void rescale( double factor );

    int m_iCount;
    double m_dxScaleLowerBound;
    double m_dxScaleUpperBound;
    double m_dyScaleLowerBound;
    double m_dyScaleUpperBound;
};

#endif // MPLOTMAGNIFIER_H
