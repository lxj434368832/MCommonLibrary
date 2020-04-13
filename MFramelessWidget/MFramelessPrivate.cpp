#include "MFramelessPrivate.h"
#include <QStyleOption>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QPainter>

MFramelessPrivate::MFramelessPrivate(QWidget *widget)
{
    this->widget = widget;
    widget->setWindowFlags(Qt::FramelessWindowHint);
    widget->setAttribute( Qt::WA_Hover, true);
}

void MFramelessPrivate::setBorderWidth(int iBorderWidth)
{
    m_iBorderWidth = iBorderWidth;
}

void MFramelessPrivate::setTitleHeight(int iTitleHeight)
{
    m_iTitleHeight = iTitleHeight;
}

void MFramelessPrivate::setMinSize(int iMinWidth, int iMinHeight)
{
    m_iMinWidth = iMinWidth;
    m_iMinHeight = iMinHeight;
}

void MFramelessPrivate::setMoveFlag(bool bMove)
{
    m_bMove = bMove;
}

void MFramelessPrivate::setResizeFlag(bool bResize)
{
    m_bResize = bResize;
}

void MFramelessPrivate::setMaxShowFlag(bool bMaxShow)
{
    m_bMaxShow = bMaxShow;
}

void MFramelessPrivate::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

        QStyleOption opt;
        opt.init(widget);
        QPainter p(widget);
        widget->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, widget);
}

bool MFramelessPrivate::event(QEvent *event)
{
    if (QEvent::HoverMove == event->type() )
    {
        QHoverEvent* pHoverEvent = static_cast<QHoverEvent *>(event);
        setMouseStatus(pHoverEvent->pos());
    }
    return true;
}

void MFramelessPrivate::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && Default != m_eMouseStatus)
    {
        m_bDrag = true;
        m_posDrag = event->pos();

        m_posResizeDown = event->globalPos();
        m_rectResizeDown = widget->rect();
    }
}

void MFramelessPrivate::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        QPoint pos = event->globalPos();
        switch(m_eMouseStatus)
        {
        case Move:
            widget->move(pos - m_posDrag);
            break;
        case Default:
            break;
        default:
            handleResize(pos);
            break;
        }
    }
}

void MFramelessPrivate::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        switch (m_eMouseStatus)
        {
        case Move:
            handleMove(event->globalPos()); //鼠标放开后若超出屏幕区域自动吸附于屏幕顶部/左侧/右侧
            emit signalPositionChanged(widget->x(), widget->y());
            break;

        case North:
        case West:
        case NorthWest:
        case NorthEast:
        case SouthWest:
            emit signalSizeChanged(widget->width(), widget->height());
            emit signalPositionChanged(widget->x(), widget->y());
            break;

        case East:
        case SouthEast:
        case South:
            emit signalSizeChanged(widget->width(), widget->height());
            break;

        default:
            break;
        }
        m_bDrag = false;
    }

    m_eMouseStatus = Default;
    setMouseCursor(m_eMouseStatus);
}

void MFramelessPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
    event;
    if(false == m_bMaxShow) return;

    QRect moveRect = widget->rect();
     moveRect.setHeight(m_iTitleHeight);
     if(false == moveRect.contains(event->pos())) return;

     //只有双击的是标题栏才进行最大化的操作
    if (widget->isMaximized())
     {
         widget->showNormal();
     }
     else
     {
         widget->showMaximized();
     }
}

void MFramelessPrivate::setMouseStatus(QPoint mouseCursorPos)
{
    if(m_bDrag) return;
    if(widget->isMaximized())
    {
        if(Default != m_eMouseStatus)
        {
            m_eMouseStatus= Default;
            setMouseCursor(m_eMouseStatus);
        }
        return;   //最大化时不进行缩放操作
    }

    QRect rect = widget->rect();
    QRect resizeInnerRect(m_iBorderWidth, m_iBorderWidth,
                          rect.width() - 2 * m_iBorderWidth, rect.height() - 2 * m_iBorderWidth);

     QRect moveRect = resizeInnerRect;
     moveRect.setHeight(m_iTitleHeight);

    if(rect.contains(mouseCursorPos))
    {
        if(!resizeInnerRect.contains(mouseCursorPos))   //risize region
        {
            if(m_bResize) m_eMouseStatus = getResizeRegion(mouseCursorPos);
        }
        else if(moveRect.contains(mouseCursorPos))       //move region
        {
            if(m_bMove) m_eMouseStatus = Move;
        }
        else
        {
            m_eMouseStatus = Default;
        }

        setMouseCursor(m_eMouseStatus);
//        qDebug()<<i++<<"current m_eResizeRegion: "<<m_eResizeRegion;
    }

}

