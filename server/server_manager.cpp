#include <unistd.h>
#include <sys/epoll.h>
#include "server_manager.h"
#include "security_server.h"
#include "socket_server.h"
#include "./../common/logger.h"
#include "security_impl.h"


CServerManager::CServerManager(/* args */)
{
    m_pCom = new CSocketServer();
}

CServerManager::~CServerManager()
{
}

CServerManager &CServerManager::getInstance()
{
    static CServerManager instance;
    return instance;
}

bool CServerManager::initService()
{
    if (COM_OK != CSecurityImpl::getInstance().Open())
    {
        return false;
    }

    return true;
}

bool CServerManager::startService()
{
    return m_pCom->startService();
}

bool CServerManager::massageLoop()
{
    return m_pCom->massageLoop();
}

bool CServerManager::stopService()
{
    CYBERLOG_INFO("stopService!");
    m_pCom->stopService();
    CSecurityImpl::getInstance().Close();

    return true;
}

void CServerManager::stopMonitor()
{
    m_pCom->stopMonitor();
}
