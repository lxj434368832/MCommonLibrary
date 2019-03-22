#ifndef TESTREADWRITEEXCEL_H
#define TESTREADWRITEEXCEL_H

#include <QMainWindow>

namespace Ui {
class ReadWriteExcel;
}

class TestReadWriteExcel : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestReadWriteExcel(QWidget *parent = 0);
    ~TestReadWriteExcel();

    void TutorialExcel();
private slots:
    void on_btnReadExcel_clicked();

private:
    Ui::ReadWriteExcel *ui;
};

#endif // TESTREADWRITEEXCEL_H
