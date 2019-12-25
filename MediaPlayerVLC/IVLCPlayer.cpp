#include "IVLCPlayer.h"
#include "vlc.h"
#include <QDebug>

libvlc_instance_t* IVLCPlayer::m_vlcInst = NULL;     //It represents a libvlc instance
unsigned int IVLCPlayer::s_unReferenceCount = 0; //���ü��������ڱ�֤GlobalInit��GlobalReleaseֻ����һ��
QMutex         IVLCPlayer::s_mutexReferenceCount; //���ü���������


IVLCPlayer::IVLCPlayer(QObject *parent)
    :IMediaPlayer(parent)
{
    s_mutexReferenceCount.lock();
    if(0 == s_unReferenceCount++)   //��һ�ε�ʱ����ã��Ժ�ÿ�ζ������ü�����1
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
