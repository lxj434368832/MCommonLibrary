#include "MFramelessWidget.h"
#include "MFramelessPrivate.h"

MFramelessWidget::MFramelessWidget(QWidget *parent) :
    QWidget(parent)
{
    d_ptr=new MFramelessPrivate(this) ;
    connect(d_ptr,SIGNAL(signal_position_changed(int,int)),this,SIGNAL(signal_position_changed(int,int)));
    connect(d_ptr,SIGNAL(signal_size_changed(int,int)), this, SIGNAL(signal_size_changed(int,int)));

}

MFramelessWidget::~MFramelessWidget()
{
    delete d_ptr;
}

void MFramelessWidget::setBorderWidth(int iBorderWidth)
{
     if(d_ptr) d_ptr->setBorderWidth(iBorderWidth);
}

void MFramelessWidget::setTitleHeight(int iTitleHeight)
{
     if(d_ptr) d_ptr->setTitleHeight(iTitleHeight);
}

void MFramelessWidget::setMinSize(int iMinWidth, int iMinHeight)
{
     if(d_ptr) d_ptr->setMinSize(iMinWidth,iMinHeight );
}

void MFramelessWidget::setMoveFlag(bool bMove)
{
     if(d_ptr) d_ptr->setMoveFlag(bMove);
}

void MFramelessWidget::setResizeFlag(bool bResize)
{
    if(d_ptr) d_ptr->setResizeFlag(bResize);
}

bool MFramelessWidget::event(QEvent *event)
{
    if(d_ptr) d_ptr->event(event);
    return QWidget::event(event);
}

void MFramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if(d_ptr) d_ptr->mousePressEvent(event);
}

void MFramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
     if(d_ptr) d_ptr->mouseMoveEvent(event);
}

void MFramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(d_ptr)  d_ptr->mouseReleaseEvent(event);
}

void MFramelessWidget::paintEvent(QPaintEvent *event)
{
    if(d_ptr) d_ptr->paintEvent(event);
}
