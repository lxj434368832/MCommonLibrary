#include "MLog.h"
#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    MLog log;

//    QTextCodec::codecForName("GBK")->toUnicode("中文");
    // 打印信息
//    LOGM("%s", "test macro define");

//    logm()<<"测试宏定义！";
    qDebug("%s", "中文");
    qDebug()<<QString::fromLocal8Bit("中文不行呢！");
    qWarning("This is a warning message.");
    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
    a.exec();

    return 0;
}
