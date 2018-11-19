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
        handleMove(event->globalPos()); //���ſ�����������Ļ�����Զ���������Ļ����/���/�Ҳ�
    }
}

void MMoveWidget::handleMove(QPoint pt)
{
    QPoint currentPos = pt - m_posPress;
    QDesktopWidget *desktop = qApp->desktop();
    if(currentPos.x()<desktop->x())
    { //��������Ļ���
        currentPos.setX(desktop->x());
    }
    else if (currentPos.x()+this->width()>desktop->width())
    { //��������Ļ�Ҳ�
        currentPos.setX(desktop->width()-this->width());
    }

    if(currentPos.y()<desktop->y())
    { //��������Ļ����
        currentPos.setY(desktop->y());
    }
//    else if(currentPos.y() + height() > desktop->height())
//    {
//        currentPos.setY(desktop->height() - height());
//    }
    move(currentPos);
}