void MFramelessPrivate::setMouseCursor(MouseStatus region)
{
    switch (region)
    {
//    case Move:
//        widget->setCursor(Qt::PointingHandCursor);
//        break;
    case North:
    case South:
        widget->setCursor(Qt::SizeVerCursor);
        break;
    case East:
    case West:
        widget->setCursor(Qt::SizeHorCursor);
        break;
    case NorthWest:
    case SouthEast:
        widget->setCursor(Qt::SizeFDiagCursor);
        break;
    case NorthEast:
    case SouthWest:
        widget->setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        widget->setCursor(Qt::ArrowCursor);
        break;
    }
}

MouseStatus MFramelessPrivate::getResizeRegion(QPoint clientPos)
{
    if (clientPos.y() <= m_iBorderWidth)
    {
        if (clientPos.x() <= m_iBorderWidth)
            return NorthWest;
        else if (clientPos.x() >= widget->width() - m_iBorderWidth)
            return NorthEast;
        else
            return North;
    }
    else if (clientPos.y() >= widget->height() - m_iBorderWidth)
    {
        if (clientPos.x() <= m_iBorderWidth)
            return SouthWest;
        else if (clientPos.x() >= widget->width() - m_iBorderWidth)
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

void MFramelessPrivate::handleMove(QPoint pt)
{
    QPoint currentPos = pt - m_posDrag;
    QDesktopWidget *desktop = qApp->desktop();
//    if(currentPos.x() < desktop->x())
//    { //吸附于屏幕左侧
//        currentPos.setX(desktop->x());
//    }
//    else if (currentPos.x() + widget->width() > desktop->width())
//    { //吸附于屏幕右侧
//        currentPos.setX(desktop->width() - widget->width());
//    }
    if(currentPos.y() < desktop->y())
    { //吸附于屏幕顶部
        currentPos.setY(desktop->y());
    }
//    else if(currentPos.y() + widget->height() > desktop->height())
//    { //吸附于屏幕底部
//        currentPos.setY(desktop->height() - widget->height());
//    }

   widget-> move(currentPos);
}

void MFramelessPrivate::handleResize(QPoint pt)
{
    int xdiff =pt.x() - m_posResizeDown.x();
    int ydiff = pt.y() - m_posResizeDown.y();

    int iMinWidth = widget->minimumWidth();
    int iMinHeight = widget->minimumHeight();
    iMinWidth =qMax(iMinWidth, m_iMinWidth);
    iMinHeight = qMax(iMinHeight, m_iMinHeight);

    switch (m_eMouseStatus)
    {
    case East:
        add_diff(m_rectResizeDown.width() , xdiff, iMinWidth);

        widget->resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height());
        break;

    case West:
        decrease_diff(m_rectResizeDown.width() , xdiff, iMinWidth);

       widget->resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height());
        widget->move(m_posResizeDown.x()+xdiff, widget->y());
        break;

    case South:
        add_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

        widget->resize(widget->width(),m_rectResizeDown.height()+ydiff);
        break;

    case North:
        decrease_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

        widget->resize(widget->width(), m_rectResizeDown.height() - ydiff);
        widget->move(widget->x(), m_posResizeDown.y() + ydiff);
        break;

    case SouthEast:
        add_diff(m_rectResizeDown.width() , xdiff, iMinWidth);
        add_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

        widget->resize(m_rectResizeDown.width() + xdiff, m_rectResizeDown.height() + ydiff);
        break;

    case NorthEast:
        add_diff(m_rectResizeDown.width() , xdiff, iMinWidth);
        decrease_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

        widget->resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height()-ydiff);
        widget->move(widget->x(), m_posResizeDown.y()+ydiff);
        break;

    case NorthWest:
        decrease_diff(m_rectResizeDown.width() , xdiff, iMinWidth);
        decrease_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

        widget->resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height()-ydiff);
        widget->move(m_posResizeDown.x()+xdiff, m_posResizeDown.y()+ydiff);
        break;

    case SouthWest:
        decrease_diff(m_rectResizeDown.width() , xdiff, iMinWidth);
        add_diff(m_rectResizeDown.height() , ydiff, iMinHeight);

       widget->resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height()+ydiff);
        widget->move(m_posResizeDown.x()+xdiff, widget->y());
        break;
    }
}

void MFramelessPrivate::add_diff(int orig, int &diff, int min)
{
    if(orig + diff < min)
        diff = min - orig;
//    else if(orig + diff > max)
    //        diff = max - orig;
}

void MFramelessPrivate::decrease_diff(int orig, int &diff, int min)
{
    if(orig - diff < min)
        diff = orig - min;
}
