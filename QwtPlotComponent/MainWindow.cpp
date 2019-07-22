#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PlotBloodOxygenWave.h"
#include "MSamplingThread.h"
#include "PlotPulseOxygen.h"
#include <QDebug>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_pPlotMng = new PlotBloodOxygenWave(ui->qwtPlot);
    m_pPlotMngTwo = new PlotPulseOxygen(ui->qwtPlotTwo);

	m_pSamplingTh = new MSamplingThread(m_pPlotMng);
	m_pSamplingTh->setInterval(1000);
	
    //���ò���
    on_btnConfirmModify_clicked();
    //��������
	m_pPlotMng->BuildPlot();   
	m_pPlotMng->AddPanner();
	m_pPlotMng->AddMagnifier();
    m_pPlotMng->AddMarker();
	ui->qwtPlot->enableAxis(QwtPlot::xBottom, true);

    m_pPlotMngTwo->BuildPlot();

    connect(m_pPlotMngTwo, SIGNAL(signalMarkerPosition(double)), m_pPlotMng, SLOT(slotMarkerPosition(double)));
}

MainWindow::~MainWindow()
{
    m_pSamplingTh->stop();
    if(false == m_pSamplingTh->wait(2000))
    {
        qDebug()<<"ֹͣ�����̳߳�ʱ������ǿ����ֹ��";
//        m_thSampling->terminate();
    }
	//delete m_thSampling;

    m_pPlotMng->Stop();

	delete m_pPlotMng;
	m_pPlotMng = nullptr;
	delete m_pPlotMngTwo;
	m_pPlotMngTwo = nullptr;

    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    if("start" == ui->btnStart->text()  )
    {
        ui->btnStart->setText("stop");
		m_pPlotMng->Reset();

		m_pPlotMng->SetMagnifierEnabled(false);
		m_pPlotMng->SetPannerEnabled(false);
		m_pPlotMng->SetMarkerEnabled(false);
        m_pPlotMng->Start();
        m_pSamplingTh->start();
    }
    else
    {
        ui->btnStart->setText("start");
        m_pPlotMng->Stop();
        m_pSamplingTh->stop();

		m_pPlotMng->SetMagnifierEnabled(true);
		m_pPlotMng->SetPannerEnabled(true);
		m_pPlotMng->SetMarkerEnabled(true);
    }
}

/*
//ʵ����ͼ��
    QwtLegend *legend = new QwtLegend;
    //ͼ������ѡ��,Checkable
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    //pwtPlot�в���ͼ��
    ui->qwtPlot->insertLegend(legend, QwtPlot::LeftLegend );
    //���Ӳۣ�����ѡ���¼�
    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
        SLOT( legendChecked( const QVariant &, bool ) ) );
//ͼ��ѡ���¼�
void MainWindow::legendChecked( const QVariant &itemInfo, bool on )
{
   //��ȡ����
   QwtPlotItem *plotItem = ui->qwtPlot->infoToItem( itemInfo );
   //����ͼ��ѡ��״̬�������������ػ�����ʾ
   if ( plotItem )
       plotItem->setVisible(on);
   //��Ҫ���������û�в�������
   ui->qwtPlot->replot();
}  */

void MainWindow::on_btnConfirmModify_clicked()
{
    unsigned uValue = 0;
    unsigned uSamplingRate = 0;
    QString qstrInterval;
    uSamplingRate = ui->leSamplingRate->text().toUInt();

    uValue =  1000 / uSamplingRate;
    qstrInterval += QString::number( uValue).append("ms / ");
    uValue = 1000 % uSamplingRate;
    qstrInterval += QString::number( uValue).append("ms");
    ui->leIntervalTime->setText(qstrInterval);

	m_pSamplingTh->SetSamplingRate(uSamplingRate);
    m_pPlotMng->SetParam(uSamplingRate, ui->leXAxisWidth->text().toDouble(),  ui->lePlotIntervalTime->text().toUInt());
    m_pPlotMngTwo->SetParam(1, ui->leXAxisWidth->text().toDouble(), 2000);
}

void MainWindow::on_btnDrawHistory_clicked()
{
	if ("stop" == ui->btnStart->text())
		on_btnStart_clicked();

    unsigned uSamplingRate = ui->leSamplingRate->text().toUInt();
    qint64    uDrawLength = ui->leDrawLength->text().toDouble() * uSamplingRate * 3600;
    QVector<unsigned char> vctHistory;
    vctHistory.reserve(uDrawLength);
    for(qint64 i = 0; i < uDrawLength; i++)
    {
        vctHistory.append(rand() % 255);
    }

    QElapsedTimer   time;
    time.start();
    m_pPlotMng->DrawHistoryData(QDateTime::currentMSecsSinceEpoch(), vctHistory);
    qDebug()<<QStringLiteral("������ʱ:%1ms").arg(QString::number(time.elapsed()));
}

void MainWindow::on_btnSecondDrawHistory_clicked()
{
    qint64    uDrawLength = ui->leSecondDrawLength->text().toDouble() * 3600;
    QVector<unsigned char> vctSPOHistory;
    vctSPOHistory.reserve(uDrawLength);
    QVector<unsigned char> vctPulseHistory;
    vctPulseHistory.reserve(uDrawLength);

    unsigned char uValue = 0;
    for(qint64 i = 0; i < uDrawLength; i++)
    {
       uValue = 70 + rand() % 10;
        vctSPOHistory.append(uValue);
        uValue = 60 + rand() % 10;
        vctPulseHistory.append(uValue);
    }

    QElapsedTimer   time;
    time.start();
    m_pPlotMngTwo->DrawHistoryData(QDateTime::currentMSecsSinceEpoch(), vctSPOHistory, vctPulseHistory);
    qDebug()<<QStringLiteral("������ʱ:%1ms").arg(QString::number(time.elapsed()));
}

void MainWindow::on_btnAddMarker_clicked()
{
    static int Id = 10;
    QwtInterval interval = ui->qwtPlotTwo->axisInterval(QwtPlot::xBottom);
    qint64 ulTime  = rand() % (qint64)interval.width();
    std::string strText = std::to_string(ulTime);
    ulTime += interval.minValue();
    m_pPlotMngTwo->UpdateEventMarker(Id++, ulTime,  strText);
}
