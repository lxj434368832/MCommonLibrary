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
    void setMoveFlag(bool bMove = true);                        //Ĭ�Ͽ���
    void setResizeFlag(bool bResize = true);                 //���ı䴰���С��ʶ
    void setMaxShowFlag(bool bMaxShow = true);       //˫�������ʾ��ʶ

signals:
    void signalPositionChanged(int px, int py);
    void signalSizeChanged(int width, int height);

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
