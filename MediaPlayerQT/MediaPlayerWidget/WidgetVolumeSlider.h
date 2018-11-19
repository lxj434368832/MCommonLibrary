#ifndef WIDGETVOICESLIDER_H
#define WIDGETVOICESLIDER_H

#include <QWidget>

namespace Ui {
class WidgetVolumeSlider;
}

class WidgetVolumeSlider : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVolumeSlider(QWidget *parent = 0);
    ~WidgetVolumeSlider();

signals:
    void signalVolumeChanged(int value);

private slots:
    void on_sliderVolumeValue_valueChanged(int value);

private:
    Ui::WidgetVolumeSlider *ui;
};

#endif // WIDGETVOICESLIDER_H
