#ifndef FORMIMAGEVIEWER_H
#define FORMIMAGEVIEWER_H

#include <QWidget>
#include <QList>
#include <QVector>
#include <QLabel>

namespace Ui {
class FormImageViewer;
}
class QGridLayout;
class FormSingleImage;

class FormImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit FormImageViewer(QWidget *parent = 0);
    ~FormImageViewer();

protected:
    void resizeEvent(QResizeEvent *) override;

private slots:
    void slotModeButtonClicked(int);

    void on_btnInsertToReport_clicked();

    void on_btnDelete_clicked();

    void slot_focus_widget_nt(void *);

private:

    /*************************************************************************
    * function: ÷ÿ≈≈Õº∆¨≤ºæ÷
    *************************************************************************/
    void ReLayoutImage();

private:
    Ui::FormImageViewer *ui;

    QGridLayout     *m_layoutGrid;
    int             m_iRowCount;
    int             m_iColCount;
	int				m_iImageWidth;
	int				m_iImageHeight;
	
    QVector<FormSingleImage*>          m_vctImageWidget;
    FormSingleImage                    *m_focusWidget;
};

#endif // FORMIMAGEVIEWER_H
