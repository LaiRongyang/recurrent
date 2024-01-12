#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include <string>

class CSocket
{
public:
    CSocket(/* args */);
    explicit CSocket(std::int32_t hSocket);

    // 拷贝构造函数
    CSocket(CSocket const &) = default;

    // 移动构造函数
    CSocket(CSocket &&) = default;

    // 复制赋值操作符
    CSocket &operator=(const CSocket &) = default;

    // 移动赋值操作符
    CSocket &operator=(CSocket &&) = default;

    virtual ~CSocket();

    /**
    * @brief Create function
    * @details 创建一个新的服务端
    * @param pszPath [in] 网络连接IP与端口(ip:port)，或本地连接虚拟文件名(name)
    * @return true 成功；false 失败
    */
    bool Create(const std::string &strPath);

    /**
    * @brief Accept function
    * @details 接收客户端连接请求
    * @param hSocketClient [out] 客户端套接字
    * @return true 成功；false 失败
    */
    bool Accept(std::int32_t &hSocketClient);

    /**
    * @brief Connect function
    * @details 创建一个新的客户端
    * @param pszPath [in] 网络连接IP与端口(ip:port)，或本地连接虚拟文件名(name)
    * @return true 成功；false 失败
    */
    bool Connect(const std::string &strPath);

    /**
    * @brief Send function
    * @details 发送数据
    * @param pBuf [in] 发送的数据
    * @param nLen [in] 发送数据长度
    * @return true 成功；false 失败
    */
    bool Send(void *pBuf, std::int32_t nLen);

    /**
    * @brief Recv function
    * @details 接收数据
    * @param pBuf [in] 接收的数据
    * @param nLen [in] 接收的数据长度
    * @return 返回实际读取到的长度
    */
    std::int32_t Recv(void *pBuf, std::int32_t nLen);

    /**
    * @brief Close function
    * @details 断开连接
    * @return true 成功；false 失败
    */
    bool Close();

    /**
    * @brief GetState function
    * @details 获取连接状态
    * @return false：未连接；true：已连接
    */
    bool GetState();

    /**
    * @brief GetSocketId function
    * @details 获取socket id
    * @return socket id
    */
    std::int32_t GetSocketId();

private:
    bool AnalysisString(const std::int8_t *pszAddr, std::string &strIp, std::int32_t &iPort);
    void UpdateSendBufSize();
    std::uint32_t GetTickCount();

private:
    std::int32_t m_hSocket;
    std::int32_t m_nSendBufSize;
};

#endif // _MY_SOCKET_H_