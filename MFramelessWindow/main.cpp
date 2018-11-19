#include "MFramelessWindow.h"
#include "MMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MFramelessWindow w;
    w.show();

    return a.exec();
}
