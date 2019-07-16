#pragma once

#include <qwt_series_data.h>
#include <QMutex>


class MCurveData: public QwtSeriesData<QPointF>
{
public:
    MCurveData();
	~MCurveData() override;

	void SetParam(unsigned uSamplingRate, double dXAxisWidth);

	//��ȡ���ƶ�ʱ��
    unsigned GetDrawTime();
	//��ȡ������
	unsigned GetSamplingRate();
	//��ȡx����
    QwtInterval GetXAxisInterval();

	//����������ݣ����������е����ݸ���
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
    QList<unsigned char>	m_listPendingValue;

    double          m_dXAxisWidth;          //x��Ŀ��
    unsigned		m_uSamplingRate;        //�����ʣ�ÿ�����ɵ����ݸ���
    unsigned        m_uIntervalTime;		//���ʱ��
    unsigned        m_uIntervalRemain;		//���ʱ������

    quint64         m_ulCumulativeTime;     //�ۼ�ʱ��
    unsigned        m_uCumulativeCount;		//�ۼӴ���
};
