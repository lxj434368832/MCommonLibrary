#include "CustomLabelSurface.h"
#include <QDateTime>
#include <QVideoSurfaceFormat>

CustomLabelSurface::CustomLabelSurface(QObject *parent)
    :QAbstractVideoSurface(parent)
    ,m_lbWidget(NULL)
{
}

CustomLabelSurface::~CustomLabelSurface()
{
}

void CustomLabelSurface::setWnd(QLabel *wnd)
{
    m_lbWidget = wnd;
}

QList<QVideoFrame::PixelFormat> CustomLabelSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle)
    {
        return QList<QVideoFrame::PixelFormat>()
                <<  QVideoFrame::Format_ARGB32
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
    else
    {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool CustomLabelSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool CustomLabelSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
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

        QVideoFrame currentFrame = frame;
        if(!currentFrame.map(QAbstractVideoBuffer::ReadOnly))
        {
            setError(ResourceError);
            return false;
        }

        //this is a shallow operation. it just refer the frame buffer
        m_currentPixmap =  QPixmap::fromImage(QImage(
                    currentFrame.bits(),
                    currentFrame.width(),
                    currentFrame.height(),
                    currentFrame.bytesPerLine(),
                    m_imageFormat));

        //we can release the data
        currentFrame.unmap();

        m_lbWidget->setPixmap(m_currentPixmap.scaled(m_lbWidget->size(), Qt::KeepAspectRatio));
        return true;
    }
}

bool CustomLabelSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty())
    {
        m_imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);
        return true;
    }
    else
    {
        return false;
    }
}

void CustomLabelSurface::stop()
{
    m_currentPixmap = QPixmap();
    QAbstractVideoSurface::stop();
    m_lbWidget->clear();
}

void CustomLabelSurface::snapshot(QString strPath)
{
    m_currentPixmap.save(strPath);
}
