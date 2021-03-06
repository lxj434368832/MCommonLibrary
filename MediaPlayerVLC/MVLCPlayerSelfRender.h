#ifndef MVLCPLAYERWINID_H
#define MVLCPLAYERWINID_H

#include "MVLCPlayer.h"
#include <QImage>

class MVLCPlayerSelfRender : public MVLCPlayer
{
    Q_OBJECT

public:
    MVLCPlayerSelfRender(QObject *parent = 0);
    virtual ~MVLCPlayerSelfRender();
    bool init() override;
    void setPlayWnd(void* wnd) override;
    bool setMedia(const char* url) override;
    bool play() override;
    bool SetSize(int width, int height) override;
    //此函数直接保存当前点的图片，保证能成功，但是质量不好
    bool cutPictureEx(const char * strFilePath);

signals:
    void signalShowImage(void* pImageData);

private slots:
    void slotShowImage(void *pImageData);

private:

    static unsigned libvlc_video_format_cb(void **opaque, char *chroma,
                                               unsigned *width, unsigned *height,
                                               unsigned *pitches,
                                               unsigned *lines);

    //回调显示图像的三个回调函数
    /*********************************************************************************
     * function:Callback prototype to allocate and lock a picture buffer.
        Whenever a new video frame needs to be decoded,
        the lock callback is invoked. Depending on the video chroma,
        one or three pixel planes of adequate dimensions must be returned via the second parameter.
        Those planes must be aligned on 32-bytes boundaries.
     * opaque in:  private pointer as passed to libvlc_video_set_callbacks()
     * planes out: start address of the pixel planes (LibVLC allocates the array of void pointers,
     *  this callback must initialize the array)
     * 返回值:a private pointer for the display and unlock callbacks to identify the picture buffers
     *********************************************************************************/
    static void* libvlc_video_lock_cb(void *opaque, void **planes);

    /*********************************************************************************
     * function:Callback prototype to unlock a picture buffer.
        When the video frame decoding is complete, the unlock callback is invoked.
        This callback might not be needed at all.
        It is only an indication that the application can now read the pixel values if it needs to.
       Warning:A picture buffer is unlocked after the picture is decoded, but before the picture is displayed.
     * opaque in:  private pointer as passed to libvlc_video_set_callbacks()
     * picture in: private pointer returned from the libvlc_video_lock_cb callback
     * planes in:  pixel planes as defined by the libvlc_video_lock_cb callback (this parameter is only for convenience)
     * 返回值:   无
     *********************************************************************************/
    static void libvlc_video_unlock_cb(void *opaque, void *picture, void *const *planes);

    /*********************************************************************************
     * function:Callback prototype to display a picture.
        When the video frame needs to be shown, as determined by the media playback clock,
        the display callback is invoked.
     * opaque in:  private pointer as passed to libvlc_video_set_callbacks()
     * picture in: private pointer returned from the libvlc_video_lock_cb callback
     * 返回值:   无
     *********************************************************************************/
    static void libvlc_video_display_cb(void *opaque, void *picture);

    static void libvlc_audio_play_cb(void *data, const void *samples, unsigned count, qint64 pts);

private:
    QMutex                            m_mutexBuffer;                   //图像缓冲区的互斥量
    unsigned                         m_ibufferSize;

    unsigned                         m_uImageWidth;                 //图像的宽
    unsigned                         m_uImageHeigh;                 //图像的高

    QImage                              m_currentImage;             //当前图像
    unsigned char                   *m_uaImageBuffer;         //图像数据的缓冲区
};

#endif // MVLCPLAYERWINID_H
