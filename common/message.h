#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <cstring>
#include <vector>
#include <cstdlib>
#include "protocol.h"


////////////////////////////////////////////////
class CDataMgr
{
public:
    void *data;

public:
    CDataMgr(/* args */);

    CDataMgr(void *buf, std::int32_t len);

    // 拷贝构造函数
    CDataMgr(CDataMgr const &) = default;

    // 移动构造函数
    CDataMgr(CDataMgr &&) = default;

    // 复制赋值操作符
    CDataMgr &operator=(const CDataMgr &) = default;

    // 移动赋值操作符
    CDataMgr &operator=(CDataMgr &&) = default;

    virtual ~CDataMgr();

    void *MallocData(size_t len);

    void FreeData();
};

#endif // _MY_MESSAGE_H_
