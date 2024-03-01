#include "Logger.h"
#include <iostream>
using namespace mprpc;

Logger::Logger()
        :loglevel_(INFO),
        isExit_(false),
        lckQueue_()
{
    //启动专门的写日志线程
    std::thread writeLogTask([&](){

        while(!isExit())
        {
            //获取当前日期，然后取日志信息，写入相应的文件当中
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);
            char filename[128]={0};
            sprintf(filename,"%d-%d-%d-log.txt",
                                nowtm->tm_year+1900,
                                nowtm->tm_mon+1,
                                nowtm->tm_mday);
            FILE *pf = fopen(filename, "a+");
            if(pf == nullptr)
            {
                std::cout<<"open filename"<<filename<<" error\n";
                exit(EXIT_FAILURE);
            }
            std::string msg = lckQueue_.pop();

            //获取时间
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d=>",nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec);
            msg.insert(0, time_buf);
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });
    writeLogTask.detach();
}

Logger::~Logger()
{
    setIsExit(true);
}


//写日志
void Logger::Log(std::string msg)
{
    std::string head;
    switch (loglevel_)
    {
    case INFO:
        head = "[INFO] ";
        break;
    case DEBUG:
        head = "[DEBUG] ";
        break;
    case ERROR:
        head = "[ERROR] ";
        break;
    case FATAL:
        head = "[FATAL] ";
        break;
    default:
        head = "[INFO] ";
        break;
    }
    head +=msg;
    lckQueue_.push(head);
}

/// @brief 获取日志单例
/// @return 
Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}