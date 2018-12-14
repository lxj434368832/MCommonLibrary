#ifndef DOCK_WIDGET_H
#define DOCK_WIDGET_H

#include <QtWidgets>
#include <QDockWidget>

class dock_title_bar;
class little_cam_wgt;

class dock_widget : public QDockWidget
{
    Q_OBJECT
public:
    dock_widget(QWidget *parent = NULL );
    ~dock_widget(){};

	void set_user_info(const QAtomicInt &index, quint64 user_id, const QString title);    // 设置用户信息
    void clear_user_info(quint64 user_id);
	inline quint64 get_user_id(){ return m_user_id; }
	inline const QAtomicInt get_index(){ return m_index; }
    HWND get_hwnd();

signals :
    void is_Floating(bool is_show);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *e);

private slots:
    void on_title_bar_double_clicked();
    void on_floating_button_clicked();

private:
    dock_title_bar *m_title_bar_ = nullptr;
    little_cam_wgt *m_widget;

    quint64 m_user_id = 0;
	QAtomicInt m_index;
    // 鼠标位置
    bool        m_is_moveing_ = false;
    QPoint      m_start_point_;             // 移动前的位置
    QPoint      m_moved_pos_;				// 移动后的位置
};

class dock_title_bar : public QWidget
{
    Q_OBJECT
public:
    explicit dock_title_bar(QWidget *parent = Q_NULLPTR);
    void set_title(const QString &title);
    inline QPushButton * get_button(){return m_btn_floating_; }

signals:
    void signal_floating_button_clicked();
    void signal_title_bar_double_clicked();
public:

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    //    inline QLabel * get_label(){ return m_lb_show_name_;}

private:
    QLabel *m_lb_show_ioc_ = nullptr;
    QLabel *m_lb_show_name_ = nullptr;
    QPushButton *m_btn_floating_ = nullptr;
};
#endif // DOCK_WIDGET_H
