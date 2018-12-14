#ifndef CAMSCREEN_WIDGET_H
#define CAMSCREEN_WIDGET_H

#include <QtWidgets>
#include <QLabel>
#include <QImage>

class yuan_decoder;
class widget;
class little_cam_wgt : public QWidget
{
    Q_OBJECT
public:
    little_cam_wgt(QWidget *parent = nullptr);
    ~little_cam_wgt();
    HWND hwnd(){ return m_wnd_; }

private:
    widget *m_cam_wnd_;
    HWND    m_wnd_;
    QLabel *m_microphone_state_;    //Âó¿Ë·ç×´Ì¬
    QLabel *m_loudspeaker_state_;   //ÑïÉùÆ÷×´Ì¬
    QLabel *m_webcam_state_;        //ÉãÏñÍ·×´Ì¬
    QLabel *m_signal_state_;        //ÍøÂçĞÅºÅ×´Ì¬
    QLabel *m_state_hint_;          //×´Ì¬ÎÄ×ÖÌáÊ¾
};

class widget : public QWidget
{
    Q_OBJECT
public:
    widget(QWidget *parent = nullptr);
    ~widget();

signals :
    void mouse_click();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event)override;
    void paintEvent(QPaintEvent *);
};

#endif 
