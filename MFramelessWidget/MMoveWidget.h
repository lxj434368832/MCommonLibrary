#ifndef MMOVEWIDGET_H
#define MMOVEWIDGET_H
/************************************************************************
* function：Qt自定义窗体移动的实现
* author :  	明巧文
* datetime：2018-07-19
* company:
/************************************************************************/

#include <QWidget>

class MMoveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MMoveWidget(QWidget *parent = 0);
    virtual ~MMoveWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void handleMove(QPoint pt);

private:
    bool          m_bMove = false;
    QPoint      m_posPress ;
    const int    s_iResizeBorderWidth = 10;
};

#endif // MMOVEWIDGET_H
