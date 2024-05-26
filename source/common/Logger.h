#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "cpplang.h"

#include <cstdarg>
#include <cstdio>

BEGIN_NAMESPACE(SoftRender)

#define LOGI(...) SoftRender::Logger::log(SoftRender::LogLevel::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOGD(...) SoftRender::Logger::log(SoftRender::LogLevel::DEBUG,__FILE__, __LINE__, __VA_ARGS__)
#define LOGW(...) SoftRender::Logger::log(SoftRender::LogLevel::WARNING,__FILE__, __LINE__, __VA_ARGS__)
#define LOGE(...) SoftRender::Logger::log(SoftRender::LogLevel::ERROR,__FILE__, __LINE__, __VA_ARGS__)

static constexpr int MAX_LOG_LENGTH = 1024;

typedef void (*LogFunc)(int level, const char *msg);

enum LogLevel
{
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

class Logger
{
public:
    static void setLogFunc(LogFunc func);
    static void setMinLogLevel(LogLevel level);
    static void log(LogLevel level, const char * file, int line, const char* msg, ...);

private:
    static LogFunc s_logFunc;
    static LogLevel s_minLevel;

    static char s_buf[MAX_LOG_LENGTH];
};

END_NAMESPACE(SoftRender)

#endif // _LOGGER_H_