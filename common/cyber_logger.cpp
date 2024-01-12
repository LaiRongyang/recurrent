/**
 * @file cyber_logger.h
 * @brief cyber_logger
 * @details
 * @author jiang.qi (jiang.qi4@byd.com)
 * @version 1.0.0
 * @date 2023/02/09 17:30:00
 * @copyright Copyright (c) 2023 比亚迪股份有限公司
 */
#include "cyber_logger.h"
#include "logger.h"

static FILE *s_fp{nullptr};

cyber_logger::cyber_logger()
{
}

cyber_logger::~cyber_logger()
{
    if (s_fp != nullptr)
    {
        if (0 != fclose(s_fp))
        {
            printf("fclose failed.");
        }
    }
}

cyber_logger &cyber_logger::getInstance(void)
{
    static cyber_logger logserver;
    return logserver;
}



bool cyber_logger::log_init()
{
    // if (-1 == system("mkdir -p /dimw/log/security"))
    // {
    //     printf("%s%s(%d):mkdir command failed!\n", __FILE__, __FUNCTION__, __LINE__);
    //     return false;
    // }
    // else
    // {
    //     printf("%s%s(%d):mkdir command success\n", __FILE__, __FUNCTION__, __LINE__);
    // }

    // std::stringstream logfile;
    
    logfile << "/optdata/log/dimw/log/file/security/" << "security_client" << ".log";
    logfilebak << "/optdata/log/dimw/log/file/security/" << "security_client_bak" << ".log";
    if (nullptr == s_fp)
    {
        printf("log start success:%s\n", logfile.str().c_str());
        s_fp = fopen(logfile.str().c_str(), "a+");
        if (s_fp != nullptr)
        {
            setbuf(s_fp, nullptr);
        }
        else
        {
            printf("open log file:%s failed\n", logfile.str().c_str());
        }
    }

    // s_logLevel = level;
    return s_fp != nullptr;
}

void cyber_logger::log_print(const char *file, const int &fileLine, const char *func, const int &level, const char *fmt, ...)
{
    std::string slevel = "INFO";

    std::stringstream prefix;

    switch (level)
    {
    case LOG_DEBUG:
        slevel = "DEBUG";
        break;

    case LOG_INFO:
        slevel = "INFO";
        break;

    case LOG_WARN:
        slevel = "WARN";
        break;

    case LOG_ERROR:
        slevel = "ERROR";
        break;

    case LOG_FATAL:
        slevel = "FATAL";
        break;

    default:
        slevel = "INFO";
        break;
    }

    prefix << "[" << getCurrentTime() << "] [" << slevel << "] [" << file << ":" << fileLine << "] [" << func << "] ";
    prefix << fmt << "\n";
    va_list arglist;
    va_start(arglist, fmt);
    vprintf(prefix.str().c_str(), arglist);
    va_end(arglist);
    if (nullptr != s_fp)
    {
        std::unique_lock<std::mutex> lck(log_mtx);

        // 获取文件大小
        fseek(s_fp, 0, SEEK_END);
        long size = ftell(s_fp);

        // 如果文件大于10MB,清空原有内容
        if (size > LOG_MAX_SIZE)
        {
            fclose(s_fp);
            std::rename(logfile.str().c_str(), logfilebak.str().c_str());
            // remove(logfile.str().c_str());
            s_fp = fopen(logfile.str().c_str(), "a+");
        }

        va_start(arglist, fmt);
        vfprintf(s_fp, prefix.str().c_str(), arglist);
        va_end(arglist);
    }
    if (level == LOG_FATAL)
    {
        abort();
    }
}

std::string cyber_logger::getCurrentTime(void)
{
    std::stringstream log_time;
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_t tt = micros / (std::int64_t)(1000 * 1000);
    log_time << std::put_time(std::localtime(&tt), "%Y-%m-%d-%X.") << (micros % (1000 * 1000));
    return log_time.str();
}
