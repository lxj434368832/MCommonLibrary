#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H
/************************************************************************
* function：Qt自定义窗体移动和缩放的实现，注意窗体中需要留出border，否则不能正常工作
* author :  	明巧文
* datetime：2018-07-19
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

    bool        m_bMove = true;           //用于控制是否移动的开关
    bool        m_bResize = true;        //用于控制是否缩放的开关

    int m_min_width = 348;			//窗体最小宽度
    int m_min_height = 220;			//窗体最小高度

    bool                    m_bDrag = false;           //基类维护的用户标识是否拖动
    QPoint               m_posDrag ;
    ResizeRegion   m_eResizeRegion = Default;
    QPoint               m_posResizeDown;
    QRect                m_rectResizeDown;

};

#endif // FRAMELESSWIDGET_H
