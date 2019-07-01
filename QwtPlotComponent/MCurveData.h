#pragma once

#include <qwt_series_data.h>
#include <QMutex>


class MCurveData: public QwtSeriesData<QPointF>
{
public:
    MCurveData();
	~MCurveData() override;

    unsigned GetTimeInterval();

    QwtInterval GetXAxisInterval();

    void SetParam(unsigned uSamplingRate ,double dXAxisWidth);

	//添加请求数据，返回请求中的数据个数
    unsigned AddPendingValues(QList<unsigned char> list);
    virtual bool UpdateCurveData(qint64 elapsed);
    virtual void Reset();

    void AddHistoryData(quint64 ulStartTime, QVector<unsigned char> vctHstry);

    size_t size() const override;
    QPointF sample( size_t i ) const override;
    QRectF boundingRect() const override;

protected:
    QVector<QPointF> m_vctValue;
    unsigned		 m_uMaxPointCount;
    QRectF           m_boundingRect;

    QMutex                          mutex; // protecting pendingValues
    QList<unsigned char>	pendingValues;

    double          m_dXAxisWidth;         //x轴的宽度
    unsigned		m_uSamplingRate;       //采样率，每秒生成的数据个数
    unsigned        m_uIntervalTime;
    unsigned        m_uIntervvalRemainder;

    quint64         m_ulCumulativeTime;     //累加时间
    unsigned        m_uCumulativeCount;		//累加次数
};
