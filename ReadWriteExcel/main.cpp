#include "TestReadWriteExcel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestReadWriteExcel w;
    w.show();

    return a.exec();
}
