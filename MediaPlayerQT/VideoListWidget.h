#pragma once
#include <QMenu>
#include <QListWidget>
#include <QStandardItemModel>

class VideoListWidgetItem :public QListWidgetItem
{
public:
	VideoListWidgetItem(std::string image_path, std::string dcm_path, std::string pacs_id);
	~VideoListWidgetItem();
    VideoListWidgetItem(const VideoListWidgetItem& other);

	inline std::string get_image_path() {
		return m_image_path;
	}

	inline std::string get_dcm_path() {
		return m_dcm_path;
	}

	inline std::string get_pacs_id() {
		return m_pacs_id;
	}
private:
	std::string m_image_path;
	std::string m_dcm_path;
	std::string m_pacs_id;
};

class VideoListWidget : public QListWidget
{
	Q_OBJECT
public:
	VideoListWidget(QWidget *parent = 0);
	~VideoListWidget();
	virtual	void contextMenuEvent(QContextMenuEvent * event);
signals:
	//将图像加入报告
	void signal_join_image_into_reoprt(std::string, std::string);
	void signal_remove_image(std::string, std::string);
	void open_video(std::string, std::string);
    void signal_open_video(QList<QListWidgetItem*>, int current_row);

public slots:
	void update_list();
	void view_item();
	void delete_item();
private:
	QMenu* m_contextMenu;
	QAction* update_action;
	QAction* play_action;
	QAction* delete_action;
};