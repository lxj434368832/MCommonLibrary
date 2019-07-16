#pragma once
/************************************************************************
* filename:	PlotPulseOxygen
* function:	绘制脉氧
* author :	明巧文
* datetime:	2019-04-28
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <QObject>

class QwtPlot;

class PlotPulseOxygen1 : public QObject
{
	Q_OBJECT

public:
    PlotPulseOxygen1(QwtPlot *plot);
    ~PlotPulseOxygen1();

    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    void BuildPlot();

    void Start();
    void Stop();

    //添加波形数据
    void AddWaveData(QList<unsigned char> list);

    //绘制历史数据
    void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> vctSPO2, QVector<unsigned char> vctPulse);

    void InitPlot();
    //添加网格
    void AddPlotGrid();

    //添加标记
    void AddMarker();

	void SetMarkerValue(double, double);

    //添加曲线图例(显示曲线名称)
    void AddLegend();

    //添加左右平移
	void AddPanner();

	// 添加滚轮放大/缩小
	void AddMagnifier();

    //添加按键缩放
    void AddZoomer();

	//添加拾取器
	void AddPicker();

    //添加波形曲线
    void AddWaveCurve();

signals:
	void signalMarkerPosition(double xPos);

protected:

	virtual bool eventFilter(QObject *, QEvent *);
    virtual void timerEvent( QTimerEvent * );
    //更新画布
    void UpdateCurve(qint64 elapsed);

private:
    QwtPlot              *m_pPlot;

	class PrivateData;
    PrivateData			 *data;
};
