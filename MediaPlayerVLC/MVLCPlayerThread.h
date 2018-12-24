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
#include <QMutex>
#include <QThread>
#include "IVLCPlayer.h"

struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
class QLabel;

class  MVLCPlayerThread : public IVLCPlayer
{
    Q_OBJECT

public:
    MVLCPlayerThread(QObject *parent = 0);
    virtual ~MVLCPlayerThread();
    bool init() override;

    void setPlayWnd(void* wnd) override;
	//设置播放地址，要保证当前是stop状态
    bool setMedia(const char* url)override;
    bool play() override;
    void pause() override;
    void stop() override;
    void jump(qint64 position) override;
    bool cutPicture(const char * strFilePath) override;
    void setVolume(int value) override;
    EPlayState getPlayState() override;

protected:
    void setPlayRate(float rate);
    void nextFrame();
    void forward();
    void backward();
    virtual bool SetSize(int width, int height);

signals:
    void signal_set_media(std::string url);
	void signal_play();
	void signal_pause();
	void signal_stop();
    void signal_jump(qint64 position);
    void signal_cutPicture(std::string strPath);
    void signal_setVolume(int value);

private slots:
    void slot_set_media(std::string url);
	void slot_play();
	void slot_pause();
	void slot_stop();
    void slot_jump(qint64 position);
    void slot_cutPicture(std::string strPath);
    void slot_setVolume(int value);

private:
    bool installVLCEvent();

    /*********************************************************************************
     * function: vlc的事件处理回调函数
     * event:    事件的类型 libvlc_event_e
     *opaque:    注册回调的时候传入的数据
     *********************************************************************************/
    static void libvlc_event_hander(const libvlc_event_t *event, void *opaque);

private:
    enum
    {
        en_jump_step = 20   //快进快退的时间，单位为秒
    };

    libvlc_media_t               *m_vlcMedia;					//代表一个抽象的可播放的媒体
    libvlc_media_player_t   *m_mediaPlayer;					//A LibVLC media player plays one media

	QThread					m_threadWork;
    QLabel                  *m_renderWidget;				//渲染窗体
};

#endif // MVLCPLAYER_H
