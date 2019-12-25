#ifndef DIALOGTESTMAXIMUM_H
#define DIALOGTESTMAXIMUM_H

#include "../MFramelessWidget/MFramelessDialog.h"
#include <QDialog>

namespace Ui {
class DialogTestMaximum;
}

class DialogTestMaximum : public MFramelessDialog
{
    Q_OBJECT

public:
    explicit DialogTestMaximum(QWidget *parent = 0);
    ~DialogTestMaximum();

private:
    Ui::DialogTestMaximum *ui;
};

#endif // DIALOGTESTMAXIMUM_H
