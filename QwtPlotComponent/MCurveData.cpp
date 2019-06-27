#include "MCurveData.h"
#include <QDebug>

MCurveData::MCurveData()
{
    m_dXAxisWidth = 5;    //单位秒
}

MCurveData::~MCurveData()
{
}

unsigned MCurveData::GetTimeInterval()
{
    return m_uIntervalTime;
}

QwtInterval MCurveData::GetXAxisInterval()
{
    //double dMin = m_vctValue.first().x();
    //QwtInterval interval = QwtInterval(dMin, dMin + m_dXAxisWidth * 1000);
    //return interval;
	if (m_vctValue.size() < m_uMaxPointCount)
	{
		return QwtInterval(0, m_dXAxisWidth * 1000);
	}
	else
	{
		return QwtInterval(m_vctValue.first().x(), m_vctValue.last().x());
	}
}

unsigned MCurveData::AddPendingValues(QList<unsigned char> list)
{
    mutex.lock();
    if(0 == pendingValues.size())
    {
        for(int i = 0; i < m_uSamplingRate; i++)
            pendingValues.append(0);
    }
    pendingValues.append(list);
    mutex.unlock();
	return pendingValues.size();
}

bool MCurveData::UpdateCurveData(qint64 elapsed)
{
	QPointF point(0, 0);

//	if (m_lCumulativeTime - elapsed >= m_uIntervalTime)
//	{
        //qWarning() << QString::fromLocal8Bit("波形绘制太快，将忽略本次绘制。");
//		return false;
//	}
	point.setX(m_ulCumulativeTime);

	do
	{
		QMutexLocker lck(&mutex);
		if (pendingValues.empty())
		{
			//qDebug() << QStringLiteral("未取到波形数据！");
			break;
		}

		point.setY(pendingValues.front());
		pendingValues.pop_front();
	} while (false);

	int iCount = m_vctValue.size();
	if (iCount < m_uMaxPointCount)
	{
		m_vctValue.append(point);
	}
	else
	{
		int idx = 0;
		//所有数据前移移位，首位被覆盖
		for (idx = 0; idx < iCount - 1; idx++)
		{
			m_vctValue[idx] = m_vctValue[idx + 1];
		}
		m_vctValue[idx] = point;
	}

	//计算范围
	if (m_boundingRect.width() < 0 || m_boundingRect.height() < 0)
	{
		m_boundingRect.setRect(point.x(), point.y(), 0.0, 0.0);
	}
	else
	{
		m_boundingRect.setRight(point.x());

		if (point.y() > m_boundingRect.bottom())
			m_boundingRect.setBottom(point.y());

		if (point.y() < m_boundingRect.top())
			m_boundingRect.setTop(point.y());
	}

	//累加时间
	m_ulCumulativeTime += m_uIntervalTime;
	if (0 == m_uCumulativeCount++ % m_uSamplingRate)
		m_ulCumulativeTime += m_uIntervvalRemainder;

    return true;
}

void MCurveData::AddHistoryData(quint64 ulStartTime, QVector<unsigned char> vctHstry)
{
    Reset();
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

void MCurveData::Reset()
{
	m_ulCumulativeTime = m_uIntervalTime;
	m_uCumulativeCount = 2;
	m_uMaxPointCount = m_uSamplingRate * m_dXAxisWidth + 1;

	m_vctValue.clear();
	m_vctValue.reserve(m_uMaxPointCount);
	m_vctValue.append(QPointF(0, 0));

	pendingValues.clear();

	m_boundingRect = QRectF(1.0, 1.0, -2.0, -2.0); // invalid;
}

void MCurveData::SetParam(unsigned uSamplingRate, double dXAxisWidth)
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

size_t MCurveData::size() const
{
    return m_vctValue.size();
}

QPointF MCurveData::sample(size_t i) const
{
    return m_vctValue[i];
}

QRectF MCurveData::boundingRect() const
{
    qDebug()<<"call MCurveData::boundingRect() .";
    return m_boundingRect;
}
