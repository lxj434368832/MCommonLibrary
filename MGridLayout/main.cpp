#include "FormImageViewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormImageViewer w;
    w.show();

    return a.exec();
}
