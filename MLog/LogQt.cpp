#include "LogQt.h"
#include<mutex>
#include<QDir>
#include<QTime>
#include <QDebug>
/*
WriteLog: 写日志函数---如果日志文件超过指定大小 则从0开始
*/

int g_nLogLevel = LOG_DEBUG;
int g_nLogFileSize = MAX_LOG_FILE_SIZE;

char g_szFileName[LOG_FILE_PATH] = { 0 };
std::recursive_mutex g_cs;
std::recursive_mutex g_avcs;
bool g_bInit=false;

void InitLog()
{
    QString path;
    QDir dir;
    path=dir.currentPath();
    sprintf(g_szFileName,"%s//DATAEXCHANGE.log", path.toLocal8Bit().data());

    g_nLogLevel = LOG_RTM;
    g_nLogFileSize = MAX_LOG_FILE_SIZE;
    g_bInit = true;
}
void SetLogParam(int nLogLevel, int nLogFileSize)
{
    g_nLogLevel = nLogLevel;
    g_nLogFileSize = nLogFileSize;
}

void LOG(LOG_TYPE t, char* format, ...)
{
    char szMsg[MAX_LOG_ROW_SIZE] = { 0 };
    if( false == g_bInit ) InitLog();
    va_list arg;
    va_start(arg, format);
    vsprintf(szMsg, format, arg);
    va_end(arg);

    if (t > g_nLogLevel) return;

    g_cs.lock();

    char uchStrTime[LOG_FILE_PATH] = { 0 };
    //获取当前时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString strTime = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");

    /* 文件处理 */
    FILE *pFile = nullptr;
   pFile = fopen( g_szFileName, "a+");
    if (pFile == nullptr)
    {
        goto END_LOG;
    }

    //限制大小
    int dwFileSize = 0;
    fseek(pFile, 0, SEEK_END);
    dwFileSize = ftell(pFile);
    if (dwFileSize > g_nLogFileSize)
    {
        fclose(pFile);
        pFile = nullptr;
        pFile = fopen( g_szFileName, "w");
        dwFileSize = 0;
        if (pFile == nullptr)
        {
            goto END_LOG;
        }
    }


    fprintf(pFile, "%s\t", strTime.toStdString().c_str());
    switch (t)
    {
        case LOG_NONE:
            break;
        case LOG_ERROR:
            fprintf(pFile, "[ERR]");
            break;
        case LOG_RTM:
            fprintf(pFile, "[RTM]");
            break;
        case LOG_DEBUG:
            fprintf(pFile, "[DBG]");
            break;
    }

    //处理参数
    va_list arg;
    va_start(arg, format);
    vfprintf(pFile, format, arg);
    fprintf(pFile, "\n");
    fflush(pFile);
    va_end(arg);

    fclose(pFile);

END_LOG:
    g_cs.unlock();
}
