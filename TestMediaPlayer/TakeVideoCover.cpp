#include "TakeVideoCover.h"
#include "../MediaPlayerVLC/MVLCPlayerSelfRender.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

TakeVideoCover*  TakeVideoCover::s_instance = nullptr;

TakeVideoCover::TakeVideoCover()
{
    m_playerCover = new MVLCPlayerSelfRender();
    m_playerCover->init();
    connect(m_playerCover, SIGNAL(signalLengthChanged(qint64)), this, SLOT(slotLengthChanged(qint64)));
    connect(m_playerCover, SIGNAL(signalPositionChanged(qint64)),this, SLOT(slotPositionChanged(qint64)));

}

TakeVideoCover::~TakeVideoCover()
{
    delete m_playerCover;
    m_playerCover = nullptr;
}

TakeVideoCover *TakeVideoCover::instance()
{
    if(nullptr == s_instance)
        s_instance = new TakeVideoCover;
    return s_instance;
}

void TakeVideoCover::deleteInstance()
{
    delete s_instance;
    s_instance = nullptr;
}

bool TakeVideoCover::GetFileCover(std::string strFilePath, std::string strCoverPath)
{
    m_strCoverPath = strCoverPath;
    m_nTakePosition = LLONG_MAX;
    m_bFinish = false;
    m_bSuccess = false;


    if(false == m_playerCover->setMedia(strFilePath.c_str()))
    {
        return m_bSuccess;
    }

    if(false == m_playerCover->play())
        return m_bSuccess;

    QTime t;
    t.start();

    while(false == m_bFinish &&  t.elapsed() < 10000)
        QCoreApplication::processEvents();
    qDebug()<<"get cover elapse :"<<t.elapsed();

    return m_bSuccess;
}

void TakeVideoCover::slotLengthChanged(qint64 length)
{
    if(0 == length)
    {
        m_bFinish = true;
        m_playerCover->stop();
        return;
    }

    if (length < TAKE_COVER_POSITION * 2)
    {
        m_nTakePosition = length / 4;
    }
    else
        m_nTakePosition = TAKE_COVER_POSITION;

    if (m_nTakePosition > 2000)
        m_playerCover->jump(m_nTakePosition);
}

void TakeVideoCover::slotPositionChanged(qint64 position)
{
    if (position >= m_nTakePosition && false == m_bFinish )
    {
        if (m_playerCover->cutPicture(m_strCoverPath.c_str()))
        {
            m_bSuccess = true;
            m_bFinish = true;
        }
    }
}
