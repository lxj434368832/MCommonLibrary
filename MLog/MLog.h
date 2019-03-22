#ifndef MLOG_H
#define MLOG_H

/****************************************************************
 * function: 日志文件
 * 1、pro文件中添加 DEFINES += QT_MESSAGELOGCONTEXT
 * 2、在mai函数QApplication 后加MLog log
 * 3、如果需要输出到控制台，在pro加CONFIG += console
****************************************************************/

#include <QMutex>
#include <QString>
#include <QtDebug>

//#pragma execution_character_set("utf-8")

class MLog
{
public:
    MLog();
    ~MLog();
    void InitLog();
    static void MessageOutput(QtMsgType, const QMessageLogContext&,const QString&);

private:
    static MLog* s_instance;
    QMutex m_mutex;
    QString m_qstrLogPath;
    QtMessageHandler m_oldHander;
};

#endif // MLOG_H
