#ifndef FORMTESTTABWIDGET_H
#define FORMTESTTABWIDGET_H

#include <QWidget>

namespace Ui {
class FormTestTabWidget;
}

class FormTestTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormTestTabWidget(QWidget *parent = 0);
    ~FormTestTabWidget();

private slots:
    void on_btnHideSecondTab_clicked();

private:
    Ui::FormTestTabWidget *ui;
};

#endif // FORMTESTTABWIDGET_H
