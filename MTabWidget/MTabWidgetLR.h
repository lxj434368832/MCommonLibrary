#pragma once
/************************************************************************
* filename:MTabWidgetLR
* function: 自定义实现左右位置的TabWidget，相关接口的含义参考QTabWidget。
* 窗体主要由MTabBar(QWidget)和MTabPane(QStackWidget) 实现，每个tab项为QRadioButton。
* author :   明巧文
* datetime:2018-11-28
* company:
/************************************************************************/

#include <QWidget>

namespace Ui {
class MTabWidgetLR;
}
class QButtonGroup;

class MTabWidgetLR : public QWidget
{
    Q_OBJECT

public:
    explicit MTabWidgetLR(QWidget *parent = 0);
    ~MTabWidgetLR();

    int addTab(QWidget *widget, const QString &);
    int addTab(QWidget *widget, const QIcon& icon, const QString &label);

    int insertTab(int index, QWidget *widget, const QString &);
    int insertTab(int index, QWidget *widget, const QIcon& icon, const QString &label);

    void removeTab(int index);

    QString tabText(int index) const;
    void setTabText(int index, const QString &);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon & icon);

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(QWidget *widget) const;
    int count() const;

    QSize iconSize() const;
    void setIconSize(const QSize &size);

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

signals:
        void currentChanged(int index);

private:
    Ui::MTabWidgetLR *ui;
    QSize                   m_sizeIcon;
    QButtonGroup    *m_group;
};

