#ifndef CUSTOMLABELSURFACE_H
#define CUSTOMLABELSURFACE_H

#include <QLabel>
#include <QAbstractVideoSurface>

class CustomLabelSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit CustomLabelSurface(QObject *parent = 0);
    ~CustomLabelSurface();
    void setWnd(QLabel *wnd);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
        bool isFormatSupported(const QVideoSurfaceFormat &format) const;
        bool present(const QVideoFrame &frame);

        bool start(const QVideoSurfaceFormat &format);
        void stop();
        void snapshot(QString strPath);

private:
        QPixmap          m_currentPixmap;
        QImage::Format   m_imageFormat;
        QLabel           *m_lbWidget;
        qint64           m_currentTime = 0;
};

#endif // CUSTOMLABELSURFACE_H
