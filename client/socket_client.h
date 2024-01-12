#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "security_client.h"

class CSocketClient : public CSecurityClient
{
private:
    /* data */
    class CSocket *m_pSocket;

public:
    CSocketClient(/* args */);
    virtual ~CSocketClient();
    // 拷贝构造函数
    CSocketClient(CSocketClient const &) = default;

    // 移动构造函数
    CSocketClient(CSocketClient &&) = default;

    // 复制赋值操作符
    CSocketClient &operator=(const CSocketClient &) = default;

    // 移动赋值操作符
    CSocketClient &operator=(CSocketClient &&) = default;

    virtual std::uint32_t ConnectServer(void) override;

    virtual std::uint32_t DisconnectServer(void) override;

protected:
    virtual std::uint32_t Send(void *pData, std::int32_t nLen) override;

    virtual std::uint32_t Recv(std::vector<uint8_t> &data) override;
};

#endif // _SOCKET_CLIENT_H_
