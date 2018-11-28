#include "MTabWidgetLR.h"
#include "ui_MTabWidgetLR.h"
#include <QButtonGroup>
#include <QRadioButton>

MTabWidgetLR::MTabWidgetLR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MTabWidgetLR),
    m_sizeIcon(16,16)
{
    ui->setupUi(this);
    m_group = new QButtonGroup(this);
    connect(m_group,SIGNAL(buttonClicked(int)), ui->MTabPane,SLOT(setCurrentIndex(int)));
    connect(ui->MTabPane, SIGNAL(currentChanged(int)), this, SIGNAL(currentChanged(int)));
}

MTabWidgetLR::~MTabWidgetLR()
{
    delete ui;
}

int MTabWidgetLR::addTab(QWidget *widget, const QString &label)
{
    int idx = ui->MTabPane->addWidget(widget);
    QAbstractButton *button = new QRadioButton(label);
    ui->MTabBarLayout->insertWidget(idx, button);
    m_group->addButton(button, idx);
    if(nullptr == m_group->checkedButton())
    {
        button->setChecked(true);
    }
    return idx;
}

int MTabWidgetLR::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    int idx = ui->MTabPane->addWidget(widget);
    QAbstractButton *button = new QRadioButton(label);
    button->setIcon(icon);
    ui->MTabBarLayout->insertWidget(idx, button);
    m_group->addButton(button, idx);
    return idx;
}

int MTabWidgetLR::insertTab(int index, QWidget *widget, const QString &label)
{
    int idx = ui->MTabPane->insertWidget(index, widget);
    QAbstractButton *button = new QRadioButton(label);
    ui->MTabBarLayout->insertWidget(idx, button);
    m_group->addButton(button, idx);
    return idx;
}

int MTabWidgetLR::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
    int idx = ui->MTabPane->insertWidget(index, widget);
    QAbstractButton *button = new QRadioButton(label);
    button->setIcon(icon);
    ui->MTabBarLayout->insertWidget(idx, button);
    m_group->addButton(button, idx);
    return idx;
}

void MTabWidgetLR::removeTab(int index)
{
    QWidget *widget = ui->MTabPane->widget(index);
    ui->MTabPane->removeWidget(widget);

    QAbstractButton *button = m_group->button(index);
    m_group->removeButton(button);
    ui->MTabBarLayout->removeWidget(button);
//    widget->deleteLater();
//    button->deleteLater();
}

QString MTabWidgetLR::tabText(int index) const
{
    QAbstractButton *button = m_group->button(index);
    if(button)
        return m_group->button(index)->text();
    else
        return QString();
}

void MTabWidgetLR::setTabText(int index, const QString &label)
{
    QAbstractButton *button = m_group->button(index);
    if(button)
        return m_group->button(index)->setText(label);
}

QIcon MTabWidgetLR::tabIcon(int index) const
{
    QAbstractButton *button = m_group->button(index);
    if(button)
        return m_group->button(index)->icon();
    else
        return QIcon();
}

void MTabWidgetLR::setTabIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = m_group->button(index);
    if(button)
        return m_group->button(index)->setIcon(icon);
}

int MTabWidgetLR::currentIndex() const
{
    return ui->MTabPane->currentIndex();
}

QWidget *MTabWidgetLR::currentWidget() const
{
    return ui->MTabPane->currentWidget();
}

QWidget *MTabWidgetLR::widget(int index) const
{
    return ui->MTabPane->widget(index);
}

int MTabWidgetLR::indexOf(QWidget *widget) const
{
    return ui->MTabPane->indexOf(widget);
}

int MTabWidgetLR::count() const
{
    return ui->MTabPane->count();
}

QSize MTabWidgetLR::iconSize() const
{
    return m_sizeIcon;
}

void MTabWidgetLR::setIconSize(const QSize &size)
{
    if(size != m_sizeIcon)
    {
        m_sizeIcon = size;
        foreach (QAbstractButton *button, m_group->buttons())
        {
            button->setIconSize(size);
        }
    }
}

void MTabWidgetLR::setCurrentIndex(int index)
{
    if(index != ui->MTabPane->currentIndex())
    {
        ui->MTabPane->setCurrentIndex(index);
        index = ui->MTabPane->currentIndex();
        QAbstractButton *button =  m_group->button(index);
        button->setChecked(true);
    }

}

void MTabWidgetLR::setCurrentWidget(QWidget *widget)
{
    int idx = ui->MTabPane->indexOf(widget);
    setCurrentIndex(idx);
}
