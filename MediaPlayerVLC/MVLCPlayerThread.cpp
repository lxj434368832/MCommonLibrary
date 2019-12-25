#include "MVLCPlayerThread.h"
#include "vlc.h"
#include <QDebug>
#include <QLabel>

MVLCPlayerThread::MVLCPlayerThread(QObject *parent)
    :IVLCPlayer(parent)
{
    m_mediaPlayer = NULL;
    qRegisterMetaType<IMediaPlayer::EPlayState>("IMediaPlayer::EPlayState");
    qRegisterMetaType<std::string>("std::string");
	
    connect(this,SIGNAL(signal_set_media(std::string)),this, SLOT(slot_set_media(std::string)));
	connect(this, SIGNAL(signal_play()), this, SLOT(slot_play()));
	connect(this, SIGNAL(signal_pause()), this, SLOT(slot_pause()));
	connect(this, SIGNAL(signal_stop()), this, SLOT(slot_stop()));
    connect(this, SIGNAL(signal_jump(qint64)), this, SLOT(slot_jump(qint64)));
//    connect(this, SIGNAL(signal_cutPicture(std::string)), this, SLOT(slot_cutPicture(std::string)));
    connect(this, SIGNAL(signal_setVolume(int)), this, SLOT(slot_setVolume(int)));

	moveToThread(&m_threadWork);
	m_threadWork.start();
}

MVLCPlayerThread::~MVLCPlayerThread()
{
    qDebug() << "begin delete MVLCPlayerThread!";
    int iState = libvlc_media_player_get_state(m_mediaPlayer);
    if(iState && iState < ept_stopped)
        emit signal_stop();

    qDebug()<<"begin quit vlc player thread.";
    m_threadWork.quit();
    m_threadWork.wait();
    qDebug()<<"vlc player thread quit success.";

	// 释放VLC资源
	if (m_mediaPlayer != NULL)
	{
		libvlc_media_player_release(m_mediaPlayer);
		m_mediaPlayer = NULL;
	}
}

bool MVLCPlayerThread::init()
{
    if (m_mediaPlayer == NULL)
    {
        m_mediaPlayer = libvlc_media_player_new(m_vlcInst);
        if(NULL == m_mediaPlayer)
        {
            qDebug()<<"create VLC Media player failed!";
            return false;
        }

        // 事件管理
        if(false == installVLCEvent())
            return false;
    }
    return true;
}

void MVLCPlayerThread::setPlayWnd(void *wnd)
{
	m_renderWidget = (QLabel*)wnd;
	libvlc_media_player_set_hwnd(m_mediaPlayer, (void*)m_renderWidget->winId());
}

bool MVLCPlayerThread::setMedia(const char *url)
{
    emit signal_set_media(std::string(url));
	return true;
}

bool MVLCPlayerThread::play()
{
	emit signal_play();
    return true;
}

void MVLCPlayerThread::pause()
{
	emit signal_pause();
}

void MVLCPlayerThread::stop()
{
	emit signal_stop();
}

void MVLCPlayerThread::jump(qint64 position)
{
    emit signal_jump(position);
}

bool MVLCPlayerThread::cutPicture(const char *strFilePath)
{
    if(-1 == libvlc_video_take_snapshot(m_mediaPlayer, 0, strFilePath,0,0))
    {
        qDebug()<<"截图失败！";
        return false;
    }
    return true;
}

void MVLCPlayerThread::setVolume(int value)
{
    emit signal_setVolume(value);
}

MVLCPlayerThread::EPlayState MVLCPlayerThread::getPlayState()
{
    return  (EPlayState)libvlc_media_player_get_state(m_mediaPlayer);
}

void MVLCPlayerThread::setPlayRate(float rate)
{
    if(-1 == libvlc_media_player_set_rate(m_mediaPlayer,(float)rate))
    {
        qDebug()<<"libvlc_media_player_set_rate failed!";
    }
}

void MVLCPlayerThread::nextFrame()
{
    libvlc_media_player_next_frame(m_mediaPlayer);
}

void MVLCPlayerThread::forward()
{
    libvlc_time_t currentTime = libvlc_media_player_get_time(m_mediaPlayer);
    libvlc_media_player_set_time(m_mediaPlayer, currentTime - en_jump_step * 1000);
    //qDebug()<<"forward current time:"<<currentTime;
}

void MVLCPlayerThread::backward()
{
    libvlc_time_t currentTime = libvlc_media_player_get_time(m_mediaPlayer);
    libvlc_media_player_set_time(m_mediaPlayer, currentTime + en_jump_step * 1000);
    //qDebug()<<"backward current time:"<<currentTime;
}

bool MVLCPlayerThread::SetSize(int width ,int height)
{
    width, height;
    return true;
}

