#include "dock_container.h"
#include "dock_widget.h"

dock_container::dock_container(QWidget *parent)
    : QMainWindow(parent)
{
    this->setDockNestingEnabled(true);
    this->setStatusBar(nullptr);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    //this->setLayout(layout);
    m_tab_wgt_ = new tab_widget();
    //layout->addWidget(m_tab_wgt_);

    QObject::connect((tab_bar *)m_tab_wgt_->tabBar(), &tab_bar::tab_is_dragging, this, &dock_container::on_tab_is_dragging);
    QObject::connect((tab_bar *)m_tab_wgt_->tabBar(), &tab_bar::tab_drag_stopped, this, &dock_container::on_tab_drag_stop);
}

dock_container::~dock_container()
{

}

void dock_container::add_dock_widget(dock_widget *dock_wgt)
{
    if (!dock_wgt)
        return;
    for (auto &iter : m_docks_list_)
        if (iter.second == dock_wgt)
            return;
    if (m_docks_list_.size() == 0)
    {
        this->addDockWidget(Qt::BottomDockWidgetArea, dock_wgt);
    }
    else
    {
        this->tabifyDockWidget(m_pre_dock_,dock_wgt);
    }
    m_pre_dock_ = dock_wgt;
    m_docks_list_[m_docks_list_.size()] = dock_wgt;
    return;
    int index = m_tab_wgt_->addTab(dock_wgt, dock_wgt->windowTitle());
    dock_wgt->set_tab_index(index);
}

void dock_container::set_tab_position(QTabWidget::TabPosition position)
{
    this->setTabPosition(Qt::BottomDockWidgetArea, position);

    m_tab_wgt_->setTabPosition(position);
}

void dock_container::on_tab_is_dragging(int index, QPoint pos)
{
    if (m_docks_list_.find(index) == m_docks_list_.end())
        return;
    m_docks_list_[index]->setFloating(true);
    //m_docks_list_[index]->set_title(true);
    m_docks_list_[index]->move(pos);
}

void dock_container::on_tab_drag_stop(int index, QPoint pos)
{
    if (m_docks_list_.find(index) == m_docks_list_.end())
        return;
    dock_widget *dock = m_docks_list_[index];
    dock->setFloating(true);
    //dock->set_title(true);
    m_tab_wgt_->tabBar()->removeTab(index);
}

tab_bar::tab_bar(QWidget *parent) 
    :QTabBar(parent)
{

}

void tab_bar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_start_point_ = event->globalPos();
        m_is_pressed_ = true;
        m_pressed_index_ = currentIndex();
    }
    QTabBar::mousePressEvent(event);
}

void tab_bar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_is_pressed_)
    {
        m_is_moveing_ = true;
        m_end_point_ = event->globalPos();
        this->move(this->pos() + m_end_point_ - m_start_point_);
        m_start_point_ = m_end_point_;
        //emit tab_is_dragging(m_pressed_index_, m_end_point_);
    }
    event->ignore();
}

void tab_bar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_is_moveing_)
    {
        m_is_moveing_ = false;
        m_is_pressed_ = false;
        //emit tab_drag_stopped(m_pressed_index_, m_end_point_);
        event->ignore();
    }
    else
    {
        QTabBar::mouseReleaseEvent(event);
    }
}

void tab_bar::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit tab_drag_stopped(m_pressed_index_, m_end_point_);
    event->ignore();
}

tab_widget::tab_widget(QWidget *parent)
    :QTabWidget(parent)
{
    m_tbar_ = new tab_bar();
    m_tbar_->autoHide();
    this->setTabBar(m_tbar_);
}