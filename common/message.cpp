#include "message.h"

//////////////////////////////////////////////////////////////
CDataMgr::CDataMgr(/* args */)
{
    // PRINT_ERR("****");
    data = nullptr;
}

CDataMgr::CDataMgr(void *buf, std::int32_t len)
{
    // PRINT_ERR("****");
    data = malloc(len);
    memcpy(data, buf, len);
}

CDataMgr::~CDataMgr()
{
    // PRINT_ERR("****");
    if (nullptr != data)
    {
        free(data);
        data = nullptr;
    }
}

void *CDataMgr::MallocData(size_t len)
{
    if (nullptr != data)
    {
        free(data);
        data = nullptr;
    }
    data = malloc(len);
    return data;
}

void CDataMgr::FreeData()
{
    if (nullptr != data)
    {
        free(data);
        data = nullptr;
    }
}