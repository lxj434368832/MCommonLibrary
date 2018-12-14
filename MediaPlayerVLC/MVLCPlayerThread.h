#ifndef MVLCPLAYER_H
#define MVLCPLAYER_H
/***************************************************************
 * filename: MVLCPlayerThread.h
 * function: 使用VLC进行视频播放。采用异步控制的方式，防止底层异常导致主线程卡死。
                VLC本身不能实现上一帧和倒播功能。
 * author:   明巧文
 * datetime: 2016-07-18
 ***************************************************************/
#include <QObject>
#include "IMediaPlayer.h"
#include <QMutex>
#include <QThread>

struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
class QLabel;

class  MVLCPlayerThread : public IMediaPlayer
{
    Q_OBJECT

public:
    MVLCPlayerThread(QObject *parent = 0);
    virtual ~MVLCPlayerThread();
	virtual void init() override;

    virtual void setPlayWnd(void* wnd) override;
	//设置播放地址，要保证当前是stop状态
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

signals:
    void signal_set_media(const char* url);
	void signal_play();
	void signal_pause();
	void signal_stop();

private slots:
	void slot_set_media(const char* url);
	void slot_play();
	void slot_pause();
	void slot_stop();

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

    static unsigned int      s_unReferenceCount;			//引用计数，用于保证GlobalInit和GlobalRelease只调用一次
    static QMutex            s_mutexReferenceCount;			//引用计数互斥量
    static libvlc_instance_t *m_vlcInst;                    //It represents a libvlc instance

    libvlc_media_t          *m_vlcMedia;					//代表一个抽象的可播放的媒体
    libvlc_media_player_t   *m_mediaPlayer;					//A LibVLC media player plays one media

	QThread					m_threadWork;
    QLabel                  *m_renderWidget;				//渲染窗体
};

#endif // MVLCPLAYER_H
