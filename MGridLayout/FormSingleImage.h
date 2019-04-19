#ifndef FORMSINGLEIMAGE_H
#define FORMSINGLEIMAGE_H

#include <QWidget>
#include <QPixmap>

//资源类型
enum report_media_resource_type
{
    pic = 0,		//图像
    mp4 = 1,		//录像
    replay_video	//回放视频
};

//图片类型
enum  enum_image_type
{
    e_history_image = 1,	//历史图片
    e_report_image = 2,		//报告显示图片
};

//截图或视频资源
struct media_resource_struct
{
    unsigned	rcd_id = 0;		//web记录id
    std::string title;       //名称
    std::string image_name;  //资源名称
    unsigned	imageType = enum_image_type::e_history_image;      //1、历史图片2、报告显示图片
    std::string	pacs_id;                            //pacs唯一ID
    unsigned	pacs_serviceId = 1;                 //pacs服务器数据库记录ID（如何获取业务暂未实现，默认为1）
    quint64    	history_media_order_number = 0;    //历史图片排序号
    unsigned    	source_type = report_media_resource_type::pic;	//资源类型
    quint64      report_image_order_number = 0;					//报告图片排序号

    std::string video_start_time;					//视频开始时间
    std::string video_end_time;						//视频结束时间
    std::string image_path;
    std::string dcm_path;
    std::string video_path;
    inline bool is_in_report()
    {
        return enum_image_type::e_report_image == imageType;
    }

    //bool operator< (const media_resource_struct& other) const
    //{
    //	return this->rcd_id < other.rcd_id;
    //}
};

namespace Ui {
class FormSingleImage;
}

class FormSingleImage : public QWidget
{
    Q_OBJECT

public:
    explicit FormSingleImage(QWidget *parent = 0);
    ~FormSingleImage();
	//设置媒体资源信息
    void set_media_resource_info(media_resource_struct media_info);
	//设置图片类型
	void set_image_type(int);
	//改变焦点状态
    void change_focus_status(bool bFocus);
	//获取焦点状态
    bool get_focus_status();

signals:
	//焦点窗体通知
    void signal_focus_widget_nt(void *);

protected:
    void paintEvent(QPaintEvent *event);        //防止设置样式表后无效
    //焦点事件
    void focusInEvent(QFocusEvent *event);
    //function:窗口大小变化
    void resizeEvent(QResizeEvent * event);

    void UpdateWidgetFocusStatus();

private:
    Ui::FormSingleImage *ui;
    QPixmap                      m_pmImage;
    bool                               m_bFocus;
    media_resource_struct m_media_info;
};

#endif // FORMSINGLEIMAGE_H
