#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QRect>
#include <QSize>

class MCustomTabStyle : public QProxyStyle
{
public:
    MCustomTabStyle(int TabWidth = 0, int TabHeight = 0, QStyle *style=0);
    void setColor(QColor normal, QColor horver, QColor selected);
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

private:
    int m_iTabWidth = 120;
    int m_iTabHeight = 39;
    QColor  m_normal;
    QColor  m_hover;
    QColor  m_selected;

};

#endif // CUSTOMTABSTYLE_H
