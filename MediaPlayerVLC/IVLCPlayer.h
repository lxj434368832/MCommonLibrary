#pragma once
/***************************************************************
 * filename: IVLCPlayer.h
 * function:  用于所有使用vlc库的资源管理器
 * author:   明巧文
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
    static unsigned int        s_unReferenceCount;         //引用计数，用于保证GlobalInit和GlobalRelease只调用一次
    static QMutex               s_mutexReferenceCount;    //引用计数互斥量
    static libvlc_instance_t *m_vlcInst;                            //It represents a libvlc instance

};
