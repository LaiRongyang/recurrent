/**
* @file securityservice.h
* @brief 安全服务引擎
* @details
* @author liu.zhen (liu.zhen30@byd.com) jiang.qi (jiang.qi4@byd.com)
* @version 1.1.0
* @date 2023/03/13 17:30:00
* @copyright Copyright (c) 2023 比亚迪股份有限公司
*/

#ifndef _SECURITY_SERVICE_H_
#define _SECURITY_SERVICE_H_


/**
* @enum ErrorCode
* @brief Error code
*/
#define ERR_CODE_OK                     0x00000000U // 成功
#define ERR_CODE_NO_DEVICE              0x00000001  // 无设备
#define ERR_CODE_COMMUNICATION_FAILURE  0x00000002  // 通信错误
#define ERR_CODE_EXECUTE_FAILURE        0x00000003  // 执行失败
#define ERR_CODE_INVALID_PARAMETER      0x00000004  // 无效参数
#define ERR_CODE_UNSUPPORT_COMMAND      0x00000005  // 不支持的命令
#define ERR_CODE_SOFTWARE_ERROR         0x00000006  // 软件错误
#define ERR_CODE_HARDWARE_ERROR         0x00000007  // 硬件错误
#define ERR_CODE_PROCEDURE_ERROR        0x00000008  // 程序错误，接口调用顺序不对
#define ERR_CODE_SPACE_NOT_ENOUGH       0x00000009  // 没有足够的空间
#define ERR_CODE_DATA_ERROR             0x0000000a  // 数据长度、格式等与与其不符
#define ERR_CODE_NO_POWER               0x0000000b  // 无权限
#define ERR_CODE_TIMEOUT                0x0000000c  // 超时错误
#define ERR_CODE_SYSTEM_ERROR           0x0000000d  // 系统错误
#define ERR_CODE_CONFIGURE_ERROR        0x0000000e  // 配置读取失败
#define ERR_CODE_POINTER_NULL           0x0000000f  // 指针为空
#define ERR_CODE_UNKNOWN                0x000000ff  // 未定义的错误类型


// CSS_read_certificate
#define CERT_ID_ROOT            0x01   // 根证书
#define CERT_ID_SUB             0x02    // 子证书
#define CERT_ID_USER            0x03   // 用户证书
#define CERT_ID_CSR             0x04    //
#define CERT_ID_SERVER          0x05 //服务器证书

// CSS_write_file/CSS_read_file
#define KET_FACTOR_1            0x01
#define KET_FACTOR_2            0x02

// CSS_read_key
#define ECU_KEY                 0x01
#define OTA_KEY_FACTOR          0x02
#define SECOC_KEY               0x03

// 对称密钥索引号
#define KEY_ID_SECOC    0x01 // AES128
#define KEY_ID_PATAM    0x02 // AES128
#define KEY_ID_DATA     0x03 // AES256

typedef void* TLS_HANLDE; 

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief open_service function
* @details 打开引擎服务
* @return error code
*/
unsigned int CSS_open_service();

/**
* @brief close_service function
* @details 关闭引擎服务
* @return error code
*/
void CSS_close_service();


/**
* @brief CSS_generate_random function
* @details 生成随机数
* @param len [in] 随机数长度
* @param rand [out] 随机数
* @return error code
*/
unsigned int CSS_generate_random(unsigned int len, unsigned char* rand);

#ifdef __cplusplus
}
#endif

#endif  // _SECURITY_SERVICE_H_
