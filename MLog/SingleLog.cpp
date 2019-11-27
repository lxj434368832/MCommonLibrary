#include "SingleLog.h"
#include <QString>
#include <QtDebug>

#ifdef WIN32
#include <Windows.h>
#endif

SingleLog::SingleLog(ELogLevel level)
{
    m_eLevel = level;
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    sprintf_s(m_szLineLog, "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
    m_strStream << m_szLineLog;
}

SingleLog::~SingleLog()
{
    m_strStream << "\n";
    QString qstrLog = QString::fromLocal8Bit(m_strStream.str().data());

    switch (m_eLevel) {
    case ELogLevel::ELL_INFO:
        qInfo() << qstrLog;
        break;
    case ELogLevel::ELL_DEBUG:
        qDebug() << qstrLog;
        break;
    case ELogLevel::ELL_WARN:
        qWarning() << qstrLog;
        break;
    case ELogLevel::ELL_ERROR:
        qCritical() << qstrLog;
        break;
    case ELogLevel::ELL_FATAL:
        qFatal(qstrLog.toStdString().data()) ;
        break;
    default:
        qInfo() << qstrLog;
        break;
    }
}

void SingleLog::AddLog(char* format, ...)
{
    va_list argList;
    va_start(argList, format);
    vsprintf_s(m_szLineLog, format, argList);
    va_end(argList);

    m_strStream << m_szLineLog;
}
