#include "MFtp.h"
#include <QDebug>
#include <QMessageBox>
#include <utils/profiles.h>

MFtp::MFtp(QObject *parent) :
    QObject(parent),
    m_pFtp(0),
    m_strHost(""),
    m_iPort(21),
    m_strUserName(""),
    m_strPassword(""),
    m_enTaskType(ETT_NO_TASK)
{
    Utils::Profiles::instance()->setDefault(INI_SECTION_NAME,INI_FTP_SERVER_IP,"192.168.1.163");

   m_strHost = Utils::Profiles::instance()->value(INI_SECTION_NAME,INI_FTP_SERVER_IP).toString();

    m_pFtp = new QFtp(this);
    connect(m_pFtp, SIGNAL(commandFinished(int,bool)), this, SLOT(slotCommandFinished(int,bool)));
    connect(m_pFtp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(slotListInfo(QUrlInfo)));
    connect(m_pFtp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SIGNAL(signalDataTransferProgress(qint64,qint64)));
}

MFtp::MFtp(QObject *parent, QString strHost, qint16 iPort, QString strUserName, QString strPassword):
    QObject(parent),
    m_pFtp(0),
    m_strHost(strHost),
    m_iPort(iPort),
    m_strUserName(strUserName),
    m_strPassword(strPassword),
    m_enTaskType(ETT_NO_TASK)
{
    m_pFtp = new QFtp();
    connect(m_pFtp, SIGNAL(commandFinished(int,bool)), this, SLOT(slotCommandFinished(int,bool)));
    connect(m_pFtp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(slotListInfo(QUrlInfo)));
    connect(m_pFtp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SIGNAL(signalDataTransferProgress(qint64,qint64)));
}

MFtp::~MFtp()
{
    if(m_pFtp)
    {
        m_pFtp->close();
        m_pFtp->deleteLater();
        m_pFtp = 0;
    }
}

void MFtp::setTaskType(ETaskType taskType)
{
    m_enTaskType = taskType;
}

ETaskType MFtp::getTaskType()
{
    return m_enTaskType;
}

void MFtp::connectToHost()
{
    if(QFtp::Connected > m_pFtp->state())               //如果还没有连接成功则发送连接命令
        m_pFtp->connectToHost(m_strHost, m_iPort);
}

/*
bool MFtp::connectToHost()
{
    bool bRet = false;

    if(0 == m_pFtp)
    {
        QMessageBox::warning(0, "warning", "please init Ftp module");
        return bRet;
    }

    QFtp::State ftpState = m_pFtp->state();
    qDebug()<<"ftp current connect state is:"<<ftpState;

    if(QFtp::Unconnected == ftpState || QFtp::Closing == ftpState)
    {
        m_enTaskType = ETT_CONNECT_HOST;

        emit signalConnectToHost();             //发送连接到主机命令
        m_mutex.lock();
        bRet =  m_wait.wait(&m_mutex, 3000);
        m_mutex.unlock();

        if(false == bRet)
        {
            qWarning()<<"connect to host timeout 3 seconds!";
            return bRet;
        }
        else if(false == m_strErrorString.isEmpty())     //只有当执行没有错误时才算成功
        {
            qWarning()<<"connect to host encounter error!";
            return bRet;
        }

        emit signalLogin();                   //调用线程阻塞，工作线程进行连接操作
        m_mutex.lock();
        bRet =  m_wait.wait(&m_mutex, 3000);
        m_mutex.unlock();
        if(false == bRet)
        {
            qWarning()<<"Login is timeout 3 seconds!";
        }
        else if(m_strErrorString.isEmpty())     //只有当执行没有错误时才算成功
        {
            bRet = true;
        }

    }
    else if(QFtp::LoggedIn == ftpState)
    {
        bRet = true;
    }
    return bRet;
}*/

void MFtp::list(QString strDir)
{
    m_pFtp->list(strDir);
}

void MFtp::abortCommand()
{
    m_pFtp->abort();
}

