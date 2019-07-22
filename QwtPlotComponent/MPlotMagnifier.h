#ifndef MPLOTMAGNIFIER_H
#define MPLOTMAGNIFIER_H
/*
 * function:    �Զ���Ŵ����࣬����ʵ����Сʱ����������ʱ��������С
 * author:      ������
 * corporation: �ó����ſƼ����޹�˾�ɶ��ֹ�˾
 * datetime:    2017-03-15
*/

#include "qwt_plot_magnifier.h"


class MPlotMagnifier : public QwtPlotMagnifier
{
public:
    MPlotMagnifier(QWidget *widget);

    /************************************************
     * function:    �˺�����������x��ķ�Χ
     * dMin :        ��ʾ��Χ����Сֵ
     * dMax��      ��ʾ��Χ�����ֵ
    ************************************************/
    void setXAxisRange(double dMin, double dMax );

    /************************************************
     * function:    �˺�����������Y��ķ�Χ
     * dMin :        ��ʾ��Χ����Сֵ
     * dMax��      ��ʾ��Χ�����ֵ
    ************************************************/
    void setYAxisRange(double dMin, double dMax );

    /************************************************
     * function:    �˺������ڱ�ʶ��Ҫ���»�ȡ��Χֵ��������ʱ��Ч�������ԣ������Ƽ�ǰ����������ֱ�Ӹ�����Χֵ
     * dMin :        ��ʾ��Χ����Сֵ
     * dMax��      ��ʾ��Χ�����ֵ
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
