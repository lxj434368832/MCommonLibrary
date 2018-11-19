#include "MFramelessWidget.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

MFramelessWidget::MFramelessWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute( Qt::WA_Hover, true);
    setMinimumSize(m_min_width, m_min_height);
}

MFramelessWidget::~MFramelessWidget()
{
}

bool MFramelessWidget::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove)
    {
        QHoverEvent* pHoverEvent = static_cast<QHoverEvent *>(event);
        setMouseStatus(pHoverEvent->pos());
    }

    return QWidget::event(event);
}

void MFramelessWidget::setMouseStatus(QPoint mouseCursorPos)
{
    static int i = 0;
    if(m_bDrag) return;
    QRect rect = this->rect();
    QRect resizeInnerRect(m_iBorderWidth, m_iBorderWidth, rect.width() - 2*m_iBorderWidth, rect.height() - 2*m_iBorderWidth);
     QRect moveRect = resizeInnerRect;
     moveRect.setHeight(m_iTitleHeight);

    if(rect.contains(mouseCursorPos))
    {
        if(!resizeInnerRect.contains(mouseCursorPos))   //risize region
        {
            if(m_bResize) m_eResizeRegion = getResizeRegion(mouseCursorPos);
        }
        else if(moveRect.contains(mouseCursorPos))       //move region
        {
            if(m_bMove) m_eResizeRegion = Move;
        }
        else
        {
            m_eResizeRegion = Default;
        }

        setResizeCursor(m_eResizeRegion);
//        qDebug()<<i++<<"current m_eResizeRegion: "<<m_eResizeRegion;
    }
}

void MFramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
           m_bDrag = true;
           m_posDrag = event->pos();

           m_posResizeDown = event->globalPos();
           m_rectResizeDown = this->rect();
       }
}

void MFramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        QPoint pos = event->globalPos();
        switch(m_eResizeRegion)
        {
        case Move:
            move(pos - m_posDrag);
            break;
        case Default:
            break;
        default:
            handleResize(pos);
            break;
        }
    }
}

void MFramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(Move == m_eResizeRegion)
    {
        handleMove(event->globalPos()); //鼠标放开后若超出屏幕区域自动吸附于屏幕顶部/左侧/右侧
        emit signal_position_changed(x(), y());
    }
    else if(Default != m_eResizeRegion) //resized
    {
        emit signal_size_changed(width(), height());
    }

    m_bDrag = false;
    m_eResizeRegion = Default;
    setResizeCursor(m_eResizeRegion);
}

void MFramelessWidget::setResizeCursor(ResizeRegion region)
{
    switch (region)
    {
    case North:
    case South:
        setCursor(Qt::SizeVerCursor);
        break;
    case East:
    case West:
        setCursor(Qt::SizeHorCursor);
        break;
    case NorthWest:
    case SouthEast:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case NorthEast:
    case SouthWest:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

ResizeRegion MFramelessWidget::getResizeRegion(QPoint clientPos)
{
    if (clientPos.y() <= m_iBorderWidth)
    {
        if (clientPos.x() <= m_iBorderWidth)
            return NorthWest;
        else if (clientPos.x() >= this->width() - m_iBorderWidth)
            return NorthEast;
        else
            return North;
    }
    else if (clientPos.y() >= this->height() - m_iBorderWidth)
    {
        if (clientPos.x() <= m_iBorderWidth)
            return SouthWest;
        else if (clientPos.x() >= this->width() - m_iBorderWidth)
            return SouthEast;
        else
            return South;
    }
    else {
        if (clientPos.x() <= m_iBorderWidth)
            return West;
        else
            return East;
    }
}

void MFramelessWidget::handleMove(QPoint pt)
{
    QPoint currentPos = pt - m_posDrag;
    QDesktopWidget *desktop = qApp->desktop();
    if(currentPos.x() < desktop->x())
    { //吸附于屏幕左侧
        currentPos.setX(desktop->x());
    }
    else if (currentPos.x() + this->width() > desktop->width())
    { //吸附于屏幕右侧
        currentPos.setX(desktop->width() - this->width());
    }
    if(currentPos.y() < desktop->y())
    { //吸附于屏幕顶部
        currentPos.setY(desktop->y());
    }
//    else if(currentPos.y() + height() > desktop->height())
//    {
//        currentPos.setY(desktop->height() - height());
//    }

    move(currentPos);
}

void MFramelessWidget::handleResize(QPoint pt)
{
    int xdiff =pt.x() - m_posResizeDown.x();
    int ydiff = pt.y() - m_posResizeDown.y();

    switch (m_eResizeRegion)
    {
    case East:
    {
        add_diff(m_rectResizeDown.width() , xdiff, m_min_width);

        resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height());
        break;
    }
    case West:
    {
        decrease_diff(m_rectResizeDown.width() , xdiff, m_min_width);

        resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height());
        move(m_posResizeDown.x()+xdiff, this->y());
        break;
    }
    case South:
    {
        add_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(this->width(),m_rectResizeDown.height()+ydiff);
        break;
    }
    case North:
    {
        decrease_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(this->width(), m_rectResizeDown.height() - ydiff);
        move(this->x(), m_posResizeDown.y() + ydiff);
        break;
    }
    case SouthEast:
    {
        add_diff(m_rectResizeDown.width() , xdiff, m_min_width);
        add_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(m_rectResizeDown.width() + xdiff, m_rectResizeDown.height() + ydiff);
        break;
    }
    case NorthEast:
    {
        add_diff(m_rectResizeDown.width() , xdiff, m_min_width);
        decrease_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height()-ydiff);
        move(this->x(), m_posResizeDown.y()+ydiff);
        break;
    }
    case NorthWest:
    {
        decrease_diff(m_rectResizeDown.width() , xdiff, m_min_width);
        decrease_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height()-ydiff);
        move(m_posResizeDown.x()+xdiff, m_posResizeDown.y()+ydiff);
        break;
    }
    case SouthWest:
    {
        decrease_diff(m_rectResizeDown.width() , xdiff, m_min_width);
        add_diff(m_rectResizeDown.height() , ydiff, m_min_height);

        resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height()+ydiff);
        move(m_posResizeDown.x()+xdiff, this->y());
        break;
    }
    }
}

//计算添加的量
void MFramelessWidget::add_diff(int orig, int &diff, int min)
{
    if(orig + diff < min)
        diff = min - orig;
//    else if(orig + diff > max)
    //        diff = max - orig;
}

//计算减少的量
void MFramelessWidget::decrease_diff(int orig, int &diff, int min)
{
    if(orig - diff < min)
        diff = orig - min;
}