void MVLCPlayerThread::slot_set_media(std::string url)
{
    int iState = libvlc_media_player_get_state(m_mediaPlayer);
     if(iState && iState < ept_stopped) slot_stop();

    qDebug() << "vlc url:" << url.c_str();
    libvlc_media_t *m_vlcMedia = libvlc_media_new_location(m_vlcInst, url.c_str());
    if (NULL == m_vlcMedia)
	{
		qDebug() << "create VLC Media failed!";
		return;
	}

	libvlc_media_player_set_media(m_mediaPlayer, m_vlcMedia);
	libvlc_media_release(m_vlcMedia);
}

void MVLCPlayerThread::slot_play()
{
	if (-1 == libvlc_media_player_play(m_mediaPlayer))
	{
        qWarning() << "play failed!";
	}
}

void MVLCPlayerThread::slot_pause()
{
	libvlc_media_player_pause(m_mediaPlayer);
    qDebug() << "vlc play!";
}

void MVLCPlayerThread::slot_stop()
{
	if (m_mediaPlayer != NULL)
	{
		libvlc_media_player_stop(m_mediaPlayer);
    }
    qDebug() << "vlc stop!";
}

void MVLCPlayerThread::slot_jump(qint64 position)
{
    libvlc_media_player_set_time(m_mediaPlayer, position);
    qDebug()<<"Jump time is:"<<position;
}

void MVLCPlayerThread::slot_cutPicture(std::string strPath)
{
    if(-1 == libvlc_video_take_snapshot(m_mediaPlayer, 0, strPath.c_str(),0,0))
    {
        qDebug()<<"截图失败！";
    }
}

void MVLCPlayerThread::slot_setVolume(int value)
{
    if(value > 99) value = 99;
    if(-1 == libvlc_audio_set_volume(m_mediaPlayer, value))
    {
        qDebug()<<"声音设置失败！";
    }
}

bool MVLCPlayerThread::installVLCEvent()
{
    // 事件管理
    libvlc_event_manager_t *vlcEventManage = libvlc_media_player_event_manager(m_mediaPlayer);
    if(nullptr ==  vlcEventManage)
    {
        qDebug()<<"new vlc event manage object failed!";
        return false;
    }
    for (int i = libvlc_MediaPlayerMediaChanged ; i<= libvlc_MediaPlayerAudioVolume; i++ )
    {
        if(ENOMEM == libvlc_event_attach(vlcEventManage, i, libvlc_event_hander, this))
        {
            qDebug()<<"attach vlc event failed!";
            return false;
        }
    }
    return true;
}

void MVLCPlayerThread::libvlc_event_hander(const libvlc_event_t *event, void *opaque)
{
    QString strEventType;
    libvlc_time_t length, time;
     float volume;

     MVLCPlayerThread *pThis = (MVLCPlayerThread*)opaque;

    switch(event->type)
    {
    case libvlc_MediaPlayerLengthChanged:
        strEventType = "libvlc_MediaPlayerLengthChanged";
        length = event->u.media_player_length_changed.new_length;
        //qDebug()<<"current media length:"<< length;
        emit pThis->signalLengthChanged(length);
        break;
    case libvlc_MediaPlayerTimeChanged:
//        strEventType = "libvlc_MediaPlayerTimeChanged";
        time = event->u.media_player_time_changed.new_time;
//        qDebug()<<"current media time:"<< time;
        emit pThis->signalPositionChanged(time);
        break;
    case libvlc_MediaPlayerAudioVolume:
        strEventType = "libvlc_MediaPlayerAudioVolume";
        volume = event->u.media_player_audio_volume.volume;
        qDebug()<<"current media volume:"<< volume;
		if (volume >= 0)
		{
			volume = volume * 100;
		}
        emit pThis->signalVolumeChanged(volume);
        break;
    case libvlc_MediaPlayerPlaying:
        strEventType = "libvlc_MediaPlayerPlaying";
        emit pThis->signalPlayStateChanged(ept_playing);
        break;
    case libvlc_MediaPlayerPaused:
        strEventType = "libvlc_MediaPlayerPaused";
        emit pThis->signalPlayStateChanged(ept_paused);
        break;
    case libvlc_MediaPlayerStopped:
        strEventType = "libvlc_MediaPlayerStopped";
        emit pThis->signalPlayStateChanged(ept_stopped);
        break;
    case libvlc_MediaPlayerEndReached:
        strEventType = "libvlc_MediaPlayerEndReached";
        emit pThis->signalPlayStateChanged(ept_ended);
        break;
    case libvlc_MediaPlayerEncounteredError:
        strEventType = "libvlc_MediaPlayerEncounteredError";
        emit pThis->signalPlayStateChanged(ept_error);
        break;

    case libvlc_MediaPlayerForward:
        strEventType = "libvlc_MediaPlayerForward";
        break;
    case libvlc_MediaPlayerBackward:
        strEventType = "libvlc_MediaPlayerBackward";
        break;
    case libvlc_MediaPlayerSnapshotTaken:
        strEventType = "libvlc_MediaPlayerSnapshotTaken";
        break;
    default:
		strEventType = "unknown";
        break;
    }
	//if (!strEventType.isEmpty())
//        qDebug() << "libvlc event type is:" << strEventType << "value is:" << event->type;

}
