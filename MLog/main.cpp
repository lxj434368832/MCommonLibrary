#include "MLog.h"
#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    MLog log;

//    QTextCodec::codecForName("GBK")->toUnicode("����");
    // ��ӡ��Ϣ
//    LOGM("%s", "test macro define");

//    logm()<<"���Ժ궨�壡";
    qDebug("%s", "����");
    qDebug()<<QString::fromLocal8Bit("���Ĳ����أ�");
    qWarning("This is a warning message.");
    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
    a.exec();

    return 0;
}
