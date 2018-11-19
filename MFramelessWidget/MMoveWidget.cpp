#include "MMoveWidget.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

MMoveWidget::MMoveWidget(QWidget *parent) :
    QWidget(parent)
{
//    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    resize(640, 560);
}

MMoveWidget::~MMoveWidget()
{
}

void MMoveWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
          m_bMove = true;
          m_posPress = event->pos();
    }
}

void MMoveWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMove && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPos() - m_posPress);
    }
}

void MMoveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
        handleMove(event->globalPos()); //Êó±ê·Å¿ªºóÈô³¬³öÆÁÄ»ÇøÓò×Ô¶¯Îü¸½ÓÚÆÁÄ»¶¥²¿/×ó²à/ÓÒ²à
    }
}

void MMoveWidget::handleMove(QPoint pt)
{
    QPoint currentPos = pt - m_posPress;
    QDesktopWidget *desktop = qApp->desktop();
    if(currentPos.x()<desktop->x())
    { //Îü¸½ÓÚÆÁÄ»×ó²à
        currentPos.setX(desktop->x());
    }
    else if (currentPos.x()+this->width()>desktop->width())
    { //Îü¸½ÓÚÆÁÄ»ÓÒ²à
        currentPos.setX(desktop->width()-this->width());
    }

    if(currentPos.y()<desktop->y())
    { //Îü¸½ÓÚÆÁÄ»¶¥²¿
        currentPos.setY(desktop->y());
    }
//    else if(currentPos.y() + height() > desktop->height())
//    {
//        currentPos.setY(desktop->height() - height());
//    }
    move(currentPos);
}
