#include "MediaPlayerWidget.h"
#include "WidgetVolumeSlider.h"
#include "ui_MediaPlayerWidget.h"
#include "MMediaPlayer/MVLCPlayer.h"
//#include "../VideoListWidget.h"
#include <QDebug>
#include <QDateTime>
#include <QPainter>

extern const QString  g_strTimeFormat;

MediaPlayerWidget::MediaPlayerWidget(QWidget *parent) : 
    QFrame(parent),
    ui(new Ui::MediaPlayerWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint );
    setMinimumSize(640,300);

//    ui->titleBarWidget->hide();
    ui->lwVideoList->setFixedWidth(226);

	m_widgetVoiceSlider = new WidgetVolumeSlider(ui->widgetVideoWidget);
	m_widgetVoiceSlider->setVisible(false);

	m_player = new MVLCPlayer();
	m_player->init();
	m_player->setPlayWnd(ui->mediaWidget);
	m_player->setVolume(100);

	connect(m_widgetVoiceSlider, SIGNAL(signalVolumeChanged(int)), this, SLOT(slotVolumeChanged(int)));

	connect(m_player, SIGNAL(signalLengthChanged(qint64)), this, SLOT(slotLengthChanged(qint64)));
	connect(m_player, SIGNAL(signalPositionChanged(qint64)), this, SLOT(slotPositionChanged(qint64)));
    connect(m_player,SIGNAL(signalPlayStateChanged(IMediaPlayer::EPlayState)),
            this,SLOT(slotPlayStatechanged(IMediaPlayer::EPlayState)));
}

MediaPlayerWidget::~MediaPlayerWidget()
{
    delete m_player;
    delete ui;
}

void MediaPlayerWidget::on_btnMaximum_clicked()
{
    if(isMaximized())
        showNormal();
    else
        showMaximized();
}

void MediaPlayerWidget::on_btnClose_clicked()
{
	close();
}

void MediaPlayerWidget::closeEvent(QCloseEvent *)
{
    if(IMediaPlayer::ept_stopped != m_player->getPlayState())
        m_player->stop();
}

void MediaPlayerWidget::slot_open_video(unsigned report_id, QList<QListWidgetItem*> list, int current_row)
{
	show();
	raise();
	activateWindow();

	m_report_id = report_id;
    ui->lwVideoList->clear();
    foreach (auto item, list)
    {
        ui->lwVideoList->addItem(item);
    }

    ui->lwVideoList->setCurrentRow(current_row);
    emit ui->lwVideoList->itemDoubleClicked(ui->lwVideoList->currentItem());    //播放当前item
}

void MediaPlayerWidget::on_btnPlayOrPause_clicked()
{
    if(m_strPlayFilePath.empty())
	{
//		return;
        m_strPlayFilePath = "D:\\ProgramSrc\\MCommonLibrary\\TestMediaFile\\h265-2M.mp4";
        m_player->setMedia(m_strPlayFilePath.c_str());
		//m_player->setMedia(m_strPlayFilePath.c_str());
    }

	IMediaPlayer::EPlayState state = m_player->getPlayState();
	if (IMediaPlayer::ept_playing == state)
    {
        m_player->pause();
    }
	else
	{
		m_player->play();
    }
}

void MediaPlayerWidget::on_btnStop_clicked()
{
    m_player->stop();
}

void MediaPlayerWidget::on_sliderPlayProcess_sliderReleased()
{
    m_player->jump(ui->sliderPlayProcess->value());
}

void MediaPlayerWidget::on_btnNext_clicked()
{
    int idx = m_iCurrentIndex+ 1;
    if(idx == ui->lwVideoList->count())
        idx = 0;
    ui->lwVideoList->setCurrentRow(idx);
    emit ui->lwVideoList->itemDoubleClicked(ui->lwVideoList->currentItem());    //播放当前item
}

void MediaPlayerWidget::on_btnCutPicture_clicked()
{
	emit signal_media_player_get_snapshot_path(m_report_id);
}

void MediaPlayerWidget::slot_snapshot_path_to_media_player(std::string path)
{
	m_player->cutPicture(path.c_str());
    emit signal_media_player_snapshot_done(path);
}

