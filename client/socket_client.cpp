#include "socket_client.h"
#include "./../common/my_socket.h"
#include "./../common/logger.h"
#include "./../common/message.h"
#include <string.h>
#define NAME_LEN 21

#define ERR_CODE_OK 0x00000000U                   // 成功
#define ERR_CODE_NO_DEVICE 0x00000001             // 无设备
#define ERR_CODE_COMMUNICATION_FAILURE 0x00000002 // 通信错误
#define ERR_CODE_EXECUTE_FAILURE 0x00000003       // 执行失败
#define ERR_CODE_INVALID_PARAMETER 0x00000004     // 无效参数
#define ERR_CODE_UNSUPPORT_COMMAND 0x00000005     // 不支持的命令
#define ERR_CODE_SOFTWARE_ERROR 0x00000006        // 软件错误
#define ERR_CODE_HARDWARE_ERROR 0x00000007        // 硬件错误
#define ERR_CODE_PROCEDURE_ERROR 0x00000008       // 程序错误，接口调用顺序不对
#define ERR_CODE_SPACE_NOT_ENOUGH 0x00000009      // 没有足够的空间
#define ERR_CODE_DATA_ERROR 0x0000000a            // 数据长度、格式等与与其不符
#define ERR_CODE_NO_POWER 0x0000000b              // 无权限
#define ERR_CODE_TIMEOUT 0x0000000c               // 超时错误
#define ERR_CODE_SYSTEM_ERROR 0x0000000d          // 系统错误
#define ERR_CODE_CONFIGURE_ERROR 0x0000000e       // 配置读取失败
#define ERR_CODE_POINTER_NULL 0x0000000f          // 指针为空

CSocketClient::CSocketClient(/* args */)
    : CSecurityClient()
    , m_pSocket(nullptr)
{
}

CSocketClient::~CSocketClient()
{
    DisconnectServer();
}

std::uint32_t CSocketClient::ConnectServer(void)
{
    m_pSocket = new CSocket();
    std::string strAddr{"/tmp/css.sock"};
    if ((nullptr == m_pSocket) || !m_pSocket->Connect(strAddr))
    {
        CYBERLOG_ERROR("socket connect failed!");
        return ERR_CODE_NO_DEVICE;
    }
    m_nChannelId = m_pSocket->GetSocketId();
    CYBERLOG_INFO("connect success, socket(%d).", m_nChannelId);
    m_bConnect = true;

    return ERR_CODE_OK;
}

std::uint32_t CSocketClient::DisconnectServer(void)
{
    if (m_bConnect && (nullptr != m_pSocket))
    {
        m_pSocket->Close();
        m_bConnect = false;
        delete m_pSocket;
        m_pSocket = nullptr;
    }

    return ERR_CODE_OK;
}

std::uint32_t CSocketClient::Send(void *pData, std::int32_t nLen)
{
    if (nLen > MSG_MAX_LENGTH)
    {
        return SendFile(pData, nLen);
    }

    ssize_t msg_len = m_pSocket->Send(pData, nLen);
    if (msg_len <= 0)
    {
        CYBERLOG_ERROR("msg len: %ld\n", msg_len);
        m_pSocket->Close();
        return ERR_CODE_INVALID_PARAMETER;
    }

    return ERR_CODE_OK;
}

std::uint32_t CSocketClient::Recv(std::vector<uint8_t> &data)
{
    void *pData = reinterpret_cast<void*>(data.data());
    ssize_t msg_len = m_pSocket->Recv(pData, MSG_MAX_LENGTH);
    if (msg_len <= 0)
    {
        CYBERLOG_ERROR("msg len: %ld\n", msg_len);
        m_pSocket->Close();
        return ERR_CODE_COMMUNICATION_FAILURE;
    }


    return ERR_CODE_OK;
}
