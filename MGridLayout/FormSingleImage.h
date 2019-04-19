#ifndef FORMSINGLEIMAGE_H
#define FORMSINGLEIMAGE_H

#include <QWidget>
#include <QPixmap>

//��Դ����
enum report_media_resource_type
{
    pic = 0,		//ͼ��
    mp4 = 1,		//¼��
    replay_video	//�ط���Ƶ
};

//ͼƬ����
enum  enum_image_type
{
    e_history_image = 1,	//��ʷͼƬ
    e_report_image = 2,		//������ʾͼƬ
};

//��ͼ����Ƶ��Դ
struct media_resource_struct
{
    unsigned	rcd_id = 0;		//web��¼id
    std::string title;       //����
    std::string image_name;  //��Դ����
    unsigned	imageType = enum_image_type::e_history_image;      //1����ʷͼƬ2��������ʾͼƬ
    std::string	pacs_id;                            //pacsΨһID
    unsigned	pacs_serviceId = 1;                 //pacs���������ݿ��¼ID����λ�ȡҵ����δʵ�֣�Ĭ��Ϊ1��
    quint64    	history_media_order_number = 0;    //��ʷͼƬ�����
    unsigned    	source_type = report_media_resource_type::pic;	//��Դ����
    quint64      report_image_order_number = 0;					//����ͼƬ�����

    std::string video_start_time;					//��Ƶ��ʼʱ��
    std::string video_end_time;						//��Ƶ����ʱ��
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
	//����ý����Դ��Ϣ
    void set_media_resource_info(media_resource_struct media_info);
	//����ͼƬ����
	void set_image_type(int);
	//�ı佹��״̬
    void change_focus_status(bool bFocus);
	//��ȡ����״̬
    bool get_focus_status();

signals:
	//���㴰��֪ͨ
    void signal_focus_widget_nt(void *);

protected:
    void paintEvent(QPaintEvent *event);        //��ֹ������ʽ�����Ч
    //�����¼�
    void focusInEvent(QFocusEvent *event);
    //function:���ڴ�С�仯
    void resizeEvent(QResizeEvent * event);

    void UpdateWidgetFocusStatus();

private:
    Ui::FormSingleImage *ui;
    QPixmap                      m_pmImage;
    bool                               m_bFocus;
    media_resource_struct m_media_info;
};

#endif // FORMSINGLEIMAGE_H
