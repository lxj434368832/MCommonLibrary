#include "MCycleCurveData.h"

MCycleCurveData::MCycleCurveData()
{
    m_dXAxisWidth = 5;    //单位秒
}

MCycleCurveData::~MCycleCurveData()
{
}

unsigned MCycleCurveData::GetTimeInterval()
{
    return m_uIntervalTime;
}

QwtInterval MCycleCurveData::GetXAxisInterval()
{
    double dMin = m_vctValue.first().x();
    QwtInterval interval = QwtInterval(dMin, dMin + m_dXAxisWidth * 1000);
    return interval;
    //if(m_vctValue.size() < m_uMaxPointCount)
    //{
    //    return QwtInterval(0,  m_dXAxisWidth * 1000);
    //}
    //else
    //{
    //    return QwtInterval(m_vctValue.first().x(), m_vctValue.last().x());
    //}
}

unsigned MCycleCurveData::AddPendingValues(QList<unsigned char> list)
{
    mutex.lock();
    pendingValues.append(list);
	mutex.unlock();
	return pendingValues.size();
}

bool MCycleCurveData::UpdateCurveData(qint64 elapsed)
{
	unsigned char ucValue = 0;

	//if (m_lCumulativeTime - elapsed >= m_uSamplingRate)
	//	return false;

    do
    {
       QMutexLocker lck(&mutex);
        if(pendingValues.empty()) break;

		ucValue = pendingValues.front();
        pendingValues.pop_front();
    }while(false);

	m_uCurrentPoint++;
	m_uCurrentPoint %= m_uMaxPointCount;
	if (0 == m_uCurrentPoint)
	{
		m_uCurrentPoint = 1;
	}
	m_vctValue[m_uCurrentPoint].setY(ucValue);
	//m_vctValue[(m_uCurrentPoint + 1) % m_uMaxPointCount].setY(0);


	//累加时间
	//m_lCumulativeTime += m_uIntervalTime;
	//if (0 == m_uCumulativeCount++ % m_uSamplingRate)
	//	m_lCumulativeTime += m_uIntervvalRemainder;

    return true;
}

void MCycleCurveData::AddHistoryData(quint64 ulStartTime, QVector<unsigned char> vctHstry)
{
    int iCount = vctHstry.count();
    m_vctValue.resize(iCount);

	m_ulCumulativeTime += ulStartTime;
    for(int i = 0; i < iCount; i++)
    {
        m_vctValue[i] = QPointF(m_ulCumulativeTime, vctHstry[i]);
        //累加时间
       m_ulCumulativeTime += m_uIntervalTime;
        if(0 == m_uCumulativeCount++ % m_uSamplingRate)
            m_ulCumulativeTime += m_uIntervvalRemainder;
    }
}

void MCycleCurveData::Reset()
{
    m_uCurrentPoint = 0;
	m_ulCumulativeTime = m_uIntervalTime;
	m_uCumulativeCount = 2;
	m_uMaxPointCount = m_uSamplingRate * m_dXAxisWidth + 1;

	m_vctValue.resize(m_uMaxPointCount);
	for (int i = 0; i < m_uMaxPointCount; i++)
	{
		m_vctValue[i] = QPointF(m_ulCumulativeTime, 0);
		//累加时间
		m_ulCumulativeTime += m_uIntervalTime;
		if (0 == m_uCumulativeCount++ % m_uSamplingRate)
			m_ulCumulativeTime += m_uIntervvalRemainder;
	}
	pendingValues.clear();
	m_boundingRect = QRectF(1.0, 1.0, -2.0, -2.0); // invalid;
}

void MCycleCurveData::SetParam(unsigned uSamplingRate, double dXAxisWidth)
{
    if(uSamplingRate >0 && m_uSamplingRate != uSamplingRate)
    {
        m_uSamplingRate = uSamplingRate;
        m_uIntervalTime  = qMax(int(1000 / m_uSamplingRate), 10);
        m_uIntervvalRemainder = 1000 % m_uSamplingRate;
    }

    if ( dXAxisWidth > 0.0 && dXAxisWidth != m_dXAxisWidth )
    {
        m_dXAxisWidth = dXAxisWidth;
    }

    Reset();
}

size_t MCycleCurveData::size() const
{
	return m_vctValue.size();
	//return 0;
}

QPointF MCycleCurveData::sample(size_t i) const
{
    return m_vctValue[i];
}

QRectF MCycleCurveData::boundingRect() const
{
    qDebug()<<"call MCurveData::boundingRect() .";
    return m_boundingRect;
}
