#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>

namespace Ui {
class TestForm;
}

class MediaPlayerWidget;

class TestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = 0);
    ~TestForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestForm *ui;
    MediaPlayerWidget   *m_playerWidget;
};

#endif // TESTFORM_H
