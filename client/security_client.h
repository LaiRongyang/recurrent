#ifndef _SECURITY_CLIENT_H_
#define _SECURITY_CLIENT_H_

#include "./../common/message.h"

class CSecurityClient
{
protected:
    bool m_bConnect;
    std::int32_t m_nReqId;
    std::int32_t m_nSessionId;
    std::int32_t m_nChannelId;

public:
    CSecurityClient(/* args */);
    virtual ~CSecurityClient();

    bool IsConnect(void) { return m_bConnect; }

    virtual std::uint32_t Execute(std::int32_t req, std::vector<uint8_t> &in, std::vector<uint8_t> &out);

    virtual std::uint32_t ConnectServer(void) = 0;
    virtual std::uint32_t DisconnectServer(void) = 0;

    // 拷贝构造函数
    CSecurityClient(CSecurityClient const &) = default;

    // 移动构造函数
    CSecurityClient(CSecurityClient &&) = default;

    // 复制赋值操作符
    CSecurityClient &operator=(const CSecurityClient &) = default;

    // 移动赋值操作符
    CSecurityClient &operator=(CSecurityClient &&) = default;

protected:

    virtual std::uint32_t HandleComErrCode(std::uint32_t nErr);

    virtual std::uint32_t Send(void *pData, std::int32_t nLen) = 0;

    virtual std::uint32_t Recv(std::vector<uint8_t> &data) = 0;

    virtual std::uint32_t SendFile(void *pData, std::int32_t nLen);
};

#endif
