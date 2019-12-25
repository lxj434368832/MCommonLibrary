#include "QWinTTS.h"

QWinTTS::QWinTTS(void) :
m_bInit(0),
m_bReading(0)
{
}

QWinTTS::~QWinTTS(void)
{
}


bool QWinTTS::initSpeech()
{

    if(m_bInit)
        return true;

    m_bInit = this->m_axObjVoice.setControl("96749377-3391-11D2-9EE3-00C04F797396");

    if(m_bInit)
    {
        connect(&this->m_axObjVoice,SIGNAL(signal(QString, int, void*)), this, SLOT(dealevent(QString, int, void*)));
    }

    return m_bInit;
}

bool QWinTTS::speak(QString & txt)
{
    if(!m_bInit)
        return false;

    int result = this->m_axObjVoice.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags)", txt ,1).toInt();
    m_bReading = true;
    return result;
}

void QWinTTS::pause()
{
    if(!m_bInit)
        return;
    m_bReading = false;
    this->m_axObjVoice.dynamicCall("Pause()");
}

void QWinTTS::resume()
{
    if(!m_bInit)
        return;
    m_bReading = true;
    this->m_axObjVoice.dynamicCall("Resume()");
}

void QWinTTS::stop()
{
    if(!m_bInit)
        return;
    m_bReading = false;
    int result = this->m_axObjVoice.dynamicCall("Speak(QString, SpeechVoiceSpeakFlags)", "" ,2).toInt();
}

bool QWinTTS::isSpeaking()
{
    return m_bReading;
}

//rate range : -10 - 10
int QWinTTS::rate()
{
    if(!m_bInit)
        return -99999;

    return this->m_axObjVoice.property("Rate").toInt();
}

void QWinTTS::setRate(int rate)
{
    if(!m_bInit)
        return;

    this->m_axObjVoice.dynamicCall("SetRate(int)", rate);
}

//volume range : 0 - 100
int QWinTTS::volume()
{

    if(!m_bInit)
        return -99999;

    return this->m_axObjVoice.property("Volume").toInt();
}

void QWinTTS::setVolume(int value)
{
    if(!m_bInit)
        return;

    this->m_axObjVoice.dynamicCall("SetVolume(int)", value);
}

void QWinTTS::dealevent(QString name, int arc , void* argv)
{
    if(name == "EndStream(int,QVariant)")
    {
        m_bReading = false;
        emit speakComplete();
    }
}
