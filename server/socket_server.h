#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include <map>
#include "security_server.h"

class CSocketServer : public CSecurityServer
{
private:
    std::map<std::int32_t, CSocket *> m_mapSocket;
    class CSocket *m_pSocket;
    std::int32_t m_nEpoll;

public:
    CSocketServer(/* args */);

    // 拷贝构造函数
    CSocketServer(CSocketServer const &) = default;

    // 移动构造函数
    CSocketServer(CSocketServer &&) = default;

    // 复制赋值操作符
    CSocketServer &operator=(const CSocketServer &) = default;

    // 移动赋值操作符
    CSocketServer &operator=(CSocketServer &&) = default; // 拷贝构造函数

    virtual ~CSocketServer();

    virtual bool startService() override;
    // virtual bool massageLoop() override;

protected:
    virtual std::uint32_t Send(void *pData, std::int32_t nLen) override;

    virtual void MonitorEvent() override;

    virtual std::uint32_t ExecuteCreate(LPREQHEAD lpReq) override;

    virtual std::uint32_t ExecuteDestory(LPREQHEAD lpReq);

private:
};

#endif // _SOCKET_CLIENT_H_
