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

    // �ı�ȱʡ��Ϣ�����������
    qSetMessagePattern("%{time [yyyy-MM-dd hh:mm:ss zzz]}[%{function}:%{line}]<%{if-debug}Debug%{endif}%{if-info}Info%{endif}"
                       "%{if-warning}Warn%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}> %{message}");

   m_bRun = true;
   m_iTimerId = startTimer(3600000, Qt::VeryCoarseTimer);        //ÿ��1Сʱ����ļ���

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
        ::OutputDebugStringA( "�˳���־�̳߳�ʱ3s��");
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
    /*//����log�����ʽ
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
    while(m_bRun || false == m_listLog.isEmpty())	//����ֹͣ��Ҳ�õ����е���־��д����
    {
        QString qstrLog;
        {
            QMutexLocker lck(&m_mutex);
            if(m_listLog.isEmpty())
                m_wait.wait(&m_mutex);

            while(false == m_listLog.isEmpty())	//�����������־��һ����д���ļ�
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
    qstrTxtMsg.replace("\\n", "\r\n");  //����\n��Ϊ���з�
    qstrTxtMsg.replace("\\t", "\t");  //����\t
    qstrTxtMsg.replace("\\", "\"");  //����\"*/
    qstrTxtMsg.append("\n");          //��ӽ�β����
    std::string strTxt = qstrTxtMsg.toLocal8Bit().data() ;
        //�������׼���
        fprintf(stdout, "%s", strTxt.data());
    #ifdef WIN32
        ::OutputDebugStringA(strTxt.data());
    #endif

        //������ļ�(д��׷��ģʽ)
        QFile file(m_qstrLogPath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream out(&file);
            out<<qstrTxtMsg;
        }
        else
        {
            fprintf(stderr, "����־�ļ�:%s ʧ�ܣ�", m_qstrLogPath.toLocal8Bit().data());
        }
        file.flush();
        file.close();
}
