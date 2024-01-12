#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <thread>
#include "security_server.h"
#include "./../common/message.h"
#include "../common/logger.h"

CSecurityServer::CSecurityServer(/* args */)
{
    m_pThread = nullptr;
    m_nSessionId = 0;
    m_bMonitor = false;
}

CSecurityServer::~CSecurityServer()
{
    if (nullptr != m_pThread)
    {
        delete m_pThread;
        m_pThread = nullptr;
    }
}

std::uint32_t CSecurityServer::ExecuteCreate(LPREQHEAD lpReq)
{
    return COM_OK;
}

std::uint32_t CSecurityServer::ExecuteDestory(LPREQHEAD lpReq)
{
    CSecuritySession *pServer = GetServer(lpReq->msg_head.session_id);
    if (nullptr != pServer)
    {
        if (!Remove(pServer))
        {
            return COM_SOFTWARE_ERROR;
        }
    }

    return COM_OK;
}

std::uint32_t CSecurityServer::Execute(LPREQHEAD lpReq)
{
    CYBERLOG_INFO("cmd id: %d", lpReq->msg_head.cmd_id);
    std::vector<uint8_t> resp;
    if ((EN_MSG_TYPE_CREATE == lpReq->msg_head.msg_type) ||
             (EN_CMD_ID_CREATE == lpReq->msg_head.cmd_id))
    {
        resp.resize(sizeof(RESPHEAD));
        (reinterpret_cast<LPRESPHEAD>(resp.data()))->data_len = 0;
        (reinterpret_cast<LPRESPHEAD>(resp.data()))->error_code = ExecuteCreate(lpReq);
    }
    else if (EN_MSG_TYPE_DESTORY == lpReq->msg_head.msg_type 
    || (EN_CMD_ID_DESTORY == lpReq->msg_head.cmd_id))
    {
        return ExecuteDestory(lpReq);
    }
    else
    {
        std::int32_t nBufLen = MSG_MAX_LENGTH - static_cast<int32_t>(sizeof(RESPHEAD));
        resp.resize(MSG_MAX_LENGTH);
        LPRESPHEAD lppResp = reinterpret_cast<LPRESPHEAD>(resp.data());
        lppResp->data_len = nBufLen;

        CSecuritySession *pServer = GetServer(lpReq->msg_head.session_id);
        if (nullptr != pServer)
        {
            uint32_t error_code = pServer->ReqExecute(lpReq, resp);
            lppResp = reinterpret_cast<LPRESPHEAD>(resp.data());
            if (COM_OK != error_code)
            {
                lppResp->error_code = error_code;
                lppResp->data_len = 0U;
            }
        }
        else
        {
            CYBERLOG_ERROR("CSecuritySession failed.");
            lppResp->data_len = 0U;
            lppResp->error_code = COM_SOFTWARE_ERROR;
        }
    }

    LPRESPHEAD lpResp = reinterpret_cast<LPRESPHEAD>(resp.data());
    lpResp->msg_head.msg_type = EN_MSG_TYPE_RESP;
    lpResp->msg_head.channel_id = lpReq->msg_head.channel_id;
    lpResp->msg_head.session_id = lpReq->msg_head.session_id;
    lpResp->msg_head.req_id = lpReq->msg_head.req_id;
    lpResp->msg_head.cmd_id = lpReq->msg_head.cmd_id;
    lpResp->msg_head.msg_size = lpResp->data_len + sizeof(RESPHEAD);

    return Send(lpResp, lpResp->msg_head.msg_size);
}

bool CSecurityServer::massageLoop()
{
    while (m_bMonitor)
    {
        CDataMgr *req;
        if (m_msgQueue.wait(req, 500))
        {
            LPREQHEAD lpReq = reinterpret_cast<LPREQHEAD>(req->data);
            if (!Execute(lpReq))
            {
                // break;
            }
            delete req;
        }
    }
    CYBERLOG_INFO("massageLoop end!");

    return true;
}

CSecuritySession *CSecurityServer::GetServer(std::int32_t nSession)
{
    std::unique_lock<std::mutex> lck(m_mtxServerList);
    CSecuritySession *pServer = nullptr;
    for (std::list<CSecuritySession *>::iterator it = m_lstServer.begin(); it != m_lstServer.end(); ++it)
    {
        if ((*it)->GetSessionId() == nSession)
        {
            pServer = *it;
            break;
        }
    }

    return pServer;
}

bool CSecurityServer::stopService()
{
    m_bMonitor = false;
    if (nullptr != m_pThread)
    {
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }

    return true;
}

void CSecurityServer::stopMonitor()
{
    m_bMonitor = false;
}

bool CSecurityServer::StartMonitorThread()
{
    if (nullptr == m_pThread)
    {
        m_bMonitor = true;
        m_pThread = new std::thread([&]()
                                    { this->MonitorEvent(); 
                                    });
        if (nullptr == m_pThread)
        {
            return false;
        }
    }

    return true;
}

bool CSecurityServer::Insert(CSecuritySession *pServer)
{
    if (nullptr == pServer)
    {
        return false;
    }

    std::unique_lock<std::mutex> lck(m_mtxServerList);

    bool bFind = false;
    for (std::list<CSecuritySession *>::iterator it = m_lstServer.begin(); it != m_lstServer.end(); ++it)
    {
        if (pServer->GetSessionId() == (*it)->GetSessionId())
        {
            bFind = true;
            break;
        }
    }
    if (!bFind)
    {
        m_lstServer.push_back(pServer);
    }

    return true;
}

bool CSecurityServer::Remove(CSecuritySession *pServer)
{
    std::unique_lock<std::mutex> lck(m_mtxServerList);
    for (auto it = m_lstServer.begin(); it != m_lstServer.end(); ++it)
    {
        if (*it == pServer)
        {
            delete pServer;
            m_lstServer.erase(it);
            break;
        }
    }

    return true;
}

std::uint32_t CSecurityServer::HandleComErrCode(std::uint32_t nErr)
{
    // 将通信层错误码COM_XXX转化成客户段错误码ERR_CODE_XXX
    // 因为目前两边定义一致，可以直接相等
    return nErr;
}


