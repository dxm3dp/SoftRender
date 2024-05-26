#include "Logger.h"

BEGIN_NAMESPACE(SoftRender)

LogFunc Logger::s_logFunc = nullptr;
LogLevel Logger::s_minLevel = LogLevel::INFO;
char Logger::s_buf[MAX_LOG_LENGTH] = {};

void Logger::setLogFunc(LogFunc func)
{
    s_logFunc = func;
}

void Logger::setMinLogLevel(LogLevel level)
{
    s_minLevel = level;
}

void Logger::log(LogLevel level, const char * file, int line, const char* msg, ...)
{
    if (level < s_minLevel)
        return;

    va_list argPtr;
    va_start(argPtr, msg);
    vsnprintf(s_buf, MAX_LOG_LENGTH - 1, msg, argPtr);
    va_end(argPtr);
    s_buf[MAX_LOG_LENGTH - 1] = '\0';

    if (s_logFunc != nullptr)
    {
        s_logFunc(level, s_buf);
        return;
    }

    switch (level)
    {
#ifdef LOG_SOURCE_LINE
    case LogLevel::INFO:
        fprintf(stdout, "[INFO] %s:%d %s\n", file, line, s_buf);
        break;
    case LogLevel::DEBUG:
        fprintf(stdout, "[DEBUG] %s:%d %s\n", file, line, s_buf);
        break;
    case LogLevel::WARNING:
        fprintf(stdout, "[WARNING] %s:%d %s\n", file, line, s_buf);
        break;
    case LogLevel::ERROR:
        fprintf(stderr, "[ERROR] %s:%d %s\n", file, line, s_buf);
        break;
#else
    case LogLevel::INFO:
        fprintf(stdout, "[INFO] %s\n", s_buf);
        break;
    case LogLevel::DEBUG:
        fprintf(stdout, "[DEBUG] %s\n", s_buf);
        break;
    case LogLevel::WARNING:
        fprintf(stdout, "[WARNING] %s\n", s_buf);
        break;
    case LogLevel::ERROR:
        fprintf(stderr, "[ERROR] %s\n", s_buf);
        break;
#endif
    }
    fflush(stdout);
    fflush(stderr);
}

END_NAMESPACE(SoftRender)