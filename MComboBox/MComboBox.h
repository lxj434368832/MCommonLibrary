#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H

#include <QWidget>

namespace Ui {
class MComboBox;
}

class MComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit MComboBox(QWidget *parent = 0);
    ~MComboBox();

    void addBlankForCenterLayout(QFontMetrics &metrics, QString &txt, int widgetWidth);

private:
    Ui::MComboBox *ui;
};

#endif // MCOMBOBOX_H
