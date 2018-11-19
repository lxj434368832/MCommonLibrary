#include "MWidgetLabelButton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MWidgetLabelButton w;
    w.show();

    return a.exec();
}
