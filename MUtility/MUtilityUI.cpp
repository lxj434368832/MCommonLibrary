#include "MUtilityUI.h"

void MUtilityUI::addBlankForCenterLayout(QFontMetrics &metrics, QString &txt, int widgetWidth)
{
    int txtWidth = metrics.width(txt);
    int blankWidth = metrics.width(' ');
    int icount = (widgetWidth - txtWidth) / 2.0 / blankWidth ;
    QByteArray array(icount, ' ');
    txt.insert(0, array);

}
