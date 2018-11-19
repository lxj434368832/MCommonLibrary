#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class QWidget;

class IMediaPlayer : public QObject
{
    Q_OBJECT

public:
	IMediaPlayer(QObject *parent = 0):QObject(parent){}
    virtual ~IMediaPlayer(){}

    virtual void setPlayWnd(void*) = 0;
    virtual void setMedia(const char*) =0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void jump(qint64) {}
    virtual void cutPicture(QString){}
    virtual void setVolume(int){}

    virtual int getPlayState() = 0;

signals:
    void signalLengthChanged(qint64);
    void signalPositionChanged(qint64);
    void signalVolumeChanged(int);
    void signalPlayStateChanged(QMediaPlayer::State);
    void signalMediaStatusChanged(QMediaPlayer::MediaStatus);

};

#endif // IMEDIAPLAYER_H
