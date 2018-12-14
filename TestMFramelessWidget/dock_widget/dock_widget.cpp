#include "dock_widget.h"
#include "../cameras_widget/little_cam_wgt.h"

dock_widget::dock_widget(QWidget *parent)
    : QDockWidget(parent)
{
    setMinimumSize(348,220);

    m_title_bar_ = new dock_title_bar(this);                    //标题栏
    m_title_bar_->setObjectName("dock_title_widget");
    setTitleBarWidget(m_title_bar_);

    m_widget = new little_cam_wgt(this);                        //主窗体部分
    setWidget(m_widget);

    connect(m_title_bar_, &dock_title_bar::signal_title_bar_double_clicked, this, &dock_widget::on_title_bar_double_clicked);
    connect(m_title_bar_, &dock_title_bar::signal_floating_button_clicked, this, &dock_widget::on_floating_button_clicked);
}

void dock_widget::set_user_info(const QAtomicInt &index, quint64 user_id, const QString title)
{
	m_index = 0;
    m_user_id =user_id;
    m_title_bar_->set_title(title);
}

void dock_widget::clear_user_info(quint64 user_id)
{
    if(user_id == m_user_id )
    {
        m_user_id = 0;
        m_title_bar_->set_title("");
    }
}

HWND dock_widget::get_hwnd()
{
    return m_widget->hwnd();
}

void dock_widget::on_title_bar_double_clicked()
{
    if (false == isFloating())
    {
        setFloating(true);
        move(pos().x() +  width(), pos().y() + 40);
        emit is_Floating(true);
        raise();
    }
    /*else
    {
        if (isMaximized())
        {
            showNormal();
        }
        else
        {
            showMaximized();
        }
        emit is_Floating(false);
    }*/
}

void dock_widget::on_floating_button_clicked()
{
    if (isFloating())
    {
        setFloating(false);
        emit is_Floating(false);
    }
    else
    {
        setFloating(true);
        move(pos().x() + width(), pos().y() + 40);
        emit is_Floating(true);
        raise();
    }
}


//重写鼠标按下事件  
void dock_widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        this->isFloating())
    {
        m_start_point_ = event->globalPos();
        m_is_moveing_ = true;
        qDebug()<<"mousePressEvent isFloating"<< this->isFloating();
    }
}

//重写鼠标移动事件  
void dock_widget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_is_moveing_)
    {
        m_moved_pos_ = event->globalPos();
        this->move(this->pos() + m_moved_pos_ - m_start_point_);
        m_start_point_ = m_moved_pos_;
    }
}

void dock_widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_is_moveing_ = false;
    }
}

void dock_widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(55, 55, 55, 255));  //QColor最后一个参数80代表背景的透明度

    if (!this->isFloating())
    {
        m_title_bar_->get_button()->setStyleSheet("border-image:url(:/res/Resources/main/max.png);");
    }
    else
    {
        m_title_bar_->get_button()->setStyleSheet("border-image:url(:/res/Resources/main/close.png);");
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
dock_title_bar::dock_title_bar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(40);
    QHBoxLayout *pHLayout = new QHBoxLayout(this);
    pHLayout->setContentsMargins(5, 0, 3, 0);
    m_lb_show_ioc_ = new QLabel(this);
    m_lb_show_ioc_->setObjectName("dock_title_bar_show_ioc");
    m_lb_show_ioc_->setFixedHeight(15);
    m_lb_show_ioc_->setFixedWidth(15);

    m_lb_show_name_ = new QLabel(this);
    m_lb_show_name_->setObjectName("dock_title_bar_show_name");
    m_lb_show_name_->setFixedHeight(36);

    m_btn_floating_ = new QPushButton(this);
    m_btn_floating_->setCursor(Qt::PointingHandCursor);
    m_btn_floating_->setObjectName("dock_title_bar_floating");
    m_btn_floating_->setFixedHeight(25);
    m_btn_floating_->setFixedWidth(25);

    pHLayout->addWidget(m_lb_show_ioc_);
    pHLayout->addWidget(m_lb_show_name_);
    pHLayout->addStretch();
    pHLayout->addWidget(m_btn_floating_);

    connect(m_btn_floating_, SIGNAL(clicked()), this, SIGNAL(signal_floating_button_clicked()));
}

void dock_title_bar::set_title(const QString &title)
{
        m_lb_show_name_->setText(title);
}

void dock_title_bar::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signal_title_bar_double_clicked();
}

