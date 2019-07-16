#pragma once
/************************************************************************
* filename:MSamplingThread.h
* function: 模拟采样线程，1.setInterval(double), 2.start()
* author :	明巧文
* datetime:2018-12-14
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <qwt_sampling_thread.h>

class PlotCurveBase;

class MSamplingThread: public QwtSamplingThread
{
public:
    MSamplingThread(PlotCurveBase *plotMng);

    void SetSamplingRate(unsigned uSamplingRate);

protected:
    virtual void sample( double elapsed );

private:
	PlotCurveBase     *m_pPlotMng;
    unsigned				m_uSamplingRate;

	QList<unsigned char>    m_listWaveData;
};
