#pragma once
/***************************************************************
 * filename: IVLCPlayer.h
 * function:  ��������ʹ��vlc�����Դ������
 * author:   ������
 * datetime: 2016-07-18
 ***************************************************************/
#include "IMediaPlayer.h"

struct libvlc_instance_t;

class IVLCPlayer : public IMediaPlayer
{
public:
    IVLCPlayer(QObject *parent = 0);
    virtual ~IVLCPlayer();

protected:
    static unsigned int        s_unReferenceCount;         //���ü��������ڱ�֤GlobalInit��GlobalReleaseֻ����һ��
    static QMutex               s_mutexReferenceCount;    //���ü���������
    static libvlc_instance_t *m_vlcInst;                            //It represents a libvlc instance

};
