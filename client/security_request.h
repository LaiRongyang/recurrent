#ifndef _SECURITY_REQUEST_H_
#define _SECURITY_REQUEST_H_

#include <cstdint>


#define HEX_LENGTH 16
#define PATH_MAX_LEN 260
#define NULL_LENGTH 4
#define KEY_LENGTH 32

// 消息头结构
typedef struct tagTLPointerToNum
{
    unsigned short number; // 指针名称
} POINTERTONUM, *LPPOINTERTONUM;

class CSecurityRequest
{
private:
    class CSecurityClient *m_client;

    CSecurityRequest(/* args */);
    ~CSecurityRequest();

public:
    static CSecurityRequest &getInstance();

    // /**
    // * @brief RegisterProgressCallBack function
    // * @details register Progress CallBack function
    // * @param func [in] callback function pointer
    // * @param user [in] user data
    // * @param units [in] units
    // * @return error code
    // */

    /**
     * @brief OpenDevice
     * @details Open Device
     */
    std::uint32_t OpenDevice();

    /**
     * @brief CloseDevice
     * @details Close Device
     */
    std::uint32_t CloseDevice();

    std::uint32_t GenerateRandom(std::uint32_t len, std::uint8_t *rand);

    // 拷贝构造函数
    CSecurityRequest(CSecurityRequest const &) = default;

    // 移动构造函数
    CSecurityRequest(CSecurityRequest &&) = default;

    // 复制赋值操作符
    CSecurityRequest &operator=(const CSecurityRequest &) = default;

    // 移动赋值操作符
    CSecurityRequest &operator=(CSecurityRequest &&) = default;
};

#endif
