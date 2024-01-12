#ifndef _SECURITY_IMPL_H_
#define _SECURITY_IMPL_H_

#include <map>
#include <mutex>
#include "./../common/protocol.h"

class CSecurityImpl
{
public:
    static CSecurityImpl &getInstance(void);

    std::uint32_t ReqExecute(std::int32_t cmd_id, void *req_data, std::int32_t req_len, void *resp_data, std::int32_t *resp_len);

    std::uint32_t Open(void);

    std::uint32_t Close(void);

    std::uint32_t GenerateRandom(void *req_data, std::int32_t req_len, void *resp_data, std::int32_t *resp_len);

private:
    CSecurityImpl();

    // 移动构造函数
    CSecurityImpl(CSecurityImpl &&) = default;

    // 移动赋值操作符
    CSecurityImpl &operator=(CSecurityImpl &&) = default; // 拷贝构造函数

    virtual ~CSecurityImpl();

    CSecurityImpl(const CSecurityImpl &);
    CSecurityImpl &operator=(const CSecurityImpl &);

    

private:
    // std::mutex m_mtxSecurity;
    std::int32_t m_nOpenCount;
};

#endif
