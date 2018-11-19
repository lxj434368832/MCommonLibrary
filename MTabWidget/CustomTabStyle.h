#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QRect>
#include <QSize>

class CustomTabStyle : public QProxyStyle
{
public:

    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
        {
            s.transpose();
            s.rwidth() = 150; // ����ÿ��tabBar��item�Ĵ�С
            s.rheight() = 50;
        }

        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QRect allRect = tab->rect;
                allRect.setWidth(allRect.width() - 5);
                allRect.setHeight(allRect.height() - 2);
                //ѡ��״̬
                if (tab->state & QStyle::State_Selected)
                {
                    //save���Ա������꣬restore�����˳�״̬
                    painter->save();
                    painter->setBrush(QBrush(0x101010));
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
                    painter->setBrush(QBrush(0x323232));
                    painter->drawRoundedRect(allRect, 4, 4);
                    painter->restore();
                }
                else
                {
                    painter->save();
                    painter->setBrush(QBrush(0x646464));
                    painter->drawRoundedRect(allRect, 4, 4);
                    painter->restore();
                }

                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                painter->setFont(widget->font());
//                painter->setFont(QFont("����", 18, QFont::Bold));
//                painter->setPen(0xffffff);
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        else if (element == CE_TabBarTab)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

#endif // CUSTOMTABSTYLE_H
