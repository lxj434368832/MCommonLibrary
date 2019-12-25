#include "MFramelessDialog.h"
#include "MFramelessPrivate.h"

MFramelessDialog::MFramelessDialog(QWidget *parent) :
    QDialog(parent)
{
    d_ptr=new MFramelessPrivate(this) ;
   setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    connect(d_ptr,SIGNAL(signal_position_changed(int,int)),this,SIGNAL(signal_position_changed(int,int)));
    connect(d_ptr,SIGNAL(signal_size_changed(int,int)), this, SIGNAL(signal_size_changed(int,int)));
}

MFramelessDialog::~MFramelessDialog()
{
    delete d_ptr;
}

void MFramelessDialog::setBorderWidth(int iBorderWidth)
{
         if(d_ptr) d_ptr->setBorderWidth(iBorderWidth);
}

void MFramelessDialog::setTitleHeight(int iTitleHeight)
{
     if(d_ptr) d_ptr->setTitleHeight(iTitleHeight);
}

void MFramelessDialog::setMinSize(int iMinWidth, int iMinHeight)
{
     if(d_ptr) d_ptr->setMinSize(iMinWidth,iMinHeight );
}

void MFramelessDialog::setMoveFlag(bool bMove)
{
     if(d_ptr) d_ptr->setMoveFlag(bMove);
}

void MFramelessDialog::setResizeFlag(bool bResize)
{
    if(d_ptr) d_ptr->setResizeFlag(bResize);
}

void MFramelessDialog::setMaxShowFlag(bool bMaxShow)
{
    if(d_ptr) d_ptr->setMaxShowFlag(bMaxShow);
}

bool MFramelessDialog::event(QEvent *event)
{
    if(d_ptr) d_ptr->event(event);
    return QWidget::event(event);
}

void MFramelessDialog::mousePressEvent(QMouseEvent *event)
{
    if(d_ptr) d_ptr->mousePressEvent(event);
}

void MFramelessDialog::mouseMoveEvent(QMouseEvent *event)
{
     if(d_ptr) d_ptr->mouseMoveEvent(event);
}

void MFramelessDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(d_ptr)  d_ptr->mouseReleaseEvent(event);
}

void MFramelessDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
        if(d_ptr)  d_ptr->mouseDoubleClickEvent(event);
}
