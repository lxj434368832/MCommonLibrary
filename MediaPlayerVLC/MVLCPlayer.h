#ifndef MVLCPLAYER_H
#define MVLCPLAYER_H
/***************************************************************
 * filename: MVLCPlayer.h
 * function: 使用VLC2.2.4进行视频播放，VLC本身不能实现上一帧和倒播功能
 * author:   明巧文
 * datetime: 2016-07-18
 ***************************************************************/
#include <QObject>
#include "IMediaPlayer.h"
#include <QMutex>

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
class QLabel;

class  MVLCPlayer : public IMediaPlayer
{
    Q_OBJECT

public:
    MVLCPlayer(QObject *parent = 0);
    virtual ~MVLCPlayer();
    virtual void init() override;

    virtual void setPlayWnd(void* wnd) override;
    //要播放的文件地址，目录只能用斜杠分隔，不能用反斜杠，否则会出错
    virtual bool setMedia(const char* url)override;
    bool play() override;
    void pause() override;
    void stop() override;
    void jump(qint64 position) override;
    void cutPicture(const char * strFilePath) override;
    void setVolume(int value) override;
    EPlayState getPlayState() override;


    void setPlayRate(float rate);
    void nextFrame();
    void forward();
    void backward();
    virtual bool SetSize(int width, int height);

private:
    void installVLCEvent();

    /*********************************************************************************
     * function: vlc的事件处理回调函数
     * event:    事件的类型 libvlc_event_e
     *opaque:    注册回调的时候传入的数据
     *********************************************************************************/
    static void libvlc_event_hander(const libvlc_event_t *event, void *opaque);

protected:
    enum
    {
        en_jump_step = 20   //快进快退的时间，单位为秒
    };

    static unsigned int        s_unReferenceCount;         //引用计数，用于保证GlobalInit和GlobalRelease只调用一次
    static QMutex               s_mutexReferenceCount;    //引用计数互斥量
    static libvlc_instance_t *m_vlcInst;                        //It represents a libvlc instance

    libvlc_media_t                *m_vlcMedia;               //代表一个抽象的可播放的媒体
    libvlc_media_player_t   *m_mediaPlayer;                //A LibVLC media player plays one media

    QLabel                              *m_renderWidget;            //渲染窗体
};

#endif // MVLCPLAYER_H
