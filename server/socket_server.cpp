#include "socket_server.h"
#include "./../common/my_socket.h"
#include "./../common/logger.h"
#include "./../common/message.h"
#include "security_server.h"
#include <string.h>
#include <sys/epoll.h>

#define MAX_EVENT_COUNT 20
CSocketServer::CSocketServer(/* args */)
    : CSecurityServer()
    , m_pSocket(nullptr)
{
    m_nEpoll = 0;
}

CSocketServer::~CSocketServer()
{
    if ((nullptr != m_pSocket) && m_pSocket->GetState())
    {
        m_pSocket->Close();
        delete m_pSocket;
        m_pSocket = nullptr;
    }
}

bool CSocketServer::startService()
{
    m_nEpoll = epoll_create1(0);
    m_pSocket = new CSocket();
    std::string strAddr{"/tmp/css.sock"};
    if ((nullptr == m_pSocket) || !(m_pSocket->Create(strAddr)))
    {
        CYBERLOG_ERROR("create socket failed!");
        return false;
    }

    return StartMonitorThread();
}

std::uint32_t CSocketServer::Send(void *pData, std::int32_t nLen)
{

    CSocket *pSocket = m_mapSocket[((LPMSGHEAD)pData)->session_id];
    if (nullptr == pSocket)
    {
        CYBERLOG_ERROR("not find socket.");
        return COM_COMMUNICATION_FAILURE;
    }

    ssize_t msg_len = pSocket->Send(pData, nLen);
    if (msg_len <= 0)
    {
        CYBERLOG_ERROR("msg len: %ld", msg_len);
        pSocket->Close();
        return COM_INVALID_PARAMETER;
    }

    return COM_OK;
}

void CSocketServer::MonitorEvent()
{
    std::int32_t listenfd = m_pSocket->GetSocketId();
    struct epoll_event ev;
    //设置与要处理的事件相关的文件描述符
    ev.data.fd = listenfd;
    //设置要处理的事件类型
    ev.events = EPOLLIN;
    epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, listenfd, &ev);

    std::int32_t connfd = 0;
    std::int32_t sockfd = 0;
    std::int32_t nfds = 0;
    struct epoll_event events[MAX_EVENT_COUNT];

    while (m_bMonitor)
    {
        const std::int32_t timeout = 500;
        nfds = epoll_wait(m_nEpoll, events, MAX_EVENT_COUNT, timeout);
        //处理所发生的所有事件
        // CYBERLOG_DEBUG("epoll: %d, nfds: %d", m_nEpoll, nfds);

        for (std::int32_t i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listenfd) //如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            {
                if (!(m_pSocket->Accept(connfd)) || (connfd < 0))
                {
                    continue;
                }
                CSocket *socket = new CSocket(connfd);
                if (nullptr != socket)
                {
                    // struct epoll_event ev;
                    //设置与要处理的事件相关的文件描述符
                    ev.data.fd = connfd;
                    //设置要处理的事件类型
                    // ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, connfd, &ev);
                }

                CSecuritySession *pServer = new CSecuritySession(++m_nSessionId);
                if (!Insert(pServer))
                {
                    socket->Close();
                    delete socket;
                    continue;
                }
                m_mapSocket[m_nSessionId] = socket;
            }
            else if (events[i].events & EPOLLIN) //如果是已经连接的用户，并且收到数据，那么进行读入。
            {
                sockfd = events[i].data.fd;
                if (sockfd < 0)
                {
                    continue;
                }
                m_mtxServerList.lock();
                std::map<std::int32_t, CSocket *> temp = m_mapSocket;
                m_mtxServerList.unlock();

                for (auto it = temp.begin(); it != temp.end(); ++it)
                {
                    CSocket *pSocket = it->second;
                    std::int32_t nSocket = pSocket->GetSocketId();
                    if (nSocket == sockfd)
                    {
                        CDataMgr *req = new CDataMgr();
                        req->MallocData(MSG_MAX_LENGTH);
                        void *pBuf = req->data;
                        std::int32_t nMsgLen = pSocket->Recv(pBuf, MSG_MAX_LENGTH);
                        if (nMsgLen > 0)
                        {
                            LPREQHEAD lpReq = reinterpret_cast<LPREQHEAD>(pBuf);
                            if ((EN_MSG_TYPE_CREATE == lpReq->msg_head.msg_type) ||
                                (EN_CMD_ID_CREATE == lpReq->msg_head.cmd_id))
                            {
                                lpReq->msg_head.session_id = it->first;
                            }
                            m_msgQueue.push(req);
                        }
                        else
                        {
                            CYBERLOG_ERROR("%d read data failed!", pSocket->GetSocketId());
                            events[i].data.fd = -1;
                        }
                        break;
                    }
                }
            }
            else
            {
                
            }
        }
    }
    CYBERLOG_INFO("Monitor end!");
}

std::uint32_t CSocketServer::ExecuteCreate(LPREQHEAD lpReq)
{
    return COM_OK;
}

std::uint32_t CSocketServer::ExecuteDestory(LPREQHEAD lpReq)
{
    std::uint32_t nRet = CSecurityServer::ExecuteDestory(lpReq);
    if (COM_OK != nRet)
    {
        return nRet;
    }

    CSocket *pSocket = m_mapSocket[lpReq->msg_head.session_id];
    if (nullptr != pSocket)
    {
        delete pSocket;
        m_mapSocket.erase(lpReq->msg_head.session_id);
    }

    return COM_OK;
}
