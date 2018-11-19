#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QMainWindow>

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = 0);
    ~WidgetMain();

private:
    Ui::WidgetMain *ui;
};

#endif // WIDGETMAIN_H
