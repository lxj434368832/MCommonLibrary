#include "MLog.h"
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>

 MLog* MLog::s_instance = nullptr;

MLog::MLog()
{
    QString logDirectory  =QCoreApplication::applicationDirPath() + "/Log/";
    QDir dir(logDirectory);
    if(!dir.exists())
        dir.mkdir(logDirectory);

    QString qstrTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    m_qstrLogPath = logDirectory + (QCoreApplication::applicationName() + qstrTime+".log");

    s_instance = this;
    m_oldHander = qInstallMessageHandler(MLog::MessageOutput);

    // �ı�ȱʡ��Ϣ�����������
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss zzz]}|[%{function}:%{line}]<%{if-debug}Debug%{endif}%{if-info}Info%{endif}"
                       "%{if-warning}Warn%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}> %{message}");

}

MLog::~MLog()
{
    qInstallMessageHandler(m_oldHander);
    s_instance = nullptr;
}

void MLog::MessageOutput(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
    QString formatMsg = qFormatLogMessage( type, context, msg);
    QString txtMessage = formatMsg;
    /*//����log�����ʽ
    txtMessage += QString("[%1][%2:%3]")
            .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
            .arg(context.function)
            .arg(context.line);

    switch (type)
    {
    case QtDebugMsg:
        txtMessage += QString("|<Debug> %1").arg(msg);
        break;
    case QtWarningMsg:
        txtMessage += QString("<Warning> %1").arg(msg);
        break;
    case QtCriticalMsg:
        txtMessage += QString("<Critical> %1").arg(msg);
        break;
    case QtFatalMsg:
        txtMessage += QString("<Fatal> %1").arg(msg);
        abort();
        break;
    default:
        txtMessage += QString("<UnKnown> %1").arg(msg);
        break;
    }*/

    //�������׼���
//    fprintf(stderr, "%s\n", txtMessage.toLocal8Bit().data());

    txtMessage += QString("\r\n");

    //����
    s_instance->m_mutex.lock();
    //������ļ�(д��׷��ģʽ)
    QFile file(s_instance->m_qstrLogPath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&file);
        out<<txtMessage;
    }
    else
    {
//        fprintf(stderr, "open file:%s ʧ�ܣ�", s_instance->m_qstrLogPath.toLocal8Bit().data());
    }
    file.flush();
    file.close();
    // ����
     s_instance->m_mutex.unlock();
}
