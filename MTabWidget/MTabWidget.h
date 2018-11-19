#ifndef MTABWIDGET_H
#define MTABWIDGET_H

#include <QWidget>

namespace Ui {
class MTabWidget;
}

class MTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MTabWidget(QWidget *parent = 0);
    ~MTabWidget();

private:
    Ui::MTabWidget *ui;

};

#endif // MTABWIDGET_H
