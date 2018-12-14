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

    void set_camera_type(camera_type ty);                         //��������ͷ����
    void set_camera_start_position(int px, int py);                      //���ñ����忪ʼ��λ��
    void set_camera_widget_size(int width, int heigh);			  //���õ����������ĳߴ�
    // ����û�
    void add_user(const QAtomicInt &index, quint64 user_id, const QString& user_name);
    // ɾ���û�
    void del_user(const QAtomicInt &index, quint64 user_id);
    // ����Indexɾ��һϵ���û�
    void clear_all_user(const QAtomicInt &index);

signals:
    //�������û�֪ͨ
    void signal_add_camera_user_notify(const QAtomicInt &index, quint64 user_id, int cam_index, WId hwnd);
    void signal_open_camera_list(bool open);                          //��ʾ����б�֪ͨ
    void signal_camera_widget_size_change(int width, int heigh);      //������ڴ�С�ı�֪ͨ
    void signal_camera_widget_position_change(int px, int py);        //�������λ�øı�֪ͨ

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
    std::map<int, dock_widget*> m_dock_list_;           // ����ͣ������
    std::map<quint64, int> m_map_userlist;              // �����û��б�userid��index��ӳ��
    std::size_t m_used_cam_count = 0;                   // ����ʹ�õ�����ͷ��

    const int m_cam_min_width = 348;			//���С������С���
    const int m_cam_min_heigh = 220;			//���С������С�߶�
    int         m_camera_widget_width = 348;	//���С���嵱ǰ���
	int         m_camera_widget_heigh = 220;	//���С���嵱ǰ�߶�

	bool m_camera_first_show = true;			//�����һ����ʾ��ʶ

	bool        m_is_moveing_ = false;    // ���λ��
    QPoint      m_start_point_;
    QPoint      m_end_point_;
};

#endif // CAMSCREEN_LIST_WIDGET_H
