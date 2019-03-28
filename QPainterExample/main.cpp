#include "FormLotteryNumber.h"
#include "QPainterExample.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPainterExample w;
//    FormLotteryNumber w;
    w.show();

    return a.exec();
}
