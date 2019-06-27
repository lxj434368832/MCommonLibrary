#ifndef MLOG_H
#define MLOG_H

/****************************************************************
 * function: 日志文件
 * 1、pro文件中添加 DEFINES += QT_MESSAGELOGCONTEXT
 * 2、在mai函数QApplication 后加MLog log
 * 3、如果需要输出到控制台，在pro加CONFIG += console
****************************************************************/

#include <QMutex>
#include <QStringList>
#include <QtDebug>
#include <QThread>
#include <QWaitCondition>

//#pragma execution_character_set("utf-8")

class MLog : public QThread
{
    Q_OBJECT
public:
    MLog();
    ~MLog() override;

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    static void MessageOutput(QtMsgType, const QMessageLogContext&,const QString&);
    virtual void run() override;

    void CheckFileName();
    void AddLog(QString &qstrTxtMsg);
    void WriteLog(QString &qstrTxtMsg);

private:
    static MLog* s_instance;

    QString m_qstrLogPath;
    QtMessageHandler m_oldHander;

    bool                     m_bRun;
    QStringList         m_listLog;
    QMutex                m_mutex;
    QWaitCondition  m_wait;

    int                         m_iTimerId;     //检测文件名的定时器
    int                         m_iDay;           //将文件按天分段
};

#endif // MLOG_H
