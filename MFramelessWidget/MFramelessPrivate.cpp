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
    if (event->type() == QEvent::HoverMove)
    {
        QHoverEvent* pHoverEvent = static_cast<QHoverEvent *>(event);
        setMouseStatus(pHoverEvent->pos());
    }
    return true;
}

void MFramelessPrivate::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
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
        switch(m_eResizeRegion)
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
        switch (m_eResizeRegion)
        {
        case Move:
            handleMove(event->globalPos()); //���ſ�����������Ļ�����Զ���������Ļ����/���/�Ҳ�
            emit signal_position_changed(widget->x(), widget->y());
            break;

        case North:
        case West:
        case NorthWest:
        case NorthEast:
        case SouthWest:
            emit signal_size_changed(widget->width(), widget->height());
            emit signal_position_changed(widget->x(), widget->y());
            break;

        case East:
        case SouthEast:
        case South:
            emit signal_size_changed(widget->width(), widget->height());
            break;

        default:
            break;
        }
        m_bDrag = false;
    }

    m_eResizeRegion = Default;
    setResizeCursor(m_eResizeRegion);
}

void MFramelessPrivate::setMouseStatus(QPoint mouseCursorPos)
{
    static int i = 0;
    if(m_bDrag) return;
    QRect rect = widget->rect();
    QRect resizeInnerRect(m_iBorderWidth, m_iBorderWidth,
                          rect.width() - 2 * m_iBorderWidth, rect.height() - 2 * m_iBorderWidth);

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

void MFramelessPrivate::setResizeCursor(ResizeRegion region)
{
    switch (region)
    {
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

ResizeRegion MFramelessPrivate::getResizeRegion(QPoint clientPos)
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
    if(currentPos.x() < desktop->x())
    { //��������Ļ���
        currentPos.setX(desktop->x());
    }
    else if (currentPos.x() + widget->width() > desktop->width())
    { //��������Ļ�Ҳ�
        currentPos.setX(desktop->width() - widget->width());
    }
    if(currentPos.y() < desktop->y())
    { //��������Ļ����
        currentPos.setY(desktop->y());
    }
//    else if(currentPos.y() + widget->height() > desktop->height())
//    {
//        currentPos.setY(desktop->height() - widget->height());
//    }

   widget-> move(currentPos);
}

void MFramelessPrivate::handleResize(QPoint pt)
{
    int xdiff =pt.x() - m_posResizeDown.x();
    int ydiff = pt.y() - m_posResizeDown.y();

    switch (m_eResizeRegion)
    {
    case East:
        add_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);

        widget->resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height());
        break;

    case West:
        decrease_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);

       widget->resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height());
        widget->move(m_posResizeDown.x()+xdiff, widget->y());
        break;

    case South:
        add_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

        widget->resize(widget->width(),m_rectResizeDown.height()+ydiff);
        break;

    case North:
        decrease_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

        widget->resize(widget->width(), m_rectResizeDown.height() - ydiff);
        widget->move(widget->x(), m_posResizeDown.y() + ydiff);
        break;

    case SouthEast:
        add_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);
        add_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

        widget->resize(m_rectResizeDown.width() + xdiff, m_rectResizeDown.height() + ydiff);
        break;

    case NorthEast:
        add_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);
        decrease_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

        widget->resize(m_rectResizeDown.width()+xdiff, m_rectResizeDown.height()-ydiff);
        widget->move(widget->x(), m_posResizeDown.y()+ydiff);
        break;

    case NorthWest:
        decrease_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);
        decrease_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

        widget->resize(m_rectResizeDown.width()-xdiff, m_rectResizeDown.height()-ydiff);
        widget->move(m_posResizeDown.x()+xdiff, m_posResizeDown.y()+ydiff);
        break;

    case SouthWest:
        decrease_diff(m_rectResizeDown.width() , xdiff, m_iMinWidth);
        add_diff(m_rectResizeDown.height() , ydiff, m_iMinHeight);

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
