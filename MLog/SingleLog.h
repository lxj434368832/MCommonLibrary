#pragma once
/*******************************************************************
* @版权信息：
* @文件名称：LogFile.h
* @摘    要：日志操作定义头文件
* @作    者：明巧文
* @当前版本：1.0.0
* @日    期：2019年5月21日
* @备    注：线程安全日志类
*******************************************************************/

#include <sstream>

enum class ELogLevel
{
    ELL_UNKNOWN,
    ELL_INFO,
    ELL_DEBUG,
    ELL_WARN,
    ELL_ERROR,
    ELL_FATAL
};

class SingleLog
{
public:
    SingleLog(ELogLevel level);
    ~SingleLog();
    void AddLog(char* format, ...);

    template <typename T>
    inline SingleLog& operator<<(const T& log)
    {
        m_strStream << log;
        return *this;
    }

    inline SingleLog& operator<<(std::ostream& (*log)(std::ostream&))
    {
        m_strStream << log;
        return *this;
    }

private:
    ELogLevel       m_eLevel;
    char                    m_szLineLog[512];
    std::stringstream m_strStream;
};

#define logm() (SingleLog(ELogLevel::ELL_INFO))
#define logd() (SingleLog(ELogLevel::ELL_DEBUG))
#define logw() (SingleLog(ELogLevel::ELL_WARN))
#define loge() (SingleLog(ELogLevel::ELL_ERROR))
#define logf() (SingleLog(ELogLevel::ELL_FATAL))

//以下调用方式的消息长度不能超过256
#define LOGM(format, ...)  do{  SingleLog log(ELogLevel::ELL_INFO); \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGD(format, ...)  do{  SingleLog log(ELogLevel::ELL_DEBUG); \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGW(format, ...)  do{  SingleLog log(ELogLevel::ELL_WARN); \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGE(format, ...)  do{  SingleLog log(ELogLevel::ELL_ERROR); \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGF(format, ...)  do{  SingleLog log(ELogLevel::ELL_FATAL); \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
