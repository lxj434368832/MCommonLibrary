#pragma once
/************************************************************************
* filename:	PlotBloodOxygenWave
* function:	绘制血氧描述波
* author :	明巧文
* datetime:	2019-04-28
* company:  安碧捷科技股份有限公司
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
    * function: 设置初始化参数
    * param uSamplingRate: 采样率
    * param dXAxisWidth:     X轴显示宽度，单位为s
    * param uPlotIntervalTime: 绘制间隔时间，单位为ms
    * return:
    *************************************************************************/
	virtual void SetParam(unsigned uSamplingRate , double dXAxisWidth, unsigned uPlotIntervalTime);

	virtual void BuildPlot();

	virtual void Start();
	virtual void Stop();
	virtual void Reset();

    //添加波形数据
    void AddWaveData(QList<unsigned char> list);

    //绘制历史数据
	virtual void DrawHistoryData(quint64 ulStartTime, QVector<unsigned char> list);

	virtual void InitPlot();
    //添加网格
	virtual void AddPlotGrid();

    //添加曲线图例(显示曲线名称)
	virtual void AddLegend();

    // 添加滚轮放大/缩小
	virtual void AddMagnifier();
	void SetMagnifierEnabled(bool on);

    //添加左右平移
	virtual void AddPanner();
	void SetPannerEnabled(bool on);

    //添加缩放
	virtual void AddZoomer();

	//添加标记
	virtual void AddMarker();
	void SetMarkerValue(double, double);
	void SetMarkerEnabled(bool on);

	//添加拾取器
	virtual void AddPicker();

    //添加波形曲线
	virtual void AddCurve();

	//更新画布
	virtual void UpdateCurve();

protected:

    void timerEvent( QTimerEvent * ) override;

protected:
	QwtPlotData			 *data;
};
