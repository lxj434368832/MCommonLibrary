#include "little_cam_wgt.h"
#include <QPixmap>
#include "cameras_widget.h"

little_cam_wgt::little_cam_wgt(QWidget *parent)
    :QWidget(parent)
{
    this->setObjectName(QStringLiteral("clw_little_camera_widget"));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(2, 0, 2, 2);
    setMinimumSize(260,180);
    
    m_cam_wnd_ = new widget(this);
    m_cam_wnd_->setObjectName("clw_camscreen_widget");
    m_wnd_ = (HWND)m_cam_wnd_->winId();
    layout->addWidget(m_cam_wnd_);
}

little_cam_wgt::~little_cam_wgt()
{
}


///////////////////////////////////////////以下是widget的实现////////////////////////////////////////////////////////////
widget::widget(QWidget *parent)
    :QWidget(parent)
{
}

widget::~widget()
{
}

void widget::mouseDoubleClickEvent(QMouseEvent *event)
{
   setUpdatesEnabled(false);
    if (isFullScreen())
    {
        setWindowFlags(Qt::SubWindow);
       showNormal();
    }
    else
    {
        setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        showFullScreen();
    }
   setUpdatesEnabled(true);
}

void widget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


