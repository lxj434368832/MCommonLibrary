#include "IVLCPlayer.h"
#include "vlc.h"
#include <QDebug>

libvlc_instance_t* IVLCPlayer::m_vlcInst = NULL;     //It represents a libvlc instance
unsigned int IVLCPlayer::s_unReferenceCount = 0; //引用计数，用于保证GlobalInit和GlobalRelease只调用一次
QMutex         IVLCPlayer::s_mutexReferenceCount; //引用计数互斥量


IVLCPlayer::IVLCPlayer(QObject *parent)
    :IMediaPlayer(parent)
{
    s_mutexReferenceCount.lock();
    if(0 == s_unReferenceCount++)   //第一次的时候调用，以后每次都给引用计数加1
    {
        if(NULL == m_vlcInst)
        {
            m_vlcInst = libvlc_new(0, nullptr);
        }
        qDebug()<<"libvlc_new Funtion has called!";
    }
    s_mutexReferenceCount.unlock();
}

IVLCPlayer::~IVLCPlayer()
{
    s_mutexReferenceCount.lock();
    if(--s_unReferenceCount <= 0)
    {
        if (m_vlcInst != NULL)
        {
            libvlc_release(m_vlcInst);
            m_vlcInst = NULL;
            qDebug()<<"Global released!";
        }
    }
    s_mutexReferenceCount.unlock();
}
