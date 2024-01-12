#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include "security_client.h"
#include "./../common/message.h"
#include "./../common/logger.h"

#define NAME_LEN 32
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
CSecurityClient::CSecurityClient(/* args */)
    : m_bConnect(false), m_nReqId(0), m_nSessionId(0)
{
}

CSecurityClient::~CSecurityClient()
{
}

std::uint32_t CSecurityClient::Execute(std::int32_t req, std::vector<uint8_t> &in, std::vector<uint8_t> &out)
{
    std::int32_t nBufLen = sizeof(REQHEAD) + in.size();
    void *pBuf = malloc(nBufLen);
    if (pBuf == nullptr)
    {
        CYBERLOG_ERROR("malloc failed.");
        return ERR_CODE_SYSTEM_ERROR;
    }

    if (nullptr == memset(pBuf, 0, nBufLen))
    {
        CYBERLOG_WARN("memset failed.");
    }
    printf("cmd id: %d", req);

    LPREQHEAD lpReq = reinterpret_cast<LPREQHEAD>(pBuf);
    lpReq->msg_head.msg_type = static_cast<std::int32_t>(EN_MSG_TYPE_REQ);
    lpReq->msg_head.cmd_id = req;
    ++m_nReqId;
    lpReq->msg_head.req_id = m_nReqId;
    lpReq->msg_head.session_id = m_nSessionId;
    lpReq->msg_head.msg_size = static_cast<std::uint32_t>(nBufLen);
    lpReq->msg_head.channel_id = m_nChannelId;
    lpReq->data_len = static_cast<std::uint32_t>(in.size());

    if ((!in.empty()) && (nullptr == memcpy(lpReq + 1, in.data(), in.size())))
    {
        CYBERLOG_ERROR("memcpy failed.");
    }

    std::uint32_t nRet = Send(pBuf, nBufLen);
    free(pBuf);
    if ((ERR_CODE_OK != nRet) || (EN_CMD_ID_DESTORY == req)) // 销毁命令不关心结果
    {
        return nRet;
    }

    std::vector<uint8_t> resp(MSG_MAX_LENGTH, 0);
    nRet = Recv(resp);

    if (ERR_CODE_OK != nRet)
    {
        return nRet;
    }

    LPRESPHEAD lpResp = reinterpret_cast<LPRESPHEAD>(resp.data());
    if (EN_CMD_ID_CREATE == req)
    {
        m_nSessionId = lpResp->msg_head.session_id;
    }

    if (nullptr == lpResp)
    {
        CYBERLOG_ERROR("lpResp == nullptr");
        return ERR_CODE_SOFTWARE_ERROR;
    }

    if ((lpResp->msg_head.session_id != m_nSessionId) ||
        (lpResp->msg_head.req_id != m_nReqId) ||
        (lpResp->msg_head.cmd_id != (std::int32_t)req))
    {
        CYBERLOG_ERROR("session id: %d > %d", m_nSessionId, lpResp->msg_head.session_id);
        CYBERLOG_ERROR("req id: %d > %d", m_nReqId, lpResp->msg_head.req_id);
        CYBERLOG_ERROR("cmd id: %d > %d", req, lpResp->msg_head.cmd_id);
        out.clear();

        return ERR_CODE_EXECUTE_FAILURE;
    }

    out.resize(lpResp->data_len);

    if (out.size() == lpResp->data_len)
    {
        if (nullptr == memcpy((void*)out.data(), (void *)(lpResp + 1), lpResp->data_len))
        {
            CYBERLOG_ERROR("memcpy failed.");
        }
    }

    return HandleComErrCode(lpResp->error_code);
}

std::uint32_t CSecurityClient::SendFile(void *pData, std::int32_t nLen)
{
    LPREQHEAD lpReq = static_cast<LPREQHEAD>(pData);
    std::int8_t cName[PATH_MAX_LEN] = {0};
    sprintf((char *)cName, "/optdata/log/coredump/secrity-req[%d-%d]", lpReq->msg_head.channel_id, lpReq->msg_head.req_id);
    FILE *fp = fopen((char *)cName, "wb");
    if (fp == nullptr)
    {
        CYBERLOG_ERROR(("Cannot open file: file=%s"), cName);
        return ERR_CODE_SOFTWARE_ERROR;
    }

    fwrite(pData, nLen, 1, fp);
    fclose(fp);

    lpReq->msg_head.msg_type = EN_MSG_TYPE_FILE;
    lpReq->data_len = 0;

    return Send(lpReq, sizeof(REQHEAD));
}



std::uint32_t CSecurityClient::HandleComErrCode(std::uint32_t nErr)
{
    // 将通信层错误码COM_XXX转化成客户段错误码ERR_CODE_XXX
    // 因为目前两边定义一致,可以直接相等
    return nErr;
}
