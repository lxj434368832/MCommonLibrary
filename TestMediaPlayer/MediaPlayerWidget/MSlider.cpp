#include "MSlider.h"
#include <QStyle>
#include <QStyleOptionSlider>
#include <QMouseEvent>
#include <QToolTip>
#include<QDebug>
#include <QTime>

extern const QString  g_strTimeFormat = "hh:mm:ss";

MSlider::MSlider(QWidget *parent) :
    QSlider(parent)
{
}

bool MSlider::event(QEvent *event)
{
    if(QEvent::ToolTip == event->type())
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);

        qint64 unPointValue = maximum() * helpEvent->x() / width();
        QTime dtTime(0,0);
        dtTime =  dtTime.addMSecs(unPointValue);
        QToolTip::showText(helpEvent->globalPos() + QPoint(-20, -45),  dtTime.toString(g_strTimeFormat));

//        int iSliderx = width() * value() / maximum();   //滑块的x坐标
//        if(abs(iSliderx - helpEvent->x()) < 10)
//        {
//            QToolTip::showText(helpEvent->globalPos()/* + QPoint(-80,-60)*/, m_strCurrentTime);
//        }
//        else
//        {
//            QToolTip::hideText();
//            event->ignore();
//        }

        return true;
    }
    return QSlider::event(event);
}

void MSlider::mousePressEvent(QMouseEvent *ev)
{
    if((ev->button() & Qt::LeftButton) && this->isEnabled())
    {
        int dur = this->maximum();
        int pos = dur * ((double)ev->x() / this->width());
        if(pos != this->sliderPosition())
        {
            this->setValue(pos);
            emit sliderReleased();
        }
    }
}