void MediaPlayerWidget::on_btnCircle_clicked()
{
    if(m_bCircle)
    {
        ui->btnCircle->setIcon(QIcon(":/PlayIcon/Resource/CircleDisabled.png"));
        m_bCircle = false;
    }
    else
    {
        ui->btnCircle->setIcon(QIcon(":/PlayIcon/Resource/Circle.png"));
        m_bCircle = true;
    }
}

void MediaPlayerWidget::on_btnVolume_clicked()
{
    QPoint pos_slider = ui->btnVolume->mapTo(ui->widgetVideoWidget, QPoint(0, 0));
    pos_slider += QPoint((ui->btnVolume->width() - m_widgetVoiceSlider->width()) / 2, -12 - m_widgetVoiceSlider->height());
    m_widgetVoiceSlider->move(pos_slider);
    m_widgetVoiceSlider->show();
    m_widgetVoiceSlider->setFocus();
}

void MediaPlayerWidget::on_btnFullScreen_clicked()
{
    if(ui->widgetVideoWidget->isFullScreen())
    {
        ui->widgetVideoWidget->setWindowFlags(Qt::SubWindow);
        ui->widgetVideoWidget->showNormal();
    }
    else
	{
        ui->widgetVideoWidget->setWindowFlags(Qt::Window);
        ui->widgetVideoWidget->showFullScreen();
    }
}

void MediaPlayerWidget::on_lwVideoList_itemDoubleClicked(QListWidgetItem *item)
{
//    VideoListWidgetItem *current_item = dynamic_cast<VideoListWidgetItem*>(item);
//	m_strPlayFilePath = current_item->get_dcm_path();

//	m_player->setMedia(m_strPlayFilePath.c_str());
//	on_btnPlayOrPause_clicked();

//	int pos = m_strPlayFilePath.find_last_of('.');
//	m_strPlayFileName = m_strPlayFilePath.substr(0, pos);

//    m_iCurrentIndex = ui->lwVideoList->currentRow();
}

//以下为接收相关播放状态的槽
void MediaPlayerWidget::slotLengthChanged(qint64 length)
{
	if (length <= 10)
		return;
    QTime dtTime(0,0);
    dtTime =  dtTime.addMSecs(length);
    ui->lbTotalTime->setText(dtTime.toString(g_strTimeFormat));
    ui->sliderPlayProcess->setMaximum(length);
}

void MediaPlayerWidget::slotPositionChanged(qint64 position)
{
	bool bupdate = ui->sliderPlayProcess->updatesEnabled();
	QTime dtTime(0, 0);
	dtTime = dtTime.addMSecs(position);
	QString strTime = dtTime.toString(g_strTimeFormat) + '/';
	if (strTime != ui->lbCurrentTime->text())
	{
		ui->lbCurrentTime->setText(strTime);
	}
    ui->sliderPlayProcess->setValue(position);
}

void MediaPlayerWidget::slotVolumeChanged(int value)
{
    m_player->setVolume(value);
}

void MediaPlayerWidget::slotPlayStatechanged(IMediaPlayer::EPlayState state)
{
    //qDebug()<<"current play State:"<<state;
    switch (state)
    {
    case IMediaPlayer::ept_playing:
        ui->btnPlayOrPause->setIcon(QIcon(":/PlayIcon/Resource/Pause.png"));
        ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("暂停"));
        ui->btnStop->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->btnCutPicture->setEnabled(true);
        break;

    case IMediaPlayer::ept_paused:
        ui->btnPlayOrPause->setIcon(QIcon(":/PlayIcon/Resource/Play.png"));
        ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("播放"));
		break;

	case IMediaPlayer::ept_stopped:
        ui->btnPlayOrPause->setIcon(QIcon(":/PlayIcon/Resource/Play.png"));
		ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("播放"));
		ui->btnStop->setEnabled(false);
		ui->btnNext->setEnabled(false);
		ui->lbCurrentTime->setText("00:00/");
		ui->lbTotalTime->setText("00:00");
		ui->btnCutPicture->setEnabled(false);
		ui->sliderPlayProcess->setValue(0);
		break;

    case IMediaPlayer::ept_ended:
        if (m_bCircle)
        {
            m_player->stop();
            on_btnPlayOrPause_clicked();
        }
		else
        {
            m_player->stop();
		}

        break;

    default:
        break;
    }
}
