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

//    QTextCodec::codecForName("GBK")->toUnicode("����");
    // ��ӡ��Ϣ
//    LOGM("%s", "test macro define");
    std::string strText = "a\n\t��\n\"a\"";
    logm() << strText;
    qDebug() << QString::fromLocal8Bit(strText.data());
    qDebug()<<"a \t a";
//    qDebug()<<QString::fromLocal8Bit("���Ĳ����أ�");
//    qWarning("This is a warning message.");
//    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
//    a.exec();
//    system("pause");
    return 0;
}

