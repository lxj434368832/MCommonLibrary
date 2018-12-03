#ifndef MFRAMELESSDIALOG_H
#define MFRAMELESSDIALOG_H
/************************************************************************
* function：Qt自定义对话框移动和缩放的实现，注意窗体中需要留出border，否则不能正常工作
* author :  	明巧文
* datetime：2018-12-04
* company:
/************************************************************************/

#include <QDialog>

class MFramelessPrivate;

class MFramelessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MFramelessDialog(QWidget *parent = 0);
    ~MFramelessDialog();

    void setBorderWidth(int iBorderWidth = 6);
    void setTitleHeight(int iTitleHeight = 36);
    void setMinSize(int iMinWidth, int iMinHeight);
    void setMoveFlag(bool bMove = true);
    void setResizeFlag(bool bResize = true);

signals:
    void signal_position_changed(int px, int py);

    //窗体大小改变的消息通知，如果外部调用resize(),此时不会引发此信号
    void signal_size_changed(int width, int height);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    friend class MFramelessPrivate;
    MFramelessPrivate   *d_ptr = nullptr;
};

#endif // MFRAMELESSDIALOG_H
