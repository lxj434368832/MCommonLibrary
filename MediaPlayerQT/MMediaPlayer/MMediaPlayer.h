#ifndef MMEDIAPLAYER_H
#define MMEDIAPLAYER_H

#include <QMediaPlayer>
#include "IMediaPlayer.h"
class VideoWidgetSurface;

class MMediaPlayer : public IMediaPlayer
{
    Q_OBJECT

public:
    MMediaPlayer(QObject *parent = 0);
    ~MMediaPlayer();
    virtual void setPlayWnd(void* wnd) override;
    void setMedia(const char* url) override;
    void play() override;
    void pause() override;
    void stop() override;
    void jump(qint64 position) override;
    void cutPicture(QString strFilePath) override;
    void setVolume(int value) override;
    int getPlayState() override;

private:
    VideoWidgetSurface                *m_surface;
    QMediaPlayer                            *m_mediaPlayer;
};

#endif // MMEDIAPLAYER_H
