#include "MComboBox.h"
#include "ui_MComboBox.h"

MComboBox::MComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MComboBox)
{
    ui->setupUi(this);
    QFontMetrics metric = ui->comboBox->fontMetrics();
    int width =  ui->comboBox->width() - 16;

    int icount = ui->comboBox->count();
    for(int i = 0; i < icount; i++)
    {
        QString txt = ui->comboBox->itemText(i);
        addBlankForCenterLayout(metric, txt, width);
        ui->comboBox->setItemText(i, txt);
    }
}

MComboBox::~MComboBox()
{
    delete ui;
}

void MComboBox::addBlankForCenterLayout(QFontMetrics &metrics, QString &txt, int widgetWidth)
{
    int txtWidth = metrics.width(txt);
    int blankWidth = metrics.width(' ');
    int icount = (widgetWidth - txtWidth) / 2.0 / blankWidth ;
    QByteArray array(icount, ' ');
    txt.insert(0, array);
}