void MFtp::slotCommandFinished(int id, bool err)
{
    QFtp::Command command = m_pFtp->currentCommand();
    QFtp::State state = m_pFtp->state();
    if(true == err)     //如果出现了错误
    {
        m_strErrorString = m_pFtp->errorString();
        qWarning()<<QString("excute command:%1 failed, error message:%2").arg(command).arg(m_strErrorString);
    }
    else
    {
        m_strErrorString.clear();
    }


    switch(command)
    {
    case QFtp::ConnectToHost:
        qDebug()<<"current FTP connect state is :"<< state;

        if(QFtp::Connected == state)        //如果已经连接，则登陆
            m_pFtp->login();
        break;
    case QFtp::Login:
        qDebug()<<"current FTP connect state is :"<< state;

        if(QFtp::LoggedIn == state)
            emit signalConnectToHostSucessed();
        else
            qWarning()<<"login to host failed,please check username and password!";
        break;
    case QFtp::List:
        m_wait.wakeOne();
        break;
    case QFtp::Get:
        emit signalFileDownloadFinished(m_strErrorString);
        break;
    default:
        qDebug()<<"unprocess ftp command:"<<command;
        break;
    }
}

void MFtp::slotListInfo(const QUrlInfo &urlInfo)
{
    if(m_bIsFolder == urlInfo.isDir())
    {
        m_strFileList.append(urlInfo.name());
    }
}

QStringList MFtp::getFolderOrFileList(QString strDir, bool bIsFolder)
{
    m_bIsFolder = bIsFolder;
    m_strFileList.clear();

    if(QFtp::LoggedIn == m_pFtp->state())   //如果已经登陆
    {
//        m_enTaskType = ETT_GET_FILE_LIST;
        emit list(strDir);       //执行获取文件列表命令,如果执行成功，m_strFIleList存储了获取到的文件或者文件夹列表
        m_mutex.lock();
        bool bRet =  m_wait.wait(&m_mutex, 5000);
        m_mutex.unlock();
//        m_enTaskType = ETT_NO_TASK;
        if(false == bRet)    //获取文件列表超时
        {
            qWarning()<<"connect to host timeout 5 seconds!";
        }
    }
    else
    {
        qWarning()<<"host is not connected! current connect status is:"<<m_pFtp->state();
    }
    return m_strFileList;
}

bool MFtp::getReplayDataInfo(ReplayDataInfo &info)
{
    connectToHost();
    QString strDir = "hrdac";
    QStringList strDirList = getFolderOrFileList(strDir, true);
    if(strDirList.isEmpty())
    {
        qWarning()<<"getReplayDataInfo get folder list is failed!";
        return false;
    }

    for(int i = 0; i < strDirList.count(); i++)
    {
        QString strFileName = strDirList[i];
        if(false == getDateInfo(QString("%1/%2").arg(strDir).arg(strFileName), info[strFileName]))
        {
            return false;
        }
    }
    return true;
}

bool MFtp::getDateInfo(QString strDir, DateInfo &info)
{
    QStringList strDirList = getFolderOrFileList(strDir, true);
    if(strDirList.isEmpty())
    {
        qWarning()<<"getDateInfoList get folder list is failed!";
    }
    for(int i = 0; i < strDirList.count(); i++)
    {
        QString strFileName = strDirList[i];

        if(false == getModuleInfo(QString("%1/%2").arg(strDir).arg(strFileName), info[strFileName]))
        {
            return false;
        }
    }
    return true;
}

bool MFtp::getModuleInfo(QString strDir, ModuleInfo &info)
{
    QStringList strDirList = getFolderOrFileList(strDir, true);
    if(strDirList.isEmpty())
    {
        qWarning()<<"getModuleInfoList get folder list is failed!";
    }

    for(int i = 0; i < strDirList.count(); i++)
    {
        QString strFileName = strDirList[i];

        if(false == getFileList(QString("%1/%2").arg(strDir).arg(strFileName), info[strFileName]))
        {
            return false;
        }
    }
    return true;
}

bool MFtp::getFileList(QString strDir, FileList &list)
{
    QStringList strDirList = getFolderOrFileList(strDir, false);
    if(strDirList.isEmpty())
    {
        qWarning()<<"getFileList get file list is failed!";
    }
    list = strDirList;
    return true;
}

bool MFtp::downloadFile(QString strFilePath, QIODevice *dev)
{
    bool bRet = false;

    if(QFtp::LoggedIn == m_pFtp->state())
    {
        m_pFtp->get(strFilePath, dev);
        bRet = true;
    }
    else
    {
        qWarning()<<"host is not connected!";
    }
    return bRet;
}
