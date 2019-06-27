#include "MSamplingThread.h"
#include "PlotBloodOxygenWave.h"

MSamplingThread::MSamplingThread(PlotBloodOxygenWave *plotMng ):
    QwtSamplingThread( plotMng ),
    m_pPlotMng(plotMng)
{
	m_uSamplingRate = 20;
	m_listWaveData = {0,22,107,214,255,246,221,207,200,188,
		161,132,117,115,112,103,89,73,60,49,
		39,31,26,21,18,14,10,8,6,4,3,1,0};
}

void MSamplingThread::SetSamplingRate(unsigned uSamplingRate)
{
    if(uSamplingRate > 0 && uSamplingRate != m_uSamplingRate)
        m_uSamplingRate = uSamplingRate;
}

void MSamplingThread::sample(double elapsed)
{
    elapsed;
 //   QList<unsigned char>   listWaveData;

	//for (int i = 0; i < m_uSamplingRate; i++)
	//{
	//	//listWaveData.append(200);
	//	listWaveData.append(rand() % 230);
	//}

	//m_pPlotMng->AddWaveData(listWaveData);
	//unsigned char uValue = m_listWaveData.front();
	//m_listWaveData.pop_front();
	//m_listWaveData.append(uValue);
    m_pPlotMng->AddWaveData(m_listWaveData);
}
