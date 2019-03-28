#ifndef FORMIMAGEVIEWER_H
#define FORMIMAGEVIEWER_H

#include <QWidget>

namespace Ui {
class FormImageViewer;
}

class FormImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit FormImageViewer(QWidget *parent = 0);
    ~FormImageViewer();

private:
    Ui::FormImageViewer *ui;
};

#endif // FORMIMAGEVIEWER_H
