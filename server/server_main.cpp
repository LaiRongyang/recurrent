#include <thread>
#include "server_manager.h"
#include "./../common/logger.h"

/* Quit flag. */

std::int32_t main(std::int32_t argc, char **argv)
{
    logger::getInstance().logOutStart();
    CYBERLOG_INFO("--------Start security server--------");
    do
    {
        CYBERLOG_INFO("--------init server--------");
        if (!CServerManager::getInstance().initService())
        {
            CYBERLOG_ERROR("initService failed!");
            break;
        }

        CYBERLOG_INFO("--------start server--------");
        if (!CServerManager::getInstance().startService())
        {
            CYBERLOG_ERROR("startService failed!");
            break;
        }
        CYBERLOG_INFO("--------massage loop--------");
        if (!CServerManager::getInstance().massageLoop())
        {
            CYBERLOG_ERROR("massageLoop failed!");
        }

        CYBERLOG_INFO("--------stop server--------");
        if (!CServerManager::getInstance().stopService())
        {
            CYBERLOG_ERROR("stopService failed!");
        }
    } while (0);


    CYBERLOG_INFO("--------End security server--------");
    return 0;
}
