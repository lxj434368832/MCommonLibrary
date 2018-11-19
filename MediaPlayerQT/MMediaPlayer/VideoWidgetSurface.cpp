#include "videowidgetsurface.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>
#include <QDateTime>
#include <QDebug>

VideoWidgetSurface::VideoWidgetSurface(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent)
    , m_widget(widget)
    , m_imageFormat(QImage::Format_Invalid)
{
}

//! [0]
QList<QVideoFrame::PixelFormat> VideoWidgetSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_ARGB32
                       << QVideoFrame::Format_ARGB32_Premultiplied
                       << QVideoFrame::Format_RGB32
                       << QVideoFrame::Format_RGB24
                       << QVideoFrame::Format_RGB565
                       << QVideoFrame::Format_RGB555
                       << QVideoFrame::Format_ARGB8565_Premultiplied
                       << QVideoFrame::Format_BGRA32
                       << QVideoFrame::Format_BGRA32_Premultiplied
                       << QVideoFrame::Format_BGR32
                       << QVideoFrame::Format_BGR24
                       << QVideoFrame::Format_BGR565
                       << QVideoFrame::Format_BGR555
                       << QVideoFrame::Format_BGRA5658_Premultiplied
                       << QVideoFrame::Format_AYUV444
                       << QVideoFrame::Format_AYUV444_Premultiplied
                       << QVideoFrame::Format_YUV444
                       << QVideoFrame::Format_YUV420P
                       << QVideoFrame::Format_YV12
                       << QVideoFrame::Format_UYVY
                       << QVideoFrame::Format_YUYV
                       << QVideoFrame::Format_NV12
                       << QVideoFrame::Format_NV21
                       << QVideoFrame::Format_IMC1
                       << QVideoFrame::Format_IMC2
                       << QVideoFrame::Format_IMC3
                       << QVideoFrame::Format_IMC4
                       << QVideoFrame::Format_Y8
                       << QVideoFrame::Format_Y16
                       << QVideoFrame::Format_Jpeg
                       << QVideoFrame::Format_CameraRaw
                       << QVideoFrame::Format_AdobeDng;
}
//! [0]

//! [1]
bool VideoWidgetSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}
//! [1]

//! [2]
bool VideoWidgetSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty())
    {
        m_imageFormat = imageFormat;
        m_sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        m_widget->updateGeometry();
        updateVideoRect();

        return true;
    }
    else
    {
        return false;
    }
}
//! [2]

//! [3]
void VideoWidgetSurface::stop()
{
    m_pixmap = QPixmap();
    m_targetRect = QRect();

    QAbstractVideoSurface::stop();

    m_widget->update();
}
//! [3]

//! [4]
bool VideoWidgetSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size())
    {
        setError(IncorrectFormatError);
        stop();

        return false;
    }
    else
    {
        qint64 time =  QDateTime::currentDateTime().toMSecsSinceEpoch();
        if(time - m_currentTime < 40)			//进行间隔时间控制
            return true;
        m_currentTime = time;

        m_currentFrame = frame;
        m_widget->repaint(m_targetRect);

        return true;
    }
}
//! [4]

//! [5]
void VideoWidgetSurface::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(m_widget->size().boundedTo(size), Qt::KeepAspectRatio);

    m_targetRect = QRect(QPoint(0, 0), size);
    m_targetRect.moveCenter(m_widget->rect().center());
}
//! [5]

//! [6]
void VideoWidgetSurface::paint(QPainter *painter)
{
    if (m_currentFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop)
        {
           painter->scale(1, -1);
           painter->translate(0, -m_widget->height());
        }

        m_pixmap = QPixmap::fromImage(QImage(m_currentFrame.bits(),
                                                m_currentFrame.width(),
                                                m_currentFrame.height(),
                                                m_currentFrame.bytesPerLine(),
                                                m_imageFormat));

        //we can release the data
        m_currentFrame.unmap();
        painter->drawPixmap(m_targetRect, m_pixmap, m_sourceRect);

        painter->setTransform(oldTransform);
    }
}
//! [6]

void VideoWidgetSurface::snapshot(QString strFilePath)
{
    m_pixmap.save(strFilePath);
}
