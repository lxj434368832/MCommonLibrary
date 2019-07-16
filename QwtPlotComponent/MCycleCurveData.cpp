#include "MCycleCurveData.h"

MCycleCurveData::MCycleCurveData():
    MCurveData()
{
}

bool MCycleCurveData::UpdateCurveData(qint64 elapsed)
{
	unsigned char ucValue = 0;

    do
    {
       QMutexLocker lck(&mutex);
        if(m_listPendingValue.empty()) break;

		ucValue = m_listPendingValue.front();
        m_listPendingValue.pop_front();
    }while(false);

	int iCount = m_vctValue.size();
	if (iCount < m_uMaxPointCount)
	{
		m_vctValue.append(QPointF(m_ulCumulativeTime, ucValue));
		//ÀÛ¼ÓÊ±¼ä
		m_ulCumulativeTime += m_uIntervalTime;
		if (0 == m_uCumulativeCount++ % m_uSamplingRate)
			m_ulCumulativeTime += m_uIntervalRemain;
		m_uCurrentPoint = iCount;
	}
	else
	{
		m_uCurrentPoint++;
		m_uCurrentPoint %= m_uMaxPointCount;
		if (0 == m_uCurrentPoint)
		{
			m_uCurrentPoint = 1;
		}
		m_vctValue[m_uCurrentPoint].setY(ucValue);
	}

    return true;
}

void MCycleCurveData::Reset()
{
    m_uCurrentPoint = 0;
    MCurveData::Reset();
}
