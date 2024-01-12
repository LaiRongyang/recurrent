#include <string.h>
#include <stdio.h>
#include "security_impl.h"
#include "./../common/message.h"
#include "./../common/logger.h"

#define KEY_LEN_16 16
#define KEY_LEN_24 24
#define KEY_LEN_32 32

CSecurityImpl::CSecurityImpl()
{
    m_nOpenCount = 0;
}

CSecurityImpl::~CSecurityImpl()
{

}

CSecurityImpl &CSecurityImpl::getInstance(void)
{
    static CSecurityImpl instance;
    return instance;
}

std::uint32_t CSecurityImpl::ReqExecute(std::int32_t cmd_id, void *req, std::int32_t req_len, void *resp, std::int32_t *resp_len)
{
    uint32_t ret = COM_OK;
    switch (cmd_id)
    {
    case EN_CMD_ID_CREATE:
        *resp_len = 0;
        CYBERLOG_INFO("EN_CMD_ID_CREATE");
        break;
    case EN_CMD_ID_OPENDEV:
        *resp_len = 0;
        ret = Open();
        break;
    case EN_CMD_ID_CLOSEDEV:
        *resp_len = 0;
        ret =  Close();
        break;
    case EN_CMD_ID_GEN_RAND:
        ret =  GenerateRandom(req, req_len, resp, resp_len);
        break;
    
    default:
        if (nullptr != resp_len)
        {
            *resp_len = 0;
        }
        ret =  COM_INVALID_PARAMETER;
    }
    return ret;
}

std::uint32_t CSecurityImpl::Open()
{
    return COM_OK;
}

std::uint32_t CSecurityImpl::Close()
{
    return COM_OK;
}

std::uint32_t CSecurityImpl::GenerateRandom(void *req_data, std::int32_t req_len, void *resp_data, std::int32_t *resp_len)
{
    if ((*resp_len < req_len) || (sizeof(REQRANDOM) > (std::uint64_t)req_len))
    {
        CYBERLOG_ERROR("input data invalid.");
        return COM_INVALID_PARAMETER;
    }

    LPREQRANDOM lpIn = reinterpret_cast<LPREQRANDOM>(req_data);
    LPRESPRANDOM lpOut = reinterpret_cast<LPRESPRANDOM>(resp_data);
    std::int8_t *pRand = lpOut->random;
    memcpy(pRand, "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef", 128);
    lpOut->data_len = 128;
    *resp_len = static_cast<int32_t>(128) + static_cast<int32_t>(sizeof(std::uint32_t));
    return COM_OK;
}