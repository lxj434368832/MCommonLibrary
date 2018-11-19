#include "MMediaPlayer.h"
#include "VideoWidget.h"

MMediaPlayer::MMediaPlayer(QObject *parent):
    IMediaPlayer(parent)
{
    m_mediaPlayer = new QMediaPlayer();

    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)),
            this,SIGNAL(signalLengthChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SIGNAL(signalPositionChanged(qint64)));
    connect(m_mediaPlayer,SIGNAL(volumeChanged(int)),
            this, SIGNAL(signalVolumeChanged(int)));
    connect(m_mediaPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SIGNAL(signalPlayStateChanged(QMediaPlayer::State)));
    connect(m_mediaPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SIGNAL(signalMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MMediaPlayer::~MMediaPlayer()
{
    delete m_mediaPlayer;
}

void MMediaPlayer::setPlayWnd(void* wnd)
{
    m_surface =qobject_cast<VideoWidgetSurface*>( ((VideoWidget*)wnd)->videoSurface());
    m_mediaPlayer->setVideoOutput(m_surface);
}

void MMediaPlayer::setMedia(const char *url)
{
    QString filePath = QString::fromLocal8Bit(url);
    if(filePath.isEmpty())
        return;

    m_mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));
}

void MMediaPlayer::play()
{
     m_mediaPlayer->play();

}

void MMediaPlayer::pause()
{
    m_mediaPlayer->pause();
}

void MMediaPlayer::stop()
{
    m_mediaPlayer->stop();
}

void MMediaPlayer::jump(qint64 position)
{
    m_mediaPlayer->setPosition(position);
}

void MMediaPlayer::cutPicture(QString strFilePath)
{
    m_surface->snapshot(strFilePath);
}

void MMediaPlayer::setVolume(int value)
{
    m_mediaPlayer->setVolume(value);
}

int MMediaPlayer::getPlayState()
{
    return m_mediaPlayer->state();
}
