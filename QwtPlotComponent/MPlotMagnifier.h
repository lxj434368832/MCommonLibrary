#ifndef MPLOTMAGNIFIER_H
#define MPLOTMAGNIFIER_H
/*
 * function:    自定义放大器类，用于实现缩小时当窗口铺满时不能再缩小
 * author:      明巧文
 * corporation: 置晨海信科技有限公司成都分公司
 * datetime:    2017-03-15
*/

#include "qwt_plot_magnifier.h"


class MPlotMagnifier : public QwtPlotMagnifier
{
public:
    MPlotMagnifier(QWidget *widget);

    /************************************************
     * function:    此函数用于设置x轴的范围
     * dMin :        表示范围的最小值
     * dMax：      表示范围的最大值
    ************************************************/
    void setXAxisRange(double dMin, double dMax );

    /************************************************
     * function:    此函数用于设置Y轴的范围
     * dMin :        表示范围的最小值
     * dMax：      表示范围的最大值
    ************************************************/
    void setYAxisRange(double dMin, double dMax );

    /************************************************
     * function:    此函数用于标识需要重新获取范围值，但是有时间效果不明显，所以推荐前两个函数，直接给定范围值
     * dMin :        表示范围的最小值
     * dMax：      表示范围的最大值
    ************************************************/
    void resetMagnifierStatus();

protected:
    virtual void rescale( double factor );

private:
    int m_iInitFlag;

    double m_dXAxisMin;
    double m_dXAxisMax;
    double m_dYAxisMin;
    double m_dYAxisMax;
};

#endif // MPLOTMAGNIFIER_H
