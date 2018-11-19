#include "VideoListWidget.h"


VideoListWidgetItem::VideoListWidgetItem(std::string image_path, std::string dcm_path, std::string pacs_id)
	:m_image_path(image_path)
	,m_dcm_path(dcm_path)
	,m_pacs_id(pacs_id)
{
	//为单元项设置属性
	setIcon(QIcon(m_image_path.c_str()));
	setText(m_pacs_id.c_str());
	setSizeHint(QSize(200, 160));
}

VideoListWidgetItem::~VideoListWidgetItem()
{

}

VideoListWidgetItem::VideoListWidgetItem(const VideoListWidgetItem &other):
    QListWidgetItem(other)
{
	//setSizeHint(QSize(200, 160));
    m_image_path = other.m_image_path;
    m_dcm_path = other.m_dcm_path;
	m_pacs_id = other.m_pacs_id;
	//重新设置单元项图片的宽度和高度
}

///////////////////////////////////////VideoListWidget的实现///////////////////////////////////////
VideoListWidget::VideoListWidget(QWidget *parent)
	: QListWidget(parent)
{
	m_contextMenu = new QMenu(this);
	update_action = new QAction(QString::fromLocal8Bit("刷新录像列表"), this);
	QAction *separator_action = new QAction(this);
	separator_action->setSeparator(true);
	play_action  = new QAction(QString::fromLocal8Bit("播放"), this);
	delete_action = new QAction(QString::fromLocal8Bit("删除"), this);
	//export_action = new QAction(QString::fromLocal8Bit("导出"), this);
	m_contextMenu->addAction(update_action);
	m_contextMenu->addAction(separator_action);
	m_contextMenu->addAction(play_action);
	m_contextMenu->addAction(delete_action);
	//m_contextMenu->addAction(export_action);

	connect(update_action, &QAction::triggered, this, &VideoListWidget::update_list);
	connect(play_action, &QAction::triggered, this, &VideoListWidget::view_item);
	connect(delete_action, &QAction::triggered, this, &VideoListWidget::delete_item);
}


VideoListWidget::~VideoListWidget()
{
}

void VideoListWidget::contextMenuEvent(QContextMenuEvent * event)
{
	if (this->itemAt(mapFromGlobal(QCursor::pos())) != NULL) //如果有选中item
 	{
		delete_action->setDisabled(false);
		play_action->setDisabled(false);
		delete_action->setDisabled(false);
		//export_action->setDisabled(false);
 	}
	else
	{
		delete_action->setDisabled(true);
		play_action->setDisabled(true);
		delete_action->setDisabled(true);
		//export_action->setDisabled(true);
	}

	m_contextMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}

void VideoListWidget::update_list()
{
   
}

void VideoListWidget::view_item()
{
    VideoListWidgetItem*item;
	QList<QListWidgetItem*> list;
    int icount = this->count();

    for(int i = 0; i < icount; i++)
    {
        item = dynamic_cast<VideoListWidgetItem*>(this->item(i));
        item = new VideoListWidgetItem(*item);
        list.append(item);
    }

    if (item)
	{
		//emit open_video(current_item->get_image_path(), current_item->get_pacs_id());
        emit signal_open_video(list, currentRow());
	}
}

void VideoListWidget::delete_item()
{
	VideoListWidgetItem* item = dynamic_cast<VideoListWidgetItem*>(this->currentItem());
	if (item)
	{
		emit signal_remove_image(item->get_image_path(), item->get_pacs_id());
		this->removeItemWidget(this->currentItem());
		delete this->currentItem();
	}
}