#ifndef _SECURITY_SESSION_H_
#define _SECURITY_SESSION_H_

#include "./../common/message.h"

class CSecuritySession
{
public:
    CSecuritySession(std::int32_t nSessionId = 0);

    // 拷贝构造函数
    CSecuritySession(CSecuritySession const &) = default;

    // 移动构造函数
    CSecuritySession(CSecuritySession &&) = default;

    // 复制赋值操作符
    CSecuritySession &operator=(const CSecuritySession &) = default;

    // 移动赋值操作符
    CSecuritySession &operator=(CSecuritySession &&) = default; // 拷贝构造函数

    ~CSecuritySession();

    std::uint32_t ReqExecute(LPREQHEAD lpReq, std::vector<uint8_t> &resp);

    std::int32_t GetSessionId() { return m_nSession; }

private:
    std::int32_t m_nSession;
};

#endif // _SECURITY_SESSION_H_
