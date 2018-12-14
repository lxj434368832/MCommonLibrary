#ifndef MFRAMELESSPRIVATE_H
#define MFRAMELESSPRIVATE_H

#include <QWidget>

enum MouseStatus
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

class MFramelessPrivate : public QObject
{
    Q_OBJECT

public:
    MFramelessPrivate(QWidget *widget);

    void setBorderWidth(int iBorderWidth);
    void setTitleHeight(int iTitleHeight);
    void setMinSize(int iMinWidth, int iMinHeight);
    void setMoveFlag(bool bMove);
    void setResizeFlag(bool bResize);

    void paintEvent(QPaintEvent *event);        //��ֹ������ʽ�����Ч
    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void signal_position_changed(int px, int py);
    void signal_size_changed(int width, int height);

private:
    void setMouseStatus(QPoint mouseCursorPos);
    void setMouseCursor(MouseStatus region);
    MouseStatus getResizeRegion(QPoint clientPos);
    void handleMove(QPoint pt);
    void handleResize(QPoint pt);

    void add_diff(int orig, int &diff, int min);
    void decrease_diff(int orig, int &diff, int min);

private:
    QWidget         *widget;
    int             m_iBorderWidth = 6;
    int             m_iTitleHeight = 36;

    int             m_iMinWidth = 200;
    int             m_iMinHeight = 100;

    bool        m_bMove = true;           //���ڿ����Ƿ��ƶ��Ŀ���
    bool        m_bResize = true;        //���ڿ����Ƿ����ŵĿ���


    bool                   m_bDrag = false;           //����ά�����û���ʶ�Ƿ��϶�
    QPoint               m_posDrag ;
    MouseStatus   m_eMouseStatus = Default;
    QPoint               m_posResizeDown;
    QRect                m_rectResizeDown;
};

#endif // MFRAMELESSPRIVATE_H
