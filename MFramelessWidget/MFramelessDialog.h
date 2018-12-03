#ifndef MFRAMELESSDIALOG_H
#define MFRAMELESSDIALOG_H
/************************************************************************
* function��Qt�Զ���Ի����ƶ������ŵ�ʵ�֣�ע�ⴰ������Ҫ����border����������������
* author :  	������
* datetime��2018-12-04
* company:
/************************************************************************/

#include <QDialog>

class MFramelessPrivate;

class MFramelessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MFramelessDialog(QWidget *parent = 0);
    ~MFramelessDialog();

    void setBorderWidth(int iBorderWidth = 6);
    void setTitleHeight(int iTitleHeight = 36);
    void setMinSize(int iMinWidth, int iMinHeight);
    void setMoveFlag(bool bMove = true);
    void setResizeFlag(bool bResize = true);

signals:
    void signal_position_changed(int px, int py);

    //�����С�ı����Ϣ֪ͨ������ⲿ����resize(),��ʱ�����������ź�
    void signal_size_changed(int width, int height);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    friend class MFramelessPrivate;
    MFramelessPrivate   *d_ptr = nullptr;
};

#endif // MFRAMELESSDIALOG_H
