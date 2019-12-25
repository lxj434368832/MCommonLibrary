#pragma once

#include <QObject>

class ITTS : public QObject
{
    Q_OBJECT
public:
    ITTS(void){}
    virtual ~ITTS(void){}

    virtual bool initSpeech() = 0;
    virtual bool speak(QString & txt) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    //rate range : -10 - 10
    virtual int rate() = 0;
    virtual void setRate(int rate) = 0;

    //volume range : 0 - 100
    virtual int volume() = 0;
    virtual void setVolume(int value)  = 0;
    virtual bool isSpeaking() = 0;

signals:
    void speakComplete();
};
