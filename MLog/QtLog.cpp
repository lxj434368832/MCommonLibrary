#include "QtLog.h"
#include <QString>
#include <QtDebug>

#ifdef WIN32
#include <Windows.h>
#endif

QtLog::QtLog()
{
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    sprintf_s(m_szLineLog, "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
    m_strStream << m_szLineLog;
}

QtLog::~QtLog()
{
    qInfo().noquote() << QString::fromLocal8Bit(m_strStream.str().data());
}

void QtLog::AddLog(char* format, ...)
{
    va_list argList;
    va_start(argList, format);
    vsprintf_s(m_szLineLog, format, argList);
    va_end(argList);

    m_strStream << m_szLineLog;
}
