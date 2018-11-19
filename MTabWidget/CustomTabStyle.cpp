#include "CustomTabStyle.h"

CustomTabStyle::CustomTabStyle(int TabWidth, int TabHeight, QStyle *style)
    :QProxyStyle(style)
{
    if(TabWidth)    m_iTabWidth = TabWidth;
    if(TabHeight) m_iTabHeight = TabHeight;

    m_normal = 0xf0f0f0;
    m_hover = 0xd8eaf9;
    m_selected = 0xffffff;
}

void CustomTabStyle::setColor(QColor normal, QColor horver, QColor selected)
{
    m_normal = normal;
    m_hover = horver;
    m_selected = selected;
}

QSize CustomTabStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize sz = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab)
    {
        sz.transpose();
        sz.rwidth() = m_iTabWidth; // 设置每个tabBar中item的大小
        sz.rheight() = m_iTabHeight;
    }

    return sz;
}

void CustomTabStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_TabBarTabLabel)
    {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
        {
            QRect allRect = tab->rect;
            allRect.setWidth(allRect.width() - 4);
            allRect.setHeight(allRect.height() - 2);
            //选中状态
            if (tab->state & QStyle::State_Selected)
            {
                //save用以保护坐标，restore用来退出状态
                painter->save();
                painter->setBrush(QBrush(m_selected));
                painter->setPen(0xffffff);
                //矩形
                //painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                //带有弧线矩形
                painter->drawRoundedRect(tab->rect, 4, 4);
                painter->restore();
            }
            //hover状态
            else if(tab->state & QStyle::State_MouseOver)
            {
                painter->save();
                painter->setBrush(QBrush(m_hover));
                painter->setPen(0xffffff);
                painter->drawRoundedRect(allRect, 4, 4);
                painter->restore();
            }
            else
            {
                painter->save();
                painter->setBrush(QBrush(m_normal));
                painter->setPen(0xffffff);
                painter->drawRoundedRect(allRect, 4, 4);
                painter->restore();
            }

            QTextOption option;
            option.setAlignment(Qt::AlignCenter);
            painter->setFont(widget->font());
            painter->drawText(allRect, tab->text, option);
            return;
        }
    }

    else if (element == CE_TabBarTab)
    {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}
