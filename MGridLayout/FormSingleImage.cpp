#include "FormSingleImage.h"
#include "ui_FormSingleImage.h"
#include <QPainter>

FormSingleImage::FormSingleImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSingleImage)
{
	ui->setupUi(this);
    setStyleSheet("background-color: rgb(245, 245, 245)");
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    m_bFocus = false;
}

FormSingleImage::~FormSingleImage()
{
    delete ui;
}

void FormSingleImage::set_media_resource_info(media_resource_struct media_info)
{
    m_media_info = media_info;
    m_pmImage = QPixmap(QString::fromLocal8Bit(media_info.image_path.c_str()));
    ui->lbImage->setPixmap(m_pmImage.scaled(ui->lbImage->size()/*, Qt::KeepAspectRatio*/));
	set_image_type(media_info.imageType);

    ui->lbImageName->setText(QString::fromLocal8Bit(media_info.title.c_str()));
}

void FormSingleImage::set_image_type(int type)
{
	m_media_info.imageType = type;
	if (e_report_image == m_media_info.imageType)
	{
		//ui->lbImageType->setPixmap(QPixmap(QString(":/PlayIcon/Resource/Volumn.png")));
		ui->lbImageType->setPixmap(QPixmap(QString("D:\\ProgramSrc\\MCommonLibrary\\MGridLayout\\ReportImageFlag.png")));
	}
	else
		ui->lbImageType->setPixmap(QPixmap());
}

void FormSingleImage::change_focus_status(bool bFocus)
{
    if(bFocus != m_bFocus)
    {
        m_bFocus = bFocus;
        UpdateWidgetFocusStatus();
    }
}

bool FormSingleImage::get_focus_status()
{
    return m_bFocus;
}

void FormSingleImage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FormSingleImage::focusInEvent(QFocusEvent *event)
{
    event;
    m_bFocus = true;
    UpdateWidgetFocusStatus();

	emit signal_focus_widget_nt(this);
}

void FormSingleImage::resizeEvent(QResizeEvent *event)
{
    event;
    ui->lbImage->setPixmap(m_pmImage.scaled(ui->lbImage->size()/*, Qt::KeepAspectRatio*/));
}

void FormSingleImage::UpdateWidgetFocusStatus()
{
    if(m_bFocus)
    {
        setStyleSheet("QWidget{background-color: rgb(245, 245, 245); border:2px solid #0baaf6; } QLabel{ border:none; }");
    }
    else
    {
        //setStyleSheet("QWidget{ border:2px solid rgb(128,128,128); } QLabel{ border:none; }");
//        setStyleSheet("");
        setStyleSheet("background-color: rgb(245, 245, 245)");
    }
}
