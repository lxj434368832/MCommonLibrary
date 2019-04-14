#ifndef MLOG_H
#define MLOG_H

/****************************************************************
 * function: ��־�ļ�
 * 1��pro�ļ������ DEFINES += QT_MESSAGELOGCONTEXT
 * 2����mai����QApplication ���MLog log
 * 3�������Ҫ���������̨����pro��CONFIG += console
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
    void InitLog();
    static void MessageOutput(QtMsgType, const QMessageLogContext&,const QString&);

private:
    virtual void run() override;
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
};

#endif // MLOG_H
