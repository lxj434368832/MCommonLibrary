#ifndef READWRITEEXCEL_H
#define READWRITEEXCEL_H

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

#endif // READWRITEEXCEL_H
