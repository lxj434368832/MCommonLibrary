#include "MLog.h"
#include <QCoreApplication>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MLog log;

    // 打印信息
    qDebug("中文不行呢");
    qWarning("This is a warning message.");
    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
    a.exec();

    return 0;
}
