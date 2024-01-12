#ifndef _SECURITY_SERVER_H_
#define _SECURITY_SERVER_H_

#include <list>
#include <thread>
#include <mutex>
#include "./../common/my_socket.h"
#include "./../common/message.h"
#include "message_queue.hpp"
#include "security_session.h"

class CSecurityServer
{
protected:
    MessageQueue<CDataMgr*> m_msgQueue;
    std::thread *m_pThread;
    std::list<CSecuritySession *> m_lstServer;
    std::int32_t m_nSessionId;
    bool m_bMonitor;
    std::mutex m_mtxServerList;

public:
    CSecurityServer(/* args */);

    // 拷贝构造函数
    CSecurityServer(CSecurityServer const &) = default;

    // 移动构造函数
    CSecurityServer(CSecurityServer &&) = default;

    // 复制赋值操作符
    CSecurityServer &operator=(const CSecurityServer &) = default;

    // 移动赋值操作符
    CSecurityServer &operator=(CSecurityServer &&) = default; // 拷贝构造函数

    virtual ~CSecurityServer();

    virtual bool startService() = 0;

    virtual bool massageLoop();
    virtual bool stopService();
    virtual void stopMonitor();

protected:
    virtual std::uint32_t HandleComErrCode(std::uint32_t nErr);

    virtual std::uint32_t Send(void *pData, std::int32_t nLen) = 0;


    bool Insert(CSecuritySession *pServer);
    bool Remove(CSecuritySession *pServer);

    virtual bool StartMonitorThread();

    virtual void MonitorEvent() = 0;

    virtual std::uint32_t Execute(LPREQHEAD lpReq);
    
    virtual std::uint32_t ExecuteCreate(LPREQHEAD lpReq);

    virtual std::uint32_t ExecuteDestory(LPREQHEAD lpReq);

    CSecuritySession *GetServer(std::int32_t nSession);

};

#endif // _SECURITY_SERVER_H_
