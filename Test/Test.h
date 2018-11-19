#ifndef TEST_H
#define TEST_H

#include <QMainWindow>

namespace Ui {
class Test;
}

class Test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = 0);
    ~Test();

private slots:
    void on_btnExport_clicked();

    void on_btnImport_clicked();

private:
    Ui::Test *ui;
};

#endif // TEST_H
