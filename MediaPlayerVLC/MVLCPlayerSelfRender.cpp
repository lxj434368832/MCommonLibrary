#include "MVLCPlayerSelfRender.h"
#include "vlc.h"
#include <QLabel>
#include <QDebug>
#include <QFile>

MVLCPlayerSelfRender::MVLCPlayerSelfRender(QObject *parent):
    MVLCPlayer(parent)
{
    m_renderWidget = nullptr;
    m_uImageWidth = 1280;
    m_uImageHeigh = 720;
    m_ibufferSize = 1920 * 1090 *4;
    m_uaImageBuffer = new unsigned char[m_ibufferSize ];

    connect(this,SIGNAL(signalShowImage(void*)),this, SLOT(slotShowImage(void*)));
}

MVLCPlayerSelfRender::~MVLCPlayerSelfRender()
{
    if(ept_playing == getPlayState())
        stop();

    if(m_uaImageBuffer)
    {
        delete [] m_uaImageBuffer;
        m_uaImageBuffer = NULL;
    }
}

bool MVLCPlayerSelfRender::init()
{
    if(false == MVLCPlayer::init())
        return false;

    //设置回调函数
    libvlc_video_set_callbacks(m_mediaPlayer, libvlc_video_lock_cb, libvlc_video_unlock_cb
                               ,libvlc_video_display_cb, this);

    libvlc_audio_set_callbacks(m_mediaPlayer, libvlc_audio_play_cb, NULL, NULL, NULL, NULL, this);

    //        libvlc_video_set_format_callbacks(m_mediaPlayer, libvlc_video_format_cb, NULL);
    return true;
}

void MVLCPlayerSelfRender::setPlayWnd(void *wnd)
{
    m_renderWidget =(QLabel*) wnd;
}

bool MVLCPlayerSelfRender::setMedia(const char *url)
{
    if(false == MVLCPlayer::setMedia(url))
        return false;

    if(m_renderWidget)
    {
        m_uImageWidth = m_renderWidget->width();
        m_uImageHeigh = m_renderWidget->height();
    }
    //设置vlc格式参数
    SetSize(m_uImageWidth, m_uImageHeigh );


    return true;
}

bool MVLCPlayerSelfRender::play()
{
    if(false == MVLCPlayer::play())
        return false;

    return true;
}

bool MVLCPlayerSelfRender::SetSize(int width, int height)
{
    if(0 == width || 0 == height)
        return false;

    m_uImageWidth = width;
    m_uImageHeigh = height;

    EPlayState state = getPlayState();
    if(ept_playing == state)
    {
        stop();
        //设置vlc格式参数
        libvlc_video_set_format(m_mediaPlayer,"RGBA", m_uImageWidth, m_uImageHeigh, m_uImageWidth * 4);
        play();
    }
    else
    {
        //设置vlc格式参数
//          libvlc_video_set_format(m_mediaPlayer,"RV32", m_uImageWidth, m_uImageHeigh, m_uImageWidth * 4);
        libvlc_video_set_format(m_mediaPlayer,"RGBA", m_uImageWidth, m_uImageHeigh, m_uImageWidth * 4);
    }

    return true;
}

bool MVLCPlayerSelfRender::cutPictureEx(const char *strFilePath)
{
    m_mutexBuffer.lock();
    m_currentImage.save(strFilePath, 0, 100);
    m_mutexBuffer.unlock();

    return QFile::exists(strFilePath);
}

unsigned MVLCPlayerSelfRender::libvlc_video_format_cb(void **opaque, char *chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines)
{
    MVLCPlayerSelfRender *pthis = static_cast<MVLCPlayerSelfRender *>(*opaque);
    if(pthis->m_uImageWidth != *width || pthis->m_uImageHeigh != *height)
    {
        pthis->m_uImageWidth = *width;
        pthis->m_uImageHeigh = *height;
        //        libvlc_video_set_format(pthis->m_mediaPlayer,"RV32", pthis->m_uImageWidth, pthis->m_uImageHeigh, pthis->m_uImageWidth * 4);
    }

    return  pthis->m_ibufferSize;
}

void *MVLCPlayerSelfRender::libvlc_video_lock_cb(void *opaque, void **planes)
{
    MVLCPlayerSelfRender *pthis = static_cast<MVLCPlayerSelfRender *>(opaque);
    pthis->m_mutexBuffer.lock();
    *planes = pthis->m_uaImageBuffer;
    return *planes;
}

void MVLCPlayerSelfRender::libvlc_video_unlock_cb(void *opaque, void *picture, void * const *planes)
{
    MVLCPlayerSelfRender *pthis = static_cast<MVLCPlayerSelfRender *>(opaque);
    pthis->m_mutexBuffer.unlock();
}

void MVLCPlayerSelfRender::libvlc_video_display_cb(void *opaque, void *picture)
{
    MVLCPlayerSelfRender *pthis = static_cast<MVLCPlayerSelfRender *>(opaque);
    pthis-> m_currentImage = QImage((uchar*)picture, pthis->m_uImageWidth,
                                    pthis->m_uImageHeigh,QImage::Format_ARGB32);
    emit pthis->signalShowImage(picture);
}

void MVLCPlayerSelfRender::libvlc_audio_play_cb(void *data, const void *samples, unsigned count, qint64 pts)
{
    data;
    samples;
    count;
    pts;
}

void MVLCPlayerSelfRender::slotShowImage(void *pImageData)
{
    Q_UNUSED(pImageData);
    if(m_renderWidget)
    {
//                m_renderWidget->setPixmap(QPixmap::fromImage( m_currentImage.scaled(m_renderWidget->size()/*, Qt::KeepAspectRatio*/)));
        m_renderWidget->setPixmap(QPixmap::fromImage( m_currentImage));
    }
}
