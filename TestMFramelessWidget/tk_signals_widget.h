#pragma once
#include "MFramelessWidget/MFramelessWidget.h"
#include <QtWidgets>

class tk_signals_title_widget;
class tk_signals_widget : public MFramelessWidget
{
    Q_OBJECT
public:
    tk_signals_widget(QWidget *parent = nullptr);
    ~tk_signals_widget();
	bool add_widget(int idx, QString title, QWidget *wnd);

signals :
	//相机窗口大小改变通知
	void signal_widget_size_change(int width, int heigh);

	//相机窗口位置改变通知
	void signal_widget_position_change(int px, int py);

	void signal_widget_closed(int idx, QWidget *wnd, QString title);

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	int							m_idx = -1;
	QWidget						*m_wnd;
    QVBoxLayout					*m_this_layout = nullptr;
    tk_signals_title_widget     *m_title_widget = nullptr;
    QLabel						*ui_lb_title = nullptr;
};

class tk_signals_title_widget : public QWidget
{
    Q_OBJECT

public:
    tk_signals_title_widget(QWidget *parent);
    ~tk_signals_title_widget();

private:
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *)override;
};
