#pragma once
/************************************************************************
* filename:	PlotBloodOxygenWave
* function:	����Ѫ��������
* author :	������
* datetime:	2019-04-28
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include <QObject>

class QwtPlotData;
class QwtPlot;

class PlotCurveBase : public QObject
{
    Q_OBJECT
public:
    PlotCurveBase(QwtPlot *plot);
    virtual ~PlotCurveBase();

    /*************************************************************************
    * function: ���ó�ʼ������
    * param uSamplingRate: ������
    * param dXAxisWidth:     X����ʾ��ȣ���λΪs
    * param uPlotIntervalTime: ���Ƽ��ʱ�䣬��λΪms
    * return:
    *************************************************************************/
	virtual void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

	virtual void BuildPlot();

	virtual void Start();
	virtual void Stop();
	virtual void Reset();

    //��Ӳ�������
    void AddWaveData(QList<unsigned char> list);

    //������ʷ����
	virtual void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list);

	virtual void InitPlot();
    //�������
	virtual void AddPlotGrid();

    //�������ͼ��(��ʾ��������)
	virtual void AddLegend();

    // ��ӹ��ַŴ�/��С
	virtual void AddMagnifier();
	void SetMagnifierEnabled(bool on);

    //�������ƽ��
	virtual void AddPanner();
	void SetPannerEnabled(bool on);

    //�������
	virtual void AddZoomer();

	//��ӱ��
	virtual void AddMarker();
	void SetMarkerValue(double, double);
	void SetMarkerEnabled(bool on);

	//���ʰȡ��
	virtual void AddPicker();

    //��Ӳ�������
	virtual void AddCurve();

	//���»���
	virtual void UpdateCurve();

protected:

    void timerEvent( QTimerEvent * ) override;

protected:
	QwtPlotData			 *data;
};
