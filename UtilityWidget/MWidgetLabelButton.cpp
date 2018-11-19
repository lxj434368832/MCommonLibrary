#include "MWidgetLabelButton.h"
#include <QHBoxLayout>
#include <QStyle>

MWidgetLabelButton::MWidgetLabelButton(QWidget *parent) :
QWidget(parent)
{
	setObjectName(QStringLiteral("MWigetLabelButton"));
	resize(224, 22);
	QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
	horizontalLayout->setSpacing(2);
	horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
	horizontalLayout->setContentsMargins(1, 1, 5, 1);

	label = new QLabel(this);
	label->setObjectName(QStringLiteral("wlbLabel"));
	horizontalLayout->addWidget(label);

    button = new QPushButton(this);
	button->setObjectName(QStringLiteral("wlbButton"));
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setFocusPolicy(Qt::NoFocus);
	horizontalLayout->addWidget(button);
}

void MWidgetLabelButton::on_button_clicked(bool)
{
    emit signal_button_clicked(label->text());
}

void MWidgetLabelButton::setButtonEnable(bool benable)
{
	if (benable)
	{
		button->setCursor(Qt::PointingHandCursor);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(on_button_clicked(bool)));
		button->setStyleSheet("#wlbButton{ border:none;background-color: none;}#wlbButton:hover{background-color: rgb(229, 241, 251); }\n"
			"#wlbButton:pressed{background-color: rgb(204, 228, 247);}");
	}
	else
	{
        button->setCursor(Qt::ArrowCursor);
        disconnect(button, SIGNAL(clicked(bool)));
		button->setStyleSheet(" #wlbButton,#wlbButton:hover,#wlbButton:pressed { border:none;background-color:none; }");
	}

	style()->polish(button);
}

void MWidgetLabelButton::setLabelText(QString txt)
{
    label->setText(txt);
}

void MWidgetLabelButton::setButtonText(QString txt)
{
    button->setText(txt);
}

void MWidgetLabelButton::setButtonIcon(QIcon& icon)
{
    button->setIcon(icon);
}