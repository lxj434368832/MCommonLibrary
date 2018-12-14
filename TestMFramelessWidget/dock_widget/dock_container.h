#ifndef DOCK_CONTAINER_H
#define DOCK_CONTAINER_H

#include <QtWidgets>

class dock_widget;
class tab_widget;
class dock_container : public QMainWindow
{
    Q_OBJECT

public:
    dock_container(QWidget *parent = nullptr);
    ~dock_container();

public:
    void add_dock_widget(dock_widget *dock_wgt);
    void set_tab_position(QTabWidget::TabPosition position);

    public slots:
    void on_tab_is_dragging(int index, QPoint pos);
    void on_tab_drag_stop(int index, QPoint pos);

private:
    std::map<int, dock_widget *> m_docks_list_;
    tab_widget *m_tab_wgt_ = nullptr;
    QDockWidget *m_pre_dock_ = nullptr;
};

class tab_bar : public QTabBar
{
    Q_OBJECT
public:
    tab_bar(QWidget *parent = nullptr);
    ~tab_bar() = default;

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;

private:
    bool m_is_pressed_ = false;
    bool m_is_moveing_ = false;
    QPoint m_start_point_;
    QPoint m_end_point_;
    int m_pressed_index_;

signals:
    void tab_is_dragging(int index, QPoint pos);
    void tab_drag_stopped(int index, QPoint pos);
};

class tab_widget :public QTabWidget
{
    Q_OBJECT
public:
    tab_widget(QWidget *parent = nullptr);
    ~tab_widget() = default;

public:
    tab_bar *m_tbar_;
};

#endif // DOCK_CONTAINER_H
