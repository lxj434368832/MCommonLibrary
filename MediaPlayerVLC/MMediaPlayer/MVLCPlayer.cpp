#include "MVLCPlayer.h"
#include "vlc.h"
#include <QDebug>
#include <QLabel>

libvlc_instance_t* MVLCPlayer::m_vlcInst = NULL;  //It represents a libvlc instance
unsigned int MVLCPlayer::s_unReferenceCount = 0; //引用计数，用于保证GlobalInit和GlobalRelease只调用一次
QMutex         MVLCPlayer::s_mutexReferenceCount;//引用计数互斥量


MVLCPlayer::MVLCPlayer(QObject *parent)
    :IMediaPlayer(parent)
{
    m_mediaPlayer = NULL;
    qRegisterMetaType<IMediaPlayer::EPlayState>("IMediaPlayer::EPlayState");
}

MVLCPlayer::~MVLCPlayer()
{
    // 释放VLC资源
    if (m_mediaPlayer != NULL)
    {
        libvlc_media_player_release(m_mediaPlayer);
        m_mediaPlayer = NULL;
    }

    s_mutexReferenceCount.lock();
    if(--s_unReferenceCount <= 0)
    {
        qDebug()<<"call Global release!";
        if (m_vlcInst != NULL)
        {
            libvlc_release(m_vlcInst);
            m_vlcInst = NULL;
        }
    }
    s_mutexReferenceCount.unlock();
}

void MVLCPlayer::init()
{
    s_mutexReferenceCount.lock();
    if(0 == s_unReferenceCount++)   //第一次的时候调用，以后每次都给引用计数加1
    {
        if(NULL == m_vlcInst)
        {
            m_vlcInst = libvlc_new(0, NULL);
        }
        qDebug()<<"libvlc_new Funtion has called!";
    }
    s_mutexReferenceCount.unlock();

    if (m_mediaPlayer == NULL)
    {
        m_mediaPlayer = libvlc_media_player_new(m_vlcInst);
        if(NULL == m_mediaPlayer)
        {
            qDebug()<<"create VLC Media player failed!";
            return;
        }

        // 事件管理
        installVLCEvent();
    }
}

void MVLCPlayer::setPlayWnd(void *wnd)
{
    m_renderWidget =(QLabel*) wnd;
    libvlc_media_player_set_hwnd(m_mediaPlayer, (void*)m_renderWidget->winId());
}

bool MVLCPlayer::setMedia(const char *url)
{
    stop();

    qDebug()<<"vlc url:"<<url;
    libvlc_media_t *m_vlcMedia = libvlc_media_new_path(m_vlcInst, url);
    if(NULL == m_vlcMedia)
    {
        qDebug()<<"create VLC Media failed!";
        return false;
    }

    libvlc_media_player_set_media(m_mediaPlayer ,m_vlcMedia);
    libvlc_media_release(m_vlcMedia);

    return true;
}

void MVLCPlayer::play()
{
    if(-1 == libvlc_media_player_play(m_mediaPlayer))
    {
        qDebug()<<"play failed!";
    }
}

void MVLCPlayer::pause()
{
    libvlc_media_player_pause(m_mediaPlayer);
}

void MVLCPlayer::stop()
{
    if (m_mediaPlayer != NULL)
    {
        libvlc_media_player_stop(m_mediaPlayer);
    }
}

void MVLCPlayer::jump(qint64 position)
{
    libvlc_media_player_set_time(m_mediaPlayer, position);
    qDebug()<<"Jump time is:"<<position;
}

void MVLCPlayer::cutPicture(const char *strFilePath)
{
    if(-1 == libvlc_video_take_snapshot(m_mediaPlayer, 0, strFilePath,0,0))
    {
        qDebug()<<"截图失败！";
    }
}

void MVLCPlayer::setVolume(int value)
{
    if(value > 99) value = 99;
    if(-1 == libvlc_audio_set_volume(m_mediaPlayer, value))
    {
        qDebug()<<"声音设置失败！";
    }
}

MVLCPlayer::EPlayState MVLCPlayer::getPlayState()
{
    return  (EPlayState)libvlc_media_player_get_state(m_mediaPlayer);
}

void MVLCPlayer::setPlayRate(float rate)
{
    if(-1 == libvlc_media_player_set_rate(m_mediaPlayer,(float)rate))
    {
        qDebug()<<"libvlc_media_player_set_rate failed!";
    }
}

void MVLCPlayer::nextFrame()
{
    libvlc_media_player_next_frame(m_mediaPlayer);
}

void MVLCPlayer::forward()
{
    libvlc_time_t currentTime = libvlc_media_player_get_time(m_mediaPlayer);
    libvlc_media_player_set_time(m_mediaPlayer, currentTime - en_jump_step * 1000);
    qDebug()<<"forward current time:"<<currentTime;
}

void MVLCPlayer::backward()
{
    libvlc_time_t currentTime = libvlc_media_player_get_time(m_mediaPlayer);
    libvlc_media_player_set_time(m_mediaPlayer, currentTime + en_jump_step * 1000);
    qDebug()<<"backward current time:"<<currentTime;
}

bool MVLCPlayer::SetSize(int width ,int height)
{
    width, height;
    return true;
}

void MVLCPlayer::installVLCEvent()
{
    // 事件管理
    libvlc_event_manager_t *vlcEventManage = libvlc_media_player_event_manager(m_mediaPlayer);
    for (int i = libvlc_MediaPlayerMediaChanged ; i<= libvlc_MediaPlayerAudioVolume; i++ )
    {
        libvlc_event_attach(vlcEventManage, i, libvlc_event_hander, this);
    }
}

void MVLCPlayer::libvlc_event_hander(const libvlc_event_t *event, void *opaque)
{
    QString strEventType;
    libvlc_time_t length, time;
     float volume;

     MVLCPlayer *pThis = (MVLCPlayer*)opaque;

    switch(event->type)
    {
    case libvlc_MediaPlayerLengthChanged:
        strEventType = "libvlc_MediaPlayerLengthChanged";
        length = event->u.media_player_length_changed.new_length;
        qDebug()<<"current media length:"<< length;
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
        break;
    }
    if(!strEventType.isEmpty())
        qDebug()<<"libvlc event type is:"<<strEventType<<"value is:"<< event->type;

}
