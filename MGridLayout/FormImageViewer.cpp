#include "FormImageViewer.h"
#include "ui_FormImageViewer.h"
#include "FormSingleImage.h"
#include <QButtonGroup>
#include <QFileDialog>
#include <QtMath>
#include <QResizeEvent>
#include <QDebug>
#include <QElapsedTimer>

FormImageViewer::FormImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImageViewer)
{
    ui->setupUi(this);
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btnModeOne, 1);
    btnGroup->addButton(ui->btnModeTwo, 2);
    btnGroup->addButton(ui->btnModeThree, 3);
    btnGroup->addButton(ui->btnModeFour, 4);
    btnGroup->addButton(ui->btnModeFive, 5);
    btnGroup->addButton(ui->btnModeSix, 6);
    btnGroup->addButton(ui->btnModeSeven, 7);
    connect(btnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotModeButtonClicked(int)));
    m_layoutGrid = new QGridLayout(ui->ImageShowWidget);
	m_layoutGrid->setContentsMargins(0, 0, 0, 0);
    ui->btnModeThree->click();

    m_focusWidget = nullptr;
}

FormImageViewer::~FormImageViewer()
{
    delete ui;
}

void FormImageViewer::resizeEvent(QResizeEvent *event)
{
    //QSize size = event->oldSize();
    // size = event->size();
    // qDebug() << "current size:" << size;
    // QSize size1 = ui->scrollArea->size();
    // qDebug()<<"current scrollArea size:"<<size1;
    // QSize ImageWidgetSize = ui->ImageShowWidget->size();
    // qDebug()<<"current ImageWidgetSize:"<<ImageWidgetSize;
     m_iImageWidth = (ui->scrollArea->width() - 19 - 6 * (m_iColCount - 1)) / m_iColCount;
     m_iImageHeight = (ui->scrollArea->height() - 2 - 6*(m_iRowCount - 1))/ m_iRowCount;
     ReLayoutImage();
}

void FormImageViewer::slotModeButtonClicked(int id)
{
    m_iRowCount = id;
	m_iColCount = id;
	m_iImageWidth = (ui->scrollArea->width() - 19 - 6 * (m_iColCount - 1)) / m_iColCount;
	m_iImageHeight = (ui->scrollArea->height() - 2 - 6 * (m_iRowCount - 1)) / m_iRowCount;
    ReLayoutImage();
}

void FormImageViewer::ReLayoutImage()
{
    //从布局中移出所有项
    QLayoutItem *child = nullptr;
    while ((child = m_layoutGrid->takeAt(0)))
    {
        if(child->spacerItem())
            delete child;
    }

    QWidget* widget = nullptr;
	int iImageCount = m_vctImageWidget.size();
	int iRealRowCount = (iImageCount + m_iColCount - 1) / m_iColCount;
    int nCurIdx = 0;
    for(int iRow = 0; iRow < iRealRowCount; iRow++)
    {
        for(int iCol = 0; iCol < m_iColCount; iCol++)
        {
            nCurIdx = iRow * m_iColCount + iCol;
            if (nCurIdx == iImageCount)
            {
                break;
            }
            widget = m_vctImageWidget[nCurIdx];
            widget->setFixedSize(m_iImageWidth, m_iImageHeight);
            m_layoutGrid->addWidget(widget, iRow, iCol);
        }
    }

	if (iRealRowCount < m_iRowCount)
	{
		QSpacerItem *verticalSpacer = new QSpacerItem(m_iImageWidth, m_iImageHeight, QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_layoutGrid->addItem(verticalSpacer, iRealRowCount, m_iColCount - 1);
	}
}

void FormImageViewer::on_btnInsertToReport_clicked()
{
    QStringList listFilePath = QFileDialog::getOpenFileNames(nullptr, QString::fromLocal8Bit("请选择要显示的图片"),
                                                             "D:\\Myprogram\\ambition_client.uis\\Win32\\Debug\\rptsimgs",  "Images (*.png *.xpm *.jpg)");
    if(0 == listFilePath.size() )   return;

    //从布局中移出所有项
    QLayoutItem *child = nullptr;
	QWidget *wnd = nullptr;
    while ((child = m_layoutGrid->takeAt(0)))
    {
		wnd = child->widget();

		delete child;
		if (wnd) delete wnd;	//spaceItem会自动删除，但是widget不会自动删除
    }
    m_vctImageWidget.clear();

    int num = 1;
    FormSingleImage *formImage = nullptr;
    QElapsedTimer lpsd;
    lpsd.start();
    for(QString qstrPath : listFilePath)
    {
        media_resource_struct media;
        media.image_path = qstrPath.toLocal8Bit();
        media.title = "第" + std::to_string(num++) +"张图片";
        if(0 == num % 2)    media.imageType = enum_image_type::e_report_image;

       formImage = new FormSingleImage;
       formImage->set_media_resource_info(media);
	   connect(formImage, SIGNAL(signal_focus_widget_nt(void*)), this, SLOT(slot_focus_widget_nt(void*)));
        m_vctImageWidget.append(formImage);
    }

    qDebug()<<QString::fromLocal8Bit("生成窗体耗时:")<<lpsd.elapsed();
    lpsd.restart();
    m_focusWidget = m_vctImageWidget[0];
    m_focusWidget->change_focus_status(true);
    ReLayoutImage();
    qDebug()<<QString::fromLocal8Bit("窗体布局耗时:")<<lpsd.elapsed();
}

void FormImageViewer::on_btnDelete_clicked()
{

}

void FormImageViewer::slot_focus_widget_nt(void *wnd)
{
    if(m_focusWidget != wnd)
    {
        if(m_focusWidget)   m_focusWidget->change_focus_status(false);
        m_focusWidget = static_cast<FormSingleImage*>(wnd);
    }
}

