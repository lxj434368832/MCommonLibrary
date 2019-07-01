#pragma once
/************************************************************************
* filename:	PlotBloodOxygenWave
* function:	绘制血氧描述波
* author :	明巧文
* datetime:	2019-04-28
* company:  安碧捷科技股份有限公司
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
    * function: 设置初始化参数
    * param uSamplingRate: 采样率
    * param dXAxisWidth:     X轴显示宽度，单位为s
    * param uPlotIntervalTime: 绘制间隔时间，单位为ms
    * return:
    *************************************************************************/
    void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

    virtual void BuildPlot();

    virtual void Start();
    virtual void Stop();
	void Reset();

    //添加波形数据
    void AddWaveData(QList<unsigned char> list);

    //绘制历史数据
    void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list);

    void InitPlot();

    //添加网格
    void AddPlotGrid();

    //添加标记
    void AddPlotMarker();

    //添加曲线图例(显示曲线名称)
    void AddLegend();

    // 添加滚轮放大/缩小
    void AddMagnifier();
    void SetMagnifierEnabled(bool on);

    //添加左右平移
    void AddPanner();
    void SetPannerEnabled(bool on);

    //添加缩放
    void AddZoomer();

    //添加波形画布
    void AddWaveCurve();

	//更新画布
	void UpdateCurve();

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    QwtPlot              *m_pPlot;
	unsigned			m_uDrawCounter;				//绘制计数
	unsigned			m_uDataCheckCounter;		//添加波形数据计数
    unsigned			m_uDataCheckInterval;

	class PrivateData;
    PrivateData			 *data;
};
