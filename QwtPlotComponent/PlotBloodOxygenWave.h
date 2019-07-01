#pragma once
/************************************************************************
* filename:	PlotBloodOxygenWave
* function:	����Ѫ��������
* author :	������
* datetime:	2019-04-28
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include <QObject>

class QwtPlot;

class PlotBloodOxygenWave : public QObject
{
    Q_OBJECT
public:
    PlotBloodOxygenWave(QwtPlot *plot);
    ~PlotBloodOxygenWave();

    /*************************************************************************
    * function: ���ó�ʼ������
    * param uSamplingRate: ������
    * param dXAxisWidth:     X����ʾ��ȣ���λΪs
    * param uPlotIntervalTime: ���Ƽ��ʱ�䣬��λΪms
    * return:
    *************************************************************************/
    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    virtual void BuildPlot();

    virtual void Start();
    virtual void Stop();
	void Reset();

    //��Ӳ�������
    void AddWaveData(QList<unsigned char> list);

    //������ʷ����
    void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list);

    void InitPlot();

    //�������
    void AddPlotGrid();

    //��ӱ��
    void AddPlotMarker();

    //�������ͼ��(��ʾ��������)
    void AddLegend();

    // ��ӹ��ַŴ�/��С
    void AddMagnifier();
    void SetMagnifierEnabled(bool on);

    //�������ƽ��
    void AddPanner();
    void SetPannerEnabled(bool on);

    //�������
    void AddZoomer();

    //��Ӳ��λ���
    void AddWaveCurve();

	//���»���
	void UpdateCurve();

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    QwtPlot              *m_pPlot;
	unsigned			m_uDrawCounter;				//���Ƽ���
	unsigned			m_uDataCheckCounter;		//��Ӳ������ݼ���
    unsigned			m_uDataCheckInterval;

	class PrivateData;
    PrivateData			 *data;
};
