#ifndef QPAINTEREXAMPLE_H
#define QPAINTEREXAMPLE_H

#include <QWidget>

namespace Ui {
class QPainterExample;
}

class QPainterExample : public QWidget
{
    Q_OBJECT

public:
    explicit QPainterExample(QWidget *parent = 0);
    ~QPainterExample();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::QPainterExample *ui;
};

#endif // QPAINTEREXAMPLE_H
