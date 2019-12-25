#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H
/************************************************************************
* function：Qt自定义窗体移动和缩放的实现，注意窗体中需要留出border，否则不能正常工作
* author :  	明巧文
* datetime：2018-07-19
* company:
/************************************************************************/

#include <QWidget>

class MFramelessPrivate;

class MFramelessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFramelessWidget(QWidget *parent = 0);
    virtual ~MFramelessWidget();

    void setBorderWidth(int iBorderWidth = 6);
    void setTitleHeight(int iTitleHeight = 36);
    void setMinSize(int iMinWidth, int iMinHeight);
    void setMoveFlag(bool bMove = true);            //默认开启
    void setResizeFlag(bool bResize = true);       //默认开启
    void setMaxShowFlag(bool bMaxShow = true);       //默认开启

signals:
    void signal_position_changed(int px, int py);
    void signal_size_changed(int width, int height);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
     void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;        //防止设置样式表后无效

private:
    friend class MFramelessPrivate;
    MFramelessPrivate   *d_ptr = nullptr;
};

#endif // FRAMELESSWIDGET_H
