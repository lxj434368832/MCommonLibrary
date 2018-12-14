#ifndef CAMSCREEN_LIST_WIDGET_H
#define CAMSCREEN_LIST_WIDGET_H
#include <QtWidgets>
#include "../MFramelessWidget/MFramelessWidget.h"
#include "cameras_widget_global.h"
#include <map>
#include <string>
#include <mutex>

class little_cam_wgt;
class dock_widget;

enum class camera_type
{
    NORMAL,
    KRL,
};


class  cameras_widget : public MFramelessWidget
{
    Q_OBJECT
public:
    cameras_widget(QWidget *parent = nullptr);
    ~cameras_widget();

public:
	HWND my_hwnd(int index);

    void set_camera_type(camera_type ty);                         //设置摄像头类型
    void set_camera_start_position(int px, int py);                      //设置本窗体开始的位置
    void set_camera_widget_size(int width, int heigh);			  //设置单个相机窗体的尺寸
    // 添加用户
    void add_user(const QAtomicInt &index, quint64 user_id, const QString& user_name);
    // 删除用户
    void del_user(const QAtomicInt &index, quint64 user_id);
    // 根据Index删除一系列用户
    void clear_all_user(const QAtomicInt &index);

signals:
    //添加相机用户通知
    void signal_add_camera_user_notify(const QAtomicInt &index, quint64 user_id, int cam_index, WId hwnd);
    void signal_open_camera_list(bool open);                          //显示相机列表通知
    void signal_camera_widget_size_change(int width, int heigh);      //相机窗口大小改变通知
    void signal_camera_widget_position_change(int px, int py);        //相机窗口位置改变通知

public slots:
    void on_camera_list_switch(bool is_show);

protected:
    void resizeEvent(QResizeEvent *e) override;

private slots:
	void on_btn_close_clicked();
	void slot_adjust_widget_size();

private:
    void add_cameras(int count);

private:
    camera_type m_cam_type = camera_type::NORMAL;
    QVBoxLayout *m_cams_layout = nullptr;
    int         m_layout_cam_widget_count = 0;

	std::recursive_mutex m_cam_list_mutex;
    std::map<int, dock_widget*> m_dock_list_;           // 保存停靠窗口
    std::map<quint64, int> m_map_userlist;              // 保存用户列表，userid和index的映射
    std::size_t m_used_cam_count = 0;                   // 计数使用的摄像头数

    const int m_cam_min_width = 348;			//相机小窗体最小宽度
    const int m_cam_min_heigh = 220;			//相机小窗体最小高度
    int         m_camera_widget_width = 348;	//相机小窗体当前宽度
	int         m_camera_widget_heigh = 220;	//相机小窗体当前高度

	bool m_camera_first_show = true;			//相机第一次显示标识

	bool        m_is_moveing_ = false;    // 鼠标位置
    QPoint      m_start_point_;
    QPoint      m_end_point_;
};

#endif // CAMSCREEN_LIST_WIDGET_H
