#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <unistd.h>
#include "my_socket.h"
#include "logger.h"

#define SEND_BUF_SIZE (40 * 1024)

CSocket::CSocket(/* args */)
    : m_hSocket(-1), m_nSendBufSize(0)
{
}

CSocket::CSocket(std::int32_t hSocket)
    : m_hSocket(hSocket), m_nSendBufSize(0)
{
}

CSocket::~CSocket()
{
    if (!Close())
    {
        CYBERLOG_WARN("Close failed.");
    }
}

bool CSocket::Create(const std::string &strPath)
{
    m_hSocket = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (m_hSocket < 0)
    {
        CYBERLOG_ERROR("create local socket failed! m_hSocket = %d", m_hSocket);
        return false;
    }
    CYBERLOG_INFO("socket id: %d", m_hSocket);

    if(strPath.empty())
    {
        CYBERLOG_ERROR("path is empty!");
        return false;
    }
    unlink(strPath.c_str());

    struct sockaddr_un srv_addr;
    srv_addr.sun_family = AF_UNIX;
    if (nullptr == strcpy(srv_addr.sun_path, strPath.c_str()))
    {
        CYBERLOG_WARN("strcpy failed.");
    }
    // srv_addr.sun_path[0] = 0;
    // std::uint32_t server_len = strPath.length() + offsetof(sockaddr_un, sun_path);

    std::int32_t ret = bind(m_hSocket, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0)
    {
        CYBERLOG_ERROR("socket bind failed! ret = %d", ret);
        if (-1 == close(m_hSocket))
        {
            CYBERLOG_WARN("close socket %d failed.", m_hSocket);
        }
        m_hSocket = -1;
        return false;
    }
    ret = listen(m_hSocket, 100);
    if (ret < 0)
    {
        CYBERLOG_ERROR("socket listen failed! ret = %d", ret);
        if (-1 == close(m_hSocket))
        {
            CYBERLOG_WARN("close socket %d failed.", m_hSocket);
        }
        m_hSocket = -1;
        return false;
    }

    return true;
}

bool CSocket::Accept(std::int32_t &hSocketClient)
{
    if (m_hSocket < 0)
    {
        return false;
    }

    hSocketClient = accept(m_hSocket, (struct sockaddr *)nullptr, nullptr);
    if (hSocketClient < 0)
    {
        return false;
    }
    linger sLinger{1, 0};
    setsockopt(hSocketClient, SOL_SOCKET, SO_LINGER, &sLinger, sizeof(linger));

    return true;
}

bool CSocket::Connect(const std::string &strPath)
{
    m_hSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_hSocket < 0)
    {
        CYBERLOG_WARN("ERRNO ERROR : %d !", errno);
        return false;
    }

    if (strPath.empty())
    {
        CYBERLOG_ERROR("path is empty!");
        return false;
    }
    struct sockaddr_un srv_addr;
    srv_addr.sun_family = AF_UNIX;
    if (nullptr == strcpy(srv_addr.sun_path, strPath.c_str()))
    {
        CYBERLOG_WARN("strcpy failed.");
        return false;
    }
    // srv_addr.sun_path[0] = 0;
    // std::uint32_t server_len = strPath.length() + offsetof(sockaddr_un, sun_path);
    // if(server_len == 0U)
    // {
    //     CYBERLOG_WARN("SERVER_LEN ERROR!");
    // }
    std::int32_t ret = connect(m_hSocket, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (ret < 0)
    {
        if (-1 == close(m_hSocket))
        {
            CYBERLOG_WARN("close socket %d failed.", m_hSocket);
        }
        return false;
    }

    return true;
}

bool CSocket::Send(void *pBuf, std::int32_t nLen)
{
    if (m_hSocket < 0)
    {
        CYBERLOG_ERROR("socket is invald.");
        return false;
    }

    if (m_nSendBufSize > 0)
    {
        // 发送缓冲区将满时等待数据减少后再继续，否则可能导致卡死
        std::int32_t nValue{0};
        std::uint32_t u_m_nSendBufSize = m_nSendBufSize;
        ioctl(m_hSocket, SIOCOUTQ, &nValue);
        if ((nValue > 0) && ((nValue + nLen) > m_nSendBufSize))
        {
            std::int32_t nThreshold = u_m_nSendBufSize >> 1;
            std::uint32_t nBegin = GetTickCount();
            std::int32_t sleepTime{1000};
            std::uint32_t ticketcount{5000U};
            while (true)
            {
                if (ioctl(m_hSocket, SIOCOUTQ, &nValue) < 0)
                {
                    return false;
                }

                if (nValue <= nThreshold)
                {
                    break;
                }

                if ((GetTickCount() - nBegin) > ticketcount)
                {
                    return false;
                }

                usleep(sleepTime);
            }
        }
    }
     CYBERLOG_INFO("execute thread: send start ");
     int send_len=send(m_hSocket, pBuf, nLen, MSG_NOSIGNAL);
     CYBERLOG_INFO("execute thread: send end .send %d bytes",send_len);
    if ( send_len< 0)
    {
        return false;
    }

    return true;
}

std::int32_t CSocket::Recv(void *pBuf, std::int32_t nLen)
{
    if (m_hSocket < 0)
    {
        CYBERLOG_ERROR("socket enevt thread: socket is invald.socket id: %d", m_hSocket);
        return 0;
    }
    CYBERLOG_INFO("socket enevt thread: recv start");
    ssize_t  ret = recv(m_hSocket, pBuf, nLen, MSG_NOSIGNAL);
    CYBERLOG_INFO("socket enevt thread: recv end");
    return ret;
}

bool CSocket::Close(void)
{
    if (m_hSocket > 0)
    {
        //shutdown(m_hSocket, SHUT_RDWR);
        if (-1 == close(m_hSocket))
        {
            CYBERLOG_WARN("close socket %d failed.", m_hSocket);
        }
        m_hSocket = -1;
    }
    
    return true;
}

bool CSocket::GetState(void)
{
    if (-1 == m_hSocket)
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::int32_t CSocket::GetSocketId(void)
{
    return m_hSocket;
}

void CSocket::UpdateSendBufSize(void)
{
    std::uint32_t nLen = sizeof(m_nSendBufSize);
    getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, &m_nSendBufSize, &nLen);

    if (m_nSendBufSize < SEND_BUF_SIZE)
    {
        m_nSendBufSize = SEND_BUF_SIZE;
        setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, &m_nSendBufSize, sizeof(m_nSendBufSize));
        getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, &m_nSendBufSize, &nLen);
    }
}

std::uint32_t CSocket::GetTickCount(void)
{
    struct timeb t;
    ftime(&t);
    std::uint32_t sec_element{24 * 3600};
    std::uint32_t element{1000U};
    std::uint32_t sec = t.time % sec_element;
    return (sec * element) + t.millitm;
}
