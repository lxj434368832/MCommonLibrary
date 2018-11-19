#ifndef MWIGETLABELBUTTON_H
#define MWIGETLABELBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class MWidgetLabelButton : public QWidget
{
    Q_OBJECT

public:
    explicit MWidgetLabelButton(QWidget *parent = 0);
    void setLabelText(QString txt);
	void setButtonEnable(bool benable);
    void setButtonText(QString txt);
    void setButtonIcon(QIcon& icon);

signals:
    void signal_button_clicked(QString label);

private slots:
    void on_button_clicked(bool);

private:
    QLabel *label;
    QPushButton *button;
};

#endif // MWIGETLABELBUTTON_H
