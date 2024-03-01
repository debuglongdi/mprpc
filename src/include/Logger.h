#pragma once
#include "lockqueue.h"
#include "noncopyable.h"

namespace
{
enum LogLevel
{
    INFO,  //普通信息
    DEBUG, //debug
    ERROR, //error
    FATAL, //致命错误
};
}


namespace mprpc
{
/// @brief 日志系统
class Logger : noncopyable
{
public:
    void setLogLevel(LogLevel lv)
    { loglevel_ = lv; }

    //写日志
    void Log(std::string msg);
    static Logger& getInstance();
    ~Logger();
private:
    Logger();

    bool isExit() const
    { return isExit_; }
    void setIsExit( bool ex)
    { isExit_ = ex; }
    int loglevel_;//日志记录级别
    bool isExit_;
    LockQueue<std::string> lckQueue_;//日志缓冲队列
    
};

} // namespace mprpc


// 定义宏创建

#define LOG_INFO(logmsgformat, ...) \
    do \
    { \
        mprpc::Logger &logger =  mprpc::Logger::getInstance(); \
        logger.setLogLevel(INFO); \
        char str[1024] = {0}; \
        snprintf(str,1024,logmsgformat, ##__VA_ARGS__); \
        logger.Log(str); \
    } while (0)

#define LOG_DEBUG(logmsgformat, ...) \
    do \
    { \
        mprpc::Logger &logger =  mprpc::Logger::getInstance(); \
        logger.setLogLevel(DEBUG); \
        char str[1024] = {0}; \
        snprintf(str,1024,logmsgformat, ##__VA_ARGS__); \
        logger.Log(str); \
    } while (0)

#define LOG_ERROR(logmsgformat, ...) \
    do \
    { \
        mprpc::Logger &logger =  mprpc::Logger::getInstance(); \
        logger.setLogLevel(ERROR); \
        char str[1024] = {0}; \
        snprintf(str,1024,logmsgformat, ##__VA_ARGS__); \
        logger.Log(str); \
    } while (0)

#define LOG_FATAL(logmsgformat, ...) \
    do \
    { \
        mprpc::Logger &logger =  mprpc::Logger::getInstance(); \
        logger.setLogLevel(FATAL); \
        char str[1024] = {0}; \
        snprintf(str,1024,logmsgformat, ##__VA_ARGS__); \
        logger.Log(str); \
    } while (0)