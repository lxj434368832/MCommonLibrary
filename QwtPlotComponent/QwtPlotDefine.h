
#include "MCurveData.h"
#include "MPlotMagnifier.h"
#include "MPlotPanner.h"
#include <qwt_plot_directpainter.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <QElapsedTimer>
#include <windows.h>

enum EDrawStatus
{
	EDS_UNKNOWN,
	EDS_STOP,
	EDS_START,
	EDS_HISTORY			//��ʷ����
};


class QwtPlotData
{
public:
	QwtPlot             *pPlot = nullptr;
	MPlotMagnifier		*pMagnifier = nullptr;
	MPlotPanner			*pPanner = nullptr;
	QwtPicker			*pPicker = nullptr;
	QwtPlotMarker		*pMarker = nullptr;

	QVector<QwtPlotCurve*> vctCurve;
	QVector<MCurveData*>   vctCurveData;

	QwtPlotDirectPainter	*pDirectPainter = nullptr;
	QwtInterval				interval;

	int						iDrawTimerId = 0;
	HANDLE					hTimer = NULL;
	QElapsedTimer			elapsed;
	unsigned				uPlotIntervalCount = 0;
	unsigned				uPlotCounter = 0;			//���Ƽ���
	EDrawStatus				eStatus = EDS_UNKNOWN;

	unsigned				uCachePointRatio = 66;		//��������
	unsigned				uCacheCheckCounter = 0;		//���������
	unsigned				uCacheCheckInterval = 30;	//������������

	~QwtPlotData()
	{
		if (pDirectPainter)
		{
			delete pDirectPainter;
			pDirectPainter = nullptr;
		}

		if (pMarker && nullptr == pMarker->plot())
		{
			delete pMarker;
            pMarker = nullptr;
		}
	}
};
