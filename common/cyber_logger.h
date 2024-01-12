/**
 * @file cyber_logger.h
 * @brief cyber_logger
 * @details
 * @author jiang.qi (jiang.qi4@byd.com)
 * @version 1.0.0
 * @date 2023/02/09 17:30:00
 * @copyright Copyright (c) 2023 比亚迪股份有限公司
 */
#ifndef _CYBER_LOGGER_H_
#define _CYBER_LOGGER_H_

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <mutex>

#define LOG_MAX_SIZE 50 * 1024 * 1024 // 50MB

class cyber_logger
{
private:
    std::mutex log_mtx;
    std::stringstream logfile;
    std::stringstream logfilebak;
    cyber_logger();

    // 拷贝构造函数
    cyber_logger(cyber_logger const &) = default;

    // 移动构造函数
    cyber_logger(cyber_logger &&) = default;

    // 复制赋值操作符
    cyber_logger &operator=(const cyber_logger &) = default;

    // 移动赋值操作符
    cyber_logger &operator=(cyber_logger &&) = default;

    virtual ~cyber_logger();
    std::string getCurrentTime(void);

public:
    static cyber_logger &getInstance(void);

    bool log_init(void);

    void log_print(const char *file, const int &fileLine, const char *func, const int &level, const char *fmt, ...);
};

#endif //  _cyber_logger_H_