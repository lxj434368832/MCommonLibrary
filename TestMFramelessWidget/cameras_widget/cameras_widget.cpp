#include "cameras_widget.h"
#include "little_cam_wgt.h"
//#include "../public/type_define.h"
//#include "../left_window/dock_widget/dock_widget.h"
#include "../dock_widget/dock_widget.h"
#include <set>
#include <iostream>

#define  logm() std::cout

const std::size_t s_max_cams_count = 3;        //摄像头最多三个，多了也没用，没有进行渲染，
const std::size_t s_init_cams_count = 2;	   //开始必须是两个，康如来拿走两个句柄，不得不这么做

cameras_widget::cameras_widget(QWidget *parent)
    :MFramelessWidget(parent)
{
	setObjectName(QStringLiteral("camera_list_widget"));
	setStyleSheet("QWidget#camera_list_widget{background-color: rgb(40,40,40);}");
    setMinimumSize(360, 200);
//    setResizeFlag(false);

	QVBoxLayout *this_layout = new QVBoxLayout(this);		//设置布局
    this_layout->setContentsMargins(6, 0, 6, 6);
	this_layout->setSpacing(0);

	QFrame *title_bar_ = new QFrame(this);
	title_bar_->setObjectName(QStringLiteral("clw_title_bar_wgt"));
	title_bar_->setFixedHeight(50);
	QHBoxLayout *tbar_layout = new QHBoxLayout(title_bar_);
	tbar_layout->setContentsMargins(0, 0, 0, 0);
	tbar_layout->setSpacing(0);

	QLabel *lb_title_ = new QLabel(QStringLiteral("摄像头"));
	lb_title_->setObjectName(QStringLiteral("clw_camera_list_title"));
	QPushButton *btn_close_ = new QPushButton();
	btn_close_->setCursor(Qt::PointingHandCursor);
	btn_close_->setFixedSize(50, 50);
	btn_close_->setObjectName(QStringLiteral("clw_camera_list_close"));
	tbar_layout->addWidget(lb_title_);
	tbar_layout->addWidget(btn_close_);

	this_layout->addWidget(title_bar_);

	QWidget *cam_list_widget = new QWidget(this);
	cam_list_widget->setObjectName(QStringLiteral("clw_cams_list_wgt"));
	m_cams_layout = new QVBoxLayout(cam_list_widget);
	m_cams_layout->setContentsMargins(0, 0, 0, 0);
	m_cams_layout->setSpacing(6);
	this_layout->addWidget(cam_list_widget);

	add_cameras(s_init_cams_count);
	m_dock_list_[0]->set_user_info(0, -1, QStringLiteral("本地摄像头"));
	m_dock_list_[0]->show();
	m_cams_layout->addWidget(m_dock_list_[0]);
	m_layout_cam_widget_count++;

    QObject::connect(btn_close_, SIGNAL(clicked()), this, SLOT(on_btn_close_clicked()));
}

cameras_widget::~cameras_widget()
{
}

void cameras_widget::on_camera_list_switch(bool is_show)
{
    QPoint pos;
	if (is_show)
	{
		show();
		if (m_camera_first_show)
		{
            m_camera_first_show = false;

             QRect desk_rect = QApplication::primaryScreen()->availableVirtualGeometry();
             QRect cam_rect = QRect(m_start_point_.x(), m_start_point_.y(), width(), height());
            //判断水平方向是否超范围
             if(cam_rect.x() < desk_rect.x())
                 cam_rect.moveLeft(desk_rect.x());
             else if(cam_rect.x() + cam_rect.width() > desk_rect.width())
                 cam_rect.moveLeft(desk_rect.width() - cam_rect.width());

             //判断垂直方向是否超范围
            if(cam_rect.y() < desk_rect.y())
                cam_rect.moveTop(desk_rect.y());
            else if(cam_rect.y() + cam_rect.height() > desk_rect.y() + desk_rect.height())
                cam_rect.moveBottom(desk_rect.y() + desk_rect.height());

             pos = QPoint(cam_rect.x(), cam_rect.y());
            move(pos);
		}//  end if (m_camera_first_show)
	}
	else
		hide();

}

void cameras_widget::add_cameras(int count)
{
	for (int i = 0; i < count; i++)
	{
        dock_widget *dock = new dock_widget();
		dock->hide();

		m_dock_list_[m_used_cam_count] = dock;
		m_used_cam_count++;

		connect(dock, &dock_widget::is_Floating,
			[=](bool is_float)
		{
			if (is_float)
			{
				//处理窗体大小
				m_layout_cam_widget_count--;
			}
			else
			{
				m_layout_cam_widget_count++;
				emit signal_open_camera_list(true);
			}

			slot_adjust_widget_size();
		});

	} //end for
}

void cameras_widget::on_btn_close_clicked()
{
    emit signal_open_camera_list(false);
    close();
}

void cameras_widget::slot_adjust_widget_size()
{
    int iMinWidth = 12 + m_cam_min_width;
    int iMinHeight = 50 + (m_cam_min_heigh + 6) * m_layout_cam_widget_count;
    setMinSize(iMinWidth, iMinHeight);
//    setMinimumSize(iMinWidth, iMinHeight);    //如果设置了窗体的min size 则resize无效。
    resize(12 + m_camera_widget_width, 50 + (m_camera_widget_heigh + 6) * m_layout_cam_widget_count);
}

void cameras_widget::set_camera_type(camera_type ty)
{
    m_cam_type = ty;
}

