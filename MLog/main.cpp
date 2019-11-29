#include "MLog.h"
#include "QtLog.h"
#include <QCoreApplication>
#include <QTextCodec>
#include <string>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MLog log;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

//    QTextCodec::codecForName("GBK")->toUnicode("中文");
    // 打印信息
//    LOGM("%s", "test macro define");
    std::string strText = "a\n\t我\n\"a\"";
    logm() << strText;
    qDebug() << QString::fromLocal8Bit(strText.data());
    qDebug()<<"a \t a";
//    qDebug()<<QString::fromLocal8Bit("中文不行呢！");
//    qWarning("This is a warning message.");
//    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
//    a.exec();
//    system("pause");
    return 0;
}

