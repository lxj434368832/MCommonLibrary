#ifndef MFTP_H
#define MFTP_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "qftp.h"
#include "../TracksDataConstants.h"

class MFtp : public QObject
{
    Q_OBJECT
public:
    MFtp(QObject *parent);
    MFtp(QObject *parent, QString strHost, qint16 iPort = 21, QString strUserName = QString(), QString strPassword = QString());
    ~MFtp();

    void setTaskType(ETaskType taskType);
    ETaskType getTaskType();

    void connectToHost();
    void list(QString strDir);
    void abortCommand();

//    bool connectToHost();       //同步工作的方式
    QStringList getFolderOrFileList(QString strDir, bool bIsFolder);

    bool getReplayDataInfo(ReplayDataInfo &info);
    bool downloadFile(QString strFilePath,  QIODevice *dev);        //采用ftp异步的方式下载

    //通知外部的信号
signals:
    void signalConnectToHostSucessed();                //通知界面连接到主机成功
    void signalDataTransferProgress(qint64 done, qint64 total); //主要用于通知界面文件下载的进度

    //文件下载结束通知，如果strError为空代表没有错误，否则有错误
    void signalFileDownloadFinished(QString strError);

private slots:
    void slotListInfo(const QUrlInfo& urlInfo);     //用于list命令执行后接收文件信息
    void slotCommandFinished(int id, bool err);  // QFtp命令执行完成的通知


private:
    bool getDateInfo(QString strDir, DateInfo &info);
    bool getModuleInfo(QString strDir, ModuleInfo &info);
    bool getFileList(QString strDir, FileList &list);

private:
    QFtp    *m_pFtp;

    QString m_strHost;
    qint16   m_iPort;
    QString m_strUserName;
    QString m_strPassword;

    QMutex                     m_mutex;            //用于相关任务的同步处理
    QWaitCondition       m_wait;

    ETaskType            m_enTaskType;      //用于决定接收到结束命令后要如何处理
    QString                  m_strErrorString;    //用于标志命令是否执行正确

    bool                    m_bIsFolder;          //用于获取文件列表时标识文件类型
    QStringList         m_strFileList;          //用于暂存文件列表
};

#endif // MFTP_H
