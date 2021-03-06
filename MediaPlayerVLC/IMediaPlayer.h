#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <QObject>

class IMediaPlayer : public QObject
{
    Q_OBJECT

public:
    enum EPlayState
    {
        ept_unknown = 0,
        ept_opening,
        ept_buffering,
        ept_playing,
        ept_paused,
        ept_stopped,
        ept_ended,
        ept_error
    } ;

public:
	IMediaPlayer(QObject *parent = 0):QObject(parent){}
    virtual ~IMediaPlayer(){}

    virtual bool init(){return false;}
    virtual void setPlayWnd(void*) = 0;
    virtual bool setMedia(const char*) =0;
    virtual bool play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void jump(qint64) = 0;
    virtual bool cutPicture(const char *) = 0;
    virtual void setVolume(int) = 0;

    virtual EPlayState getPlayState() = 0;
    //获取视频的长度，单位为ms
    virtual qint64 getVideoLength(){return 0;}

signals:
    void signalLengthChanged(qint64);
    void signalPositionChanged(qint64);
    void signalVolumeChanged(int);
    void signalPlayStateChanged(IMediaPlayer::EPlayState);

};

#endif // IMEDIAPLAYER_H
