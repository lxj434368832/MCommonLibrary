#pragma once

#include <QObject>

#define TAKE_COVER_POSITION 5000    //获取封面的位置，单位ms

class MVLCPlayerSelfRender;

class TakeVideoCover : public QObject
{
    Q_OBJECT
public:
    TakeVideoCover();
    ~TakeVideoCover();
    bool GetFileCover(std::string strFilePath, std::string strCoverPath);
    static TakeVideoCover* instance();
    static void deleteInstance();

private slots:
    void slotLengthChanged(qint64 length);

    void slotPositionChanged(qint64 position);

private:
    static TakeVideoCover*  s_instance;
    MVLCPlayerSelfRender *m_playerCover;
    std::string                     m_strOpenFile ;
    std::string                     m_strCoverPath;
    qint64                           m_nTakePosition;
    bool                              m_bFinish;
    bool                              m_bSuccess;
};

