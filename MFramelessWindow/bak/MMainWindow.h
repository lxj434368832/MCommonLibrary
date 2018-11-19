#ifndef MMAINWINDOW_H
#define MMAINWINDOW_H

#include <QWidget>
#include "../MFramelessWidget/MFramelessWidget.h"

namespace Ui {
class MMainWindow;
}

class WidgetMain;


class MMainWindow : public MFramelessWidget
{
    Q_OBJECT

public:
    explicit MMainWindow(QWidget *parent = 0);
    ~MMainWindow();

private slots:
    void on_btnClose_clicked();

private:
    Ui::MMainWindow *ui;
    WidgetMain            *m_widgetMain;
};

#endif // MMAINWINDOW_H
