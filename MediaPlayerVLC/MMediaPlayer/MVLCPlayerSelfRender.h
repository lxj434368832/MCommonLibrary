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
    virtual void init() override;
    virtual void setPlayWnd(void* wnd) override;
    virtual bool setMedia(const char* url) override;
    virtual bool SetSize(int width, int height) override;


signals:
    void signalShowImage(void* pImageData);

private slots:
    void slotShowImage(void *pImageData);

private:

    static unsigned libvlc_video_format_cb(void **opaque, char *chroma,
                                               unsigned *width, unsigned *height,
                                               unsigned *pitches,
                                               unsigned *lines);

    //�ص���ʾͼ��������ص�����
    /*********************************************************************************
     * function:Callback prototype to allocate and lock a picture buffer.
        Whenever a new video frame needs to be decoded,
        the lock callback is invoked. Depending on the video chroma,
        one or three pixel planes of adequate dimensions must be returned via the second parameter.
        Those planes must be aligned on 32-bytes boundaries.
     * opaque in:  private pointer as passed to libvlc_video_set_callbacks()
     * planes out: start address of the pixel planes (LibVLC allocates the array of void pointers,
     *  this callback must initialize the array)
     * ����ֵ:a private pointer for the display and unlock callbacks to identify the picture buffers
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
     * ����ֵ:   ��
     *********************************************************************************/
    static void libvlc_video_unlock_cb(void *opaque, void *picture, void *const *planes);

    /*********************************************************************************
     * function:Callback prototype to display a picture.
        When the video frame needs to be shown, as determined by the media playback clock,
        the display callback is invoked.
     * opaque in:  private pointer as passed to libvlc_video_set_callbacks()
     * picture in: private pointer returned from the libvlc_video_lock_cb callback
     * ����ֵ:   ��
     *********************************************************************************/
    static void libvlc_video_display_cb(void *opaque, void *picture);

private:
    QMutex                            m_mutexBuffer;                   //ͼ�񻺳����Ļ�����
    unsigned                         m_ibufferSize;

    unsigned                         m_uImageWidth = 1024;
    unsigned                         m_uImageHeigh = 576;

    QImage                              m_currentImage;             //��ǰͼ��
    unsigned char                   *m_uaImageBuffer;              //ͼ�����ݵĻ�����
};

#endif // MVLCPLAYERWINID_H
