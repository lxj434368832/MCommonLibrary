#include "TestForm.h"
#include "ui_TestForm.h"
#include "MediaPlayerWidget/MediaPlayerWidget.h"

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    m_playerWidget = new MediaPlayerWidget;
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::on_pushButton_clicked()
{
    QSize size = m_playerWidget->size();
    m_playerWidget->resize(640,320);
    m_playerWidget->show();
    m_playerWidget->resize(size);
}
