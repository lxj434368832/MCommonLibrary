#ifndef MFRAMELESSWINDOW_H
#define MFRAMELESSWINDOW_H

#include <QWidget>
#include"../MFramelessWidget/MFramelessWidget.h"

namespace Ui {
class MFramelessWindow;
}

class MFramelessWindow : public MFramelessWidget
{
    Q_OBJECT

public:
    explicit MFramelessWindow(QWidget *parent = 0);
    ~MFramelessWindow();

private slots:
    void on_btnClose_clicked();

protected:
    Ui::MFramelessWindow *ui;
};

#endif // MFRAMELESSWINDOW_H
