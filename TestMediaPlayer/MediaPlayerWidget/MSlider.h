#ifndef CMSLIDER_H
#define CMSLIDER_H

#include <QSlider>

class MSlider : public QSlider
{
    Q_OBJECT

public:
    explicit MSlider(QWidget *parent = 0);
    bool event(QEvent *event);

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // CMSLIDER_H
