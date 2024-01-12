#ifndef _SERVER_MANAGER_H_
#define _SERVER_MANAFER_H_

#include <thread>
#include <list>
// #include <mutex>
#include "./../common/my_socket.h"
#include "./../common/message.h"
#include "security_server.h"

class CServerManager
{
public:
    static CServerManager &getInstance();
    bool initService();
    bool startService();
    bool massageLoop();
    bool stopService();
    void stopMonitor();

private:
    CServerManager(/* args */);

    // 拷贝构造函数
    CServerManager(CServerManager const &) = default;

    // 移动构造函数
    CServerManager(CServerManager &&) = default;

    // 复制赋值操作符
    CServerManager &operator=(const CServerManager &) = default;

    // 移动赋值操作符
    CServerManager &operator=(CServerManager &&) = default; // 拷贝构造函数

    virtual ~CServerManager();

private:
    CSecurityServer *m_pCom;
};

#endif