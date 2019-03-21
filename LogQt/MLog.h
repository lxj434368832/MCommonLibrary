#ifndef MLOG_H
#define MLOG_H

/****************************************************************
 * function: ��־�ļ�
 * 1��pro�ļ������ DEFINES += QT_MESSAGELOGCONTEXT
 * 2����mai����QApplication ���MLog log
 * 3�������Ҫ���������̨����pro��CONFIG += console
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
