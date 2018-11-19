#include "MVLCPlayerSelfRender.h"
#include "vlc.h"
#include <QLabel>
#include <QDebug>

MVLCPlayerSelfRender::MVLCPlayerSelfRender(QObject *parent):
    MVLCPlayer(parent)
{
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

void MVLCPlayerSelfRender::init()
{
    MVLCPlayer::init();
    //设置回调函数
    libvlc_video_set_callbacks(m_mediaPlayer, libvlc_video_lock_cb, libvlc_video_unlock_cb
                                   ,libvlc_video_display_cb, this);

//        libvlc_video_set_format_callbacks(m_mediaPlayer, libvlc_video_format_cb, NULL);

}

void MVLCPlayerSelfRender::setPlayWnd(void *wnd)
{
    m_renderWidget =(QLabel*) wnd;
}

bool MVLCPlayerSelfRender::setMedia(const char *url)
{
    if(false == MVLCPlayer::setMedia(url))
        return false;

    m_uImageWidth = 640;
    m_uImageHeigh = 480;

        if(-1 == libvlc_media_player_play(m_mediaPlayer))
        {
            qDebug()<<"play failed!";
        }
        else
        {
            libvlc_video_get_size(m_mediaPlayer,0, &m_uImageWidth, &m_uImageHeigh);
            libvlc_media_player_stop(m_mediaPlayer);
            //设置vlc格式参数
            libvlc_video_set_format(m_mediaPlayer,"RV32", m_uImageWidth, m_uImageHeigh, m_uImageWidth * 4);
        }
        return true;
}

bool MVLCPlayerSelfRender::SetSize(int width, int height)
{
    libvlc_media_player_stop(m_mediaPlayer);

    //设置vlc格式参数
    libvlc_video_set_format(m_mediaPlayer,"RV32", width, height, width * 4);

    if(-1 == libvlc_media_player_play(m_mediaPlayer))
    {
        qDebug()<<"play failed";
        return false;
    }
    return true;
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
                                   pthis->m_uImageHeigh,QImage::Format_RGB32);

    emit pthis->signalShowImage(picture);
}

void MVLCPlayerSelfRender::slotShowImage(void *pImageData)
{
    Q_UNUSED(pImageData);
    m_renderWidget->setPixmap(QPixmap::fromImage( m_currentImage.scaled(m_renderWidget->size()/*, Qt::KeepAspectRatio*/)));
}
