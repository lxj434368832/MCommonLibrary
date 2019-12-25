#ifndef QWINTTS_H
#define QWINTTS_H

#include <QAxObject>
#include "ITTS.h"

class QWinTTS : public ITTS
{
    Q_OBJECT
public:
    QWinTTS(void);
    virtual ~QWinTTS(void);

    virtual bool initSpeech();
    virtual bool speak(QString & txt);
    virtual void pause();
    virtual void resume();
    virtual void stop();

    //rate range : -10 - 10
    virtual int rate();
    virtual void setRate(int rate);

    //volume range : 0 - 100
    virtual int volume();
    virtual void setVolume(int value) ;
    virtual bool isSpeaking();
private slots:

    void dealevent(QString name, int arc , void* argv);

private:
    QAxObject  m_axObjVoice;
    bool             m_bInit;
    bool            m_bReading;
};

#endif // QWINTTS_H
