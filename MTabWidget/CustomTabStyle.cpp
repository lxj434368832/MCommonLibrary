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
        sz.rwidth() = m_iTabWidth; // ����ÿ��tabBar��item�Ĵ�С
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
            //ѡ��״̬
            if (tab->state & QStyle::State_Selected)
            {
                //save���Ա������꣬restore�����˳�״̬
                painter->save();
                painter->setBrush(QBrush(m_selected));
                painter->setPen(0xffffff);
                //����
                //painter->drawRect(allRect.adjusted(0, 0, 0, 0));
                //���л��߾���
                painter->drawRoundedRect(tab->rect, 4, 4);
                painter->restore();
            }
            //hover״̬
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
