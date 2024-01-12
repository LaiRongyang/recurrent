#include <string.h>
#include <stdio.h>
#include "security_session.h"
#include "security_impl.h"
#include "./../common/logger.h"

CSecuritySession::CSecuritySession(std::int32_t nSessionId /* = 0*/)
{
    m_nSession = nSessionId;
}

CSecuritySession::~CSecuritySession()
{
}

std::uint32_t CSecuritySession::ReqExecute(LPREQHEAD lpReq, std::vector<uint8_t> &resp)
{
    LPRESPHEAD lpResp = reinterpret_cast<LPRESPHEAD>(resp.data());
    std::int32_t *pLen = (std::int32_t *)(&lpResp->data_len);
    return CSecurityImpl::getInstance().ReqExecute(lpReq->msg_head.cmd_id, lpReq + 1, lpReq->data_len, lpResp + 1, pLen);
}
