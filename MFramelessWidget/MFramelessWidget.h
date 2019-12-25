#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H
/************************************************************************
* function��Qt�Զ��崰���ƶ������ŵ�ʵ�֣�ע�ⴰ������Ҫ����border����������������
* author :  	������
* datetime��2018-07-19
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
    void setMoveFlag(bool bMove = true);            //Ĭ�Ͽ���
    void setResizeFlag(bool bResize = true);       //Ĭ�Ͽ���
    void setMaxShowFlag(bool bMaxShow = true);       //Ĭ�Ͽ���

signals:
    void signal_position_changed(int px, int py);
    void signal_size_changed(int width, int height);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
     void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;        //��ֹ������ʽ�����Ч

private:
    friend class MFramelessPrivate;
    MFramelessPrivate   *d_ptr = nullptr;
};

#endif // FRAMELESSWIDGET_H
