#include "tk_signals_widget.h"

tk_signals_widget::tk_signals_widget(QWidget *parent)
    : MFramelessWidget(parent)
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setMouseTracking(true);
    setObjectName("tsw_widget");

    m_this_layout = new QVBoxLayout(this);
    m_this_layout->setContentsMargins(1, 0, 1, 1);
    m_this_layout->setSpacing(0);

    m_title_widget = new tk_signals_title_widget(this);
    m_title_widget->setFixedHeight(40);
    m_title_widget->setObjectName("tsw_title_widget");
    QHBoxLayout *title_layout = new QHBoxLayout(m_title_widget);
    title_layout->setContentsMargins(0, 0, 0, 0);
    title_layout->setSpacing(0);

    ui_lb_title = new QLabel(m_title_widget);
    ui_lb_title->setText(QStringLiteral("±¨¸æ"));
    ui_lb_title->setObjectName("tsw_title_name");
    title_layout->addWidget(ui_lb_title);
    title_layout->addStretch();

    QPushButton *title_close = new QPushButton(m_title_widget);
    ui_lb_title->setText(QStringLiteral("¹Ø±Õ"));
    title_close->setCursor(Qt::PointingHandCursor);
    title_close->setObjectName("tsw_title_close");
    title_layout->addWidget(title_close);

    m_this_layout->addWidget(m_title_widget);
//    m_this_layout->addStretch();

    connect(title_close, &QPushButton::clicked, [=](){
        emit signal_widget_closed(m_idx, m_wnd, ui_lb_title->text());
        close();
        deleteLater();
    });
}

tk_signals_widget::~tk_signals_widget()
{
}

bool tk_signals_widget::add_widget(int idx, QString title, QWidget *wnd)
{
    if (wnd == NULL || m_idx == idx)
    {
		close();
		deleteLater();
        return false;
    }

	m_idx = idx;
	m_wnd = wnd;
	ui_lb_title->setText(title);
    m_this_layout->addWidget(wnd);
    return true;
}

void tk_signals_widget::resizeEvent(QResizeEvent *event)
{
    //m_title_widget->setGeometry(0, 0, m_title_widget->width(), height());
}


/////////////////////////tk_signals_title_widget/////////////////////////
tk_signals_title_widget::tk_signals_title_widget(QWidget *parent)
    : QWidget(parent)
{
}

tk_signals_title_widget::~tk_signals_title_widget()
{
}

void tk_signals_title_widget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (this->parentWidget()->isMaximized())
	{
		this->parentWidget()->showNormal();
	}
	else
	{
		this->parentWidget()->showMaximized();
	}
}

void tk_signals_title_widget::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
