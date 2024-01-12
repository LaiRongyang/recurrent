/**
* @file logger.h
* @brief logger
* @details
* @author jiang.qi (jiang.qi4@byd.com)
* @version 1.0.0
* @date 2023/02/09 17:30:00
* @copyright Copyright (c) 2023 比亚迪股份有限公司
*/
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <cstdarg>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <mutex>

#define LOG_MAX_SIZE 50 * 1024 * 1024 // 50MB
/*
** 日志级别
*/
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_FATAL 4

#ifdef CYBER_DI
#include "server_logger.h"
#else
#define CYBERLOG_INFO(fmt, args...) logger::getInstance().log_print(__FILE__, __LINE__, __FUNCTION__, LOG_INFO, (fmt), ##args)
#define CYBERLOG_DEBUG(fmt, args...) logger::getInstance().log_print(__FILE__, __LINE__, __FUNCTION__, LOG_DEBUG, (fmt), ##args)
#define CYBERLOG_WARN(fmt, args...) logger::getInstance().log_print(__FILE__, __LINE__, __FUNCTION__, LOG_WARN, (fmt), ##args)
#define CYBERLOG_ERROR(fmt, args...) logger::getInstance().log_print(__FILE__, __LINE__, __FUNCTION__, LOG_ERROR, (fmt), ##args)
#define CYBERLOG_FATAL(fmt, args...) logger::getInstance().log_print(__FILE__, __LINE__, __FUNCTION__, LOG_FATAL, (fmt), ##args)
#endif

class logger
{
private:
    bool isRuning;

    logger();

    // 拷贝构造函数
    logger(logger const &) = default;

    // 移动构造函数
    logger(logger &&) = default;

    // 复制赋值操作符
    logger &operator=(const logger &) = default;

    // 移动赋值操作符
    logger &operator=(logger &&) = default;

    ~logger();

    bool log_init(void);

    std::mutex log_mtx;
    std::stringstream logfile;
    std::stringstream logfilebak;


    std::string getCurrentTime(void);

public:
    static logger &getInstance(void);

    /**
     * @brief 启动日志输出线程
     * @param[in]
     * @param[out]
     * @return
     */
    bool logOutStart(void);

    void log_print(const char *file, const int &fileLine, const char *func, const int &level, const char *fmt, ...);
};

#endif //  _LOGGER_H_