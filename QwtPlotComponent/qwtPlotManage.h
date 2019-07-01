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

class qwtPlotManage : public QObject
{
    Q_OBJECT
public:
    qwtPlotManage(QwtPlot *plot);
    ~qwtPlotManage();

    /*************************************************************************
    * function: ���ó�ʼ������
    * param uSamplingRate: ������
    * param dXAxisWidth:     X����ʾ��ȣ���λΪs
    * param uPlotIntervalTime: ���Ƽ��ʱ�䣬��λΪms
    * return:
    *************************************************************************/
    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    void BuildPlot();

    void Start();
    void Stop();

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

    //�������ƽ��
    void AddPanner();

    //�������
    void AddZoomer();

    //��Ӳ�������
    void AddWaveCurve();


protected:
    virtual void timerEvent( QTimerEvent * );
    //���»���
    void UpdateCurve(qint64 elapsed);

private slots:
    void slotCanvasMoved(int dx, int dy);

private:
    QwtPlot              *m_pPlot;

	class PrivateData;
    PrivateData			 *data;
};
