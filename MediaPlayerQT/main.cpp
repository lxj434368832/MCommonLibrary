#include "MediaPlayerWidget/MediaPlayerWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MediaPlayerWidget w;
    w.show();

    return a.exec();
}
