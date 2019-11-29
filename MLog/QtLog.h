#pragma once
/*******************************************************************
* @��Ȩ��Ϣ��
* @�ļ����ƣ�LogFile.h
* @ժ    Ҫ����־��������ͷ�ļ�
* @��    �ߣ�������
* @��ǰ�汾��1.0.0
* @��    �ڣ�2019��5��21��
* @��    ע���̰߳�ȫ��־��
*******************************************************************/

#include <sstream>

/*
enum class ELogLevel
{
    ELL_UNKNOWN,
    ELL_INFO,
    ELL_DEBUG,
    ELL_WARN,
    ELL_ERROR
};*/

class QtLog
{
public:
    QtLog();
    ~QtLog();
    void AddLog(char* format, ...);

    template <typename T>
    inline QtLog& operator<<(const T& log)
    {
        m_strStream << log;
        return *this;
    }

    inline QtLog& operator<<(std::ostream& (*log)(std::ostream&))
    {
        m_strStream << log;
        return *this;
    }

private:
    char                    m_szLineLog[512];
    std::stringstream m_strStream;
};

#define Log() (QtLog())<< "["<< __FUNCTION__<<":"<<__LINE__<<"]"
#define logm() Log()<<"<Info> "
#define logd() Log()<<"<Debug> "
#define logw() Log()<<"<Warn> "
#define loge() Log()<<"<Error> "


//���µ��÷�ʽ����Ϣ���Ȳ��ܳ���512
#define LOGM(format, ...)  do{  QtLog log; \
    log.AddLog("[%s:%d]<Info> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGD(format, ...)  do{  QtLog log; \
    log.AddLog("[%s:%d]<Debug> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGW(format, ...)  do{  QtLog log; \
    log.AddLog("[%s:%d]<Warn> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGE(format, ...)  do{  QtLog log; \
    log.AddLog("[%s:%d]<Error> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)

