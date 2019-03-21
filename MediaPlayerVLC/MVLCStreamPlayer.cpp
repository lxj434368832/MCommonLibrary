#include "MVLCStreamPlayer.h"
#include "vlc.h"
#include <QDebug>

MVLCStreamPlayer::MVLCStreamPlayer()
{

}

bool MVLCStreamPlayer::setMedia(const char *url)
{
    stop();

    qDebug()<<"vlc url:"<<url;
    libvlc_media_t *m_vlcMedia = libvlc_media_new_location(m_vlcInst, url);
    if(NULL == m_vlcMedia)
    {
        qDebug()<<"create VLC Media failed!";
        return false;
    }

    libvlc_media_player_set_media(m_mediaPlayer ,m_vlcMedia);
    libvlc_media_release(m_vlcMedia);

    return true;
}
