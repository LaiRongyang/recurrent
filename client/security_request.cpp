#include <string.h>
#include "security_request.h"
#include "security_client.h"
#include "socket_client.h"
#include "./../common/message.h"
#include "./../common/logger.h"
#define NUMBER_THIRTYTWO 32
#define NUMBER_TWENTY 20

#define CHECKCONNECT            \
    if (!(m_client->IsConnect())) \
    {                           \
        CYBERLOG_ERROR("no connect!"); \
        return ERR_CODE_COMMUNICATION_FAILURE; \
    }

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

CSecurityRequest::CSecurityRequest(/* args */)
    : m_client(nullptr)
{
    m_client = new CSocketClient();
}

CSecurityRequest::~CSecurityRequest()
{
    if (nullptr != m_client)
    {
        delete m_client;
    }
}

CSecurityRequest &CSecurityRequest::getInstance(void)
{
    static CSecurityRequest instance;
    return instance;
}


std::uint32_t CSecurityRequest::OpenDevice(void)
{
    m_client->ConnectServer();
    CHECKCONNECT

    std::vector<uint8_t> in;
    std::vector<uint8_t> out;
    std::uint32_t nRet = m_client->Execute(EN_CMD_ID_CREATE, in, out);
    if (ERR_CODE_OK != nRet)
    {
        return nRet;
    }

    return m_client->Execute(EN_CMD_ID_OPENDEV, in, out);
}

std::uint32_t CSecurityRequest::CloseDevice(void)
{
    std::vector<uint8_t> in;
    std::vector<uint8_t> out;
    std::uint32_t nRet1 = m_client->Execute(EN_CMD_ID_CLOSEDEV, in, out);
    std::uint32_t nRet2 = m_client->Execute(EN_CMD_ID_DESTORY, in, out);

    if (ERR_CODE_OK != nRet1)
    {
        return nRet1;
    }
    return nRet2;
}

std::uint32_t CSecurityRequest::GenerateRandom(std::uint32_t len, std::uint8_t *rand)
{
    CHECKCONNECT
    std::vector<uint8_t> in(sizeof(REQRANDOM), 0);
    std::vector<uint8_t> out;
    LPREQRANDOM lpIn = reinterpret_cast<LPREQRANDOM>(in.data());
    lpIn->random_len = 128;
    std::uint32_t nRet = m_client->Execute(EN_CMD_ID_GEN_RAND, in, out);

    if (nRet != 0U)
    {
        CYBERLOG_ERROR("RANDOM EXECUTE CALCULATE ERROR!");
        return nRet;
    }
    if (out.size() == 0U)
    {
        CYBERLOG_ERROR("RANDOM OUT_LEN CALCULATE ERROR!");
        return ERR_CODE_DATA_ERROR;
    }
    LPRESPRANDOM lpResp = reinterpret_cast<LPRESPRANDOM>(out.data());
    if (lpResp->data_len != len)
    {
        CYBERLOG_ERROR("RANDOM OUT_BUF CALCULATE ERROR!");
        return ERR_CODE_DATA_ERROR;
    }

    if (rand == nullptr)
    {
        CYBERLOG_ERROR("RAND ERROR!");
        return ERR_CODE_INVALID_PARAMETER;
    }
    if(nullptr == memcpy(rand, lpResp->random, lpResp->data_len))
    {
        CYBERLOG_ERROR("memcpy failed.");
        return ERR_CODE_EXECUTE_FAILURE;
    }

    return nRet;
}
