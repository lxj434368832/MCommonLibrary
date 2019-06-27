#pragma once
/************************************************************************
* filename:MSamplingThread.h
* function: ģ������̣߳�1.setInterval(double), 2.start()
* author :	������
* datetime:2018-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include <qwt_sampling_thread.h>

class PlotBloodOxygenWave;

class MSamplingThread: public QwtSamplingThread
{
public:
    MSamplingThread(PlotBloodOxygenWave *plotMng);

    void SetSamplingRate(unsigned uSamplingRate);

protected:
    virtual void sample( double elapsed );

private:
    PlotBloodOxygenWave      *m_pPlotMng;
    unsigned            m_uSamplingRate;

	QList<unsigned char>   m_listWaveData;
};
