#include "TestForm.h"
#include "ui_TestForm.h"
#include "MediaPlayerWidget/MediaPlayerWidget.h"
#include "../MediaPlayerVLC/MVLCPlayerSelfRender.h"
#include "TakeVideoCover.h"
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    m_playerWidget = new MediaPlayerWidget;
    TakeVideoCover::instance();
}

TestForm::~TestForm()
{
    delete ui;
    delete m_playerWidget;
    m_playerWidget = nullptr;
    TakeVideoCover::deleteInstance();
}

void TestForm::on_pushButton_clicked()
{
    QSize size = m_playerWidget->size();
    m_playerWidget->resize(640,320);
    m_playerWidget->show();
    m_playerWidget->resize(size);
}

void TestForm::on_btnGetVideoCover_clicked()
{
    QString qstrFilePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要生成封面的视频"));
    qstrFilePath.replace('/', '\\');
    std::string strFilePath = qstrFilePath.toLocal8Bit().data();

    std::string picPath = strFilePath.substr(0, strFilePath.find_last_of('.'));
    picPath.append(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz").toLocal8Bit().data()).
            append(".JPG");

//    std::string picPath = strFilePath.substr(0, strFilePath.find_last_of('.')).append(".PNG");


    if(TakeVideoCover::instance()->GetFileCover(strFilePath, picPath))
    {
        qDebug()<<QString::fromLocal8Bit("获取封面成功！");
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("获取封面失败！");
    }

}
