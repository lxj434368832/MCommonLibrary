#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QVideoFrame>

//! [0]
class VideoWidgetSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    VideoWidgetSurface(QWidget *widget, QObject *parent = 0);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;

    bool start(const QVideoSurfaceFormat &format);
    void stop();

    bool present(const QVideoFrame &frame);

    QRect videoRect() const { return m_targetRect; }
    void updateVideoRect();
    void paint(QPainter *painter);

    void snapshot(QString strFilePath);

private:
    QWidget             *m_widget;
    QImage::Format m_imageFormat;
    QRect                   m_sourceRect;
    QRect                   m_targetRect;
    QVideoFrame     m_currentFrame;
    QPixmap              m_pixmap;
    qint64                    m_currentTime = 0;
};
//! [0]

#endif
