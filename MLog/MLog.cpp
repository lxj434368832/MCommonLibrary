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

MLog::MLog() : QThread()
{
    s_instance = this;
    CheckFileName();
    m_oldHander = qInstallMessageHandler(MLog::MessageOutput);

    // 改变缺省消息处理程序的输出
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss zzz]}[%{function}:%{line}]<%{if-debug}Debug%{endif}%{if-info}Info%{endif}"
                       "%{if-warning}Warn%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}> %{message}");

   m_bRun = true;
   m_iTimerId = startTimer(3600000, Qt::VeryCoarseTimer);        //每隔1小时检测文件名

    start();
}

MLog::~MLog()
{
    m_bRun = false;
    killTimer(m_iTimerId);

    m_wait.wakeOne();
	quit();
    if(false ==wait(3000))
    {
        ::OutputDebugStringA( "退出日志线程超时3s！");
        terminate();
    }

    qInstallMessageHandler(m_oldHander);
    s_instance = nullptr;
}

void MLog::MessageOutput(QtMsgType type, const QMessageLogContext & context, const QString &msg)
{
    if( msg.startsWith('[') )
        s_instance->AddLog(msg);
    else
        s_instance->AddLog(qFormatLogMessage( type, context, msg));

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
}

void MLog::timerEvent(QTimerEvent *event)
{
    CheckFileName();
}

void MLog::AddLog(const QString &qstrTxtMsg)
{
    m_mutex.lock();
    m_listLog.push_back(qstrTxtMsg);
    m_wait.wakeOne();
    m_mutex.unlock();
}

void MLog::run()
{
    while(m_bRun || false == m_listLog.isEmpty())	//就算停止了也得等所有的日志都写完了
    {
        QString qstrLog;
        {
            QMutexLocker lck(&m_mutex);
            if(m_listLog.isEmpty())
                m_wait.wait(&m_mutex);

            while(false == m_listLog.isEmpty())	//采用如果有日志就一次性写入文件
            {
                qstrLog.append(m_listLog.front());
                m_listLog.pop_front();
            }
        }

        WriteLog(qstrLog);
    }
}

void MLog::CheckFileName()
{
    QDateTime dt =  QDateTime::currentDateTime();

    if(dt.date().dayOfYear() != m_iDay)
    {
        QString logDirectory  =QCoreApplication::applicationDirPath() + "/Log/";
        QDir dir(logDirectory);
        if(!dir.exists())
            dir.mkdir(logDirectory);

        QString qstrTime = dt.toString("yyyyMMddHHmmss");
        m_mutex.lock();
        m_qstrLogPath = logDirectory + (QCoreApplication::applicationName() + qstrTime+".log");
        m_mutex.unlock();
        m_iDay = dt.date().dayOfYear();
    }
}

void MLog::WriteLog(QString &qstrTxtMsg)
{
    /*qstrTxtMsg.remove('\"');
    qstrTxtMsg.replace("\\n", "\r\n");  //修正\n符为换行符
    qstrTxtMsg.replace("\\t", "\t");  //修正\t
    qstrTxtMsg.replace("\\", "\"");  //修正\"*/
    qstrTxtMsg.append("\n");          //添加结尾换行
    std::string strTxt = qstrTxtMsg.toLocal8Bit().data() ;
        //输出到标准输出
        fprintf(stdout, "%s", strTxt.data());
    #ifdef WIN32
        ::OutputDebugStringA(strTxt.data());
    #endif

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
}
