#include "MLog.h"
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>
#include <QTextStream>
#include <QMutex>

#ifdef WIN32
#include <Windows.h>
#endif

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

    // 改变缺省消息处理程序的输出
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss zzz]}|[%{function}:%{line}]<%{if-debug}Debug%{endif}%{if-info}Info%{endif}"
                       "%{if-warning}Warn%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}> %{message}");
   m_bRun = true;
    start();
}

MLog::~MLog()
{
    m_bRun = false;
    m_wait.wakeOne();
    terminate();
    wait();
    qInstallMessageHandler(m_oldHander);
    s_instance = nullptr;
}

void MLog::MessageOutput(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
    QString qstrTxtMsg = qFormatLogMessage( type, context, msg);
   s_instance->AddLog(qstrTxtMsg);
}

void MLog::AddLog(QString &qstrTxtMsg)
{
    m_mutex.lock();
    m_listLog.push_back(qstrTxtMsg);
    m_wait.wakeOne();
    m_mutex.unlock();
}

void MLog::run()
{
    while(m_bRun)
    {
        m_mutex.lock();
        if(m_listLog.isEmpty())
            m_wait.wait(&m_mutex);

        WriteLog(m_listLog.front());
        m_listLog.pop_front();
        m_mutex.unlock();
    }
}

void MLog::WriteLog(QString &qstrTxtMsg)
{
    //    QString qstrTxtMsg = formatMsg;
        /*//设置log输出格式
        qstrTxtMsg += QString("[%1][%2:%3]")
                .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                .arg(context.function)
                .arg(context.line);

        switch (type)
        {
        case QtDebugMsg:
            qstrTxtMsg += QString("|<Debug> %1").arg(msg);
            break;
        case QtWarningMsg:
            qstrTxtMsg += QString("<Warning> %1").arg(msg);
            break;
        case QtCriticalMsg:
            qstrTxtMsg += QString("<Critical> %1").arg(msg);
            break;
        case QtFatalMsg:
            qstrTxtMsg += QString("<Fatal> %1").arg(msg);
            abort();
            break;
        default:
            qstrTxtMsg += QString("<UnKnown> %1").arg(msg);
            break;
        }*/


        //输出到标准输出
        fprintf(stdout, "%s\n", qstrTxtMsg.toLocal8Bit().data());
    #ifdef WIN32
        ::OutputDebugStringA(qstrTxtMsg.toLocal8Bit().data());
        ::OutputDebugStringA("\n");
    #endif

        qstrTxtMsg += QString("\r\n");
        //加锁
//       m_mutex.lock();

        //输出到文件(写，追加模式)
        QFile file(m_qstrLogPath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream out(&file);
            out<<qstrTxtMsg;
        }
        else
        {
            fprintf(stderr, "打开日志文件:%s 失败！", m_qstrLogPath.toLocal8Bit().data());
        }
        file.flush();
        file.close();
        // 解锁
//        m_mutex.unlock();
}
