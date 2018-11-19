#include "MediaPlayerWidget.h"
#include "WidgetVolumeSlider.h"
#include "ui_MediaPlayerWidget.h"
#include "MMediaPlayer/MMediaPlayer.h"
#include "../VideoListWidget.h"
#include <QDebug>
#include <QDateTime>

extern const QString  g_strTimeFormat;

MediaPlayerWidget::MediaPlayerWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MediaPlayerWidget)
{
    ui->setupUi(this);
    setMinimumSize(640,300);
    ui->lwVideoList->setFixedWidth(210);

    m_widgetVoiceSlider = new WidgetVolumeSlider(ui->widgetVideoWidget);
    m_widgetVoiceSlider->setVisible(false);

    m_player = new MMediaPlayer();
    m_player->setPlayWnd(ui->mediaWidget);
    m_player->setVolume(100);

    connect(m_player,SIGNAL(signalLengthChanged(qint64)),this, SLOT(slotLengthChanged(qint64)));
    connect(m_player,SIGNAL(signalPositionChanged(qint64)),this, SLOT(slotPositionChanged(qint64)));
    connect(m_widgetVoiceSlider,SIGNAL(signalVolumeChanged(int)),this,SLOT(slotVolumeChanged(int)));
    connect(m_player,SIGNAL(signalPlayStateChanged(QMediaPlayer::State)),
            this,SLOT(slotPlayStatechanged(QMediaPlayer::State)));
    connect(m_player,SIGNAL(signalMediaStatusChanged(QMediaPlayer::MediaStatus)),
            this,SLOT(slotMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MediaPlayerWidget::~MediaPlayerWidget()
{
    delete m_player;
    delete ui;
}

void MediaPlayerWidget::slot_open_video(QList<QListWidgetItem*> list, int current_row)
{
	show();
	raise();
	activateWindow();

//    QSize size = QSize(ui->lwVideoList->width()-10, 160);
//	ui->lwVideoList->setIconSize(size);
    ui->lwVideoList->clear();

    foreach (auto item, list)
    {
//        item->setSizeHint(size + QSize(0,12));
        ui->lwVideoList->addItem(item);
    }

    ui->lwVideoList->setCurrentRow(current_row);
    emit ui->lwVideoList->itemDoubleClicked(ui->lwVideoList->currentItem());    //播放当前item
}

void MediaPlayerWidget::on_btnPlayOrPause_clicked()
{
    if(m_strPlayFilePath.empty())
    {
//        return;
        m_strPlayFilePath = "D:\\ProgramSrc\\MCommonLibrary\\TestMediaFile\\TestVideo1.mp4";
        m_strPlayFileName =  "D:\\ProgramSrc\\MCommonLibrary\\TestMediaFile\\PlayerFile";
    }

    if(QMediaPlayer::PlayingState == m_player->getPlayState())
    {
        m_player->pause();
    }
    else if(QMediaPlayer::StoppedState == m_player->getPlayState() )
    {
        m_player->setMedia(m_strPlayFilePath.c_str());
        m_player->play();
    }
    else if(QMediaPlayer::PausedState == m_player->getPlayState() )
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
	m_player->cutPicture(QString("%1_%2.jpg").
		arg(QString::fromLocal8Bit(m_strPlayFileName.c_str())).
        arg(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")));
}

void MediaPlayerWidget::on_btnCircle_clicked()
{
    if(m_bCircle)
    {
        ui->btnCircle->setIcon(QIcon(":/res/Resources/MediaPlayerIcon/CircleDisabled.png"));
        m_bCircle = false;
    }
    else
    {
        ui->btnCircle->setIcon(QIcon(":/res/Resources/MediaPlayerIcon/Circle.png"));
        m_bCircle = true;
    }
}

void MediaPlayerWidget::on_btnVolume_clicked()
{
	if (m_widgetVoiceSlider->isVisible())
	{
		m_widgetVoiceSlider->hide();
	}
	else
    {
        QPoint pos_slider = ui->btnVolume->mapTo(ui->widgetVideoWidget, QPoint(0,0));
		pos_slider += QPoint((ui->btnVolume->width() - m_widgetVoiceSlider->width()) / 2, -15 - m_widgetVoiceSlider->height());
        m_widgetVoiceSlider->move(pos_slider);
        m_widgetVoiceSlider->show();
	}
}

void MediaPlayerWidget::on_btnFullScreen_clicked()
{
    if(ui->widgetVideoWidget->isFullScreen())
    {
        ui->widgetVideoWidget->setWindowFlags(Qt::SubWindow);
        ui->splitter->insertWidget(0,ui->widgetVideoWidget);
        ui->widgetVideoWidget->showNormal();
    }
    else
    {
        ui->widgetVideoWidget->setParent(0, Qt::Window);
        ui->widgetVideoWidget->showFullScreen();
    }
}

void MediaPlayerWidget::on_lwVideoList_itemDoubleClicked(QListWidgetItem *item)
{
    VideoListWidgetItem *current_item = dynamic_cast<VideoListWidgetItem*>(item);
	m_strPlayFilePath = current_item->get_dcm_path();

	m_player->setMedia(m_strPlayFilePath.c_str());
	m_player->play();

	int pos = m_strPlayFilePath.find_last_of('.');
	m_strPlayFileName = m_strPlayFilePath.substr(0, pos);

    m_iCurrentIndex = ui->lwVideoList->currentRow();
}

//以下为接收相关播放状态的槽
void MediaPlayerWidget::slotLengthChanged(qint64 length)
{
    QTime dtTime(0,0);
    dtTime =  dtTime.addMSecs(length);
    ui->lbTotalTime->setText(dtTime.toString(g_strTimeFormat));
    ui->sliderPlayProcess->setMaximum(length);
}

void MediaPlayerWidget::slotPositionChanged(qint64 position)
{
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

void MediaPlayerWidget::slotPlayStatechanged(QMediaPlayer::State state)
{
    qDebug()<<"current play State:"<<state;

    switch (state)
    {
    case QMediaPlayer::StoppedState:
//        ui->lbVideoWidget->clear();
        ui->btnPlayOrPause->setIcon( QIcon(":/res/Resources/MediaPlayerIcon/Play.png"));
        ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("播放"));
        ui->btnStop->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->lbCurrentTime->setText("00:00/");
        ui->lbTotalTime->setText("00:00");
        ui->btnCutPicture->setEnabled(false);
        ui->btnCircle->setEnabled(false);
        ui->sliderPlayProcess->setValue(0);
        break;

    case QMediaPlayer::PlayingState:
        ui->btnPlayOrPause->setIcon(QIcon(":/res/Resources/MediaPlayerIcon/Pause.png"));
        ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("暂停"));
        ui->btnStop->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->btnCutPicture->setEnabled(true);
        ui->btnCircle->setEnabled(true);
        break;

    case QMediaPlayer::PausedState:
        ui->btnPlayOrPause->setIcon(QIcon(":/res/Resources/MediaPlayerIcon/Play.png"));
        ui->btnPlayOrPause->setToolTip(QString::fromLocal8Bit("播放"));
        break;

    default:
        break;
    }
}

void MediaPlayerWidget::slotMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug()<<"current media status:"<<status;

    switch (status)
    {
    case QMediaPlayer::EndOfMedia:
        if(m_bCircle)
        {
            m_player->play();
        }
        break;
    case QMediaPlayer::InvalidMedia:

        break;
    default:
        break;
    }
}
