#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H
/************************************************************************
* function��Qt�Զ��崰���ƶ������ŵ�ʵ�֣�ע�ⴰ������Ҫ����border����������������
* author :  	������
* datetime��2018-07-19
* company:
/************************************************************************/

#include <QWidget>

enum ResizeRegion
{
    Default,
    Move,
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

class MFramelessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MFramelessWidget(QWidget *parent = 0);
    virtual ~MFramelessWidget();

signals:
    void signal_position_changed(int px, int py);
    void signal_size_changed(int width, int height);

protected:
    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setResizeCursor(ResizeRegion region);
    ResizeRegion getResizeRegion(QPoint clientPos);
    void handleMove(QPoint pt);
    void handleResize(QPoint pt);

    void setMouseStatus(QPoint mouseCursorPos);

private:
    void add_diff(int orig, int &diff, int min);
    void decrease_diff(int orig, int &diff, int min);

protected:
    int             m_iBorderWidth = 6;
    int             m_iTitleHeight = 36;

    bool        m_bMove = true;           //���ڿ����Ƿ��ƶ��Ŀ���
    bool        m_bResize = true;        //���ڿ����Ƿ����ŵĿ���

    int m_min_width = 348;			//������С���
    int m_min_height = 220;			//������С�߶�

    bool                    m_bDrag = false;           //����ά�����û���ʶ�Ƿ��϶�
    QPoint               m_posDrag ;
    ResizeRegion   m_eResizeRegion = Default;
    QPoint               m_posResizeDown;
    QRect                m_rectResizeDown;

};

#endif // FRAMELESSWIDGET_H