void cameras_widget::set_camera_start_position(int px, int py)
{
    m_start_point_ = QPoint(px, py);
}

HWND cameras_widget::my_hwnd(int index)
{
	return m_dock_list_[index]->get_hwnd();
}

void cameras_widget::set_camera_widget_size(int width, int heigh)
{
	if (width < m_cam_min_width || heigh < m_cam_min_heigh)
	{
		logm() << "摄像头小窗体的宽或高尺寸小于最小值，width:" << width << " heigh:" << heigh;
		return;
	}

	m_camera_widget_width = width;
	m_camera_widget_heigh = heigh;
	slot_adjust_widget_size();
}

void cameras_widget::add_user(const QAtomicInt &index, quint64 user_id, const QString& user_name)
{
	std::lock_guard<std::recursive_mutex> lock(m_cam_list_mutex);
    logm() << "摄像头添加了:" << user_name.toLocal8Bit().data() << " 用户的user_id：" << user_id << std::endl;
	if (m_map_userlist.end() != m_map_userlist.find(user_id))
	{
		return;
	}

    if (m_map_userlist.size() + 1 >= m_used_cam_count && m_cam_type == camera_type::NORMAL)
	{
		add_cameras(1);
		logm() << "增加了一个摄像头窗口，总用户数:" << m_map_userlist.size() << std::endl;
	}

	for (auto iter : m_dock_list_)
	{
		int idx = iter.first;
		dock_widget *dock = iter.second;
        if (dock&& !dock->get_user_id() && m_cam_type == camera_type::NORMAL)
		{
			logm() << "*****************普通模式*小摄像头:" << user_name.toLocal8Bit().data() << std::endl;
			m_map_userlist[user_id] = idx;
			dock->set_user_info(index, user_id, user_name);
			if (idx < s_max_cams_count)
			{
				dock->show();
				m_cams_layout->addWidget(dock);
				m_layout_cam_widget_count++;
				slot_adjust_widget_size();
				emit signal_add_camera_user_notify(index, user_id, iter.first, (WId)dock->get_hwnd());
			}
			break;
		}
        else if (dock && !dock->get_user_id() && m_cam_type == camera_type::KRL)
		{
			logm() << "*****************康如来模式*****************" << std::endl;
			if (m_map_userlist.size() == 2)
			{
				m_map_userlist[user_id] = iter.first;
				dock->set_user_info(index, user_id, user_name);
				dock->show();
				m_cams_layout->addWidget(dock);
				m_layout_cam_widget_count++;
				slot_adjust_widget_size();
				emit signal_add_camera_user_notify(index, user_id, m_map_userlist.size(), NULL);
			}
		}
	}//end for
}

void cameras_widget::del_user(const QAtomicInt &index, quint64 user_id)
{
	logm() << "摄像头删除了；" << "用户的user_id：" << user_id << std::endl;
	//---------原来的是摄像头用户列表和窗口一一对应，现在摄像头列表固定，用户动态匹配摄像头窗口----------

	if (m_map_userlist.end() == m_map_userlist.find(user_id))
	{
		logm() << "摄像头未找到；" << "用户的user_id：" << user_id << std::endl;
		return;
	}

	std::lock_guard<std::recursive_mutex> lock(m_cam_list_mutex);
	int idx_del = m_map_userlist.find(user_id)->second;
	dock_widget *dock_del = m_dock_list_[idx_del];
	dock_del->set_user_info(0, 0, "");
	dock_del->hide();
	m_cams_layout->removeWidget(dock_del);
	m_layout_cam_widget_count--;
	m_map_userlist.erase(user_id);

	if (idx_del < s_max_cams_count)
	{
		//查找超出范围没有显示的用户
		int i, icount = m_dock_list_.size();
		for (i = s_max_cams_count; i < icount; i++)
		{
			dock_widget *dock = m_dock_list_[i];
			if (dock && dock->get_user_id())		//由于外面只认cam_idx为1或2,所以找到隐藏的dock则交换
			{
				m_dock_list_[i] = dock_del;
				m_dock_list_[idx_del] = dock;
				m_map_userlist[dock->get_user_id()] = idx_del;
				dock->show();
				m_cams_layout->addWidget(dock);
				m_layout_cam_widget_count++;
				emit signal_add_camera_user_notify(dock->get_index(), dock->get_user_id(), idx_del, (WId)dock->get_hwnd());
			}
		}
	}
	
	slot_adjust_widget_size();
	logm() << "清除用户" << user_id << "小摄像头" << std::endl;
}

void cameras_widget::clear_all_user(const QAtomicInt &index)
{
	index;
	std::lock_guard<std::recursive_mutex> lock(m_cam_list_mutex);
	for (auto iter : m_dock_list_)
	{
		dock_widget *dock = iter.second;
		if (nullptr == dock)
			continue;

		if (0 != iter.first)
		{
			dock->set_user_info(0, 0, "");
			dock->hide();
			m_cams_layout->removeWidget(dock);
		}
	}

	m_layout_cam_widget_count = 1;
	slot_adjust_widget_size();
    m_map_userlist.clear();
}

void cameras_widget::resizeEvent(QResizeEvent *e)
{
	if (m_layout_cam_widget_count)
	{
		m_camera_widget_width = e->size().width() - 12;
		m_camera_widget_heigh = (e->size().height() - 50) / m_layout_cam_widget_count - 6;

        emit signal_camera_widget_size_change(m_camera_widget_width,
                                              m_camera_widget_heigh);
	}
}
