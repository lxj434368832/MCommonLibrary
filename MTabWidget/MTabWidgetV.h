#ifndef MTABWIDGETV_H
#define MTABWIDGETV_H

#include <QWidget>

namespace Ui {
class MTabWidgetV;
}

class MTabWidgetV : public QWidget
{
    Q_OBJECT

public:
    explicit MTabWidgetV(QWidget *parent = 0);
    ~MTabWidgetV();

private:
    Ui::MTabWidgetV *ui;
};

#endif // MTABWIDGETV_H
