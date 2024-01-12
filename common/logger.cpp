/**
* @file logger.cpp
* @brief logger
* @details
* @author jiang.qi (jiang.qi4@byd.com)
* @version 1.0.0
* @date 2023/02/09 17:30:00
* @copyright Copyright (c) 2023 比亚迪股份有限公司
*/
#include "logger.h"
#include "cyber_logger.h"

logger::logger()
{
    isRuning = false;
}

logger::~logger()
{
    isRuning = false;
}

logger &logger::getInstance(void)
{
    static logger logserver;
    return logserver;
}

bool logger::logOutStart(void)
{
    if (isRuning)
    {
        return false;
    }
    isRuning = true;
    if (log_init())
    {
         return true;
    }
    else
    {
        return false;
    }
}

bool logger::log_init(void)
{
#ifdef CYBER_DI
    return server_logger::getInstance().log_init();
#else
    return cyber_logger::getInstance().log_init();
#endif
}

void logger::log_print(const char *file, const int &fileLine, const char *func, const int &level, const char *fmt, ...)
{
    cyber_logger::getInstance().log_print(file, fileLine, func, level, fmt);
}