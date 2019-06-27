#pragma once

#include <qwt_series_data.h>
#include <QMutex>


class MCycleCurveData: public QwtSeriesData<QPointF>
{
public:
    MCycleCurveData();
	~MCycleCurveData() override;

    unsigned GetTimeInterval();

    QwtInterval GetXAxisInterval();
	inline unsigned GetCurrentPoint(){ return m_uCurrentPoint; }

	void SetParam(unsigned uSamplingRate, double dXAxisWidth);
	void Reset();

	unsigned AddPendingValues(QList<unsigned char> list);
    bool UpdateCurveData(qint64 elapsed);

    void AddHistoryData(quint64 ulStartTime, QVector<unsigned char> vctHstry);

    size_t size() const override;
    QPointF sample( size_t i ) const override;
    QRectF boundingRect() const override;

private:
    QVector<QPointF> m_vctValue;
    unsigned		 m_uMaxPointCount;
    unsigned		 m_uCurrentPoint;
    QRectF           m_boundingRect;

    QMutex					mutex; // protecting pendingValues
    QList<unsigned char>	pendingValues;

    double          m_dXAxisWidth;         //x��Ŀ��
    unsigned		m_uSamplingRate;       //�����ʣ�ÿ�����ɵ����ݸ���
    unsigned        m_uIntervalTime;
    unsigned        m_uIntervvalRemainder;

    quint64         m_ulCumulativeTime;      //�ۼ�ʱ��
    unsigned        m_uCumulativeCount;		//�ۼӴ���
};
